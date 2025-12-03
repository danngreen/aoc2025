#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <numeric>

constexpr unsigned long pow10(unsigned x) {
	int r = 1;
	while (x--) {
		r *= 10;
	}
	return r;
}
static_assert(pow10(0) == 1);
static_assert(pow10(1) == 10);
static_assert(pow10(2) == 100);
static_assert(pow10(5) == 100000);

// Numbers that repeat are the product of a "repeater" and a "factor".
// The repeater is (10^N)+1: 11, 101, 1001, 10001, 100001,...
// The factor is in the range [10^(N-1), (10^N)-1]: [10,99], [100,999], [1000,99999],...
constexpr unsigned long repeater(unsigned idx) {
	return pow10(idx) + 1;
}

constexpr std::pair<unsigned long, unsigned long> factors(int idx) {
	return {pow10(idx - 1), pow10(idx) - 1};
}

constexpr std::pair<unsigned long, unsigned long> factor_range(int idx) {
	auto [l, h] = factors(idx);
	auto r = repeater(idx);
	return {l * r, h * r};
}

static_assert(repeater(1) == 11);
static_assert(factors(1) == std::pair{1, 9});
static_assert(factor_range(1) == std::pair{11, 99});

static_assert(repeater(2) == 101);
static_assert(factors(2) == std::pair{10, 99});
static_assert(factor_range(2) == std::pair{1010, 9999});

static_assert(repeater(3) == 1001);
static_assert(factors(3) == std::pair{100, 999});
static_assert(factor_range(3) == std::pair{100100, 999999});

constexpr int overlapping_range(unsigned long a, unsigned long b) {
	int idx = 1;
	while (idx < 19) {
		auto [l, h] = factors(idx);
		auto r = repeater(idx);
		auto min = l * r;
		auto max = h * r;
		if ((a >= min && a <= max) || (b >= min && b <= max)) {
			return idx;
		}
		idx++;
	}
	return -1;
}

static_assert(overlapping_range(11, 22) == 1);
static_assert(overlapping_range(95, 115) == 1);
static_assert(overlapping_range(998, 1012) == 2);
static_assert(overlapping_range(1188511880, 1188511890) == 5);

constexpr std::pair<unsigned long, unsigned long> overlapping_factor_range(unsigned long a, unsigned long b) {
	int idx = 1;
	while (idx < 15) {
		auto [l, h] = factors(idx);
		auto r = repeater(idx);
		unsigned long min = l * r;
		unsigned long max = h * r;
		if ((a >= min && a <= max) || (b >= min && b <= max)) {
			return {min, max};
		}
		idx++;
	}
	return {0, 0};
}

static_assert(overlapping_factor_range(11, 22) == std::pair{11, 99});
static_assert(overlapping_factor_range(95, 115) == std::pair{11, 99});
static_assert(overlapping_factor_range(998, 1012) == std::pair{1010ul, 9999ul});
static_assert(overlapping_factor_range(1188511880, 1188511890) == std::pair{1000010000, 9999999999});

constexpr int overlap(unsigned long ceil_a, unsigned long floor_b, unsigned long min, unsigned long max) {
	auto overlap_min = std::max(ceil_a, min);
	auto overlap_max = std::min(floor_b, max);

	// printf("overlap([%ld, %ld] [%ld, %ld]) = [%ld,%ld]\n", ceil_a, floor_b, min, max, overlap_min, overlap_max);

	// return sum of numbers from [o_min, o_max)
	auto sum = 0;
	for (auto x = overlap_min; x <= overlap_max; x++) {
		sum += x;
	}
	return sum;
}

static_assert(overlap(9, 10, 1, 9) == 9); // 9
static_assert(overlap(1, 2, 1, 9) == 3);  // 1+2

constexpr unsigned long sum_repeatnums(unsigned long a, unsigned long b) {
	//11, 22 => 1
	//998, 115 = > 2
	auto idx = overlapping_range(a, b);
	if (idx <= 0)
		return 0;

	auto [min, max] = factors(idx);
	auto rep = repeater(idx);
	double div = rep;
	unsigned long ceil_a = std::ceil(double(a) / div);
	unsigned long floor_b = std::floor(double(b) / div);

	printf("%ld,%ld => overlaps with [%ld, %ld] * %ld (%d)\n", a, b, min, max, repeater(idx), idx);
	auto sum = overlap(ceil_a, floor_b, min, max);
	printf("=> %lu\n", sum * rep);
	printf("\n");
	return sum * rep;
}

int main() {
	// Sample data:
	// int cnt = 0;
	// for (auto [a, b] : sample_data) {
	// 	cnt += sum_repeatnums(a, b);
	// }
	// std::cout << cnt << "\n";

	unsigned long cnt = 0;
	for (auto [a, b] : data) {
		cnt += sum_repeatnums(a, b);
	}
	//3203842663 is too low
	std::cout << cnt << "\n";
}
