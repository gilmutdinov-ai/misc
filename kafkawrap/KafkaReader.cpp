#include "KafkaReader.h"
#include <stdexcept>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/time.h>

namespace misc {

static volatile sig_atomic_t run = 1;

static void sigterm(int sig) { run = 0; }

static int64_t now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((int64_t)tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

KafkaReader::KafkaReader(const std::string &_topic,
                         const std::vector<std::string> &_brokers,
                         const std::string &_group_id) {
  std::unique_ptr<RdKafka::Conf> conf{
      RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};
  if (conf->set("enable.partition.eof", "false", m_errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << m_errstr << std::endl;
    throw std::runtime_error("KafkaReader::KafkaReader " + m_errstr);
  }

  if (conf->set("group.id", _group_id, m_errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << m_errstr << std::endl;
    throw std::runtime_error("KafkaReader::KafkaReader " + m_errstr);
  }

  if (conf->set("metadata.broker.list", brokers_to_str(_brokers), m_errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << m_errstr << std::endl;
    throw std::runtime_error("KafkaReader::KafkaReader " + m_errstr);
  }

  signal(SIGINT, sigterm);
  signal(SIGTERM, sigterm);

  /* Create consumer */
  m_consumer.reset(RdKafka::KafkaConsumer::create(conf.get(), m_errstr));
  if (!m_consumer) {
    std::cerr << "Failed to create consumer: " << m_errstr << std::endl;
    exit(1);
  }

  /* Subscribe to topics */
  RdKafka::ErrorCode err = m_consumer->subscribe({_topic});
  if (err) {
    std::cerr << "Failed to subscribe to " << _topic
              << " topic: " << RdKafka::err2str(err) << std::endl;
    exit(1);
  }
}

KafkaReader::~KafkaReader() { m_consumer->close(); }

std::vector<KafkaReader::KafkaMessagePtr>
KafkaReader::consumeBatch(RdKafka::KafkaConsumer *consumer, size_t batch_size,
                          int batch_tmout) {
  std::vector<KafkaReader::KafkaMessagePtr> msgs;
  msgs.reserve(batch_size);

  int64_t end = now() + batch_tmout;
  int remaining_timeout = batch_tmout;

  while (msgs.size() < batch_size) {
    KafkaMessagePtr msg{consumer->consume(remaining_timeout)};

    switch (msg->err()) {
    case RdKafka::ERR__TIMED_OUT:
      return msgs;

    case RdKafka::ERR_NO_ERROR:
      msgs.push_back(msg);
      break;

    default:
      std::cerr << "%% Consumer error: " << msg->errstr() << std::endl;
      run = 0;
      return msgs;
    }

    remaining_timeout = end - now();
    if (remaining_timeout < 0)
      break;
  }

  return msgs;
}

void KafkaReader::stop() { run = 0; }

void KafkaReader::read(std::function<void(const std::string &mess)> _cb,
                       long long _limit) {

  long long i = 0;
  while (run) {
    auto msgs = consumeBatch(m_consumer.get(), m_batch_size, m_batch_tmout);
    // std::cout << "Accumulated " << msgs.size() << " messages:" << std::endl;

    for (auto &msg : msgs) {
      std::string payload(static_cast<const char *>(msg->payload()),
                          msg->len());
      // std::cout << " Message in " << msg->topic_name() << " ["
      //           << msg->partition() << "] at offset " << msg->offset()
      //           << std::endl
      //           << "payload: " << payload << std::endl;
      try {
        _cb(payload);
      } catch (std::exception &ex) {
        std::cerr << "KafkaReader::read ex: " << ex.what() << std::endl;
      }
      ++i;
      if (_limit != -1 && i >= _limit)
        return;
    }
  }
}

} // namespace misc
