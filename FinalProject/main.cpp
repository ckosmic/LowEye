#include "gameEngine.h"
#include "sprites.h"
#include "wall1.h"
#include "floor1.h"
#include "floor2.h"

#define mapWidth 24
#define mapHeight 24
#define doorId 4
#define num3dSprites 1
struct sprite3d {
	int graphic;
	sprite sprites[4];
	double rotation;
};

int worldMap[mapWidth][mapHeight] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1,4,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1,4,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

double tileTimers[mapWidth][mapHeight] =
{
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0},
	{1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0}
};

int ceilMap[mapWidth][mapHeight] =
{
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
	{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
};

int floorMap[mapWidth][mapHeight] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

double spriteInfo[num3dSprites][4] = {
	{20.5, 8.5, 0, 1},
	/*{18.5, 8.5, 0, 1},
	{16.5, 8.5, 0, 1},*/
};

double currentDistLT[SCREEN_HEIGHT+1];

vec2 playerPos;
vec2 playerDir;
vec2 camPlane;
double interval = 0;
double moveSpeed = 5.0;
double velocity = 0.0;
double rotSpeed = 3.0;
double zbuffer[SCREEN_WIDTH];
vector<sprite> envTextures;
sprite3d sprites3d[num3dSprites];
vector<sprite> uiSprites;
int lookingAt;
vec2Int lookTile;
char* dbg = new char[64];
int spriteOrder[num3dSprites];
double spriteDist[num3dSprites];
int menu = 1;
int nextMenu = 0;
int selectedButton = 0;
CHAR_INFO *transBuffer;
bool paused = false;

int main() {
	srand(time(0));
	loadConfig("PIXEL_SCALE=4\nNOISE_REDUCTION=0\nOPTIMIZE_TEXTURES=0\n");
	PIXEL_SCALE = stoi(getConfigValue("PIXEL_SCALE"));
	setupWindow();

	return 0;
}

void loadUiSprite(char* path, string name) {
	loadSprite(path, name);
	uiSprites.push_back(getSprite(name));
}

void onWindowCreated() {
	setWindowTitle(L"LowEye");

	playerPos.x = 21;
	playerPos.y = 12;
	playerDir.x = -1;
	playerDir.y = 0;
	camPlane.x = 0;
	camPlane.y = 0.6;

	memset(currentDistLT, 0, SCREEN_HEIGHT + 1);

	int optimizeTex = stoi(getConfigValue("OPTIMIZE_TEXTURES"));

	// Main wall texture
	loadSprite("resources\\textures\\wall.bmp", "wall1");
	envTextures.push_back(getSprite("wall1"));
	// Main floor texture
	loadSprite(optimizeTex ? "resources\\textures\\circle_floor_optimized.bmp" : "resources\\textures\\circle_floor.bmp", "circle_floor");
	envTextures.push_back(getSprite("circle_floor"));
	// Main ceiling texture
	loadSprite(optimizeTex ? "resources\\textures\\ceiling_optimized.bmp" : "resources\\textures\\ceiling.bmp", "ceiling");
	envTextures.push_back(getSprite("ceiling"));
	// Door texture
	loadSprite("resources\\textures\\door.bmp", "door");
	envTextures.push_back(getSprite("door"));

	loadSprite("resources\\textures\\chars\\1.bmp", "1");
	loadSprite("resources\\textures\\chars\\2.bmp", "2");
	loadSprite("resources\\textures\\chars\\3.bmp", "3");
	loadSprite("resources\\textures\\chars\\4.bmp", "4");
	sprites3d[0] = { 0, {
		getSprite("1"),
		getSprite("2"),
		getSprite("3"),
		getSprite("4"),
	}, 0.0 };

	//sprites3d[0] = loadSprite("resources\\textures\\testsprite.bmp");

	// UI sprites
	loadUiSprite("resources\\textures\\crosshair.bmp", "crosshair");
	
	loadUiSprite("resources\\textures\\ui\\button_start.bmp", "button_start");
	loadUiSprite("resources\\textures\\ui\\button_quit.bmp", "button_quit");
	loadUiSprite("resources\\textures\\ui\\button_border.bmp", "button_border");
	loadUiSprite("resources\\textures\\ui\\logo.bmp", "logo");
	loadUiSprite("resources\\textures\\ui\\button_options.bmp", "button_options");
	loadUiSprite("resources\\textures\\ui\\myname.bmp", "myname");

	loadUiSprite("resources\\textures\\ui\\toggle_off.bmp", "toggle0");
	loadUiSprite("resources\\textures\\ui\\toggle_on.bmp", "toggle1");
	loadUiSprite("resources\\textures\\ui\\toggle_border.bmp", "toggle_border");
	loadUiSprite("resources\\textures\\ui\\button_back.bmp", "button_back");
	loadUiSprite("resources\\textures\\ui\\button_resume.bmp", "button_resume");

	loadSprite("resources\\textures\\ui\\arrow0.bmp", "arrow0");
	loadSprite("resources\\textures\\ui\\arrow1.bmp", "arrow1");
	loadSprite("resources\\textures\\ui\\arrow2.bmp", "arrow2");
	loadSprite("resources\\textures\\ui\\arrow3.bmp", "arrow3");
	loadSprite("resources\\textures\\ui\\arrow4.bmp", "arrow4");
	loadSprite("resources\\textures\\ui\\arrow5.bmp", "arrow5");
	loadSprite("resources\\textures\\ui\\arrow6.bmp", "arrow6");
	loadSprite("resources\\textures\\ui\\arrow7.bmp", "arrow7");
}

double distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void pauseGame() {
	paused = !paused;
	if (paused) {
		clearUI();
		for (int i = 0; i < BUFFER_SIZE; i++) {
			wchar_t character = buffer[i].Char.UnicodeChar;
			if (character == PIXEL_SHADE0)
				buffer[i].Char.UnicodeChar = PIXEL_SHADE1;
			if (character == PIXEL_SHADE1)
				buffer[i].Char.UnicodeChar = PIXEL_SHADE2;
			if (character == PIXEL_SHADE2)
				buffer[i].Char.UnicodeChar = PIXEL_SHADE3;
			buffer[i].Attributes &= ~FOREGROUND_INTENSITY;
		}

		printText("Paused", 56, 10);

		menu = 5;
		selectedButton = 0;
	} else {
		clearUI();
		menu = 0;
	}
}

void combSort(int* order, double* dist, int amount) {
	int gap = amount;
	bool swapped = false;
	while (gap > 1 || swapped)
	{
		gap = (gap * 10) / 13;
		if (gap == 9 || gap == 10) gap = 11;
		if (gap < 1) gap = 1;
		swapped = false;
		for (int i = 0; i < amount - gap; i++)
		{
			int j = i + gap;
			if (dist[i] < dist[j])
			{
				std::swap(dist[i], dist[j]);
				std::swap(order[i], order[j]);
				swapped = true;
			}
		}
	}
}

bool isSpriteCollided(double x1, double x2) {
	for (int i = 0; i < num3dSprites; i++) {
		if (distance(x1, x2, spriteInfo[i][0], spriteInfo[i][1]) <= spriteInfo[i][3])
			return true;
	}
	return false;
}

// Heavily modified version of Lode's Raycasting Tutorial for the Final Project
// https://lodev.org/cgtutor/raycasting.html
void renderEnvironment() {
	// Loop through all columns of pixels and draw walls/floor/ceiling
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		double camX = 2 * x / double(SCREEN_WIDTH) - 1;
		vec2 rayDir;
		rayDir.x = playerDir.x + camPlane.x * camX;
		rayDir.y = playerDir.y + camPlane.y * camX;

		vec2 mapPos;
		mapPos.x = int(playerPos.x);
		mapPos.y = int(playerPos.y);

		vec2 sideDist;
		vec2 deltaDist;
		deltaDist.x = abs(1 / rayDir.x);
		deltaDist.y = abs(1 / rayDir.y);
		double perpWallDist;

		vec2 step;
		int hit = 0;
		int side;

		if (rayDir.x < 0) {
			step.x = -1;
			sideDist.x = (playerPos.x - mapPos.x) * deltaDist.x;
		}
		else {
			step.x = 1;
			sideDist.x = (mapPos.x + 1.0 - playerPos.x) * deltaDist.x;
		}
		if (rayDir.y < 0) {
			step.y = -1;
			sideDist.y = (playerPos.y - mapPos.y) * deltaDist.y;
		}
		else {
			step.y = 1;
			sideDist.y = (mapPos.y + 1.0 - playerPos.y) * deltaDist.y;
		}

		// Cast a ray until it hits a tile that is not 0 (air)
		int steps = 0;
		int texNum;
		while (hit == 0) {
			if (sideDist.x < sideDist.y) {
				sideDist.x += deltaDist.x;
				mapPos.x += step.x;
				side = 0;
			} else {
				sideDist.y += deltaDist.y;
				mapPos.y += step.y;
				side = 1;
			}

			texNum = worldMap[(int)mapPos.x][(int)mapPos.y];

			if (texNum > 0) {
				// Door casting
				if (texNum == doorId) {
					// Ray hit a door, so special cases are here such as moving the tile halfway in
					double mapX = mapPos.x;
					double mapY = mapPos.y;
					if (playerPos.x > mapPos.x && side == 0) mapX += 1;
					if (playerPos.y > mapPos.y && side == 1) mapY += 1;

					double adj = 1;
					double rayMult = 1;

					if (side == 1) {
						adj = mapY - playerPos.y;
						rayMult = adj / rayDir.y;
					} else {
						adj = mapX - playerPos.x;
						rayMult = adj / rayDir.x;
					}

					double rxe = playerPos.x + rayDir.x * rayMult;
					double rye = playerPos.y + rayDir.y * rayMult;

					deltaDist.x = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
					deltaDist.y = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));

					// Door geometry sliding animation
					if (side == 0) {
						double yStep = sqrt(deltaDist.x * deltaDist.x - 1);

						double halfStepY = rye + (step.y * yStep) * 0.5;
						if (floor(halfStepY) == mapY && (halfStepY - mapY) > (1.0 - tileTimers[(int)mapPos.x][(int)mapPos.y])) {
							hit = 1;
							break;
						}
					} else {
						double xStep = sqrt(deltaDist.y * deltaDist.y - 1);

						double halfStepX = rxe + (step.x * xStep) * 0.5;
						if (floor(halfStepX) == mapX && (halfStepX - mapX) > (1.0 - tileTimers[(int)mapPos.x][(int)mapPos.y])) {
							hit = 1;
							break;
						}
					}
				} else {
					hit = 1;
				}
			}
		}

		// Get information about the tile that the player is looking at
		if (x == SCREEN_WIDTH * 0.5) {
			lookingAt = texNum;
			lookTile.x = (int)mapPos.x;
			lookTile.y = (int)mapPos.y;
		}

		if (side == 0) {
			if (texNum == doorId) {
				mapPos.x += step.x * 0.5;
			}
			perpWallDist = (mapPos.x - playerPos.x + (1 - step.x) * 0.5) / rayDir.x;
		} else {
			if (texNum == doorId) {
				mapPos.y += step.y * 0.5;
			}
			perpWallDist = (mapPos.y - playerPos.y + (1 - step.y) * 0.5) / rayDir.y;
		}

		if (perpWallDist == 0) perpWallDist = 0.0001;

		zbuffer[x] = perpWallDist;

		int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
		int drawStart = -lineHeight * 0.5 + SCREEN_HEIGHT * 0.5;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight * 0.5 + SCREEN_HEIGHT * 0.5;
		if (drawEnd >= SCREEN_HEIGHT)
			drawEnd = SCREEN_HEIGHT;

		sprite wallTex;
		if (mapPos.x > mapWidth || mapPos.y > mapHeight || mapPos.x < 0 || mapPos.y < 0) {
			wallTex = INVALID_SPRITE;
		} else {
			wallTex = envTextures[texNum - 1];
		}

		double wallX;
		if (side == 0) wallX = playerPos.y + perpWallDist * rayDir.y;
		else wallX = playerPos.x + perpWallDist * rayDir.x;
		wallX -= floor(wallX);

		if (wallTex.valid) {
			int texX = int(wallX * double(wallTex.width));
			if (side == 0 && rayDir.x > 0) texX = wallTex.width - texX - 1;
			if (side == 1 && rayDir.y < 0) texX = wallTex.width - texX - 1;

			// Door texture sliding animation
			if (texNum == 4) {
				if (side == 0) {
					mapPos.x -= step.x * 0.5;
					if(playerPos.x > mapPos.x)
						texX += wallTex.width * tileTimers[(int)mapPos.x][(int)mapPos.y];
					else
						texX -= wallTex.width * tileTimers[(int)mapPos.x][(int)mapPos.y];
				} else {
					mapPos.y -= step.y * 0.5;
					if (playerPos.y > mapPos.y)
						texX -= wallTex.width * tileTimers[(int)mapPos.x][(int)mapPos.y];
					else
						texX += wallTex.width * tileTimers[(int)mapPos.x][(int)mapPos.y];
				}
			}

			for (int y = drawStart; y < drawEnd; y++) {
				int d = y * 256 - SCREEN_HEIGHT * 128 + lineHeight * 128;
				int texY = ((d * wallTex.height) / lineHeight) / 256;
				UINT pixIndex = wallTex.width * texY + texX;
				if (pixIndex > wallTex.width * wallTex.height - 1) pixIndex = wallTex.width * wallTex.height - 1;
				WORD drawColor = wallTex.colors[pixIndex] - 1;
				wchar_t drawChar = wallTex.chars[pixIndex];
				if (side == 1) {
					drawColor |= FOREGROUND_INTENSITY;
				}
				draw(x, y, drawChar, drawColor);
			}
		}

		if (texNum == 4) {
			if (side == 0) {
				mapPos.x += step.x * 0.5;
			} else {
				mapPos.y += step.y * 0.5;
			}
		}

		
		vec2 floorTexel;
		if (side == 0 && rayDir.x > 0) {
			floorTexel.x = mapPos.x;
			floorTexel.y = mapPos.y + wallX;
		} else if (side == 0 && rayDir.x < 0) {
			floorTexel.x = mapPos.x + 1;
			floorTexel.y = mapPos.y + wallX;
		} else if (side == 1 && rayDir.y > 0) {
			floorTexel.x = mapPos.x + wallX;
			floorTexel.y = mapPos.y;
		} else {
			floorTexel.x = mapPos.x + wallX;
			floorTexel.y = mapPos.y + 1;
		}

		double distWall, distPlayer, currentDist;

		distWall = perpWallDist;
		distPlayer = 0.0;

		if (drawEnd < 0) drawEnd = SCREEN_HEIGHT;

		unsigned int envTexSize = sizeof(envTextures) / sizeof(sprite);
		sprite ceilTexture;
		sprite floorTexture;
		vec2 curFloor;
		vec2Int floorTexCoords;
		vec2Int ceilTexCoords;
		for (int y = drawEnd + 1; y < SCREEN_HEIGHT + 1; y++) {
			//currentDist = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);
			currentDist = currentDistLT[y];
			if (currentDist == 0.0) {
				currentDistLT[y] = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);
				currentDist = currentDistLT[y];
			}
			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			curFloor.x = weight * floorTexel.x + (1.0 - weight) * playerPos.x;
			curFloor.y = weight * floorTexel.y + (1.0 - weight) * playerPos.y;

			int ceilIndex = ceilMap[int(curFloor.x)][int(curFloor.y)];
			int floorIndex = floorMap[int(curFloor.x)][int(curFloor.y)];
			if (ceilIndex > envTexSize - 1) ceilIndex = envTexSize - 1;
			if (floorIndex > envTexSize - 1) floorIndex = envTexSize - 1;

			ceilTexture = envTextures[ceilIndex];
			floorTexture = envTextures[floorIndex];

			floorTexCoords.x = int(curFloor.x * floorTexture.width) % floorTexture.width;
			floorTexCoords.y = int(curFloor.y * floorTexture.height) % floorTexture.height;

			ceilTexCoords.x = int(curFloor.x * ceilTexture.width) % ceilTexture.width;
			ceilTexCoords.y = int(curFloor.y * ceilTexture.height) % ceilTexture.height;

			int floorPixIndex = floorTexture.width * floorTexCoords.y + floorTexCoords.x;
			int ceilPixIndex = ceilTexture.width * ceilTexCoords.y + ceilTexCoords.x;
			if (floorTexture.valid) draw(x, y - 1, floorTexture.chars[floorPixIndex], floorTexture.colors[floorPixIndex] - 1);
			if (ceilTexture.valid) draw(x, SCREEN_HEIGHT - y, ceilTexture.chars[ceilPixIndex], (ceilTexture.colors[ceilPixIndex] - 1) | FOREGROUND_INTENSITY);
		}
		
	}

	// --Sprite drawing--
	// Sort sprites by distance from player
	for (int i = 0; i < num3dSprites; i++) {
		spriteOrder[i] = i;
		spriteDist[i] = ((playerPos.x - spriteInfo[i][0]) * (playerPos.x - spriteInfo[i][0]) + (playerPos.y - spriteInfo[i][1]) * (playerPos.y - spriteInfo[i][1]));
	}
	combSort(spriteOrder, spriteDist, num3dSprites);

	// Loop through all sprites and draw them
	for (int i = 0; i < num3dSprites; i++) {
		if (spriteDist[i] <= 100) {
			vec2 spritePos;
			spritePos.x = spriteInfo[spriteOrder[i]][0] - playerPos.x;
			spritePos.y = spriteInfo[spriteOrder[i]][1] - playerPos.y;

			double inverse = 1.0 / (camPlane.x * playerDir.y - playerDir.x * camPlane.y);

			vec2 transform;
			transform.x = inverse * (playerDir.y * spritePos.x - playerDir.x * spritePos.y);
			transform.y = inverse * (-camPlane.y * spritePos.x + camPlane.x * spritePos.y);

			int spriteX = int((SCREEN_WIDTH / 2) * (1 + transform.x / transform.y));

			int spriteHeight = abs(int(SCREEN_HEIGHT / (transform.y)));
			int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2;
			if (drawStartY < 0) drawStartY = 0;
			int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2;
			if (drawEndY >= SCREEN_HEIGHT) drawEndY = SCREEN_HEIGHT - 1;

			int spriteWidth = abs(int(SCREEN_HEIGHT / (transform.y)));
			int drawStartX = -spriteWidth / 2 + spriteX;
			if (drawStartX < 0) drawStartX = 0;
			int drawEndX = spriteWidth / 2 + spriteX;
			if (drawEndX >= SCREEN_WIDTH) drawEndX = SCREEN_WIDTH - 1;

			sprite3d rotSprite = sprites3d[int(spriteInfo[spriteOrder[i]][2])];
			double spDiff = rotSprite.rotation - atan2(spritePos.y, spritePos.x) * (180.0 / PI) + 180.0;

			if (spDiff >= 315.0 || spDiff < 45.0) rotSprite.graphic = 0;
			if (spDiff >= 45.0 && spDiff < 135.0) rotSprite.graphic = 1;
			if (spDiff >= 135.0 && spDiff < 225.0) rotSprite.graphic = 2;
			if (spDiff >= 225.0 && spDiff < 315.0) rotSprite.graphic = 3;

			sprite drawSprite = rotSprite.sprites[rotSprite.graphic];
			for (int x = drawStartX; x < drawEndX; x++) {
				int texX = int(256 * (x - (-spriteWidth / 2 + spriteX)) * drawSprite.width / spriteWidth) / 256;
				if (transform.y > 0 && x > 0 && x < SCREEN_WIDTH && transform.y < zbuffer[x]) {
					for (int y = drawStartY; y < drawEndY; y++) {
						int d = (y) * 256 - SCREEN_HEIGHT * 128 + spriteHeight * 128;
						int texY = ((d * drawSprite.height) / spriteHeight) / 256;
						WORD color = drawSprite.colors[drawSprite.width * texY + texX];
						wchar_t character = drawSprite.chars[drawSprite.width * texY + texX];
						if (color > 0)
							draw(x, y, character, color - 1);
					}
				}
			}
		}
	}

	// Reduce noise to increase performance a little bit
	// Scene rendering will look ugly though
	// But I mean, performance! Yay!
	if (getConfigValue("NOISE_REDUCTION") == "1") {
		for (int i = 1; i < BUFFER_SIZE - 1; i++) {
			CHAR_INFO cur = buffer[i];
			CHAR_INFO prev = buffer[i - 1];
			CHAR_INFO next = buffer[i + 1];
			if ((cur.Char.UnicodeChar != prev.Char.UnicodeChar && cur.Char.UnicodeChar != next.Char.UnicodeChar) || (cur.Attributes != prev.Attributes && cur.Attributes != next.Attributes)) {
				buffer[i].Char = prev.Char;
				buffer[i].Attributes = prev.Attributes;
			}
		}
	}
}

