// https://github.com/kumagi/nanahan/blob/master/map/map.cc
#include "nanahan_map.hpp"

template class nanahan::Map<int, int>;
template class nanahan::Map<uint64_t, std::string>;
template class nanahan::Map<std::string, std::string>;
template class nanahan::Map<bool, bool>;
template class nanahan::Map<std::string, int>;
template class nanahan::Map<std::string, uint32_t>;

