#include "misc/kafkawrap/ut/KafkaStaticSimTester.h"

namespace misc {

KafkaStaticSimTester::KafkaStaticSimTester() {

  // 1 topic
  {
    std::string topic{"KafkaStaticSimTester"};
    for (int i = 0; i < 10; ++i)
      KafkaStaticSim::push(topic, std::to_string(i));

    for (int i = 0; i < 10; ++i) {
      std::string b;
      REQUIRE(KafkaStaticSim::pop(topic, b));
      REQUIRE(stoi(b) == i);
    }
  }
  // 2 topics
  {
    std::string topic1{"KafkaStaticSimTester1"};
    std::string topic2{"KafkaStaticSimTester2"};
    size_t t1_size{10};
    size_t t2_size{20};
    for (int i = 0; i < t1_size; ++i)
      KafkaStaticSim::push(topic1, std::to_string(i));
    for (int i = 0; i < t2_size; ++i)
      KafkaStaticSim::push(topic2, std::to_string(i));

    for (int i = 0; i < t1_size; ++i) {
      std::string b;
      REQUIRE(KafkaStaticSim::pop(topic1, b));
      REQUIRE(stoi(b) == i);
    }
    for (int i = 0; i < t2_size; ++i) {
      std::string b;
      REQUIRE(KafkaStaticSim::pop(topic2, b));
      REQUIRE(stoi(b) == i);
    }
  }
}

} // namespace misc
