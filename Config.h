#pragma once
#include "json/json.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

/*
call parse(fn), parseJsonString(str) or onParsed if filled by hand
then validate("func or filename to find where exception")
*/

namespace misc {
class Config : public Json::Value {

  std::vector<std::string> m_arrays;
  std::vector<std::string> m_strings;
  std::vector<std::string> m_ints;

  // print help, dont validate
  std::vector<std::string> m_strings_opt;
  std::vector<std::string> m_ints_opt;

private:
  Config &operator=(const Config &);
  Config(const Config &);

  std::vector<std::function<void()>> m_on_parsed_cbs;

public:
  Config() = default;

  std::string help() const;
  void parse(const std::string &_config_path);
  void parseJsonString(const std::string &_json);

  void addOnParsedCb(std::function<void()> _cb);
  void onParsed();

  void addArrays(const std::vector<std::string> &v);
  void addInts(const std::vector<std::string> &v);
  void addStrings(const std::vector<std::string> &v);

  void addIntsOpt(const std::vector<std::string> &v);
  void addStringsOpt(const std::vector<std::string> &v);

  void validate() const;
  void validate(const std::string &_dbg) const;

  bool validateImpl(const Json::Value &cfg, std::string &err) const;

  static std::string renderArray(const Json::Value &_js_arr, char _delim);
};

} // namespace misc