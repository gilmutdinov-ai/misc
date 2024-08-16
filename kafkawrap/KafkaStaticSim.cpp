#include "KafkaStaticSim.h"

namespace misc {

void KafkaStaticSim::push(const std::string &_topic, const std::string &_mess) {

  std::unordered_map<std::string, Topic>::iterator it;
  Topic *topic;
  {
    std::lock_guard<std::mutex> g_lock(s_mtx);
    topic = &(KafkaStaticSim::s_topics[_topic]);
  }
  std::lock_guard<std::mutex> lock(topic->mtx);
  topic->q.push(_mess);
}

bool KafkaStaticSim::pop(const std::string &_topic, std::string &_mess) {

  std::unordered_map<std::string, Topic>::iterator it;
  Topic *topic;
  {
    std::lock_guard<std::mutex> g_lock(s_mtx);
    it = KafkaStaticSim::s_topics.find(_topic);
    if (it == KafkaStaticSim::s_topics.end())
      return false;
    topic = &(it->second);
  }

  std::lock_guard<std::mutex> lock(topic->mtx);
  auto &q = topic->q;
  if (q.empty())
    return false;

  _mess = q.front();
  q.pop();
  return true;
}

size_t KafkaStaticSim::size(const std::string &_topic) {
  std::unordered_map<std::string, Topic>::iterator it;
  Topic *topic;
  {
    std::lock_guard<std::mutex> g_lock(s_mtx);
    it = KafkaStaticSim::s_topics.find(_topic);
    if (it == KafkaStaticSim::s_topics.end())
      return 0;
    topic = &(it->second);
  }

  std::lock_guard<std::mutex> lock(topic->mtx);
  auto &q = topic->q;
  return q.size();
}

} // namespace misc
