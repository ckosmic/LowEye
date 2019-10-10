#include "gameEngine.h"
#include "sprites.h"
#include "wall1.h"
#include "floor1.h"
#include "floor2.h"

#define mapWidth 24
#define mapHeight 24
#define doorId 4
struct sprite3d {
	vec2 position;				// Position on the map
	int graphic;				// Which graphic to use when rotating
	sprite sprites[4];			// The rotation sprites
	double rotation;			// Rotation relative to the sprite
	double size;				// The scale of the sprite
	int offset;					// The Y offset of the sprite
	double collisionRadius;		// The radius of player-sprite collision
};

struct mapNode {
	int x;
	int y;
	int px;
	int py;
	double g;
	double h;
	double f;
};

struct playerStats {
	int hp;
	int maxHp;
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
vector<sprite3d> sprites3d;
vector<sprite> uiSprites;
int lookingAt;
vec2Int lookTile;
int lookSprite;
char* dbg = new char[64];
vector<int> spriteOrder;
vector<double> spriteDist;
int menu = 1;
int nextMenu = 0;
int selectedButton = 0;
CHAR_INFO *transBuffer;
bool paused = false;
bool mode7 = true;
double bobIntensity = 0;
double rotIntensity = 0;
playerStats pStats;

int main() {
	srand(time(NULL));
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
	loadSprite("resources\\textures\\chest.bmp", "chest");
	loadSprite("resources\\textures\\enemy_0.bmp", "enemy_0");
	loadSprite("resources\\textures\\enemy_1.bmp", "enemy_1");
	loadSprite("resources\\textures\\enemy_2.bmp", "enemy_2");
	loadSprite("resources\\textures\\enemy_3.bmp", "enemy_3");

	sprites3d.push_back({ { 20.5, 8.5 }, 0, {
		getSprite("1"),
		getSprite("2"),
		getSprite("3"),
		getSprite("4"),
	}, 0.0, 1.0, 0, 1.0 });

	sprites3d.push_back({ { 18.5, 8.5 }, 0, {
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5 });

	sprites3d.push_back({ { 20.5, 6.5 }, 0, {
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, 0.0, 0.75, 16, 0.75 });

	spriteDist.resize(sprites3d.size());
	spriteOrder.resize(sprites3d.size());

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

	loadUiSprite("resources\\textures\\gun1.bmp", "gun1");

	loadUiSprite("resources\\textures\\ui\\hp_label.bmp", "hp_label");

	loadSprite("resources\\textures\\ui\\arrow0.bmp", "arrow0");
	loadSprite("resources\\textures\\ui\\arrow1.bmp", "arrow1");
	loadSprite("resources\\textures\\ui\\arrow2.bmp", "arrow2");
	loadSprite("resources\\textures\\ui\\arrow3.bmp", "arrow3");
	loadSprite("resources\\textures\\ui\\arrow4.bmp", "arrow4");
	loadSprite("resources\\textures\\ui\\arrow5.bmp", "arrow5");
	loadSprite("resources\\textures\\ui\\arrow6.bmp", "arrow6");
	loadSprite("resources\\textures\\ui\\arrow7.bmp", "arrow7");

	pStats = {
	100,			// Current HP
	100				// Max HP
	};
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

void combSort(vector<int> &order, vector<double> &dist, int amount) {
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
	for (int i = 0; i < sprites3d.size(); i++) {
		if (distance(x1, x2, sprites3d[i].position.x, sprites3d[i].position.y) <= sprites3d[i].collisionRadius)
			return true;
	}
	return false;
}

bool operator < (const mapNode& mn1, const mapNode& mn2) {
	return mn1.f < mn2.f;
}

bool isValidNode(int x, int y) {
	int id = x + y * mapWidth;
	return false;
}

void pathfind(vec2 start, vec2 end) {
	
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
		}
		else {
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
					if (playerPos.x > mapPos.x)
						texX += wallTex.width * tileTimers[(int)mapPos.x][(int)mapPos.y];
					else
						texX -= wallTex.width * tileTimers[(int)mapPos.x][(int)mapPos.y];
				}
				else {
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
				int pixIndex = wallTex.width * texY + texX;
				if (pixIndex < 0) pixIndex = 0;
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
			}
			else {
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
	for (int i = 0; i < sprites3d.size(); i++) {
		spriteOrder[i] = i;
		spriteDist[i] = ((playerPos.x - sprites3d[i].position.x) * (playerPos.x - sprites3d[i].position.x) + (playerPos.y - sprites3d[i].position.y) * (playerPos.y - sprites3d[i].position.y));
	}
	combSort(spriteOrder, spriteDist, sprites3d.size());
	lookSprite = -1;

	// Loop through all sprites and draw them
	for (int i = 0; i < sprites3d.size(); i++) {
		if (spriteDist[i] <= 100) {
			sprite3d rotSprite = sprites3d[spriteOrder[i]];

			if (rotSprite.rotation > 180)
				sprites3d[spriteOrder[i]].rotation = -180;
			if(rotSprite.rotation < -180) 
				sprites3d[spriteOrder[i]].rotation = 180;

			vec2 spritePos;
			spritePos.x = rotSprite.position.x - playerPos.x;
			spritePos.y = rotSprite.position.y - playerPos.y;

			double inverse = 1.0 / (camPlane.x * playerDir.y - playerDir.x * camPlane.y);

			vec2 transform;
			transform.x = inverse * (playerDir.y * spritePos.x - playerDir.x * spritePos.y);
			transform.y = inverse * (-camPlane.y * spritePos.x + camPlane.x * spritePos.y);

			int spriteX = int((SCREEN_WIDTH / 2) * (1 + transform.x / transform.y));

			int spriteMove = int(rotSprite.offset / transform.y);

			int spriteHeight = abs(int(SCREEN_HEIGHT / (transform.y)));
			int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2 + spriteMove;
			if (drawStartY < 0) drawStartY = 0;
			int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2 + spriteMove;
			if (drawEndY > SCREEN_HEIGHT) drawEndY = SCREEN_HEIGHT;

			int spriteWidth = abs(int(SCREEN_HEIGHT / (transform.y)));
			int drawStartX = -spriteWidth / 2 + spriteX;
			if (drawStartX < 0) drawStartX = 0;
			int drawEndX = spriteWidth / 2 + spriteX;
			if (drawEndX > SCREEN_WIDTH) drawEndX = SCREEN_WIDTH;

			double spDiff = rotSprite.rotation - (atan2(spritePos.y, spritePos.x) * (180.0 / PI) + 180.0);
			// Clamp sprite-player rotation between 0 and 360
			if (spDiff > 360) spDiff -= 360;
			if (spDiff < 0) spDiff += 360;

			spriteWidth *= rotSprite.size;
			spriteHeight *= rotSprite.size;

			if (spDiff >= 315.0 || spDiff < 45.0) rotSprite.graphic = 0;
			if (spDiff >= 45.0 && spDiff < 135.0) rotSprite.graphic = 1;
			if (spDiff >= 135.0 && spDiff < 225.0) rotSprite.graphic = 2;
			if (spDiff >= 225.0 && spDiff < 315.0) rotSprite.graphic = 3;

			sprite drawSprite = rotSprite.sprites[rotSprite.graphic];
			for (int x = drawStartX; x < drawEndX; x++) {
				int texX = int(256 * (x - (-spriteWidth / 2 + spriteX)) * drawSprite.width / spriteWidth) / 256;
				if (transform.y > 0 && x >= 0 && x < SCREEN_WIDTH && transform.y < zbuffer[x]) {
					for (int y = drawStartY; y < drawEndY; y++) {
						int d = (y - spriteMove) * 256 - SCREEN_HEIGHT * 128 + spriteHeight * 128;
						int texY = ((d * drawSprite.height) / spriteHeight) / 256;
						if (texY < drawSprite.height && texX < drawSprite.width
							&& texY > -1 && texX > -1) {
							WORD color = drawSprite.colors[drawSprite.width * texY + texX];
							wchar_t character = drawSprite.chars[drawSprite.width * texY + texX];
							if (color > 0) {
								if (x == SCREEN_WIDTH / 2 && y == SCREEN_HEIGHT / 2)
									lookSprite = spriteOrder[i];
								draw(x, y, character, color - 1);
							}
						}
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

void drawHealthBar(int x, int y, int width, int hp, int maxHp) {
	int actualWidth = width * ((double)hp / (double)maxHp);
	wchar_t characters[3] = {
		PIXEL_SHADE0,
		PIXEL_SHADE1,
		PIXEL_SHADE1
	};
	WORD gray = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	for (int i = 0; i < 3; i++) {
		draw(x, y + i+1, PIXEL_SHADE3, gray);
		draw(x+width+1, y + i + 1, PIXEL_SHADE3, gray);
		for (int j = 0; j < actualWidth; j++) {
			WORD color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			if (i == 2) color ^= FOREGROUND_INTENSITY;
			draw(j + x + 1, i + y + 1, characters[i], color);
		}
	}
	horizLine(y, x, x + width + 1, PIXEL_SHADE3, gray);
	horizLine(y + 4, x, x + width + 1, PIXEL_SHADE3, gray);
}

// Runs every frame
void update() {
	if (menu == 0) {
		if (paused == false) {
			// Clear screen
			clearScreen();
			clearUI();

			moveSpeed = 20 * deltaTime;
			rotSpeed = 3 * deltaTime * rotIntensity;
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
				rotIntensity = lerp(rotIntensity, 1, deltaTime * 3);
			}
			if (keys[0x41].held) {
				// Key D is down
				double oldDirX = playerDir.x;
				playerDir.x = playerDir.x * cos(rotSpeed) - playerDir.y * sin(rotSpeed);
				playerDir.y = oldDirX * sin(rotSpeed) + playerDir.y * cos(rotSpeed);
				double oldPlaneX = camPlane.x;
				camPlane.x = camPlane.x * cos(rotSpeed) - camPlane.y * sin(rotSpeed);
				camPlane.y = oldPlaneX * sin(rotSpeed) + camPlane.y * cos(rotSpeed);
				rotIntensity = lerp(rotIntensity, 1, deltaTime * 3);
			}
			if (!keys[0x44].held && !keys[0x41].held) rotIntensity = 0;
			if (keys[0x45].pressed) {
				// Key E was pressed
			}
			if (keys[VK_SPACE].pressed) {
				// Key Space was pressed
				if (lookSprite >= 0)
					sprites3d[lookSprite].position.x += 1000;
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

			if (keys[0x57].held || keys[0x53].held)
				bobIntensity = lerp(bobIntensity, 1, deltaTime * 10);
			else
				bobIntensity = lerp(bobIntensity, 0, deltaTime * 10);

			// Draw crosshair
			drawSprite(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 8, uiSprites[0]);

			// Draw viewmodel
			drawSprite(SCREEN_WIDTH-54 + int(sin((double)frame / 8) * 10 * bobIntensity), SCREEN_HEIGHT-54 + int(sin((double)frame / 4) * 5 * bobIntensity), uiSprites[12]);

			drawSprite(2, 1, uiSprites[13]);
			drawHealthBar(1, 8, 64, pStats.hp, pStats.maxHp);

			//if (lookSprite >= 0)
				//sprintf(dbg, "%d", int(sprites3d[lookSprite].rotation));
			//printText(dbg, 1, 1);
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
		// Pause screen

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