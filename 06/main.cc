#include "input.hh"
#include "sample_input.hh"
#include <charconv>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

template<size_t N>
constexpr std::array<char, N> get_ops(std::string_view d) {
	std::array<char, N> ops;

	int i = 0;
	while (true) {
		if (auto op_pos = d.find_first_of("+*"); op_pos != d.npos) {
			ops[i++] = d[op_pos];
			d = d.substr(op_pos + 1);
		} else
			break;
	}

	return ops;
}

static_assert(get_ops<4>("  + *     +    *   \n") == std::array<char, 4>{'+', '*', '+', '*'});

constexpr long long parse_rows(std::span<const char> ops, std::span<const int> nums) {

	auto cols = ops.size();
	auto rows = nums.size() / cols;

	long long sum = 0;
	for (auto col = 0; col < cols; col++) {

		if (ops[col] == '+') {
			for (auto row = 0; row < rows; row++) {
				sum += nums[row * cols + col];
			}
		} else {
			long long prod = 1;
			for (auto row = 0; row < rows; row++) {
				prod *= nums[row * cols + col];
			}
			sum += prod;
		}
	}

	return sum;
}

static_assert(parse_rows(get_ops<4>(sample_data_ops), sample_data) == 4277556);

static_assert(parse_rows(get_ops<1000>(dataops), data) == 6891729672676);

int main() {
	long long cnt;

	auto sample_ops = get_ops<4>(sample_data_ops);
	std::cout << "Sample Part 1: " << parse_rows(sample_ops, sample_data) << "\n";

	auto ops = get_ops<1000>(dataops);
	std::cout << "Part 1: " << parse_rows(ops, data) << "\n";

	// std::cout << "Sample Part 2: " << extend_ranges(sample_data) << "\n";

	// std::cout << "Part 2: " << extend_ranges(data) << "\n";
}
