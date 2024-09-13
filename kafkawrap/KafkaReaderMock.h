#pragma once

#include "KafkaReader.h"
#include <algorithm>
#include <string>
#include <vector>

namespace misc {

class KafkaReaderMock : public KafkaReaderI {
  const std::vector<std::string> m_data;
  const std::string m_sim_topic; // if empty, use const data

public:
  KafkaReaderMock(const std::string &_sim_topic);
  KafkaReaderMock(const std::vector<std::string> &_data);
  // KafkaReaderMock(std::vector<std::string> &&_data);

  virtual ~KafkaReaderMock() = default;
  void read(bool _dont_block, std::function<void(const std::string &mess)> _cb,
            long long _limit = -1) override;
  void stop() override;

  static std::vector<std::string> loadNewlineFile(const std::string &_path);
};

} // namespace misc
