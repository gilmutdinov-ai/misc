#pragma once

#include "KafkaCommon.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <librdkafka/rdkafkacpp.h>

namespace misc {

class DeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
  void dr_cb(RdKafka::Message &message);
};

class KafkaWriterI {
public:
  using Ptr = std::shared_ptr<KafkaWriterI>;

  virtual ~KafkaWriterI() = default;
  virtual void write(const std::string &_topic, const std::string &_mess) = 0;
  virtual void flush(int _ms) = 0;
};

class KafkaWriter : public KafkaWriterI {

  const static int m_wait_queue_full_ms{1000};

  using KafkaMessage = RdKafka::Message;
  using KafkaMessagePtr = std::shared_ptr<RdKafka::Message>;
  using KafkaProducerPtr = std::unique_ptr<RdKafka::Producer>;

public:
  KafkaWriter(const std::vector<std::string> &_brokers);
  virtual ~KafkaWriter() = default;

  void write(const std::string &_topic, const std::string &_mess) override;
  void flush(int _ms) override;

private:
  std::string m_errstr;

  DeliveryReportCb m_dr_cb;
  KafkaProducerPtr m_producer;
};

} // namespace misc
