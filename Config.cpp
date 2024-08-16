#include "Config.h"
#include <ostream>
#include <sstream>

namespace misc {

std::string Config::help() const {

  std::stringstream ss;
  ss << "Config::help" << std::endl;
  ss << "required strings: " << std::endl;
  for (auto it : m_strings) {
    ss << " " << it << std::endl;
  }
  ss << "optional strings: " << std::endl;
  for (auto it : m_strings_opt) {
    ss << " " << it << std::endl;
  }
  ss << "required ints: " << std::endl;
  for (auto it : m_ints) {
    ss << " " << it << std::endl;
  }
  ss << "optional ints: " << std::endl;
  for (auto it : m_ints_opt) {
    ss << " " << it << std::endl;
  }
  ss << "required arrays: " << std::endl;
  for (auto it : m_arrays) {
    ss << " " << it << std::endl;
  }
  return ss.str();
}

void Config::parse(const std::string &_config_path) {
  // parse config
  std::ifstream c_stream(_config_path);
  std::string c_string((std::istreambuf_iterator<char>(c_stream)),
                       std::istreambuf_iterator<char>());

  parseJsonString(c_string);
}

void Config::parseJsonString(const std::string &_json) {
  if (!Json::Reader().parse(_json, *this)) {
    throw std::invalid_argument(
        "Config::parseJsonString could not parse config");
  }
  onParsed();
}

void Config::onParsed() {
  for (size_t i = 0; i < m_on_parsed_cbs.size(); ++i)
    m_on_parsed_cbs[i]();
}

void Config::addOnParsedCb(std::function<void()> _cb) {
  m_on_parsed_cbs.push_back(_cb);
}

void Config::addArrays(const std::vector<std::string> &v) {
  m_arrays.insert(m_arrays.end(), v.begin(), v.end());
}

void Config::addInts(const std::vector<std::string> &v) {
  m_ints.insert(m_ints.end(), v.begin(), v.end());
}

void Config::addStrings(const std::vector<std::string> &v) {
  m_strings.insert(m_strings.end(), v.begin(), v.end());
}

void Config::addIntsOpt(const std::vector<std::string> &v) {
  m_ints_opt.insert(m_ints_opt.end(), v.begin(), v.end());
}

void Config::addStringsOpt(const std::vector<std::string> &v) {
  m_strings_opt.insert(m_strings_opt.end(), v.begin(), v.end());
}

void Config::validate() const {
  std::string err;
  if (!validateImpl(*this, err))
    throw std::invalid_argument("Config::validate " + err);
}

void Config::validate(const std::string &_dbg) const {
  std::string err;
  if (!validateImpl(*this, err))
    throw std::invalid_argument("Config::validate " + _dbg + " " + err);
}

bool Config::validateImpl(const Json::Value &cfg, std::string &err) const {

  for (auto i = 0; i < m_strings.size(); ++i)
    if (!cfg[m_strings[i]].isString()) {

      err = "expected string: " + m_strings[i];
      return false;
    }
  for (auto i = 0; i < m_ints.size(); ++i)
    if (!cfg[m_ints[i]].isIntegral()) {

      err = "expected int: " + m_ints[i];
      return false;
    }
  for (auto i = 0; i < m_arrays.size(); ++i)
    if (!cfg[m_arrays[i]].isArray()) {

      err = "expected array: " + m_arrays[i];
      return false;
    }
  return true;
}

} // namespace misc