#include "input.hh"
#include "parse.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

// Each button can only be pressed 0 times or 1 time
// So we can represent the buttons to press with a bitfield
// (one bit per button).
constexpr int solve(uint16_t sum, std::span<const uint16_t> nums) {
	const unsigned max_bit_pattern = (1 << nums.size());
	unsigned bit_pat = 0;
	unsigned best_score = nums.size();

	// Test each bit pattern
	while (bit_pat < max_bit_pattern) {
		auto testsum = 0;
		for (auto bit = 0; bit < nums.size(); bit++)
			testsum ^= (bit_pat & (1 << bit)) ? nums[bit] : 0;

		// If it's a solution, then see if it's the best solution
		if (testsum == sum) {
			best_score = std::min<unsigned>(best_score, std::popcount(bit_pat));
		}
		bit_pat++;
	}
	return best_score;
}

static_assert(solve(sample_machines[0].lights, sample_machines[0].buttons) == 2);
static_assert(solve(sample_machines[1].lights, sample_machines[1].buttons) == 3);
static_assert(solve(sample_machines[2].lights, sample_machines[2].buttons) == 2);

int main() {
	{
		auto sum = std::accumulate(sample_machines.begin(), sample_machines.end(), 0, [](auto sum, auto const &mach) {
			return sum + solve(mach.lights, mach.buttons);
		});
		std::cout << "Sample Part 1: " << sum << "\n";
	}

	{
		// Only wrote a parser for the lights... so just parse those and merge with the regex formatted buttons
		for (int i = 0; auto line : data) {
			machines[i].lights = parse_light(line);
			i++;
		}
		auto sum = std::accumulate(machines.begin(), machines.end(), 0, [](auto sum, auto const &mach) {
			return sum + solve(mach.lights, mach.buttons);
		});
		std::cout << "Part 1: " << sum << "\n";
	}
}
