#include "input.hh"
#include "sample_input.hh"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <span>
#include <vector>

constexpr long area(Pos a, Pos b) {
	long dx = (a.x - b.x);
	long dy = (a.y - b.y);
	if (dx < 0)
		dx = -dx;
	if (dy < 0)
		dy = -dy;

	return (dx + 1) * (dy + 1);
}

static_assert(area({2, 5}, {11, 1}) == 50);
static_assert(area({7, 3}, {2, 3}) == 6);

template<size_t N>
using DistanceMatrix = std::array<long, N * N>;

long find_largest(std::span<const Pos> pos) {
	long largest = 0;

	for (auto x = 0u; auto a : pos) {
		for (auto y = 0u; auto b : pos) {
			if (x < y) {
				auto ar = area(a, b);
				if (ar > largest)
					largest = ar;
			}
			y++;
		}
		x++;
	}
	return largest;
}

int main() {
	{
		auto cnt = find_largest(sample_data);
		std::cout << cnt << "\n";
	}

	{
		auto cnt = find_largest(data);
		std::cout << cnt << "\n";
	}
}
