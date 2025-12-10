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

// TODO: optmize the size of this?
template<size_t N>
using DistanceMatrix = std::array<long, N * N>;

template<size_t N>
constexpr DistanceMatrix<N> create_matrix(std::array<Pos, N> const &pos) {
	DistanceMatrix<N> distances;

	for (auto x = 0u; auto a : pos) {
		for (auto y = 0u; auto b : pos) {
			long idx = x + N * y;
			if (x < y) {
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
		const auto [a, b] = get_closest<N>(distances);
		const auto group_a = groups[a];
		const auto group_b = groups[b];
		bool one_group = true;
		for (int j = 0; auto &g : groups) {
			// move all boxes that are in b's groups, into a's group
			if (g == group_b && g != group_a) {
				// printf("Move [%d] from group %d to %d\n", j, group_b, group_a);
				g = group_a;
			}
			if (g != groups[0])
				one_group = false;
			j++;
		}

		// print_groups(groups);

		if (one_group) {
			printf("One group formed by connecting [%d] and [%d]\n", a, b);
			// hack: just return the indices
			groups[0] = a;
			groups[1] = b;
			return groups;
			break;
		}
	}
	return groups;
}

template<size_t N>
constexpr std::array<int, N> count_group_sizes(std::span<const int> groups) {
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
		const auto groups = group_boxes<20>(10, matrix);
		const auto sizes = count_group_sizes<20>(groups);
		std::cout << "Sample part 1: " << sizes[0] << " * " << sizes[1] << " * " << sizes[2] << " = "
				  << (sizes[0] * sizes[1] * sizes[2]) << "\n";
	}
	{
		auto matrix = create_matrix(sample_data);
		const auto groups = group_boxes<20>(20 * 20 + 1, matrix);
		auto a = groups[0];
		auto b = groups[1];
		auto a_x = sample_data[a].x;
		auto b_x = sample_data[b].x;
		printf("Sample part 2: [%d].x = %d, [%d].x = %d, prod = [%d]\n", a, a_x, b, b_x, a_x * b_x);
	}
	{
		auto matrix = create_matrix(data);
		auto groups = group_boxes<1000>(1000, matrix);
		auto sizes = count_group_sizes<1000>(groups);
		std::cout << "Part 1: " << sizes[0] << " * " << sizes[1] << " * " << sizes[2] << " = "
				  << (sizes[0] * sizes[1] * sizes[2]) << "\n";
	}
	{
		auto matrix = create_matrix(data);
		const auto groups = group_boxes<1000>(1000 * 1000 + 1, matrix);
		auto a = groups[0];
		auto b = groups[1];
		auto a_x = data[a].x;
		auto b_x = data[b].x;
		printf("Part 2: [%d].x = %d, [%d].x = %d, prod = [%d]\n", a, a_x, b, b_x, a_x * b_x);
	}
}
