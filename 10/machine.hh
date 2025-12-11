#pragma once
#include "util/fixed_vector.hh"
#include <cstdint>

struct Machine {
	uint16_t lights;
	FixedVector<uint16_t, 32> buttons;
};

constexpr uint16_t bn(auto... bits) {
	return ((1 << bits) | ...);
}
