#include "ChWriter.h"
#include "misc/Strings.h"
#include <iostream>

namespace misc {

ChWriter::ChWriter(const std::string &_clickhouse_url,
                   const WriterSettings &_writer_settings)
    : ChWriterMergeQueue(_writer_settings.merge_interval_secs),

      m_clickhouse_url{_clickhouse_url},
      m_max_requests_in_batch{_writer_settings.max_requests_in_batch},
      m_req_batch(new ChReqBatch{}), // MergeQueue global obj
      m_client(new HttpClient{
          std::bind(&ChWriter::_onResponse, this, std::placeholders::_1),
          HttpClient::HttpSettings{_writer_settings.user_agent,
                                   _writer_settings.request_timeout_secs,
                                   _writer_settings.select_interval_usecs}}) {
  ChWriterMergeQueue::start();
}

ChWriter::~ChWriter() {
  ChWriterMergeQueue::stop();
  ChWriterMergeQueue::join();
}

void ChWriter::_onResponse(HttpClient::JobInfo _ji) {
  if (m_on_response_cb)
    (*m_on_response_cb)(_ji);
}

/*
void ChWriter::evloop() {

  if (m_req_q.size() > 100) {

    int i = 0;
    //  std::string reqs = m_clickhouse_url;
    while (!m_req_q.empty() && i < 30) {

      auto req = m_req_q.front();
      //   reqs += req;
      m_req_q.pop();
      ++i;

      if (std::rand() % 100 > 90)
        m_client->callPost(0, req, "");
    }
  }

  m_client->kick();
  m_evloop_thread.post(std::bind(&ChWriter::evloop, this));
}
*/
/*
ChWriter::ImpressionRecord::ImpressionRecord(VisitorPtr _v, uint64_t _s,
                                             uint64_t _a, uint64_t _c)
    : v(_v), s(_s), a(_a), c(_c) {}

std::string ChWriter::ImpressionRecord::operator()() const {

  // CREATE TABLE impressions (ts Date, vid UInt64, device UInt8, os UInt8,
  // browser UInt8, gender UInt8, spot_id UInt64, ad_id UInt64, campaign_id
  // UInt64, geo_id UInt32) ENGINE = MergeTree(ts, ts, 8192);

  return url_encode(
      "INSERT INTO impressions VALUES (" +
      std::to_string(DateTime().setNow().toUnixTimeStamp() / (3600 * 24)) +
      ", " + v->vid.asStr() + "," + std::to_string(v->ua->deviceId()) + "," +
      std::to_string(v->ua->OSId()) + "," + std::to_string(v->ua->browserId()) +
      "," + std::to_string(v->gender) + "," + std::to_string(s) + "," +
      std::to_string(a) + "," + std::to_string(c) + "," +
      std::to_string(v->sypex_geo.id) + ");");
}
*/

} // namespace misc
