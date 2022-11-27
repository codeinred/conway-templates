#include "snapshot.h"
#include <array>
#include <cstddef>
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


///////////////////////////////////////////////////////////
////  Snapshot Reader (takes snapshots of Game State)  ////
///////////////////////////////////////////////////////////

template <int... I>
struct Ints {};


template <class, class>
struct IntSeqHelper;

template <int... I, int... J>
struct IntSeqHelper<Ints<I...>, Ints<J...>> {
    constexpr static int Off = sizeof...(I);
    using type = Ints<I..., Off + J...>;
};


template <int Len>
struct IntSeq {
    using type = typename IntSeqHelper<
        typename IntSeq<Len / 2>::type,
        typename IntSeq<Len - Len / 2>::type>::type;
};
// clang-format off
template <> struct IntSeq<0> { using type = Ints<>; };
template <> struct IntSeq<1> { using type = Ints<0>; };
template <> struct IntSeq<2> { using type = Ints<0, 1>; };
template <> struct IntSeq<3> { using type = Ints<0, 1, 2>; };
template <> struct IntSeq<4> { using type = Ints<0, 1, 2, 3>; };
// clang-format on
template <int N>
using int_seq = typename IntSeq<N>::type;


template <int T, int X, int Y, int... I>
constexpr std::array<bool, sizeof...(I)> _take_snapshot_row(Ints<I...>) {
    return {bool(Cell<T, X + I, Y>::value)...};
}

template <int T, int X, int Y, int... I, int... J>
constexpr Snapshot<sizeof...(I), sizeof...(J)> _take_snapshot(
    Ints<I...> ii,
    Ints<J...>) {
    return {_take_snapshot_row<T, X, Y + J>(ii)...};
}

template <int T, int X, int Y, int Width, int Height>
constexpr Snapshot<Width, Height> take_snapshot() {
    return _take_snapshot<T, X, Y>(int_seq<Width> {}, int_seq<Height> {});
}

template <int X, int Y, int Width, int Height, int... T>
constexpr Snapshots<sizeof...(T), Width, Height> snapshot_sequence(Ints<T...>) {
    return {take_snapshot<T, X, Y, Width, Height>()...};
}


auto get_snapshot() -> Snapshots<NumSnapshots, 60, 30> const& {
    constexpr static auto result = snapshot_sequence<-5, -5, 60, 30>(
        int_seq<NumSnapshots> {});

    return result;
}
