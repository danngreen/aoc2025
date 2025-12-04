#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <numeric>

template<size_t N>
constexpr auto max_N_digits(std::string_view s) {

	unsigned long joltage = 0;
	auto digit = s.begin();
	for (auto n = 0u; n < N; n++) {
		digit = std::max_element(digit + (n != 0), s.end() - (N - n) + 1);
		auto num = *digit - '0';
		joltage = joltage * 10 + num;
	}

	return joltage;
}

// Sample data:
static_assert(max_N_digits<2>("98765432111111") == 98);
static_assert(max_N_digits<2>("811111111111119") == 89);
static_assert(max_N_digits<2>("234234234234278") == 78);
static_assert(max_N_digits<2>("818181911112111") == 92);
static_assert(max_N_digits<12>("98765432111111") == 987654321111);
static_assert(max_N_digits<12>("811111111111119") == 811111111119);
static_assert(max_N_digits<12>("234234234234278") == 434234234278);
static_assert(max_N_digits<12>("818181911112111") == 888911112111);

int main() {
	unsigned long cnt;
	cnt = std::accumulate(data.begin(), data.end(), 0ul, [](auto sum, auto val) { return sum + max_N_digits<2>(val); });

	std::cout << "Part 1: " << cnt << "\n";

	cnt =
		std::accumulate(data.begin(), data.end(), 0ul, [](auto sum, auto val) { return sum + max_N_digits<12>(val); });

	std::cout << "Part 2: " << cnt << "\n";
}
