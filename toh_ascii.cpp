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

constexpr auto s1 = before::repr();
constexpr auto s2 = move_text::repr();
constexpr auto s3 = after::repr();

#include <cstdio>

int main()
{
  printf("before:\n%s\n%s\nafter:\n%s",s1.data(),s2.data(),s3.data());
}
