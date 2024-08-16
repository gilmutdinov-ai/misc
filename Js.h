#pragma once
#include "json/json.h"
#include <string>
#include <vector>

namespace misc {

std::vector<std::string> get_str_array_from_json(const Json::Value &_js);

} // namespace misc
