#include "Config.h"
#include <ostream>
#include <sstream>

namespace misc {

Config &Config::operator=(const YAML::Node &_y) {
  *static_cast<YAML::Node *>(this) = _y;
  return *this;
}

Config::Config(const YAML::Node &_y) { *static_cast<YAML::Node *>(this) = _y; }

Config &Config::operator=(const std::string &s) {
  YAML::Node::operator=(s);
  return *this;
}

Config &Config::operator=(int i) {
  YAML::Node::operator=(i);
  return *this;
}

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

void Config::parseFile(const std::string &_path) {

  *this = YAML::LoadFile(_path);
  onParsed();
}

void Config::parseString(const std::string &_str) {
  *this = YAML::Load(_str);
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

bool Config::validateImpl(const YAML::Node &cfg, std::string &err) const {

  for (auto i = 0; i < m_strings.size(); ++i) {
    if (cfg[m_strings[i]].Type() != YAML::NodeType::Scalar) {
      err = "expected string: " + m_strings[i];
      return false;
    }
    std::string string_value;
    if (!YAML::convert<std::string>::decode(cfg[m_strings[i]], string_value)) {
      err = "expected string: " + m_strings[i];
      return false;
    }
  }
  for (auto i = 0; i < m_ints.size(); ++i) {
    if (cfg[m_ints[i]].Type() != YAML::NodeType::Scalar) {

      err = "expected int: " + m_ints[i];
      return false;
    }
    int int_value;
    if (!YAML::convert<int>::decode(cfg[m_ints[i]], int_value)) {
      err = "expected int: " + m_ints[i];
      return false;
    }
  }
  for (auto i = 0; i < m_arrays.size(); ++i)
    if (cfg[m_arrays[i]].Type() != YAML::NodeType::Sequence) {

      err = "expected array: " + m_arrays[i];
      return false;
    }
  return true;
}

std::vector<std::string> Config::getStrArray(const YAML::Node &_node) {
  std::vector<std::string> ret;
  for (YAML::const_iterator it = _node.begin(); it != _node.end(); ++it)
    ret.push_back(it->as<std::string>());
  return ret;
}

std::string Config::renderArray(const YAML::Node &_node, char _delim) {

  auto size = _node.size();
  size_t i = 0;
  std::stringstream ss;
  for (YAML::const_iterator it = _node.begin(); it != _node.end(); ++it) {
    ss << it->as<std::string>();
    if (i != size - 1)
      ss << _delim;
    ++i;
  }
  return ss.str();
}

} // namespace misc