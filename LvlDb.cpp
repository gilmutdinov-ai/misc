#include "misc/LvlDb.h"
#include <filesystem>
#include <stdexcept>

namespace misc {

LvlDb::LvlDb(const std::string &_db_path) {

  std::filesystem::create_directories(_db_path);

  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, _db_path.c_str(), &m_db);
  if (!status.ok())
    throw std::runtime_error("LvlDb::LvlDb leveldb !ok " + status.ToString());
}

LvlDb::~LvlDb() { delete m_db; }

void LvlDb::save(const std::string &_key, const std::string &_value) {

  auto s = m_db->Put(leveldb::WriteOptions(), _key, _value);
  if (!s.ok()) {
    throw std::runtime_error("LvlDb::save could not write leveldb " +
                             s.ToString());
  }
}

bool LvlDb::load(const std::string &_key, std::string &_value) {
  leveldb::Status s = m_db->Get(leveldb::ReadOptions(), _key, &_value);
  return s.ok();
}

void LvlDb::del(const std::string &_key) {

  auto s = m_db->Delete(leveldb::WriteOptions(), _key);
  if (!s.ok()) {
    throw std::runtime_error("LvlDb::del could not delete from leveldb " +
                             s.ToString());
  }
}

void LvlDb::scanFull(LvlDb::ScanCb _cb) {

  std::unique_ptr<leveldb::Iterator> it;
  it.reset(m_db->NewIterator(leveldb::ReadOptions()));

  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    _cb(it->key().ToString(), it->value().ToString());
  }

  if (!it->status().ok())
    throw std::runtime_error("LvlDb::scanfull " + it->status().ToString());
}

} // namespace misc
