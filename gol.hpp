#include "board.hpp"
#include "raylib.h"
#include "utils.hpp"
#include <vector>
#include <array>



class Gol{

public:
	void run();

private:
	
	constexpr static int kStartingWindowWidth{900};
	constexpr static int kStartingWindowHeight{kStartingWindowWidth};
	constexpr static char kWindowTitle[]{"Game of Life"};
	constexpr static int kStartingCellSideLength{kStartingWindowWidth / 50};
	constexpr static Color kBackgroundColor{BLACK};
	constexpr static Color kInitialCellColor{WHITE};
	constexpr static Color kGridColor{GRAY};
	constexpr static int kMinCellSideLength{2};
	constexpr static double kStartingTimeBetweenGenerations{1};
	constexpr static IntCoords kMinScreenSize{700, 700};

	constexpr static int kZoomFactor{5};
	constexpr static int kScrollFactor{6};
	constexpr static double kSpeedModFactor{0.1f};

	// para recorrer, comprobando si alguna está presionada para indexar
	// el color correspondiente
	constexpr static std::array<KeyboardKey, 9> digitKeys{
		KEY_ONE,
		KEY_TWO,
		KEY_THREE,
		KEY_FOUR,
		KEY_FIVE,
		KEY_SIX,
		KEY_SEVEN,
		KEY_EIGHT,
		KEY_NINE
	};

	constexpr static std::array<Color, 9> cellColorsAvailable{
		WHITE,
		Color{244, 136, 136, 255},
		Color{175, 217, 143, 255},
		Color{149, 187, 218, 255},
		Color{240, 167, 223, 255},
		Color{242, 235, 134, 255},
		Color{255, 183, 115, 255},
		Color{107, 217, 168, 255},
		Color{191, 220, 111, 255}
	};


	Board board{};
	int cellSideLength{kStartingCellSideLength};
	// varía con el desplazamiento de la cuadrícula
	IntCoords origin{};
	Color cellColor{kInitialCellColor};
	double dtCounter{};
	double timeBetweenGenerations{kStartingTimeBetweenGenerations};
	
	bool shouldDrawGrid{true};
	bool isPaused{true};



	// MÉTODOS
	
	void handleInput();
	void handleZoom();
	void handleScroll();
	void handleToggleGrid();
	void handleCenterView();
	void handleChangeCellColor();
	void handleTogglePause();
	void handleReset();
	void handleGiveLife();
	void handleModifySpeed();
	void handleKill();
	
	bool shouldUpdate(const double& dt);
	void update();
	void drawGrid();
	void drawLivingCells();
	IntCoords getGridStartingPoint();
	bool shouldDrawCell(const IntCoords& cellCoords, const IntCoords& gridStartingPoint);
};
