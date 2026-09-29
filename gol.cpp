#include "board.hpp"
#include "gol.hpp"
#include "raylib.h"
#include <cstdlib>


void Gol::run(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE || FLAG_VSYNC_HINT);

    InitWindow(kStartingWindowWidth, kStartingWindowHeight, kWindowTitle);

    SetWindowMinSize(kMinScreenSize.x, kMinScreenSize.y);

    while (!WindowShouldClose()) {
	handleInput();

	float dt{GetFrameTime()};
	if (shouldUpdate(dt)) update();

        BeginDrawing();
		ClearBackground(kBackgroundColor);
		drawLivingCells();
		if (shouldDrawGrid) drawGrid();
        EndDrawing();
    }

    CloseWindow();
}


void Gol::drawGrid(){
	int w = GetScreenWidth();
	int h = GetScreenHeight();

	// El punto a partir del cual se empieza a dibujar la cuadrícula
	IntCoords gridStartingPoint{getGridStartingPoint()};

	int cellsPerRow{(w - gridStartingPoint.x) / cellSideLength};
	int cellsPerCol{(h - gridStartingPoint.y) / cellSideLength};

	// TODO: Debería sumarse 1 en la condición, + 2 es un apaño temporal
	for (int row{0}; row < cellsPerCol + 1; row++){
		for (int col{0}; col < cellsPerRow + 1; col++){
			DrawRectangleLines(
					gridStartingPoint.x + cellSideLength * col,
					gridStartingPoint.y + cellSideLength * row,
					cellSideLength,
					cellSideLength,
					kGridColor
					);
		}
	}
}
	


IntCoords Gol::getGridStartingPoint(){
	int x; 
	int y;

	if (origin.x > 0){
		x = -(origin.x % cellSideLength);
	}
	else {
		x = -(origin.x % cellSideLength + cellSideLength);
	}


	if (origin.y > 0){
		y = -(origin.y % cellSideLength);
	}
	else {
		y = -(origin.y % cellSideLength + cellSideLength);
	}


	return {x, y};
}



void Gol::drawLivingCells(){
	std::unordered_set<IntCoords, IntCoordsHash> livingCellsCoords{board.getLivingCellsCoords()};
	IntCoords gridStartingPoint{getGridStartingPoint()};
	
	for (IntCoords livingCellCoords : livingCellsCoords) {
		// coordenadas en la ventana
		IntCoords trueCellCoords{
			livingCellCoords.x * cellSideLength - origin.x,
			livingCellCoords.y * cellSideLength - origin.y
		}; 

		if (shouldDrawCell(trueCellCoords, gridStartingPoint)) DrawRectangle(trueCellCoords.x, trueCellCoords.y, cellSideLength, cellSideLength, cellColor);
	}
}




bool Gol::shouldDrawCell(const IntCoords& coords, const IntCoords& gridStartingPoint){
	return coords.x < GetScreenWidth() &&
		coords.x >= gridStartingPoint.x &&
		coords.y < GetScreenHeight() &&
		coords.y >= gridStartingPoint.y;
}




void Gol::handleInput(){
	handleZoom();
	handleScroll();
	handleToggleGrid();
	handleCenterView();
	handleChangeCellColor();
	handleTogglePause();
	handleReset();
	handleGiveLife();
	handleModifySpeed();
	handleKill();
}



void Gol::update(){
	board.nextGen();
}



bool Gol::shouldUpdate(const double& dt){
	if (isPaused) return false;

	dtCounter += dt;
	if (dtCounter < timeBetweenGenerations) return false;
	dtCounter = 0;
	return true;
}



void Gol::handleZoom(){
	if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) return;

	if (IsKeyPressed(KEY_Z)){
	 	if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
	 		cellSideLength -= kZoomFactor;
	 	} else {
	 		cellSideLength += kZoomFactor;
	 	}
	}
	else {
		int zoomLevel{static_cast<int>(GetMouseWheelMove())};
		if (zoomLevel != 0 && cellSideLength + zoomLevel * kZoomFactor > kMinCellSideLength) {
	 		cellSideLength += zoomLevel * kZoomFactor;
		}
	}



}



void Gol::handleScroll(){
	if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) return;

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 mouseDelta{GetMouseDelta()};
		origin.x += static_cast<int>(-mouseDelta.x);
		origin.y += static_cast<int>(-mouseDelta.y);
		return;
	}

	if (IsKeyPressed(KEY_UP)) {
		origin.y -= GetScreenHeight() / kScrollFactor;
	}
	else if (IsKeyPressed(KEY_DOWN)) {
		origin.y += GetScreenHeight() / kScrollFactor;
	}


	if (IsKeyPressed(KEY_LEFT)) {
		origin.x -= GetScreenWidth() / kScrollFactor;
	}
	else if (IsKeyPressed(KEY_RIGHT)) {
		origin.x += GetScreenWidth() / kScrollFactor;
	}
}



void Gol::handleToggleGrid(){
	if (IsKeyPressed(KEY_G)) shouldDrawGrid = !shouldDrawGrid;
}




void Gol::handleCenterView(){
	if (IsKeyPressed(KEY_C)) origin = IntCoords();
}


void Gol::handleChangeCellColor(){
	for (int i{}; i < digitKeys.size(); i++) {
		if (IsKeyPressed(digitKeys[i])) cellColor = cellColorsAvailable[i];
	}
}



void Gol::handleTogglePause() {
	if (IsKeyPressed(KEY_SPACE)) isPaused = !isPaused;
}



void Gol::handleReset(){
	if (IsKeyPressed(KEY_R)){
		*this = Gol();
	}
}



void Gol::handleGiveLife(){
	if (!(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ||
			!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;

	Vector2 mousePos{GetMousePosition()};
	board.giveLife((static_cast<int>(mousePos.x) + origin.x) / cellSideLength, (static_cast<int>(mousePos.y) + origin.y) / cellSideLength);
}



void Gol::handleModifySpeed(){
	if (!(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))) return;

	double wheelMovement{GetMouseWheelMove()};
		if (wheelMovement != 0 && timeBetweenGenerations - wheelMovement * kSpeedModFactor * timeBetweenGenerations > 0) {
	 		timeBetweenGenerations -= wheelMovement * kSpeedModFactor * timeBetweenGenerations;
		}
	
}



void Gol::handleKill(){
	if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) return;

	Vector2 mousePos{GetMousePosition()};
	board.kill((static_cast<int>(mousePos.x) + origin.x) / cellSideLength, (static_cast<int>(mousePos.y) + origin.y) / cellSideLength);
}
