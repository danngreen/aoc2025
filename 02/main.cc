#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <numeric>

struct Form {
	int num_digits;		//2 => 10-99, 3 => 100-999, etc
	unsigned long long base; //11, 101, 1001, 10101, etc
};

// All formulas to make a repeating digit number with max 10 digits
constexpr std::array<Form, 17> forms{{
	{1, 11},
	{1, 111},
	{1, 1111},
	{1, 11111},
	{1, 111111},
	{1, 1111111},
	{1, 11111111},
	{1, 111111111},
	{1, 1111111111},
	{2, 101},
	{2, 10101},
	{2, 1010101},
	{2, 101010101},
	{3, 1001},
	{3, 1001001},
	{4, 10001},
	{5, 100001},
}};

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

static_assert(repeater(1) == 11);
static_assert(factors(1) == std::pair{1, 9});

static_assert(repeater(2) == 101);
static_assert(factors(2) == std::pair{10, 99});

static_assert(repeater(3) == 1001);
static_assert(factors(3) == std::pair{100, 999});

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

constexpr unsigned long long
overlap(unsigned long ceil_a, unsigned long floor_b, unsigned long min, unsigned long max, unsigned long rep) {
	auto overlap_min = std::max(ceil_a, min);
	auto overlap_max = std::min(floor_b, max);

	// return sum of numbers from [o_min, o_max)
	unsigned long sum = 0;
	for (auto x = overlap_min; x <= overlap_max; x++) {
		sum += x * rep;
	}
	return sum;
}

static_assert(overlap(9, 10, 1, 9, 11) == 99);
static_assert(overlap(1, 2, 1, 9, 11) == 33);

constexpr unsigned long long sum_repeatnums(unsigned long long a, unsigned long long b) {
	auto idx = overlapping_range(a, b);
	if (idx <= 0) {
		return 0;
	}

	auto [min, max] = factors(idx);
	auto rep = repeater(idx);
	double div = rep;
	unsigned long ceil_a = std::ceil(double(a) / div);
	unsigned long floor_b = std::floor(double(b) / div);

	auto sum = overlap(ceil_a, floor_b, min, max, rep);
	return sum;
}

int main() {
	// Sample data:
	unsigned long long cnt = 0;
	for (auto [a, b] : sample_data) {
		cnt += sum_repeatnums(a, b);
	}
	std::cout << "Sample: " << cnt << "\n";

	cnt = 0;
	for (auto [a, b] : data) {
		cnt += sum_repeatnums(a, b);
	}

	std::cout << "Part 1: " << cnt << "\n";
}
