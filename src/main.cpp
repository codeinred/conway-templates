#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <utility>

/////////////////////////
////  Initial state  ////
/////////////////////////

template <int W, int H, char living = 'O'>
struct InitialState {
    char state[W * H + 1];

    constexpr int operator()(int i, int j) const {
        return i < 0 || i >= W || j < 0 || j >= H
                 ? 0
                 : int(state[i + j * W] == living);
    }
};

// Based on https://conwaylife.com/wiki/New_gun_1
constexpr InitialState<49, 14> initial_state {
    "..............................OO................."
    ".............................O.O...............OO"
    ".............................O.................OO"
    ".................O...........OOO................."
    "OO...............OO.............................."
    "OO................OO............................."
    ".............OO..OO.............................."
    ".............................OOO................."
    ".............................O.................OO"
    ".............................O.O...............OO"
    ".............OO..OO...........OO................."
    "OO................OO............................."
    "OO...............OO.............................."
    ".................O..............................."};


///////////////////////////////
////  Game Of Life Engine  ////
///////////////////////////////

constexpr int next_state(int previous_state, int previous_living_neighbors) {
    return previous_living_neighbors == 2 //
             ? previous_state
             : previous_living_neighbors == 3 ? 1 : 0;
}

template <int T, int X, int Y>
struct Cell {
    constexpr static int previous_living_neighbors() {
        return Cell<T - 1, X - 1, Y - 1>::value //
             + Cell<T - 1, X - 1, Y>::value     //
             + Cell<T - 1, X - 1, Y + 1>::value //
             + Cell<T - 1, X, Y - 1>::value     //
             + Cell<T - 1, X, Y + 1>::value     //
             + Cell<T - 1, X + 1, Y - 1>::value //
             + Cell<T - 1, X + 1, Y>::value     //
             + Cell<T - 1, X + 1, Y + 1>::value;
    }
    constexpr static int previous_state() { return Cell<T - 1, X, Y>::value; }

    constexpr static int value = next_state(
        previous_state(),
        previous_living_neighbors());
};

template <int X, int Y>
struct Cell<0, X, Y> {
    constexpr static int value = initial_state(X, Y);
};

int main() {}
