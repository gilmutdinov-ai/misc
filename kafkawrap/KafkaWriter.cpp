#include "KafkaWriter.h"
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

void DeliveryReportCb::dr_cb(RdKafka::Message &message) {
  /* If message.err() is non-zero the message delivery failed permanently
   * for the message. */
  if (message.err())
    std::cout << "DeliveryReportCb "
              << "% Message delivery failed: " << message.errstr() << std::endl;

  /*  else
      std::cout << "% Message delivered to topic " << message.topic_name() << "
     ["
                << message.partition() << "] at offset " << message.offset()
                << std::endl;*/
}

KafkaWriter::KafkaWriter(const std::vector<std::string> &_brokers) {
  std::unique_ptr<RdKafka::Conf> conf{
      RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};
  /*
  if (conf->set("enable.partition.eof", "false", m_errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << m_errstr << std::endl;
    throw std::runtime_error("KafkaWriter::KafkaWriter " + m_errstr);
  }

  if (conf->set("group.id", "test_kafka", m_errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << m_errstr << std::endl;
    throw std::runtime_error("KafkaWriter::KafkaWriter " + m_errstr);
  }*/

  if (conf->set("bootstrap.servers", brokers_to_str(_brokers), m_errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << "KafkaWriter: " << m_errstr << std::endl;
    throw std::runtime_error("KafkaWriter::KafkaWriter " + m_errstr);
  }

  signal(SIGINT, sigterm);
  signal(SIGTERM, sigterm);

  /* Delivery report */
  if (conf->set("dr_cb", &m_dr_cb, m_errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << "KafkaWriter delivery report err: " << m_errstr << std::endl;
    throw std::runtime_error("KafkaWriter delivery report err: " + m_errstr);
  }

  /* Create producer */
  m_producer.reset(RdKafka::Producer::create(conf.get(), m_errstr));
  if (!m_producer) {
    std::cerr << "Failed to create producer: " << m_errstr << std::endl;
    throw std::runtime_error("Failed to create producer: " + m_errstr);
  }
}

void KafkaWriter::flush(int _ms) { m_producer->flush(_ms); }

void KafkaWriter::write(const std::string &_topic, const std::string &_mess) {
retry_label:
  RdKafka::ErrorCode err = m_producer->produce(
      /* Topic name */
      _topic,
      /* Any Partition: the builtin partitioner will be
       * used to assign the message to a topic based
       * on the message key, or random partition if
       * the key is not set. */
      RdKafka::Topic::PARTITION_UA,
      /* Make a copy of the value */
      RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
      /* Value */
      const_cast<char *>(_mess.c_str()), _mess.size(),
      /* Key */
      NULL, 0,
      /* Timestamp (defaults to current time) */
      0,
      /* Message headers, if any */
      NULL,
      /* Per-message opaque value passed to
       * delivery report */
      NULL);

  if (err != RdKafka::ERR_NO_ERROR) {

    std::cerr << "% Failed to produce to topic " << _topic << ": "
              << RdKafka::err2str(err) << std::endl;

    if (err == RdKafka::ERR__QUEUE_FULL) {
      /* If the internal queue is full, wait for
       * messages to be delivered and then retry.
       * The internal queue represents both
       * messages to be sent and messages that have
       * been sent or failed, awaiting their
       * delivery report callback to be called.
       *
       * The internal queue is limited by the
       * configuration property
       * queue.buffering.max.messages and queue.buffering.max.kbytes */
      m_producer->poll(m_wait_queue_full_ms /*block for max 1000ms*/);
      goto retry_label;
    }

  } else {
    // std::cout << "% Enqueued message (" << _mess.size() << " bytes) "
    //           << "for topic " << _topic << std::endl;
  }
}

} // namespace misc
