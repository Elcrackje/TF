#include "labs.h"


using namespace std;

int main() {
	string glyphs[] = {
	"s"
	};

	Map* map = loadMap("map.blergh");
	ConsoleInfo* ci = new ConsoleInfo;
	getConsoleInfo(ci, 2, 30, 2, 30);
	

	clear();
	drawMap(map, glyphs, ci);
	byeMap(map);


	return 0;
}
