#pragma once
#include <unordered_map>
#include <tuple>
#include <functional>
#include <cstddef>


struct IntCoords {
	int x{};
	int y{};
};

bool operator==(const IntCoords& a, const IntCoords& b);


struct IntCoordsHash {
    std::size_t operator()(const IntCoords& c) const noexcept;
};
