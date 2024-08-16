#include "Js.h"

namespace misc {

std::vector<std::string> get_str_array_from_json(const Json::Value &_js) {
  std::vector<std::string> ret;
  for (Json::Value::ArrayIndex i = 0; i < _js.size(); ++i) {
    if (!_js[i].isString())
      throw std::invalid_argument("get_str_array_from_json !_js.isString()");
    ret.push_back(_js[i].asString());
  }
  return ret;
}

} // namespace misc