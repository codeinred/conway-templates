#include <array>
#include <cstdio>

template <size_t W, size_t H>
using Snapshot = std::array<std::array<bool, W>, H>;

template <size_t T, size_t W, size_t H>
using Snapshots = std::array<Snapshot<W, H>, T>;


constexpr size_t NumSnapshots = 92;
auto get_snapshot() -> Snapshots<NumSnapshots, 60, 30> const&;
