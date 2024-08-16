#pragma once
#include "httplib.h"
#include "misc/ch/ChWriter.h"

namespace misc {
class ChWriterTester {
  std::unique_ptr<httplib::Server> m_srv;
  inline static const int m_listen_port{8081};

  inline static const std::string m_srv_hello{"Hello from fake server!"};
  std::string m_srv_recv_body;
  std::string m_cli_recv_body;

public:
  ChWriterTester();

private:
  void _testChReqBatch();
  void _testChWriter();
  void _setupFakeServer();
};
} // namespace misc
