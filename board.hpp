#pragma once
#include "utils.hpp"
#include <vector>
#include <unordered_map>
#include <cmath>
#include <unordered_set>


class Board{
public:
	const std::unordered_set<IntCoords, IntCoordsHash> getLivingCellsCoords() const;
	void nextGen();
	void reset();
	void giveLife(const int& x, const int& y);
	void kill(const int& x, const int& y);

private:
	std::unordered_set<IntCoords, IntCoordsHash> livingCellsCoords{
		IntCoords{20, 20},
		IntCoords{22, 20},
		IntCoords{21, 21},
		IntCoords{22, 21},
		IntCoords{21, 22},
	};

	std::unordered_map<IntCoords, int, IntCoordsHash> contiguousCells{};

	void fillContiguousCellsHashmap();
	void killCells();
	void giveLifeToCells();
};
