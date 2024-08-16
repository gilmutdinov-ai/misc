#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

namespace misc {
/*
add mutex
*/
class KafkaStaticSim {
  struct Topic {
    std::mutex mtx;
    std::queue<std::string> q;
  };

  inline static std::unordered_map<std::string, Topic> s_topics;
  inline static std::mutex s_mtx;

  KafkaStaticSim() = delete;

public:
  static void push(const std::string &_topic, const std::string &_mess);
  static bool pop(const std::string &_topic, std::string &_mess);

  static size_t size(const std::string &_topic);
};

} // namespace misc