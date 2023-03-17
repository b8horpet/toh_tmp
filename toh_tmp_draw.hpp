#pragma once
#include "toh_tmp_states.hpp"
#include <array>

template<char... Chs>
struct String_t
{
  template<char... other>
  constexpr static auto concat_f(String_t<other...>) -> String_t<Chs..., other...>;
  template<typename U>
  using concat = decltype(concat_f(U{}));
  static constexpr std::array<char, sizeof...(Chs)+1> repr()
  {
    return {Chs...,'\0'};
  }
};

template<char C, int N>
struct Repeat_t
{
  using type = typename String_t<C>::template concat<typename Repeat_t<C,N-1>::type>;
};

template<char C>
struct Repeat_t<C,0>
{
  using type = String_t<>;
};

template<int N>
using Empty_t = Repeat_t<' ',N>;

template<int N>
using Footer_t = Repeat_t<'_',N>;

template<int N, char C>
struct Row_base_t
{
  using type = typename Empty_t<N>::type::template concat<String_t<C>>::template concat<typename Empty_t<N>::type>;
};

template<int N, typename Tower>
using Header_t = Row_base_t<N,Tower::name>;

template<int N>
using Row_empty_t = Row_base_t<N,'|'>;

template<typename Tower,int N,int M>
struct Row_disc_t
{
  static_assert(M <= Tower::height, "wrong disc index");
  static constexpr int disc_size = TNth<Tower::height-M-1,Tower>::value;
  static_assert(disc_size < N, "too large disc");
  using left = typename Empty_t<N-disc_size-1>::type::template concat<String_t<'['>>::template concat<typename Empty_t<disc_size>::type>;
  using right = typename Empty_t<disc_size>::type::template concat<String_t<']'>>::template concat<typename Empty_t<N-disc_size-1>::type>;
  using type = typename left::template concat<String_t<'|'>>::template concat<right>;
};

template<typename Tower,int N>
struct Row_disc_t<Tower,N,-1>
{
  using type = void;
};

template<int N, int M, typename Tower>
struct Row_t
{
  using type = std::conditional_t<(Tower::height-M <= 0),typename Row_empty_t<N>::type, typename Row_disc_t<Tower,N,(Tower::height-M <= 0 ? -1 : M)>::type>;
};

template<typename T1, typename T2, typename T3>
struct Merged_row_t
{
  using type = typename T1::type::template concat<typename T2::type>::template concat<typename T3::type>;
};

template<typename Row, typename... Rows>
struct Table_t
{
  template<char Div>
  using join = typename Row::type::template concat<String_t<Div>>::template concat<typename Table_t<Rows...>::template join<Div>>;
};

template<typename Row>
struct Table_t<Row>
{
  template<char Div>
  using join = typename Row::type::template concat<String_t<Div>>;
};

template<int N, typename State>
struct Ascii_t
{
  template<int... Ns>
  static constexpr auto rows_f(std::integer_sequence<int,Ns...>) -> Table_t<
    Merged_row_t<Header_t<N,typename State::First>,Header_t<N,typename State::Second>,Header_t<N,typename State::Third>>,
    Merged_row_t<
      Row_t<N,N-1-Ns,typename State::First>,
      Row_t<N,N-1-Ns,typename State::Second>,
      Row_t<N,N-1-Ns,typename State::Third>
    > ...
    ,Footer_t<6*N+3>
  >;
  using type = typename decltype(rows_f(std::declval<std::make_integer_sequence<int,N>>()))::template join<'\n'>;
};

template<int N>
struct Int2String_t
{
  static_assert(N>0,"only non negative ints");
  using type = typename Int2String_t<N/10>::type::template concat<String_t<(N%10)+'0'>>;
};

template<>struct Int2String_t<0>{using type = String_t<'0'>;};
template<>struct Int2String_t<1>{using type = String_t<'1'>;};
template<>struct Int2String_t<2>{using type = String_t<'2'>;};
template<>struct Int2String_t<3>{using type = String_t<'3'>;};
template<>struct Int2String_t<4>{using type = String_t<'4'>;};
template<>struct Int2String_t<5>{using type = String_t<'5'>;};
template<>struct Int2String_t<6>{using type = String_t<'6'>;};
template<>struct Int2String_t<7>{using type = String_t<'7'>;};
template<>struct Int2String_t<8>{using type = String_t<'8'>;};
template<>struct Int2String_t<9>{using type = String_t<'9'>;};

template<typename Move>
struct Move_String_t
{
  using type = typename String_t<'m', 'o', 'v', 'e', ' ', 'd', 'i', 's', 'c', ' '>::template concat<
    typename Int2String_t<Move::value>::type::template concat<
      String_t< ' ', 'f', 'r', 'o', 'm', ' ', '\'', Move::from, '\'', ' ', 't', 'o', ' ', '\'', Move::to, '\'', '\n'>
    >
  >;
};

