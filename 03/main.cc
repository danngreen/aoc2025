#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <numeric>

int max_pair_digits(std::string_view s) {

	auto digit1 = std::max_element(s.begin(), s.end() - 1);
	auto digit2 = std::max_element(digit1 + 1, s.end());

	auto num1 = *digit1 - '0';
	auto num2 = *digit2 - '0';
	// printf("%c%c\n", *digit1, *digit2);
	return num1 * 10 + num2;
}

int main() {
	unsigned long cnt;

	cnt = std::accumulate(
		sample_data.begin(), sample_data.end(), 0ul, [](auto sum, auto val) { return sum + max_pair_digits(val); });

	std::cout << "Sample Part 1: " << cnt << "\n";

	cnt = std::accumulate(data.begin(), data.end(), 0ul, [](auto sum, auto val) { return sum + max_pair_digits(val); });

	std::cout << "Part 1: " << cnt << "\n";
}
