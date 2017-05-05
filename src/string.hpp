#pragma once

#include <string>

namespace parka {

// There are still a few regular char literals floating around, but set the
// stage for providing some custom string type.
using string = std::string;
using char_type = string::value_type;

} // namespace parka
