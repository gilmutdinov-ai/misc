#pragma once

#include "KafkaCommon.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <librdkafka/rdkafkacpp.h>

namespace misc {

class KafkaReaderI {
public:
  using Ptr = std::shared_ptr<KafkaReaderI>;
  virtual ~KafkaReaderI() = default;
  virtual void read(bool _dont_block,
                    std::function<void(const std::string &mess)> _cb,
                    long long _limit = -1) = 0;
  virtual void stop() = 0;
};

class KafkaReader : public KafkaReaderI {

  const static int m_batch_size = 10;
  const static int m_batch_tmout = 5000;

public:
  using KafkaMessage = RdKafka::Message;
  using KafkaMessagePtr = std::shared_ptr<RdKafka::Message>;

  using KafkaConsumerPtr = std::unique_ptr<RdKafka::KafkaConsumer>;

  KafkaReader(const std::string &_topic,
              const std::vector<std::string> &_brokers,
              const std::string &_group_id = "test_kafka");
  virtual ~KafkaReader();

  void read(bool _dont_block, std::function<void(const std::string &mess)> _cb,
            long long _limit = -1) override;

  void stop() override;

private:
  static std::vector<KafkaMessagePtr>
  consumeBatch(RdKafka::KafkaConsumer *consumer, size_t batch_size,
               int batch_tmout);

  void read_(std::function<void(const std::string &mess)> _cb, bool _one_batch,
             long long _limit = -1);

  std::string m_errstr;

  KafkaConsumerPtr m_consumer;
};

} // namespace misc
