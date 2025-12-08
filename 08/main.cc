#include "input.hh"
#include "sample_input.hh"
#include <cstddef>
#include <iostream>
#include <span>
// #include "util/zip.hh"
// #include <numeric>
// #include <vector>

// squared distance, no need to waste cycles on sqrt()
constexpr long distsq(Pos a, Pos b) {
	long dx = (a.x - b.x);
	long dy = (a.y - b.y);
	long dz = (a.z - b.z);
	return dx * dx + dy * dy + dz * dz;
}

template<size_t N>
using Matrix = std::array<long, N * N>;
// using Matrix = std::array<std::array<long, N>, N>;

template<size_t N>
Matrix<N> create_matrix(std::array<Pos, N> const &pos) {
	Matrix<N> matrix;

	for (auto x = 0u; auto a : pos) {
		for (auto y = 0u; auto b : pos) {
			if (x < y) {
				printf("[%d] %d %d %d -> [%d] %d %d %d = %ld\n", x, a.x, a.y, a.z, y, b.x, b.y, b.z, distsq(a, b));
				matrix[x + N * y] = distsq(a, b);
			} else
				matrix[x + N * y] = LONG_MAX;
			y++;
		}
		x++;
	}
	return matrix;
}

template<size_t N, size_t M>
std::array<std::pair<int, int>, M> find_shortest(Matrix<N> &matrix) {
	std::array<std::pair<int, int>, M> pairs;
	int pair_i = 0;

	for (int i = 0; i < N; i++) {
		auto closest = std::min_element(matrix.begin(), matrix.end());
		auto dist = *closest;
		auto p = std::distance(matrix.begin(), closest);
		printf("[%ld] and [%ld]: %ld\n", p % N, p / N, dist);
		pairs[pair_i++] = {p % N, p / N};
		*closest = LONG_MAX;
	}

	return pairs;
}

int main() {
	{
		auto matrix = create_matrix(sample_data);
		auto pairs = find_shortest<20, 10>(matrix);
		// TODO: use pairs to find 3 largest groups
	}

	{
		auto matrix = create_matrix(data);
		auto pairs = find_shortest<1000, 1000>(matrix);
	}
}
