# Tower of Hanoi
_solved using C++ template metaprogramming_

## Problem

[Problem statement](https://en.wikipedia.org/wiki/Tower_of_Hanoi "Tower of Hanoi on wikipedia")

Goal: Move all discs from rod A to rod C.

## Solution

The algorithm uses a recursive approach:
```
ToH(N, A, B, C):
  if(N=0):
    return
  ToH(N-1, A, C, B)
  move(A,C)
  ToH(N-1, B, A, C)
```

Suppose `ToH(N, A, B, C)` solves the problem in N depth, by moving discs from A to C with the help of the B auxilary rod. If we want to move a disc at N depth, the problem can be reduced to solving the problem of N-1 depth using the auxilary rod as target first, moving the now topmost disc to the destination, and solving again for N-1 depth moving from the auxilary rod to the destination.

By this recursive statement of the solution the base case will be N=1 where only the starting rod top disc will be moved to the destination rod. This is equivalent in the above stated pseudocode base case, as N=0 it will do nothing, making N=1 only the one desired step.

## Templates

In C++ template parameters can be types that have variable amount of numbers as parameters. The Towers are defined by a name (only for validation and printing purpose) and variable amount of numbers as disc sizes. The solver expects a depth, and 3 storage types as template argumants. The solver will manipulate the types based on the recursive rules we established before. The solver will initiate the end result state type, and the compiler initiates the steps required by it. The Moves are concatenated along the way in a type.

At the end an incomplete type is initiated with the solution moves which will print the error in the console and produce no executable. This way the proof that the computation occurs at compile time and no runtime overhead can exist is proven.

The solution can easily be extended by a constexpr array of the moves in a different type which will hold the moves as data member values. These values can be initiated from the moves types for actual runtime usage, as the array is filled with the moves data in compile time. This part got removed for the brevity of the example.

## Building

The `make` command will do the work.

```bash
CXX=clang++ make height=7
```

The `height` optional parameter is used to change the initial height of the tower A. Default value is set in the makefile to 4.

The code works with C++17 standard, tested with clang++ version 15.0.7 and g++ version 12.2.1 as of writing this.

The compile time is not too bad, height=16 finishes around 8 seconds with both clang++ and g++ on my machine, which is not the latest CPU model.

### Example output

```
$ make
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -O3 -DTOWER_HEIGHT=4    toh_tmp.cpp   -o toh_tmp
toh_tmp.cpp:114:17: error: aggregate ‘Print<Moves_t<Move_t<'A', 'B', 0>, Move_t<'A', 'C', 1>, Move_t<'B', 'C', 0>, Move_t<'A', 'B', 2>, Move_t<'C', 'A', 0>, Move_t<'C', 'B', 1>, Move_t<'A', 'B', 0>, Move_t<'A', 'C', 3>, Move_t<'B', 'C', 0>, Move_t<'B', 'A', 1>, Move_t<'C', 'A', 0>, Move_t<'B', 'C', 2>, Move_t<'A', 'B', 0>, Move_t<'A', 'C', 1>, Move_t<'B', 'C', 0> > > as_error’ has incomplete type and cannot be defined
  114 | Print<solution> as_error;
      |                 ^~~~~~~~
make: *** [<builtin>: toh_tmp] Error 1
$
```

