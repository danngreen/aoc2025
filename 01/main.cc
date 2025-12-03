#include "input.hh"
#include "test_input.hh"
#include <cstddef>
#include <iostream>
#include <numeric>

template<size_t N>
constexpr int count_accum_zeros(std::array<int, N> const &d) {
	int pos = 50;
	return std::accumulate(d.begin(), d.end(), 0, [&pos](int zeros, int turn) {
		pos += turn;

		while (pos < 100)
			pos += 100;

		while (pos >= 100)
			pos -= 100;

		if (pos == 0)
			zeros++;

		return zeros;
	});
}

template<size_t N>
constexpr int count_passing_zeros(std::array<int, N> const &d) {
	int pos = 50;
	return std::accumulate(d.begin(), d.end(), 0, [&pos](int zeros, int turn) {
		pos += turn;

		while (pos < 0) {
			zeros++;
			pos += 100;
		}

		while (pos >= 100) {
			zeros++;
			pos -= 100;
		}

		return zeros;
	});
}

int main() {
	// Sample data:
	// auto part1 = count_accum_zeros(sample_data);
	// auto part2 = count_passing_zeros(sample_data);

	static_assert(count_accum_zeros(sample_data) == 3);
	static_assert(count_passing_zeros(sample_data) == 6);

	auto part1 = count_accum_zeros(data);
	auto part2 = count_passing_zeros(data);

	std::cout << part1 << "\n";
	std::cout << part2 << "\n";
}
