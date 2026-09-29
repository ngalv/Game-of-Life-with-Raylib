#include "board.hpp"
#include "utils.hpp"
#include <vector>
#include <unordered_set>


const std::unordered_set<IntCoords, IntCoordsHash> Board::getLivingCellsCoords() const {
	return livingCellsCoords;
}


void Board::nextGen(){
	fillContiguousCellsHashmap();
	killCells();
	giveLifeToCells();
	contiguousCells.clear();
};



void Board::fillContiguousCellsHashmap(){
	for (IntCoords livingCoords : livingCellsCoords){
		contiguousCells[IntCoords(livingCoords.x - 1, livingCoords.y - 1)]++;
		contiguousCells[IntCoords(livingCoords.x, livingCoords.y - 1)]++;
		contiguousCells[IntCoords(livingCoords.x + 1, livingCoords.y - 1)]++;

		contiguousCells[IntCoords(livingCoords.x - 1, livingCoords.y)]++;
		contiguousCells[IntCoords(livingCoords.x + 1, livingCoords.y)]++;

		contiguousCells[IntCoords(livingCoords.x - 1, livingCoords.y + 1)]++;
		contiguousCells[IntCoords(livingCoords.x, livingCoords.y + 1)]++;
		contiguousCells[IntCoords(livingCoords.x + 1, livingCoords.y + 1)]++;
	}
}


void Board::killCells(){
	int contiguousLivingCellsCount;
	std::unordered_set<IntCoords, IntCoordsHash> survivingCells{};

	for (const IntCoords& currentCellCoords : livingCellsCoords){
		contiguousLivingCellsCount = 0;
		for (const IntCoords& possibleContiguousLivingCoords : livingCellsCoords){
			if (possibleContiguousLivingCoords.y == currentCellCoords.y &&
			    possibleContiguousLivingCoords.x == currentCellCoords.x) continue;

			if (std::abs(possibleContiguousLivingCoords.y - currentCellCoords.y) < 2 &&
			    std::abs(possibleContiguousLivingCoords.x - currentCellCoords.x) < 2){
				contiguousLivingCellsCount++;
			}
		}

		if (contiguousLivingCellsCount > 1 &&
		    contiguousLivingCellsCount < 4){
			survivingCells.insert(currentCellCoords);
		}
	}

	livingCellsCoords = survivingCells;
}


void Board::giveLifeToCells(){
	for (const auto& [cell, counter] : contiguousCells){
		if (counter == 3) livingCellsCoords.insert(cell);
	}
}


void Board::reset(){
	
livingCellsCoords = std::unordered_set<IntCoords, IntCoordsHash>{
		IntCoords{20, 20},
		IntCoords{22, 20},
		IntCoords{21, 21},
		IntCoords{22, 21},
		IntCoords{21, 22},
	};

}


void Board::giveLife(const int& x, const int& y){
	livingCellsCoords.insert(IntCoords(x, y));
}



void Board::kill(const int& x, const int& y){
	livingCellsCoords.erase(IntCoords(x, y));
}
