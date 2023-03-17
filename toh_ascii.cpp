#include "toh_tmp_draw.hpp"

#if !defined(TOWER_HEIGHT)
#error "tower height should be defined"
#endif
constexpr int tower_height = TOWER_HEIGHT;
#undef TOWER_HEIGHT
#if !defined(MOVE_NUM)
#error "move number should be defined"
#endif
constexpr int move_num = MOVE_NUM;
#undef MOVE_NUM

using solution = typename ToH_start<tower_height>::moves;

using move_to_print = typename Nth<move_num,solution>::type;

using before = typename Ascii_t<tower_height,move_to_print::start_t>::type;
using move_text = typename Move_String_t<move_to_print>::type;
using after = typename Ascii_t<tower_height,move_to_print::end_t>::type;

constexpr auto before_s = before::repr();
constexpr auto move_s = move_text::repr();
constexpr auto after_s = after::repr();

#include <cstdio>

int main()
{
  puts("before:");
  puts(reinterpret_cast<const char*>(&before_s));
  puts(reinterpret_cast<const char*>(&move_s));
  puts("after:");
  puts(reinterpret_cast<const char*>(&after_s));
}
