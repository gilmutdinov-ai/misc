#include "misc/ch/ut/ChWriterTester.h"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

namespace misc {

ChWriterTester::ChWriterTester() {
  _testChReqBatch();
  _testChWriter();
}

void ChWriterTester::_testChReqBatch() {

  ChReqBatch b;
  for (size_t i = 0; i < 10; ++i)
    b.acc(std::to_string(i));

  const size_t max_reqs = 3;
  std::string expect_body{"0;\n"
                          "1;\n"
                          "2;\n"};
  std::string body;
  const size_t got_reqs = b.buildBody(max_reqs, body);
  REQUIRE(got_reqs == max_reqs);
  REQUIRE(body == expect_body);
  // std::cout << body << std::endl;
  b = b.slice(got_reqs);
  REQUIRE(b.size() == 7);
  // b.print();
  REQUIRE_THROWS(b.slice(8));

  b = b.slice(b.size());
  REQUIRE(b.empty());
}

void ChWriterTester::_setupFakeServer() {

  try {
    m_srv.reset(new httplib::Server);
    m_srv->Post("/", [&](const httplib::Request &req, httplib::Response &res) {
      m_srv_recv_body = req.body;
      res.set_content(m_srv_hello, "text/plain");
    });
    m_srv->listen("127.0.0.1", m_listen_port);
  } catch (std::exception &_ex) {
    std::cout << "_setupFakeServer ex: " << _ex.what() << std::endl;
  }
}

void ChWriterTester::_testChWriter() {
  try {
    std::cout << "_testChWriter starting server thread\n";
    std::thread th([&]() {
      _setupFakeServer();
      std::cout << "exiting thread\n";
    });
    sleep(2);

    std::cout << "_testChWriter starting client\n";

    ChWriter ch{"http://127.0.0.1:" + std::to_string(m_listen_port)};

    ch.onResponse([&](auto _ji) {
      std::cout << "ChWriter::onResponse success: " << _ji.success << std::endl;
      std::cout << "ChWriter::onResponse curl_error: " << _ji.curlerror
                << std::endl;
      std::cout << "ChWriter::onResponse: " << _ji.resp << std::endl;

      m_cli_recv_body = _ji.resp;
    });

    ch.push("INSERT 1 INTO X");
    ch.push("INSERT 2 INTO X");

    const std::string expect_body{"INSERT 1 INTO X;\n"
                                  "INSERT 2 INTO X;\n"};

    ch.waitMerged();

    m_srv->stop();
    th.join();

    sleep(3);

    REQUIRE(m_srv_recv_body == expect_body);
    REQUIRE(m_cli_recv_body == m_srv_hello);

  } catch (std::exception &_ex) {
    std::cout << "_testChWriter ex: " << _ex.what() << std::endl;
  }
}

} // namespace misc
