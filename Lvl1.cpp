#include "labs.h"


using namespace std;

int main() {

	Map* map = loadMap("map.txt");
	ConsoleInfo* ci = new ConsoleInfo;
	getConsoleInfo(ci, 2, 30, 2, 30);
	

	clear();
	drawMap(map, ci);

	byeMap(map);


	return 0;
}
