#include "../common/fileparse.hh"
#include "input.hh"
#include "sample_input.hh"
#include "util/fixed_vector.hh"
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

// static_assert(parse_rows(get_ops<4>(sample_data_ops), sample_data) == 4277556);

// static_assert(parse_rows(get_ops<1000>(dataops), data) == 6891729672676);

///////////////////////
// Part 2:

long long parse(std::span<const std::string> lines) {
	auto width = lines[0].size();

	FixedVector<int, 8> group;

	long long sum = 0;

	char op = '\0';

	for (auto col = 0u; col <= width; col++) {
		int num = 0;
		if (col < width) {
			for (auto row = 0u; row < lines.size(); row++) {
				auto c = lines[row][col];
				if (isdigit(c))
					num = num * 10 + (c - '0');
				else if (c == '+' || c == '*')
					op = c;
			}
		}
		if (col == width || num == 0) {
			printf("Empty column: %d %d %d %c\n", group[0], group[1], group[2], op);
			// empty column: process last group
			if (op == '+') {
				auto res = std::accumulate(group.begin(), group.end(), 0ll);
				printf("=>%lld\n", res);
				sum += res;
			}
			if (op == '*') {
				auto res = std::accumulate(group.begin(), group.end(), 1ll, std::multiplies<long long>());
				printf("=>%lld\n", res);
				sum += res;
			}
			op = '\0';
			group.clear();
		} else {
			group.push_back(num);
		}
	}

	return sum;
}

int main() {
	long long cnt;

	auto sample_ops = get_ops<4>(sample_data_ops);
	std::cout << "Sample Part 1: " << parse_rows(sample_ops, sample_data) << "\n";

	auto ops = get_ops<1000>(dataops);
	std::cout << "Part 1: " << parse_rows(ops, data) << "\n";

	auto lines = parse_lines("sample_input.txt");
	std::cout << "Sample Part 2: " << parse(lines) << "\n";

	auto lines2 = parse_lines("input2.txt");
	std::cout << "Part 2: " << parse(lines2) << "\n";
}
