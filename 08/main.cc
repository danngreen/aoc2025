#include "input.hh"
#include "sample_input.hh"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <span>
#include <vector>

// squared distance, no need to waste cycles on sqrt()
constexpr long distsq(Pos a, Pos b) {
	long dx = (a.x - b.x);
	long dy = (a.y - b.y);
	long dz = (a.z - b.z);
	return dx * dx + dy * dy + dz * dz;
}

template<size_t N>
using DistanceMatrix = std::array<long, N * N>;

template<size_t N>
constexpr DistanceMatrix<N> create_matrix(std::array<Pos, N> const &pos) {
	DistanceMatrix<N> distances;

	for (auto x = 0u; auto a : pos) {
		for (auto y = 0u; auto b : pos) {
			long idx = x + N * y;
			if (x < y) {
				// printf("[%d] %d %d %d -> [%d] %d %d %d = [%ld] [%ld %ld] %ld\n",
				// 	   x,
				// 	   a.x,
				// 	   a.y,
				// 	   a.z,
				// 	   y,
				// 	   b.x,
				// 	   b.y,
				// 	   b.z,
				// 	   idx,
				// 	   idx % N,
				// 	   idx / N,
				// 	   distsq(a, b));
				distances[idx] = distsq(a, b);
			} else
				distances[idx] = LONG_MAX;
			y++;
		}
		x++;
	}

	return distances;
}

void print_groups(std::span<int> groups) {
	for (int i = 0; auto g : groups) {
		printf("[%d] in group %d\n", i, g);
		i++;
	}
}

template<size_t N>
constexpr std::pair<int, int> get_closest(DistanceMatrix<N> &distances) {
	auto closest = std::min_element(distances.begin(), distances.end());
	auto dist = *closest;
	auto p = std::distance(distances.begin(), closest);
	*closest = LONG_MAX;
	// printf("between [%ld] and [%ld] = distance %ld\n", p % N, p / N, dist);
	int b = p % N;
	int a = p / N;
	return {a, b};
}

template<size_t N>
constexpr std::array<int, N> group_boxes(int num_wires, DistanceMatrix<N> &distances) {
	std::array<int, N> groups;

	for (int i = 0; auto &g : groups) {
		g = i++;
	}
	// print_groups(groups);

	for (int i = 0; i < num_wires; i++) {
		// printf("\nWire %d\n", i);
		auto [a, b] = get_closest<N>(distances);
		auto group_a = groups[a];
		auto group_b = groups[b];
		for (int j = 0; auto &g : groups) {
			// move all boxes that are in b's groups, into a's group
			if (g == group_b && g != group_a) {
				// printf("Move [%d] from group %d to %d\n", j, group_b, group_a);
				g = group_a;
			}
			j++;
		}
		// print_groups(groups);
	}
	return groups;
}

template<size_t N>
constexpr std::array<int, N> count_group_sizes(std::span<int> groups) {
	std::array<int, N> sizes{};
	for (auto g : groups) {
		sizes[g]++;
	}
	std::sort(sizes.rbegin(), sizes.rend());
	return sizes;
}

int main() {
	{
		auto matrix = create_matrix(sample_data);
		auto groups = group_boxes<20>(10, matrix);
		auto sizes = count_group_sizes<20>(groups);
		std::cout << "Sample part 1: " << sizes[0] << " * " << sizes[1] << " * " << sizes[2] << " = "
				  << (sizes[0] * sizes[1] * sizes[2]) << "\n";
	}
	{
		auto matrix = create_matrix(data);
		auto groups = group_boxes<1000>(1000, matrix);
		auto sizes = count_group_sizes<1000>(groups);
		std::cout << "Part 1: " << sizes[0] << " * " << sizes[1] << " * " << sizes[2] << " = "
				  << (sizes[0] * sizes[1] * sizes[2]) << "\n";
	}
	{
		//18*17*16 = 4896 too low
	}
}
