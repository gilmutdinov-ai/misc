#pragma once
#include "yaml-cpp/yaml.h"
#include "json/json.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

/*
Конфиги подсистем наследуются от Config, чтобы не менять везде, Config в одном
месте наследуется от Config<Json::Value> или Config<YAML::Node>
*/

/*
call parse(fn), parseJsonString(str) or onParsed if filled by hand
then validate("func or filename to find where exception")
*/

namespace misc {
class Config : public YAML::Node {
protected:
private:
  std::vector<std::string> m_arrays;
  std::vector<std::string> m_strings;
  std::vector<std::string> m_ints;

  // print help, dont validate
  std::vector<std::string> m_strings_opt;
  std::vector<std::string> m_ints_opt;

private:
  Config &operator=(const Config &) = default;
  Config &operator=(const YAML::Node &);
  Config(const Config &);
  Config(const YAML::Node &);

  std::vector<std::function<void()>> m_on_parsed_cbs;

public:
  Config() = default;
  virtual ~Config() = default;

  std::string help() const;

  Config &operator=(const std::string &);
  Config &operator=(int i);

  // Format dependant implementations:
  void parseFile(const std::string &_path);
  void parseString(const std::string &_str);

  bool validateImpl(const YAML::Node &cfg, std::string &err) const;

  static std::string renderArray(const Json::Value &_js_arr, char _delim);

  static std::vector<std::string> getStrArray(const YAML::Node &_node);
  ///

  void addOnParsedCb(std::function<void()> _cb);
  void onParsed();

  void addArrays(const std::vector<std::string> &v);
  void addInts(const std::vector<std::string> &v);
  void addStrings(const std::vector<std::string> &v);

  void addIntsOpt(const std::vector<std::string> &v);
  void addStringsOpt(const std::vector<std::string> &v);

  void validate() const;
  void validate(const std::string &_dbg) const;
};

} // namespace misc