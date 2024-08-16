#pragma once

#include "core/Types.h"
#include "misc/MergeQueue.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace misc {

/*
    В качестве теста редьюсим поток UrlFreq в мап
        тк MergeQueue:
            - фоном
            - lockfree
*/

using TesterMap = std::unordered_map<Url, Cnt>;
using TesterMergeQueue = MergeQueue<UrlFreq, TesterMap>;

class MergeQueueTester : public TesterMergeQueue {

  std::unique_ptr<TesterMap> g_map;

  const Url m_url{"https://ya.ru"};
  static constexpr int merge_interval_sec{3};
  Cnt m_expect_events = 0;

public:
  MergeQueueTester() : TesterMergeQueue(MergeQueueTester::merge_interval_sec) {
    g_map.reset(new TesterMap);
    TesterMergeQueue::start(); // after g_map init :-)
    _writeSampleData();
    _checkAfterState();
  }

  ~MergeQueueTester() {
    stop();
    join();
  }

private:
  virtual void accEvent(TesterMap &_obj, const UrlFreq &_ev) {
    _obj[_ev.first] += _ev.second;
  }
  virtual TesterMap *copyGlobalObj() { return new TesterMap{*g_map}; }

  virtual void merge(TesterMap &a, const TesterMap &b) {
    for (auto it = b.begin(); it != b.end(); ++it) {
      a[it->first] += it->second;
    }
  }

  virtual void switchGlobalObj(TesterMap *a) { g_map.reset(a); };

  void _writeSampleData() {

    for (size_t i = 0; i < 100; ++i) {
      UrlFreq uf{m_url, i};
      TesterMergeQueue::push(uf);
      m_expect_events += i;
    }
  }

  void _checkAfterState() {

    TesterMergeQueue::waitMerged();

    auto lock = TesterMergeQueue::readLock();
    REQUIRE(g_map->size() == 1);
    auto it = g_map->find(m_url);
    REQUIRE(it != g_map->end());
    REQUIRE(it->second == m_expect_events);
  }
};

} // namespace misc