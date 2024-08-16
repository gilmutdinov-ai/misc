#include "KafkaWriterMock.h"
#include "KafkaStaticSim.h"

namespace misc {

KafkaWriterMock::KafkaWriterMock(const std::string &_sim_topic)
    : m_sim_topic(_sim_topic) {}

void KafkaWriterMock::write(const std::string &_topic,
                            const std::string &_mess) {
  if (_topic.empty() || m_sim_topic.empty())
    return;
  KafkaStaticSim::push(_topic, _mess);
}

} // namespace misc
