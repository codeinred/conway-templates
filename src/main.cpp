#include <cstdio>
#include <thread>
#include "snapshot.h"

////////////////////////////
////  Rendering Engine  ////
////////////////////////////

template <size_t N>
void print_row(std::array<bool, N> const& arr) {
    char line[N + 1];
    for (int i = 0; i < N; i++) {
        line[i] = arr[i] ? 'X' : ' ';
    }
    line[N] = 0;
    puts(line);
}

template <size_t W, size_t H>
void print_ss(std::array<std::array<bool, W>, H> const& arr) {
    for (auto& elem : arr) {
        print_row(elem);
    }
}


int main() {
    auto const& result = get_snapshot();
    size_t i = 0;
    while(true) {
        auto& ss = result[i];
        i += 1;
        // This particular configuration repeats after 46 steps
        if (i == NumSnapshots) i -= 46;
        print_ss(ss);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}


