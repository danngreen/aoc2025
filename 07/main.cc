#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <span>

void print_map(std::span<std::string> map) {
	for (auto &line : map) {
		printf("%s\n", line.data());
	}
	printf("\n");
}

// Brute-force line-by-line simulation:
int simulate(std::span<std::string> map) {
	int hits = 0;

	for (auto row = 1u; auto &line : map.subspan(1)) {
		auto &prev = map[row - 1];

		for (auto pos = 0u; pos < line.size(); pos++) {
			if (prev[pos] == 'S') {
				line[pos] = '|';
			}

			if (line[pos] == '^') {
				if (prev[pos] == '|') {
					hits++;
					line[pos - 1] = '|';
					line[pos + 1] = '|';
				}
			}

			if (line[pos] == '.') {
				if (prev[pos] == '|')
					line[pos] = '|';
			}
		}
		row++;
	}

	return hits;
}

// Another approach:
// Count the number of beamsplitters.
// Subtract the number that are missed (not hit).
// Hit == there's a beam splitter in (x+1, y-N) or (x-1, y-N)
// where N is 0..Y of the beamsplitter above
// Stated otherwise:
// For each beam splitter:
// scan x-1, x, x+1 for each row y-n (n = 1..y)
// if splitter in (x,y-n), then MISS
// if splitter in (x-1,y-n) or (x+1,y-n) then HIT (assuming that beam spitter was hit)

int main() {
	std::cout << "Sample Part 1: " << simulate(sample_data) << "\n";

	std::cout << "Part 1: " << simulate(data) << "\n";
}
