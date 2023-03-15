#include "toh_tmp_moves.hpp"

#if !defined(TOWER_HEIGHT)
#error "tower height should be defined"
#endif
constexpr int tower_height = TOWER_HEIGHT;
#undef TOWER_HEIGHT

using solution = typename ToH_start<tower_height>::moves;

template<typename>
class Print;
Print<solution> as_error;

