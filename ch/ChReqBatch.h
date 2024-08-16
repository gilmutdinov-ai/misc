#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace misc {

class ChReqBatch {
  std::vector<std::string> m_reqs;

public:
  void acc(const std::string &_req);
  void merge(const ChReqBatch &_a);

  size_t size() const;
  bool empty() const;
  size_t buildBody(size_t _max_inserts_in_req, std::string &_body) const;
  ChReqBatch slice(size_t _slice_count) const;

  void print() const;
};
} // namespace misc