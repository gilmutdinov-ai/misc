#include "ChReqBatch.h"

namespace misc {

void ChReqBatch::acc(const std::string &_req) { m_reqs.push_back(_req); }

void ChReqBatch::merge(const ChReqBatch &_a) {
  m_reqs.insert(m_reqs.end(), _a.m_reqs.begin(), _a.m_reqs.end());
}

size_t ChReqBatch::size() const { return m_reqs.size(); }

bool ChReqBatch::empty() const { return m_reqs.empty(); }

size_t ChReqBatch::buildBody(size_t _max_inserts_in_req,
                             std::string &_body) const {

  std::stringstream ss;
  size_t inserted_count = 0;
  for (size_t i = 0; i < std::min(_max_inserts_in_req, m_reqs.size()); ++i) {

    ss << m_reqs[i] << ";" << std::endl;
    ++inserted_count;
  }
  _body = ss.str();
  return inserted_count;
}

ChReqBatch ChReqBatch::slice(size_t _slice_count) const {
  if (_slice_count == m_reqs.size())
    return ChReqBatch{};
  if (_slice_count > m_reqs.size())
    throw std::invalid_argument("ChReqBatch::slice _slice_count > size");

  ChReqBatch ret;
  ret.m_reqs.reserve(m_reqs.size() - _slice_count);
  for (size_t i = _slice_count; i < m_reqs.size(); ++i)
    ret.m_reqs.push_back(m_reqs[i]);
  return ret;
}

void ChReqBatch::print() const {

  std::cout << "ChReqBatch: ";
  for (size_t i = 0; i < m_reqs.size(); ++i) {
    std::cout << m_reqs[i] << " ";
  }
  std::cout << std::endl;
}

} // namespace misc