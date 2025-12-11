#include "machine.hh"
#include <array>
#include <string_view>

constexpr std::string_view sample_data[] = {
	"[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}",
	"[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}",
	"[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}",
};

constexpr std::array<Machine, 3> sample_machines = {{
	{.lights = 0b0110, .buttons = {bn(3), bn(1, 3), bn(2), bn(2, 3), bn(0, 2), bn(0, 1)}},
	{.lights = 0b01000, .buttons = {bn(0, 2, 3, 4), bn(2, 3), bn(0, 4), bn(0, 1, 2), bn(1, 2, 3, 4)}},
	{.lights = 0b101110, .buttons = {bn(0, 1, 2, 3, 4), bn(0, 3, 4), bn(0, 1, 2, 4, 5), bn(1, 2)}},
}};
