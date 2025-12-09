#include "input.hh"
#include "sample_input.hh"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <span>
#include <unordered_set>
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
// using Matrix = std::array<std::array<long, N>, N>;

template<size_t N>
DistanceMatrix<N> create_matrix(std::array<Pos, N> const &pos) {
	DistanceMatrix<N> distances;

	for (auto x = 0u; auto a : pos) {
		for (auto y = 0u; auto b : pos) {
			long idx = x + N * y;
			if (x < y) {
				printf("[%d] %d %d %d -> [%d] %d %d %d = [%ld] [%ld %ld] %ld\n",
					   x,
					   a.x,
					   a.y,
					   a.z,
					   y,
					   b.x,
					   b.y,
					   b.z,
					   idx,
					   idx % N,
					   idx / N,
					   distsq(a, b));
				distances[idx] = distsq(a, b);
			} else
				distances[idx] = LONG_MAX;
			y++;
		}
		x++;
	}
	return distances;
}

template<size_t N>
std::vector<std::set<int>> connect_n_wires(int n, DistanceMatrix<N> &distances) {
	std::vector<std::set<int>> circuits;

	int wires = 0;

	while (wires < n) {
		auto closest = std::min_element(distances.begin(), distances.end());
		auto dist = *closest;
		auto p = std::distance(distances.begin(), closest);
		*closest = LONG_MAX;
		printf("%d-th shortest wire: between [%ld] and [%ld] = distance %ld==>", wires, p % N, p / N, dist);
		int b = p % N;
		int a = p / N;

		// Add or append it to the circuit
		bool found = false;
		for (auto &cir : circuits) {
			if (cir.contains(a) && cir.contains(b)) {
				found = true;
				printf("Skip, already in ");
				for (auto c : cir)
					printf("%d ", c);
				printf("\n");
				//do not count this wire
				break;
			}

			else if (cir.contains(a) || cir.contains(b))
			{
				cir.insert(a);
				cir.insert(b);
				printf("Appended: size=>%zu: ", cir.size());
				for (auto c : cir)
					printf("%d ", c);
				printf("\n");
				found = true;
				wires++;
				break;
			}
		}

		if (!found) {
			auto &newcir = circuits.emplace_back();
			printf("New circuit (sz 2)\n");
			newcir.insert(a);
			newcir.insert(b);
			wires++;
		}

		if (num_pairs < 0)
			break;
	}

	return circuits;
}

template<size_t N>
std::array<int, N> calc_circuit_sizes(auto const &cirs) {
	std::array<int, N> szs{};

	int sz_i = 0;
	for (auto &cir : cirs) {
		szs[sz_i++] = cir.size();
	}

	return szs;
}

int main() {
	{
		auto matrix = create_matrix(sample_data);
		auto circuits = connect_n_wires<20>(10, matrix);

		std::partial_sort(circuits.begin(), circuits.begin() + 3, circuits.end(), [](auto &a, auto &b) {
			return a.size() > b.size();
		});

		std::cout << circuits[0].size() << "*" << circuits[1].size() << "*" << circuits[2].size() << "="
				  << circuits[0].size() * circuits[1].size() * circuits[2].size() << "\n";
	}

	{
		auto matrix = create_matrix(data);
		auto circuits = connect_n_wires<1000>(1000, matrix);

		std::partial_sort(circuits.begin(), circuits.begin() + 3, circuits.end(), [](auto &a, auto &b) {
			return a.size() > b.size();
		});

		std::cout << circuits[0].size() << "*" << circuits[1].size() << "*" << circuits[2].size() << "="
				  << circuits[0].size() * circuits[1].size() * circuits[2].size() << "\n";

		//13*12*12 = 2028 if we count wires that connect existing circuits without adding an element
		//18*17*16 = 4896 too low
	}
}
