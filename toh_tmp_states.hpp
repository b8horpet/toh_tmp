#pragma once
#include <type_traits>
#include <utility>

template<int N, typename T, typename... Ts>
struct Nth_helper
{
  static_assert(N<=sizeof...(Ts), "cannot take nth");
  using type = typename Nth_helper<N-1, Ts...>::type;
};

template<typename T, typename... Ts>
struct Nth_helper<0,T,Ts...>
{
  using type = T;
};

template<int N, typename T>
struct Nth
{
  template<template<typename...>typename U, typename S, typename... Us>
  static constexpr auto nth_f(U<S,Us...>) -> typename Nth_helper<N,S,Us...>::type;
  using type = decltype(nth_f(std::declval<T>()));
};

struct End {};

template<typename Rod, typename Rods>
struct Ordered_t
{
  static_assert(Rod::name <= Rods::min::name, "wrong order");
  using min = Rod;
  using rest = Rods;
  template<typename R>
  using insert = std::conditional_t<(R::name < Rod::name), Ordered_t<R,Ordered_t<Rod,Rods>>, Ordered_t<Rod, typename Rods::template insert<R>>>;
};

template<typename R>
struct Ordered_t<R, End>
{
  using min = R;
  template<typename Other>
  using insert = std::conditional_t<(Other::name < R::name), Ordered_t<Other,Ordered_t<R,End>>, Ordered_t<R,Ordered_t<Other,End>>>;
};

template<typename Rod1, typename Rod2, typename Rod3>
struct State_t
{
  using order  = typename Ordered_t<Rod1,End>::template insert<Rod2>::template insert<Rod3>;
  using First  = typename order::min;
  using Second = typename order::rest::min;
  using Third  = typename order::rest::rest::min;
};

template<char From, char To, int Value, typename S1, typename S2>
struct Move_t
{
  static constexpr char from = From;
  static constexpr char to = To;
  static constexpr int value = Value;
  using start_t = S1;
  using end_t = S2;
};

template<typename... moves>
struct Moves_t
{
  template<typename... ms>
  constexpr static auto concat_f(Moves_t<ms...>) -> Moves_t<moves..., ms...>;
  template<typename U>
  using concat = decltype(concat_f(U{}));
};

template<int N, typename From, typename Aux, typename To>
struct ToH_t
{
private:
  using prev_step = ToH_t<N-1,From,To,Aux>;
  using from_before = typename prev_step::From_t;
  using aux_before = typename prev_step::To_t;
  using to_before = typename prev_step::Aux_t;
  using from_after = typename from_before::pop;
  using aux_after = aux_before;
  using to_after = typename to_before::template push<from_before::top>;
  using next_step = ToH_t<N-1,aux_after,from_after,to_after>;
public:
  using From_t = typename next_step::Aux_t;
  using Aux_t = typename next_step::From_t;
  using To_t = typename next_step::To_t;
  using moves = typename prev_step::moves::template concat<Moves_t<
    Move_t<from_after::name,to_after::name,from_before::top,
      State_t<from_before,aux_before,to_before>,State_t<from_after,aux_after,to_after>
    >
  >>::template concat<typename next_step::moves>;
};

template<typename From, typename Aux, typename To>
struct ToH_t<1,From,Aux,To>
{
  using From_t = typename From::pop;
  using Aux_t = Aux;
  using To_t = typename To::template push<From::top>;
  static_assert(To_t::height != 0, "just incremented");
  using moves = Moves_t<Move_t<From_t::name,To_t::name,From::top,State_t<From,Aux,To>,State_t<From_t,Aux_t,To_t>>>;
};

template<char Name, int... discs>
struct Tower_t
{
  static constexpr char name = Name;
  template<int top, int... other_discs>
  struct pop_h
  {
    using type = Tower_t<Name, other_discs...>;
    static constexpr int value = top;
  };
  template<int top>
  using push = Tower_t<Name, top, discs...>;
  using pop = typename pop_h<discs...>::type;
  static constexpr int top = pop_h<discs...>::value;
  static constexpr bool validate()
  {
    if constexpr (sizeof...(discs) > 1)
    {
      return top < pop::top;
    }
    return true;
  }
  static_assert(validate(), "larger on top of smaller");
  static constexpr int height = sizeof...(discs);
};

template<char Name>
struct Tower_t<Name>
{
  static constexpr char name = Name;
  template<int top>
  using push = Tower_t<Name, top>;
  static constexpr int height = 0;
};

template<int N, typename Tower>
struct TNth
{
  static constexpr int value = TNth<N-1, typename Tower::pop>::value;
};

template<typename Tower>
struct TNth<0, Tower>
{
  static constexpr int value = Tower::top;
};

template<int N>
struct ToH_start
{
  template<char Name, int... ns>
  static constexpr Tower_t<Name, ns...> create_start(std::integer_sequence<int, ns...>);
  using A_before = decltype(create_start<'A'>(std::make_integer_sequence<int,N>{}));
  static_assert(A_before::height == N, "heights should match");
  using B_before = Tower_t<'B'>;
  using C_before = Tower_t<'C'>;
  using solution = ToH_t<A_before::height, A_before, B_before, C_before>;
  using A_after = typename solution::From_t;
  using B_after = typename solution::Aux_t;
  using C_after = typename solution::To_t;
  static_assert(C_after::height == A_before::height, "heights should match");
  static_assert(B_after::height == 0, "should be empty");
  static_assert(A_after::height == 0, "should be empty");
  using moves = typename solution::moves;
};

