#include "machine.hh"
#include <string_view>

constexpr uint16_t parse_light(std::string_view light_txt) {
	uint16_t light = 0;
	for (auto i = 0; auto c : light_txt) {
		if (c == '#')
			light |= (1 << i);
		if (c == '.' || c == '#')
			i++;
		if (c == ']')
			break;
	}
	return light;
}

static_assert(parse_light("....") == 0b0000);
static_assert(parse_light("#...") == 0b0001);
static_assert(parse_light(".#..") == 0b0010);
static_assert(parse_light("..#.") == 0b0100);
static_assert(parse_light(".##.") == 0b0110);
static_assert(parse_light("[.##.] (1,3)") == 0b0110);

constexpr Machine parse(std::string_view txt) {
	Machine m;

	auto light_txt = txt.substr(1, txt.find_first_of("["));
	m.lights = parse_light(light_txt);

	// while (true) {
	// 	auto end = txt.find_first_of(")");
	// 	auto beg = txt.find_first_of("(");
	// 	if (end == txt.npos)
	// 		break;

	// 	txt = txt.substr(end);
	// 	auto btn_txt = txt.substr(beg + 1, end - beg - 1);
	// 	printf("%.*s\n", (int)btn_txt.size(), btn_txt.data());
	// }

	return m;
}
