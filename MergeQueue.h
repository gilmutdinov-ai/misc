#pragma once

#include "concurrentqueue.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <shared_mutex>
#include <thread>

namespace misc {

/*

Thread w/ lock-free queue, accumulate events, merge & switch global object
Access global object only with readLock

Optimize for non-interrupted reading
    cost: copying global object, delay in changes

*/

template <typename EVENT, typename GLOBAL_OBJ, size_t BUFF_SIZE = 128>
class MergeQueue {

  const int m_merge_interval_secs;

  std::atomic<bool> m_running;

  moodycamel::ConcurrentQueue<EVENT> m_q;
  std::unique_ptr<std::thread> m_merge_th;

  // for global obj
  std::shared_mutex m_mtx;

  // for waiting current merge finished
  std::mutex m_merge_mtx;
  std::condition_variable m_merge_cv;
  bool m_merge_finished{false};

public:
  // using GLOBAL_OBJ_PTR = std::unique_ptr<GLOBAL_OBJ>;

  MergeQueue(int _merge_interval_secs)
      : m_merge_interval_secs(_merge_interval_secs), m_running(true) {}

  void start() {
    m_running.store(true);
    m_merge_th.reset(new std::thread(std::bind(&MergeQueue::_thread, this)));
  }
  void stop() { m_running.store(false); }

  void join() { m_merge_th->join(); }

  // wait to merge real data
  void waitMerged() {

    // if (m_q.size_approx() == 0)
    //   return;

    std::unique_lock<std::mutex> lck(m_merge_mtx);
    m_merge_cv.wait(lck, [&] { return m_merge_finished; });
  }

  virtual ~MergeQueue() = default;

  std::shared_lock<std::shared_mutex> &&readLock() {
    return std::move(std::shared_lock<std::shared_mutex>(m_mtx));
  }

  void push(const EVENT &_ev) { m_q.enqueue(_ev); }
  size_t size_approx() { return m_q.size_approx(); }

  virtual void accEvent(GLOBAL_OBJ &obj, const EVENT &ev) = 0;
  virtual GLOBAL_OBJ *copyGlobalObj() = 0;
  virtual void merge(GLOBAL_OBJ &a, const GLOBAL_OBJ &b) = 0;
  virtual void switchGlobalObj(GLOBAL_OBJ *a) = 0;

private:
  void _setMergeStarted() {
    {
      std::lock_guard<std::mutex> lck(m_merge_mtx);
      m_merge_finished = false;
    }
  }

  void _setMergeFinished() {
    {
      std::lock_guard<std::mutex> lck(m_merge_mtx);
      m_merge_finished = true;
    }
    m_merge_cv.notify_all();
  }

  std::unique_lock<std::shared_mutex> &&writeLock() {
    return std::move(std::unique_lock<std::shared_mutex>(m_mtx));
  }

  void _thread() {
    while (m_running.load()) {
      _merge();
      sleep(m_merge_interval_secs);
    }
    _merge();
  }

  void _merge() {

    _setMergeStarted();

    std::unique_ptr<GLOBAL_OBJ> acc_local(new GLOBAL_OBJ);

    int i = 0;
    /*
        const size_t evs_max{BUFF_SIZE};
        EVENT evs[evs_max];
        size_t deq_count = m_q.try_dequeue_bulk(evs, evs_max);
        if (deq_count == 0)
          return;

        for (size_t j = 0; j < deq_count; ++j) {
          accEvent(*acc_local, evs[j]);
          ++i;
        }
        */

    EVENT ev;
    while (m_q.try_dequeue(ev)) {
      accEvent(*acc_local, ev);
      ++i;
    }

    if (i == 0) {
      // Waiting to merge real data
      //_setMergeFinished();
      return;
    }

    // read-lock, copy data from live
    GLOBAL_OBJ *local_copy;
    {
      auto read_lock = readLock();
      local_copy = copyGlobalObj();
    }
    // merge locally
    merge(*local_copy, *acc_local);
    // write-lock, switch live
    {
      auto write_lock = writeLock();
      switchGlobalObj(local_copy);
    }
    _setMergeFinished();
  }
};
} // namespace misc