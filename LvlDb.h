#pragma once

#include "leveldb/db.h"
#include <memory>

namespace misc {

class LvlDb {
  leveldb::DB *m_db;

public:
  using Ptr = std::shared_ptr<LvlDb>;

  LvlDb(const std::string &_db_path);
  virtual ~LvlDb();
  void save(const std::string &_key, const std::string &_value);
  bool load(const std::string &_key, std::string &_value);
  void del(const std::string &_key);

  using ScanCb = std::function<void(const std::string &, const std::string &)>;
  void scanFull(ScanCb _cb);
};

} // namespace misc