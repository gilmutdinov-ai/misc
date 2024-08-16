#include <catch2/catch_test_macros.hpp>

#include "misc/kafkawrap/ut/KafkaStaticSimTester.h"

TEST_CASE("Test KafkaStaticSim", "[KafkaSim]") { misc::KafkaStaticSimTester{}; }
