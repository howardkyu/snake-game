#include "Cell.h"

int cellToInt(Cell cell) {
	switch (cell) {
		case Cell::Empty:
			return 0;
			break;
		case Cell::Snake1:
			return 1;
			break;
		case Cell::Snake2:
			return 2;
			break;
		case Cell::Food:
			return 3;
			break;

	}
}