void update() {
	if (menu == 0) {
		if (paused == false) {
			// Clear screen
			clearScreen();

			moveSpeed = 20 * deltaTime;
			rotSpeed = 3 * deltaTime;
			velocity = lerp(velocity, 0, deltaTime * 10);

			// Key input (movement, rotation, interaction)
			double ppx = 0;
			double ppy = 0;
			if (keys[0x57].held) {
				// Key W is down
				velocity = lerp(velocity, 1, deltaTime * 3);

				ppx = playerPos.x + playerDir.x * moveSpeed;
				ppy = playerPos.y;
				if ((worldMap[int(ppx)][int(ppy)] == false ||
					tileTimers[int(ppx)][int(ppy)] == 0.0) &&
					isSpriteCollided(ppx, ppy) == false)
					playerPos.x += playerDir.x * moveSpeed * velocity;
				ppx = playerPos.x;
				ppy = playerPos.y + playerDir.y * moveSpeed;
				if ((worldMap[int(ppx)][int(ppy)] == false ||
					tileTimers[int(ppx)][int(ppy)] == 0.0) &&
					isSpriteCollided(ppx, ppy) == false)
					playerPos.y += playerDir.y * moveSpeed * velocity;
			}
			if (keys[0x53].held) {
				// Key S is down
				velocity = lerp(velocity, 1, deltaTime * 3);

				ppx = playerPos.x - playerDir.x * moveSpeed;
				ppy = playerPos.y;
				if ((worldMap[int(ppx)][int(ppy)] == false ||
					tileTimers[int(ppx)][int(ppy)] == 0.0) &&
					isSpriteCollided(ppx, ppy) == false)
					playerPos.x -= playerDir.x * moveSpeed * velocity;
				ppx = playerPos.x;
				ppy = playerPos.y - playerDir.y * moveSpeed;
				if ((worldMap[int(ppx)][int(ppy)] == false ||
					tileTimers[int(ppx)][int(ppy)] == 0.0) &&
					isSpriteCollided(ppx, ppy) == false)
					playerPos.y -= playerDir.y * moveSpeed * velocity;
			}
			if (keys[0x44].held) {
				// Key A is down
				double oldDirX = playerDir.x;
				playerDir.x = playerDir.x * cos(-rotSpeed) - playerDir.y * sin(-rotSpeed);
				playerDir.y = oldDirX * sin(-rotSpeed) + playerDir.y * cos(-rotSpeed);
				double oldPlaneX = camPlane.x;
				camPlane.x = camPlane.x * cos(-rotSpeed) - camPlane.y * sin(-rotSpeed);
				camPlane.y = oldPlaneX * sin(-rotSpeed) + camPlane.y * cos(-rotSpeed);
			}
			if (keys[0x41].held) {
				// Key D is down
				double oldDirX = playerDir.x;
				playerDir.x = playerDir.x * cos(rotSpeed) - playerDir.y * sin(rotSpeed);
				playerDir.y = oldDirX * sin(rotSpeed) + playerDir.y * cos(rotSpeed);
				double oldPlaneX = camPlane.x;
				camPlane.x = camPlane.x * cos(rotSpeed) - camPlane.y * sin(rotSpeed);
				camPlane.y = oldPlaneX * sin(rotSpeed) + camPlane.y * cos(rotSpeed);
			}
			if (keys[0x45].pressed) {
				// Key E was pressed
			}

			// Opening/closing doors based off of distance between player and door
			for (int y = 0; y < mapHeight; y++) {
				for (int x = 0; x < mapWidth; x++) {
					if (worldMap[x][y] == doorId) {
						if (distance(x, y, playerPos.x, playerPos.y) < 2.5) {
							tileTimers[x][y] -= deltaTime * 3;
						}
						else {
							tileTimers[x][y] += deltaTime * 3;
						}
						if (tileTimers[x][y] < 0.0) tileTimers[x][y] = 0.0;
						if (tileTimers[x][y] > 1.0) tileTimers[x][y] = 1.0;
					}
				}
			}

			// Render the whole scene
			renderEnvironment();

			// Draw crosshair
			drawSprite(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 8, uiSprites[0]);

			//sprintf(dbg, "%d", int(1 / deltaTime));
			sprintf(dbg, "%d", (1/tan(3)/sin(3)) == (sin(3)/tan(3)));
			printText(dbg, 1, 1);
		}

		if (keys[0x1B].pressed) {
			// Key ESC was pressed
			pauseGame();
		}
	} else if (menu == 1) {
		//Main menu

		paused = false;

		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		fillScreen(PIXEL_SHADE2, FOREGROUND_RED);
		clearUI();

		// Logo
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[4].width / 2, 10, uiSprites[4]);

		// Start button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[1].width / 2, SCREEN_HEIGHT / 2 - uiSprites[1].height / 2 - 5, uiSprites[1]);
		// Options button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[5].width / 2, SCREEN_HEIGHT / 2 - uiSprites[5].height / 2 + 13, uiSprites[5]);
		// Quit button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[2].width / 2, SCREEN_HEIGHT / 2 - uiSprites[2].height / 2 + 31, uiSprites[2]);

		// Draw selection arrow and border
		char arrowFrame[7];
		sprintf(arrowFrame, "arrow%d", ((frame/4) % 8));
		int buttonLoc = SCREEN_HEIGHT / 2 - uiSprites[3].height / 2 + (18 * selectedButton) - 5;
		drawSpriteTransparent(SCREEN_WIDTH / 2 - uiSprites[3].width / 2, buttonLoc, uiSprites[3]);
		drawSprite(30, buttonLoc, getSprite(arrowFrame));

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > 2) selectedButton = 0;
		if (selectedButton < 0) selectedButton = 2;

		// Created by text
		drawSprite(SCREEN_WIDTH - uiSprites[6].width - 1, SCREEN_HEIGHT - uiSprites[6].height - 1, uiSprites[6]);

		// Capture enter key to select button
		if (keys[0x0D].pressed) {
			if (selectedButton == 0) {
				menu = 0;
				clearUI();
			}
			if (selectedButton == 1) {
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 2;
			}
			if (selectedButton == 2) exitGame();
			selectedButton = 0;
		}

		if (transFlag) {
			mergeBuffers();
			transBuffer = new CHAR_INFO[BUFFER_SIZE];
			memset(transBuffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
			memcpy(transBuffer, buffer, BUFFER_SIZE * sizeof(CHAR_INFO));
			memset(buffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
			menu = 4;
			selectedButton = 0;
			clearUI();
		}
	} else if (menu == 2) {
		// Options menu

		bool transFlag = false;
		if (buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		fillScreen(PIXEL_SHADE2, FOREGROUND_RED);
		printText("Noise Reduction", 5, 8);

		drawSprite(SCREEN_WIDTH - 37, 5, (getConfigValue("NOISE_REDUCTION") == "1") ? uiSprites[8] : uiSprites[7]);

		drawSprite(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[10]);

		if (selectedButton == 0)
			drawSpriteTransparent(SCREEN_WIDTH - 37, 5, uiSprites[9]);
		if (selectedButton == 1)
			drawSpriteTransparent(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[3]);

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > 2) selectedButton = 0;
		if (selectedButton < 0) selectedButton = 2;

		// Capture enter key to select button
		if (keys[0x0D].pressed) {
			if (selectedButton == 0)
				setConfigValue("NOISE_REDUCTION", to_string(!stoi(getConfigValue("NOISE_REDUCTION"))).c_str());
			if (selectedButton == 1) {
				selectedButton = 0;
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 1;
			}
		}

		if (transFlag) {
			mergeBuffers();
			transBuffer = new CHAR_INFO[BUFFER_SIZE];
			memset(transBuffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
			memcpy(transBuffer, buffer, BUFFER_SIZE * sizeof(CHAR_INFO));
			memset(buffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
			menu = 4;
			selectedButton = 0;
			clearUI();
		}
	} else if (menu == 3) {
		// Transition from menu to black screen

		srand(3);
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			int rnd = rand();
			int speed = (rnd % 8 + 3);
			for (int j = 0; j < speed; j++) {
				draw(SCREEN_WIDTH + 32 + j - (selectedButton * speed) - (rnd % 32 + 1), i, PIXEL_SHADE0, 0);
			}
		}

		selectedButton++;

		if (selectedButton >= SCREEN_WIDTH/2) menu = nextMenu;
	} else if (menu == 4) {
		// Transition from black to new menu

		srand(4);
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			int rnd = rand();
			int speed = (rnd % 8 + 3);
			for (int j = 0; j < speed; j++) {
				int x = SCREEN_WIDTH + 32 + j - (selectedButton * speed) - (rnd % 32 + 1);
				int index = i * SCREEN_WIDTH + x;
				draw(x, i, transBuffer[index].Char.UnicodeChar, transBuffer[index].Attributes);
			}
		}

		selectedButton++;

		if (selectedButton >= SCREEN_WIDTH/2) {
			menu = nextMenu;
			delete[] transBuffer;
		}
	} else if (menu == 5) {
		// Quit button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[11].width / 2, SCREEN_HEIGHT / 2 - uiSprites[11].height / 2 - 5, uiSprites[11]);
		// Quit button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[2].width / 2, SCREEN_HEIGHT / 2 - uiSprites[2].height / 2 + 13, uiSprites[2]);

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > 1) selectedButton = 0;
		if (selectedButton < 0) selectedButton = 1;

		// Draw selection border
		int buttonLoc = SCREEN_HEIGHT / 2 - uiSprites[3].height / 2 + (18 * selectedButton) - 5;
		drawSpriteTransparent(SCREEN_WIDTH / 2 - uiSprites[3].width / 2, buttonLoc, uiSprites[3]);

		// Capture enter key to select button
		if (keys[0x0D].pressed) {
			if (selectedButton == 0)
				pauseGame();
			if (selectedButton == 1) {
				selectedButton = 0;
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 1;
			}
		}

		if (keys[0x1B].pressed) {
			// Key ESC was pressed
			pauseGame();
		}
	}
}