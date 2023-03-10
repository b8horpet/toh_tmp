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

template<int N, typename A, typename B, typename C>
struct ToH_t
{
private:
  using prev_step = ToH_t<N-1,A,C,B>;
  using a_before = typename prev_step::A_t;
  using b_before = typename prev_step::C_t;
  using c_before = typename prev_step::B_t;
  using a_after = typename a_before::pop;
  using b_after = b_before;
  using c_after = typename c_before::template push<a_before::top>;
  using next_step = ToH_t<N-1,b_after,a_after,c_after>;
public:
  using A_t = typename next_step::B_t;
  using B_t = typename next_step::A_t;
  using C_t = typename next_step::C_t;
  using moves = typename prev_step::moves::template concat<Moves_t<Move_t<a_after::name,c_after::name,a_before::top>>>::template concat<typename next_step::moves>;
};

template<typename A, typename B, typename C>
struct ToH_t<1,A,B,C>
{
  using A_t = typename A::pop;
  using B_t = B;
  using C_t = typename C::template push<A::top>;
  static_assert(C_t::height != 0, "just incremented");
  using moves = Moves_t<Move_t<A_t::name,C_t::name,A::top>>;
};

template<char Name, int... ds>
struct Tower_t
{
  static constexpr char name = Name;
  template<int top, int... us>
  struct pop_h
  {
    using type = Tower_t<Name, us...>;
    static constexpr int value = top;
  };
  template<int top>
  using push = Tower_t<Name, top, ds...>;
  using pop = typename pop_h<ds...>::type;
  static constexpr int top = pop_h<ds...>::value;
  static constexpr bool validate()
  {
    if constexpr (sizeof...(ds) > 1)
    {
      return top < pop::top;
    }
    return true;
  }
  static_assert(validate(), "larger on top of smaller");
  static constexpr int height = sizeof...(ds);
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
  using A = decltype(create_start<'A'>(std::make_integer_sequence<int,N>{}));
  static_assert(A::height == N, "heights should match");
  using B = Tower_t<'B'>;
  using C = Tower_t<'C'>;
  using solution = ToH_t<A::height, A, B, C>;
  using A_t = typename solution::A_t;
  using B_t = typename solution::B_t;
  using C_t = typename solution::C_t;
  static_assert(C_t::height == A::height, "heights should match");
  static_assert(B_t::height == 0, "should be empty");
  static_assert(A_t::height == 0, "should be empty");
  using moves = typename solution::moves;
};

#if !defined(TOWER_HEIGHT)
#error "tower height should be defined"
#endif
constexpr int tower_height = TOWER_HEIGHT;
#undef TOWER_HEIGHT

using solution = typename ToH_start<tower_height>::moves;

template<typename>
class Print;
Print<solution> as_error;
