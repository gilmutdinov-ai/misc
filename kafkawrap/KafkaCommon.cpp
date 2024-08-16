#include "KafkaCommon.h"
#include <sstream>

namespace misc {

std::string brokers_to_str(const std::vector<std::string> &_brokers) {
  std::stringstream ss;
  for (auto i = 0; i < _brokers.size(); ++i) {
    ss << _brokers[i];
    if (i != _brokers.size() - 1)
      ss << ",";
  }
  return ss.str();
}

} // namespace misc
