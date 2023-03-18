#include "toh_tmp_draw.hpp"

#if !defined(TOWER_HEIGHT)
#error "tower height should be defined"
#endif
constexpr int tower_height = TOWER_HEIGHT;
#undef TOWER_HEIGHT

using solution = typename ToH_start<tower_height>::moves;

#if defined(MOVE_NUM)
constexpr int move_num = MOVE_NUM;
#undef MOVE_NUM

using move_to_print = typename Nth<move_num,solution>::type;

using before = typename Ascii_t<tower_height,move_to_print::start_t>::type;
using move_text = typename Move_String_t<move_to_print>::type;
using after = typename Ascii_t<tower_height,move_to_print::end_t>::type;

using to_print = before::template concat<move_text>::template concat<after>;
#else
using to_print = typename Step_by_Step_t<tower_height, solution>::type;
#endif

constexpr auto str = to_print::repr();

#include <cstdio>

int main()
{
  puts((const char*)&str);
}
