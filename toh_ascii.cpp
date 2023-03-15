#include "toh_tmp_states.hpp"

using solution = typename ToH_start<3>::moves;

//using state = typename Nth<7,solution>::type;

using art = typename Ascii_t<4,typename Nth<0,solution>::type::start_t>::type;

#include <cstdio>

int main()
{
  constexpr auto str = art::repr();
  printf("%s",str.data());
  for(auto c : str)
  {
    printf("%02u ", c);
  }
//#error success
}
