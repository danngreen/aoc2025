#include "input.hh"
#include "sample_input.hh"
#include <charconv>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

constexpr std::pair<long long, long long> make_range(std::string_view r) {
	auto dash = r.find_first_of("-");
	if (dash == r.npos)
		return {0, 0};

	long long min = 0;
	std::from_chars(r.data(), r.data() + dash, min);

	long long max = 0;
	std::from_chars(r.data() + dash + 1, r.end(), max);
	return {min, max};
}

static_assert(make_range("12-34").first == 12);
static_assert(make_range("12-34").second == 34);
static_assert(make_range("1234").second == 0);

constexpr bool num_in_any_range(long long num, std::span<std::pair<long long, long long>> ranges) {
	for (auto &range : ranges) {
		if (num >= range.first && num <= range.second)
			return true;
	}
	return false;
}

constexpr unsigned count(const std::span<const std::string_view> lines) {
	unsigned cnt = 0;
	std::vector<std::pair<long long, long long>> ranges;

	for (auto &line : lines) {
		if (auto [min, max] = make_range(line); min > 0) {
			ranges.push_back({min, max});
		} else {
			long long num = 0;
			auto res = std::from_chars(line.begin(), line.end(), num);
			if (res.ptr == line.end()) {
				if (num_in_any_range(num, ranges)) {
					cnt++;
				}
			}
		}
	}
	return cnt;
}

// Sample Part 1
static_assert(count(sample_data) == 3);

// Part 1:
static_assert(count(data) == 701);

////////// Part 2:

constexpr bool overlaps(long long min1, long long max1, long long min2, long long max2) {
	// min1 is in range [min2, max2]
	if (min1 >= min2 && min1 <= max2)
		return true;

	// max1 is in range [min2, max2]
	if (max1 >= min2 && max1 <= max2)
		return true;

	// [min1,max1] encompasses range [min2, max2]
	if (min1 < min2 && max1 > max2)
		return true;

	// [min1,max1] is encompassed by range [min2, max2]
	if (min1 > min2 && max1 < max2)
		return true;

	return false;
}

static_assert(overlaps(10, 20, 10, 20)); //same
static_assert(overlaps(11, 20, 10, 20)); //min1 in
static_assert(overlaps(11, 21, 10, 20)); //min1 in, max2 in
static_assert(overlaps(11, 19, 10, 20)); //enc
static_assert(overlaps(10, 19, 10, 20)); //max1 in
static_assert(overlaps(9, 19, 10, 20));	 //max1 in, min2 in
static_assert(overlaps(10, 20, 11, 20)); //min2 in
static_assert(overlaps(10, 20, 10, 19)); //max2 in
static_assert(overlaps(10, 20, 11, 19)); //enc
static_assert(overlaps(10, 20, 21, 30) == false);
static_assert(overlaps(10, 20, 20, 30)); //max1==min2
static_assert(overlaps(10, 20, 9, 10));	 //max2==min1

constexpr long extend_ranges(const std::span<const std::string_view> lines) {
	std::vector<std::pair<long long, long long>> ranges;

	//parse ranges
	for (auto &line : lines) {
		if (auto [min, max] = make_range(line); min > 0) {

			for (auto &r : ranges) {
				if (overlaps(min, max, r.first, r.second)) {
					min = std::min(r.first, min);
					max = std::max(r.second, max);
					// clear old range we just extended
					r.first = 0;
					r.second = 0;
				}
			}

			ranges.push_back({min, max});

		} else
			break; //stop at first non-range line
	}

	// count
	return std::accumulate(ranges.begin(), ranges.end(), 0ll, [](auto sum, auto r) {
		if (r.second == 0)
			return sum;
		else
			return sum + (r.second - r.first + 1);
	});
}

// Sample Part 2:
static_assert(extend_ranges(sample_data) == 14);

// Part 2:
static_assert(extend_ranges(data) == 352340558684863);

int main() {
	unsigned long cnt;

	std::cout << "Sample Part 1: " << count(sample_data) << "\n";

	std::cout << "Part 1: " << count(data) << "\n";

	std::cout << "Sample Part 2: " << extend_ranges(sample_data) << "\n";

	std::cout << "Part 2: " << extend_ranges(data) << "\n";
}
