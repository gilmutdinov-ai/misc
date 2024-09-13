#include "KafkaReaderMock.h"
#include "KafkaStaticSim.h"
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

namespace misc {

KafkaReaderMock::KafkaReaderMock(const std::vector<std::string> &_data)
    : m_data(_data) {}

KafkaReaderMock::KafkaReaderMock(const std::string &_sim_topic)
    : m_sim_topic(_sim_topic) {}

void KafkaReaderMock::stop() {}

void KafkaReaderMock::read(bool _dont_block, // ignored
                           std::function<void(const std::string &mess)> _cb,
                           long long _limit) {
  if (m_sim_topic.empty()) { // const data mode
    size_t e = (_limit == -1)
                   ? m_data.size()
                   : std::min(static_cast<size_t>(_limit), m_data.size());
    for (size_t i = 0; i < e; ++i) {
      _cb(m_data[i]);
    }
  }
  // sim mode
  long long i = 0;
  std::string mess;
  while (KafkaStaticSim::pop(m_sim_topic, mess)) {
    _cb(mess);
    if (_limit != -1 && i >= _limit)
      break;
  }
}

std::vector<std::string>
KafkaReaderMock::loadNewlineFile(const std::string &_path) {

  std::vector<std::string> ret;
  std::ifstream f{_path};
  std::string line;

  while (std::getline(f, line)) {
    if (!line.empty())
      ret.push_back(line);
  }

  return ret;
}

} // namespace misc
