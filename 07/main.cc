#include "input.hh"
#include "sample_input.hh"
#include "util/zip.hh"
#include <cstddef>
#include <iostream>
#include <numeric>
#include <span>
#include <vector>

using BeamT = unsigned long long;

void print_map(std::span<const std::string_view> splitters, std::vector<std::vector<BeamT>> &beams) {
	printf("\n");
	for (auto x = 0u; x < splitters[x].size(); x++) {
		for (auto y = 0u; y < splitters.size(); y++) {
			if (splitters[x][y] != '.')
				printf("%c", splitters[x][y]);
			else {
				auto c = beams[x][y];
				// Part 1 visualization:
				// printf("%c", c == 0 ? '.' : '|');
				// Part 2 visualization:
				printf("%c",
					   c == 0  ? '.' :
					   c <= 9  ? char(c) + '0' :
					   c <= 35 ? char(c) + 'A' - 10 :
					   c <= 41 ? char(c) + 'a' - 36 :
								 '|');
			}
		}
		printf("\n");
	}
}

// Brute-force line-by-line simulation:
// We create an empty table same size as spitter map, with each cell is # of beam paths
// Then we keep track of how many paths pass each point using that table.
constexpr std::pair<int, BeamT> simulate(std::span<const std::string_view> splitters) {
	int hits = 0;

	std::vector<std::vector<BeamT>> beams;
	std::vector<BeamT> tmp_row;
	tmp_row.resize(splitters[0].size());
	beams.resize(splitters.size(), tmp_row);

	for (auto row = 1u; row < beams.size(); row++) {
		auto &beam = beams[row];
		auto &b_prev = beams[row - 1];
		const auto &splits = splitters[row];
		const auto &splits_prev = splitters[row - 1];

		for (auto x = 0u; x < beam.size(); x++) {
			// Initial beam
			if (splits_prev[x] == 'S') {
				beam[x] = 1;
			}

			if (splits[x] == '^') {
				if (b_prev[x] > 0) {
					hits++;
					beam[x - 1] += b_prev[x];
					beam[x + 1] += b_prev[x];
				}
			} else {
				beam[x] += b_prev[x];
			}
		}
	}

	// print_map(splitters, beams);

	auto paths = std::accumulate(beams.back().begin(), beams.back().end(), BeamT{0});
	return {hits, paths};
}

static_assert(simulate(sample_data) == std::pair<int, unsigned long long>{21, 40});
static_assert(simulate(data) == std::pair<int, unsigned long long>{1640, 40999072541589});

int main() {
	{
		auto [hits, paths] = simulate(sample_data);
		std::cout << "Sample Part 1: Beam Splits: " << hits << "\n";
		std::cout << "Sample Part 2: Beam Paths: " << paths << "\n";
	}

	{
		auto [hits, paths] = simulate(data);
		std::cout << "Part 1: Beam Splits: " << hits << "\n";
		std::cout << "Part 2: Beam Paths: " << paths << "\n";
	}
}
