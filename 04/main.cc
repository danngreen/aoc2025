#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <span>
#include <vector>

constexpr bool filled(char c) {
	return c != '.';
}

constexpr unsigned long count_obstacles(std::string_view prev, std::string_view line, std::string_view next, int x) {
	int obs = 0;

	if (x > 0)
		obs += filled(line[x - 1]);
	if (x < line.size() - 1)
		obs += filled(line[x + 1]);

	if (!prev.empty()) {
		obs += filled(prev[x]);
		if (x > 0)
			obs += filled(prev[x - 1]);
		if (x < line.size() - 1)
			obs += filled(prev[x + 1]);
	}

	if (!next.empty()) {
		obs += filled(next[x]);
		if (x > 0)
			obs += filled(next[x - 1]);
		if (x < line.size() - 1)
			obs += filled(next[x + 1]);
	}

	return obs;
}

static_assert(count_obstacles("", "..@@.@@@@.", "@@@.@.@.@@", 2) == 3);
static_assert(count_obstacles("", "..@@.@@@@.", "@@@.@.@.@@", 3) == 3);
static_assert(count_obstacles("", "..@@.@@@@.", "@@@.@.@.@@", 7) == 4);
static_assert(count_obstacles("..@@.@@@@.", "@@@.@.@.@@", "@@@@@.x.@@", 0) == 3);
static_assert(count_obstacles("..@@.@@@@.", "@@@.@.@.@@", "@@@@@.x.@@", 1) == 6);
static_assert(count_obstacles(".@@@@@@@@.", "@.@.@@@.@.", "", 0) == 1);
static_assert(count_obstacles(".@@@@@@@@.", "@.@.@@@.@.", "", 2) == 3);

unsigned long count_accessible(std::vector<std::string> &map, bool do_remove) {
	unsigned long free_rolls = 0;

	for (unsigned y = 0; auto &line : map) {
		for (unsigned x = 0; auto c : line) {
			if (filled(c)) {
				std::string_view prev = y > 0 ? std::string_view(map[y - 1]) : "";
				std::string_view next = y < map.size() - 1 ? std::string_view(map[y + 1]) : "";
				if (count_obstacles(prev, line, next, x) < 4) {
					free_rolls++;
					if (do_remove)
						line[x] = '.';
				}
			}
			x++;
		}
		y++;
	}
	return free_rolls;
}

unsigned long recursive_remove(std::vector<std::string> &map) {
	unsigned long total = 0;
	while (true) {
		auto rolls = count_accessible(map, true);
		if (rolls == 0)
			break;
		total += rolls;
	}
	return total;
}

int main() {
	auto copy_map = [](auto const &map) -> std::vector<std::string> {
		std::vector<std::string> m;
		for (auto &line : map) {
			m.push_back(std::string(line));
		}
		return m;
	};

	auto sample2 = copy_map(sample_data);
	auto data2 = copy_map(data);

	unsigned long cnt;

	std::cout << "Sample Part 1: " << count_accessible(sample2, false) << "\n";

	std::cout << "Part 1: " << count_accessible(data2, false) << "\n";

	std::cout << "Sample Part 2: " << recursive_remove(sample2) << "\n";

	std::cout << "Part 2: " << recursive_remove(data2) << "\n";
}
