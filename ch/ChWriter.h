#pragma once

#include "misc/MergeQueue.h"
#include "misc/ch/ChReqBatch.h"
#include "misc/httpcli/http_client.h"
#include <memory>
#include <optional>

namespace misc {

// 1 request / request batch to push
using ChWriterMergeQueue = MergeQueue<std::string, ChReqBatch>;

class ChWriter : public ChWriterMergeQueue {
public:
  struct WriterSettings {
    std::string user_agent = "misc::ChWriter";
    long request_timeout_secs = 3;
    long select_interval_usecs = 50000;
    long merge_interval_secs = 1;
    long max_requests_in_batch = 128;

    // clang? https://stackoverflow.com/a/44693603
    WriterSettings() noexcept {};
  };

private:
  // HTTP CLI
  const std::string m_clickhouse_url;
  const long m_max_requests_in_batch;

  using HttpClient = HttpClientA<int>;
  using HttpClientPtr = std::shared_ptr<HttpClient>;
  HttpClientPtr m_client;

  std::optional<std::function<void(HttpClient::JobInfo _ji)>> m_on_response_cb;

  void _onResponse(HttpClient::JobInfo _ji);

  // MERGE QUEUE
  std::unique_ptr<ChReqBatch> m_req_batch;
  void accEvent(ChReqBatch &_batch, const std::string &_req) override {

    m_req_batch->acc(_req);
    m_client->kick();
  }
  ChReqBatch *copyGlobalObj() override {
    m_client->kick();
    return new ChReqBatch{*m_req_batch};
  }
  void merge(ChReqBatch &_a, const ChReqBatch &_b) override {
    m_client->kick();
    _a.merge(_b);
  }
  void switchGlobalObj(ChReqBatch *_a) override {
    // нарезать на реальные запросы и послать в curl
    _sendToCurl(_a);
    m_req_batch.reset(_a);
    // kick
    m_client->kick();
  }
  ////
  void _sendToCurl(ChReqBatch *_a) {

    while (!_a->empty()) {
      std::string body;
      auto batch_size = _a->buildBody(m_max_requests_in_batch, body);
      *_a = _a->slice(batch_size);
      m_client->callPost(1, m_clickhouse_url, body);
    }
    m_client->kick();
  }

public:
  using Ptr = std::shared_ptr<ChWriter>;
  ChWriter(const std::string &_clickhouse_url,
           const WriterSettings &_writer_settings = WriterSettings{});
  virtual ~ChWriter();

  void push(const std::string &_req) { ChWriterMergeQueue::push(_req); }

  void onResponse(std::function<void(HttpClient::JobInfo _ji)> _cb) {
    m_on_response_cb = _cb;
  }
};

} // namespace misc
