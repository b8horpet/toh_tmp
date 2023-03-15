#pragma once
#include <type_traits>
#include <utility>

template<char From, char To, int Value>
struct Move_t
{
  static constexpr char from = From;
  static constexpr char to = To;
  static constexpr int value = Value;
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
  using moves = typename prev_step::moves::template concat<Moves_t<Move_t<from_after::name,to_after::name,from_before::top>>>::template concat<typename next_step::moves>;
};

template<typename From, typename Aux, typename To>
struct ToH_t<1,From,Aux,To>
{
  using From_t = typename From::pop;
  using Aux_t = Aux;
  using To_t = typename To::template push<From::top>;
  static_assert(To_t::height != 0, "just incremented");
  using moves = Moves_t<Move_t<From_t::name,To_t::name,From::top>>;
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

