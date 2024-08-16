#pragma once

#include "KafkaWriter.h"

namespace misc {
class KafkaWriterMock : public KafkaWriterI {
  const std::string m_sim_topic;

public:
  KafkaWriterMock() = default;
  KafkaWriterMock(const std::string &_sim_topic);
  virtual ~KafkaWriterMock() = default;

  void write(const std::string &_topic, const std::string &_mess) override;
  void flush(int _ms) override {}
};
} // namespace misc