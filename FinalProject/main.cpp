// Main game code

#include "main.h"

// Global variables
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
int frontSprite;
char* dbg = new char[64];
vector<int> spriteOrder;
vector<double> spriteDist;
int menu = 1;
int nextMenu = 0;
int selectedButton = 0;
int maxMenuItems = 0;
CHAR_INFO *transBuffer;
bool paused = false;
bool mode7 = true;
double bobIntensity = 0;
double rotIntensity = 0;
playerStats pStats;
vector<flag> flags;
string messageString;

int main() {
	srand(time(NULL));
	// Load config from settings.ini (located at same location as executable)
	loadConfig("PIXEL_SCALE=4\nNOISE_REDUCTION=0\nOPTIMIZE_TEXTURES=0\n");
	PIXEL_SCALE = stoi(getConfigValue("PIXEL_SCALE"));
	setupWindow();

	return 0;
}

// Returns an item by passing its display name
item getItemByName(string name) {
	for (int i = 0; i < sizeof(itemBank) / sizeof(itemBank[0]); i++) {
		if (string(itemBank[i].name) == name) 
			return itemBank[i];
	}
}

// Returns an attack (ability) by its display name
attack getAttackByName(string name) {
	for (int i = 0; i < sizeof(attackBank) / sizeof(attackBank[0]); i++) {
		if (string(attackBank[i].name) == name)
			return attackBank[i];
	}
}

// Save game data to save_data.dat
void saveSaveFile() {
	string data = "";

	data.append("PLAYER_X="); data.append(to_string(playerPos.x)); data.append("\n");
	data.append("PLAYER_Y="); data.append(to_string(playerPos.y)); data.append("\n");
	data.append("PLAYER_DIR_X="); data.append(to_string(playerDir.x)); data.append("\n");
	data.append("PLAYER_DIR_Y="); data.append(to_string(playerDir.y)); data.append("\n");
	data.append("PLAYER_AP="); data.append(to_string(pStats.ap)); data.append("\n");
	data.append("PLAYER_DEFENSE="); data.append(to_string(pStats.ap)); data.append("\n");
	data.append("PLAYER_HP="); data.append(to_string(pStats.hp)); data.append("\n");
	data.append("PLAYER_LEVEL="); data.append(to_string(pStats.level)); data.append("\n");
	data.append("PLAYER_MAXAP="); data.append(to_string(pStats.maxAp)); data.append("\n");
	data.append("PLAYER_MAXHP="); data.append(to_string(pStats.maxHp)); data.append("\n");
	data.append("PLAYER_MAXXP="); data.append(to_string(pStats.maxXp)); data.append("\n");
	data.append("PLAYER_STRENGTH="); data.append(to_string(pStats.strength)); data.append("\n");
	data.append("PLAYER_XP="); data.append(to_string(pStats.xp)); data.append("\n");
	data.append("CAMPLANE_X="); data.append(to_string(camPlane.x)); data.append("\n");
	data.append("CAMPLANE_Y="); data.append(to_string(camPlane.y)); data.append("\n");
	for (int i = 0; i < sprites3d.size(); i++) {
		if (sprites3d[i].active == false) {
			data.append("INACTIVE_SPRITE="); data.append(to_string(sprites3d[i].id)); data.append("\n");
		}
	}
	for (int i = 0; i < pStats.items.size(); i++) {
		string iName = string(pStats.items[i].name);
		replace(iName.begin(), iName.end(), ' ', '_');
		data.append("PLAYER_ITEM="); data.append(iName); data.append("\n");
	}
	for (int i = 0; i < pStats.attacks.size(); i++) {
		string iName = string(pStats.attacks[i].name);
		replace(iName.begin(), iName.end(), ' ', '_');
		data.append("PLAYER_ABILITY="); data.append(iName); data.append("\n");
	}

	ofstream conf("save_data.dat");
	conf << data;
	conf.close();
}

// Load game data from save_data.dat
void loadSaveFile() {
	ifstream config("save_data.dat");
	if (config.is_open()) {

		pStats.items.clear();
		pStats.attacks.clear();

		char attribute[256];
		char v[256];
		while (config.getline(attribute, 256, '=') && config.getline(v, 256, '\n')) {
			string a = string(attribute);

			if (a == "PLAYER_X") playerPos.x = atof(v);
			if (a == "PLAYER_Y") playerPos.y = atof(v);
			if (a == "PLAYER_DIR_X") playerDir.x = atof(v);
			if (a == "PLAYER_DIR_Y") playerDir.y = atof(v);
			if (a == "PLAYER_AP") pStats.ap = atoi(v);
			if (a == "PLAYER_DEFENSE") pStats.defense = atoi(v);
			if (a == "PLAYER_HP") pStats.hp = atoi(v);
			if (a == "PLAYER_LEVEL") pStats.level = atoi(v);
			if (a == "PLAYER_MAXAP") pStats.maxAp = atoi(v);
			if (a == "PLAYER_MAXHP") pStats.maxHp = atoi(v);
			if (a == "PLAYER_MAXXP") pStats.maxXp = atoi(v);
			if (a == "PLAYER_STRENGTH") pStats.strength = atoi(v);
			if (a == "PLAYER_XP") pStats.xp = atoi(v);
			if (a == "CAMPLANE_X") camPlane.x = atof(v);
			if (a == "CAMPLANE_Y") camPlane.y = atof(v);
			if (a == "INACTIVE_SPRITE") {
				for (int i = 0; i < sprites3d.size(); i++) {
					if (sprites3d[i].id == atoi(v))
						sprites3d[i].active = false;
				}
			}
			if (a == "PLAYER_ITEM") {
				string iName = string(v);
				replace(iName.begin(), iName.end(), '_', ' ');
				pStats.items.push_back(getItemByName(iName));
			}
			if (a == "PLAYER_ABILITY") {
				string aName = string(v);
				replace(aName.begin(), aName.end(), '_', ' ');
				pStats.attacks.push_back(getAttackByName(aName));
			}
		}
	}
	else {
		saveSaveFile();
		loadSaveFile();
	}

	config.close();
}

// Loads a sprite into the sprite bank and the UI sprite bank
void loadUiSprite(char* path, string name) {
	loadSprite(path, name);
	uiSprites.push_back(getSprite(name));
}

// Returns a UI sprite from the UI sprite bank
sprite getUiSprite(string name) {
	for (int i = 0; i < uiSprites.size(); i++) {
		if (uiSprites[i].name == name) {
			return uiSprites[i];
			break;
		}
	}
}

// Sets a named flag that will last for a specified amount of frames
void setFlag(string name, int length) {
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i].name == name) {
			flags[i].endFrame = frame + length;
			return;
		}
	}
	flag newFlag = {
		name,
		frame + length
	};
	flags.push_back(newFlag);
}

// Removes a flag prematurely
void removeFlag(string name) {
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i].name == name) {
			flags.erase(flags.begin() + i);
		}
	}
}

// Returns true if flag exists
bool getFlag(string name) {
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i].name == name) {
			return true;
		}
	}
	return false;
}

// Gets the amount of frames left until the specified flag gets removed
int getFlagFramesLeft(string name) {
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i].name == name) {
			return flags[i].endFrame - frame;
		}
	}
	return -1;
}

// Adds a 3D sprite to the 3D sprite bank and assigns an ID to it
void addSprite3d(sprite3d spr) {
	spr.id = sprites3d.size();
	sprites3d.push_back(spr);
}

// Called when the game's window is created (from gameEngine.h)
void onWindowCreated() {
	setWindowTitle(L"LowEye");

	menu = 1;
	//menu = 0;
	clearUI();
	loadSaveFile();

	playerPos.x = 32.5;
	playerPos.y = 31;
	playerDir.x = -1;
	playerDir.y = 0;
	camPlane.x = 0;
	camPlane.y = 0.6;

	memset(currentDistLT, 0, SCREEN_HEIGHT + 1);

	int optimizeTex = stoi(getConfigValue("OPTIMIZE_TEXTURES"));

	// --Load sprites--
	loadSprite("resources\\textures\\wall.bmp", "wall1");
	envTextures.push_back(getSprite("wall1"));	// envTextures is used for quick and easy getting of environmental textures
	loadSprite(optimizeTex ? "resources\\textures\\circle_floor_optimized.bmp" : "resources\\textures\\circle_floor.bmp", "circle_floor");
	envTextures.push_back(getSprite("circle_floor"));
	loadSprite(optimizeTex ? "resources\\textures\\ceiling_optimized.bmp" : "resources\\textures\\ceiling.bmp", "ceiling");
	envTextures.push_back(getSprite("ceiling"));
	loadSprite("resources\\textures\\door.bmp", "door");
	envTextures.push_back(getSprite("door"));
	loadSprite("resources\\textures\\circle_floor_spill.bmp", "circle_floor_spill");
	envTextures.push_back(getSprite("circle_floor_spill"));
	loadSprite("resources\\textures\\wall2.bmp", "wall2");
	envTextures.push_back(getSprite("wall2"));
	loadSprite("resources\\textures\\wall3.bmp", "wall3");
	envTextures.push_back(getSprite("wall3"));
	loadSprite("resources\\textures\\wall4.bmp", "wall4");
	envTextures.push_back(getSprite("wall4"));
	 
	loadSprite("resources\\textures\\chest.bmp", "chest");
	loadSprite("resources\\textures\\barrel.bmp", "barrel");

	loadSprite("resources\\textures\\enemy_0.bmp", "enemy_0");
	loadSprite("resources\\textures\\enemy_1.bmp", "enemy_1");
	loadSprite("resources\\textures\\enemy_2.bmp", "enemy_2");
	loadSprite("resources\\textures\\enemy_3.bmp", "enemy_3");

	loadSprite("resources\\textures\\enemy1_0.bmp", "enemy1_0");
	loadSprite("resources\\textures\\enemy1_1.bmp", "enemy1_1");
	loadSprite("resources\\textures\\enemy1_2.bmp", "enemy1_2");
	loadSprite("resources\\textures\\enemy1_3.bmp", "enemy1_3");

	loadSprite("resources\\textures\\boss_0.bmp", "boss_0");
	loadSprite("resources\\textures\\boss_1.bmp", "boss_1");
	loadSprite("resources\\textures\\boss_2.bmp", "boss_2");
	loadSprite("resources\\textures\\boss_3.bmp", "boss_3");

	loadSprite("resources\\textures\\ui\\arrow0.bmp", "arrow0");
	loadSprite("resources\\textures\\ui\\arrow1.bmp", "arrow1");
	loadSprite("resources\\textures\\ui\\arrow2.bmp", "arrow2");
	loadSprite("resources\\textures\\ui\\arrow3.bmp", "arrow3");
	loadSprite("resources\\textures\\ui\\arrow4.bmp", "arrow4");
	loadSprite("resources\\textures\\ui\\arrow5.bmp", "arrow5");
	loadSprite("resources\\textures\\ui\\arrow6.bmp", "arrow6");
	loadSprite("resources\\textures\\ui\\arrow7.bmp", "arrow7");

	loadSprite("resources\\textures\\comp_0.bmp", "comp_0");
	loadSprite("resources\\textures\\comp_1.bmp", "comp_1");
	loadSprite("resources\\textures\\comp_2.bmp", "comp_2");
	loadSprite("resources\\textures\\comp_3.bmp", "comp_3");

	loadSprite("resources\\textures\\torch.bmp", "torch");

	// --Load UI sprites--
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

	loadUiSprite("resources\\textures\\ui\\button_stats.bmp", "button_stats");

	loadUiSprite("resources\\textures\\ui\\hp_label.bmp", "hp_label");

	loadUiSprite("resources\\textures\\ui\\battle_menu.bmp", "battle_menu");
	loadUiSprite("resources\\textures\\ui\\battle_menu_half.bmp", "battle_menu_half");
	loadUiSprite("resources\\textures\\ui\\battle_menu_attack.bmp", "battle_menu_attack");
	loadUiSprite("resources\\textures\\ui\\battle_menu_items.bmp", "battle_menu_items");
	loadUiSprite("resources\\textures\\ui\\battle_menu_abilities.bmp", "battle_menu_abilities");
	loadUiSprite("resources\\textures\\ui\\battle_menu_hp.bmp", "battle_menu_hp");

	loadUiSprite("resources\\textures\\ui\\button_continue.bmp", "button_continue");
	loadUiSprite("resources\\textures\\ui\\button_quitgame.bmp", "button_quitgame");
	loadUiSprite("resources\\textures\\ui\\button_border_large.bmp", "button_border_large");
	loadUiSprite("resources\\textures\\ui\\button_newgame.bmp", "button_newgame");

	loadUiSprite("resources\\textures\\ui\\blast.bmp", "blast");
	
	
	// Initialize default player stats
	pStats = {
		100,								// HP
		100,								// Max HP
		100,								// AP
		100,								// Max AP
		3,									// Strength
		1,									// Defense
		0,									// XP
		100,								// Max XP
		1,									// Level
		{									// Attacks that the player posesses
			A_LASER_GUN
		},
		{									// Items that the player posesses
			
		}
	};

	// Define & initialize enemy types
	enemy mutant = {
		100,								// Max HP
		1,									// Strength
		1,									// Defense
		75,									// XP to reward the player once defeated
		{ A_BASIC },						// Attacks that the enemy posesses
		"Mutant"							// Enemy name
	};

	enemy warrior = {
		125,
		2,
		2,
		110,
		{ A_BASIC, A_BITE },
		"Warrior"
	};

	enemy boss = {
		1000,
		5,
		5,
		10000,
		{ A_BASIC, A_BITE, A_GOO, A_OFFLECT },
		"LowEye"
	};

	// Add 3D sprites to the scene
	// addSprite3d accepts a sprite3d, adds it to the scene, and assigns an ID to it; used for saving/loading

	addSprite3d({ { 24.5, 36.5 }, 0, {
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, -90.0, 0.75, 16, 0.75, "mutant", 1, mutant });
	addSprite3d({ { 21.5, 49.5 }, 0, {
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, -90.0, 0.75, 16, 0.75, "mutant", 1, mutant });
	addSprite3d({ { 8, 19 }, 0, {
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, 90.0, 0.75, 16, 0.75, "mutant", 1, mutant });
	addSprite3d({ { 49.5, 24.5 }, 0,{
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, -90.0, 0.75, 16, 0.75, "mutant", 1, mutant });
	addSprite3d({ { 61.5, 44.5 }, 0,{
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, 90.0, 0.75, 16, 0.75, "mutant", 1, mutant });
	addSprite3d({ { 23.5, 10.5 }, 0,{
		getSprite("enemy_0"),
		getSprite("enemy_1"),
		getSprite("enemy_2"),
		getSprite("enemy_3"),
	}, 0.0, 0.75, 16, 0.75, "mutant", 1, mutant });

	addSprite3d({ { 43.5, 47.5 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 0, 0.75, 16, 0.75, "warrior", 1, warrior });
	addSprite3d({ { 43.5, 59.5 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 0, 0.75, 16, 0.75, "warrior", 1, warrior });
	addSprite3d({ { 23, 23 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 180.0, 0.75, 16, 0.75, "warrior", 1, warrior });
	addSprite3d({ { 29, 21.5 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 180.0, 0.75, 16, 0.75, "warrior", 1, warrior });
	addSprite3d({ { 50.5, 9.5 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 0.0, 0.75, 16, 0.75, "warrior", 1, warrior });
	addSprite3d({ { 41.5, 7.5 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 0.0, 0.75, 16, 0.75, "warrior", 1, warrior });
	addSprite3d({ { 8.5, 8.5 }, 0,{
		getSprite("enemy1_0"),
		getSprite("enemy1_1"),
		getSprite("enemy1_2"),
		getSprite("enemy1_3"),
	}, 0.0, 0.75, 16, 0.75, "warrior", 1, warrior });

	addSprite3d({ { 27.5, 35.5 }, 0, {
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2, {}, { NULL, &I_POTION } });
	addSprite3d({ { 18.5, 39.5 }, 0, {
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2, {}, { &A_FLAME, NULL } });
	addSprite3d({ { 1.5, 45.5 }, 0, {
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2, {}, { NULL, &I_POTION } });
	addSprite3d({ { 29.5, 28.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ NULL, &I_MAXAP } });
	addSprite3d({ { 31.5, 28.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ NULL, &I_ELIXIR } });
	addSprite3d({ { 43.5, 51.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ &A_FREEZE, NULL } });
	addSprite3d({ { 55.5, 39.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ NULL, &I_MAXHP } });
	addSprite3d({ { 57.5, 6.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ NULL, &I_ELIXIR2 } });
	addSprite3d({ { 33.5, 6.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ NULL, &I_POTION2 } });
	addSprite3d({ { 19.5, 10.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ NULL, &I_POTION3 } });
	// Hidden chest out of bounds (outside of boss room) - contains most powerful attack
	addSprite3d({ { 7.5, 12.5 }, 0,{
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
		getSprite("chest"),
	}, 0.0, 0.5, 32, 0.5, "chest", 2,{},{ &A_FINAL, NULL } });

	addSprite3d({ { 1.5, 43.5 }, 0, {
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
	}, 0.0, 0.75, 16, 0.75, "barrel", 0 });
	addSprite3d({ { 1.5, 42.5 }, 0, {
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
	}, 0.0, 0.75, 16, 0.75, "barrel", 0 });
	addSprite3d({ { 2, 37.5 }, 0, {
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
	}, 0.0, 0.75, 16, 0.75, "barrel", 0 });
	addSprite3d({ { 56.5, 25.5 }, 0,{
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
	}, 0.0, 0.75, 16, 0.75, "barrel", 0 });
	addSprite3d({ { 56.5, 26.5 }, 0,{
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
		getSprite("barrel"),
	}, 0.0, 0.75, 16, 0.75, "barrel", 0 });

	addSprite3d({ { 9.5, 33.5 }, 0, {
		getSprite("comp_0"),
		getSprite("comp_1"),
		getSprite("comp_2"),
		getSprite("comp_3"),
	}, 90.0, 0.75, 16, 0.5, "computer", 0 });
	addSprite3d({ { 60.5, 51.5 }, 0,{
		getSprite("comp_0"),
		getSprite("comp_1"),
		getSprite("comp_2"),
		getSprite("comp_3"),
	}, 90.0, 0.75, 16, 0.5, "computer", 0 });
	addSprite3d({ { 8.5, 7.5 }, 0,{
		getSprite("comp_0"),
		getSprite("comp_1"),
		getSprite("comp_2"),
		getSprite("comp_3"),
	}, 0.0, 0.75, 16, 0.5, "computer", 0 });

	addSprite3d({ { 3.5, 7.5 }, 0,{
		getSprite("torch"),
		getSprite("torch"),
		getSprite("torch"),
		getSprite("torch"),
	}, 0.0, 0.75, 16, 0.5, "torch", 0 });
	addSprite3d({ { 3.5, 9.5 }, 0,{
		getSprite("torch"),
		getSprite("torch"),
		getSprite("torch"),
		getSprite("torch"),
	}, 0.0, 0.75, 16, 0.5, "torch", 0 });

	addSprite3d({ { 2.5, 8.5 }, 0, {
		getSprite("boss_0"),
		getSprite("boss_1"),
		getSprite("boss_2"),
		getSprite("boss_3"),
	}, 0.0, 1, 0, 1, "boss", 1, boss });

	// Arrays used for 3D sprite sorting
	spriteDist.resize(sprites3d.size());
	spriteOrder.resize(sprites3d.size());
}

// Returns the distance between two points (who would've guessed)
double distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Set up game for pause screen
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

		printText("Paused", 56, 10, UPPER);

		menu = 5;
		selectedButton = 0;
	} else {
		clearUI();
		menu = 0;
	}
}

// Sorting function used for 3D sprite sorting
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

// Returns true if player is collided with a 3D sprite
bool isSpriteCollided(double x1, double x2) {
	for (int i = 0; i < sprites3d.size(); i++) {
		if (sprites3d[i].active && distance(x1, x2, sprites3d[i].position.x, sprites3d[i].position.y) <= sprites3d[i].collisionRadius)
			return true;
	}
	return false;
}


// Set up game for a battle!
void battleTransition() {
	selectedButton = 0;
	menu = 3;
	mergeBuffers();
	clearUI();
	nextMenu = 6;
	enemy en = sprites3d[lookSprite].enemyType;
	battleData.eStats = {
		en.maxHp,		// HP
		en.maxHp,		// Max HP
		en.strength,
		en.defense,
		en.attacks,
		en.name,
		en.xp
	};
	battleData.prevEHp = battleData.eStats.hp;
	battleData.turn = 0;
	battleData.battleMenu = 0;
	battleData.scrollPos = 0;
	battleData.timerFrame = frame;
	maxMenuItems = 3;
}

// Set up game for game over :(
void gameOver() {
	menu = 8;
	battleData.timerFrame = frame;
	maxMenuItems = 2;
	selectedButton = 0;
}

// Removes an item from the player's inventory
void removeItem(char* name) {
	for (int i = 0; i < pStats.items.size(); i++) {
		if (strcmp(pStats.items[i].name, name) == 0) {
			pStats.items.erase(pStats.items.begin() + i);
			break;
		}
	}
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

			if (rotSprite.active) {
				if (rotSprite.rotation > 180)
					sprites3d[spriteOrder[i]].rotation = -180;
				if (rotSprite.rotation < -180)
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
							if (texY < drawSprite.height && texX < drawSprite.width && texY > -1 && texX > -1) {
								WORD color = drawSprite.colors[drawSprite.width * texY + texX];
								wchar_t character = drawSprite.chars[drawSprite.width * texY + texX];
								if (color > 0) {
									if (x == SCREEN_WIDTH / 2 && y == SCREEN_HEIGHT / 2)
										lookSprite = spriteOrder[i];
									draw(x, y, character, color - 1);
								}
								if (x == SCREEN_WIDTH / 2 && y == SCREEN_HEIGHT / 2)
									frontSprite = spriteOrder[i];
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

// Draws a value bar to the UI buffer
void drawHealthBar(int x, int y, int width, int hp, int maxHp, WORD color) {
	int actualWidth = width * ((double)hp / (double)maxHp);
	wchar_t characters[3] = {
		PIXEL_SHADE0,
		PIXEL_SHADE1,
		PIXEL_SHADE1
	};
	WORD gray = (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED) + 1;
	for (int i = 0; i < 3; i++) {
		drawUI(x, y + i+1, PIXEL_SHADE3, 1);
		drawUI(x+width+1, y + i + 1, PIXEL_SHADE3, 1);
		for (int j = 0; j < actualWidth; j++) {
			if (i == 2) color ^= FOREGROUND_INTENSITY;
			drawUI(j + x + 1, i + y + 1, characters[i], color);
		}
		for (int j = 0; j < width - actualWidth; j++) {
			drawUI(j + x + 1 + actualWidth, i + y + 1, PIXEL_SHADE3, gray);
		}
	}
	horizLineUI(y, x, x + width + 1, PIXEL_SHADE3, 1);
	horizLineUI(y + 4, x, x + width + 1, PIXEL_SHADE3, 1);
}

// Used for screen wipe transition
void readyForClrTrans() {
	mergeBuffers();
	transBuffer = new CHAR_INFO[BUFFER_SIZE];
	memset(transBuffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
	memcpy(transBuffer, buffer, BUFFER_SIZE * sizeof(CHAR_INFO));
	memset(buffer, 0, BUFFER_SIZE * sizeof(CHAR_INFO));
	menu = 4;
	selectedButton = 0;
	clearUI();
}

// Called each frame, used for drawing enemy and its animations
void enemyTick(int newHp, int x, int y, double scale, sprite spr) {
	vec2Int offset = { 0, 0 };
	if (newHp != battleData.eStats.hp) {
		int diff = abs(battleData.eStats.hp - newHp);
		bool diffPos = newHp > battleData.eStats.hp;
		battleData.eStats.hp += (diffPos ? 1 : -1);
		if (!diffPos) {
			offset.x = (rand() % 3 - 1) * sqrt(diff);
			offset.y = (rand() % 3 - 1) * sqrt(diff);
		}
	}
	drawSpriteScaled(x + offset.x, y + offset.y, scale, spr);
}

// Get number of unique items in player's inventory
int getNumStackedItems() {
	int num = 0;
	vector<string> tmpItems;
	for (int i = 0; i < pStats.items.size(); i++) {
		if (find(tmpItems.begin(), tmpItems.end(), pStats.items[i].name) == tmpItems.end()) {
			num++;
			tmpItems.push_back(pStats.items[i].name);
		}
	}

	return num;
}

// Gets the number of a specific item the player has
int getNumItems(char* itemName) {
	int num = 0;
	for (int i = 0; i < pStats.items.size(); i++) {
		if (strcmp(pStats.items[i].name, itemName) == 0) num++;
	}

	return num;
}

// Returns a vector of unique items only
vector<item> getStackedItems() {
	vector<item> stacked;
	vector<string> itemNames;
	for (int i = 0; i < pStats.items.size(); i++) {
		if (find(itemNames.begin(), itemNames.end(), pStats.items[i].name) == itemNames.end()) {
			itemNames.push_back(pStats.items[i].name);
			stacked.push_back(pStats.items[i]);
		}
	}

	return stacked;
}

// Runs every frame (from gameEngine.h)
void update() {
	bool action = keys[0x0D].pressed || keys[VK_SPACE].pressed;

	switch (menu) {
	// Main environment
	case 0: {
		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		if (paused == false) {
			// Clear screen
			clearScreen();
			clearUI();

			moveSpeed = 20 * deltaTime;
			rotSpeed = 4 * deltaTime * rotIntensity;
			velocity = lerp(velocity, 0, deltaTime * 10);

			// Key input (movement, rotation, interaction)
			double ppx = 0;
			double ppy = 0;
			// Key W is down
			if (keys[0x57].held) {
				velocity = lerp(velocity, 1, deltaTime * 3);

				ppx = playerPos.x + playerDir.x * moveSpeed;
				ppy = playerPos.y;
				//if ((worldMap[int(ppx)][int(ppy)] == false || tileTimers[int(ppx)][int(ppy)] == 0.0) && isSpriteCollided(ppx, ppy) == false)
					playerPos.x += playerDir.x * moveSpeed * velocity;
				ppx = playerPos.x;
				ppy = playerPos.y + playerDir.y * moveSpeed;
				//if ((worldMap[int(ppx)][int(ppy)] == false || tileTimers[int(ppx)][int(ppy)] == 0.0) && isSpriteCollided(ppx, ppy) == false)
					playerPos.y += playerDir.y * moveSpeed * velocity;
			}
			// Key S is down
			if (keys[0x53].held) {
				velocity = lerp(velocity, 1, deltaTime * 3);

				ppx = playerPos.x - playerDir.x * moveSpeed;
				ppy = playerPos.y;
				//if ((worldMap[int(ppx)][int(ppy)] == false || tileTimers[int(ppx)][int(ppy)] == 0.0) && isSpriteCollided(ppx, ppy) == false)
					playerPos.x -= playerDir.x * moveSpeed * velocity;
				ppx = playerPos.x;
				ppy = playerPos.y - playerDir.y * moveSpeed;
				//if ((worldMap[int(ppx)][int(ppy)] == false || tileTimers[int(ppx)][int(ppy)] == 0.0) && isSpriteCollided(ppx, ppy) == false)
					playerPos.y -= playerDir.y * moveSpeed * velocity;
			}
			// Key A is down
			if (keys[0x44].held) {
				double oldDirX = playerDir.x;
				playerDir.x = playerDir.x * cos(-rotSpeed) - playerDir.y * sin(-rotSpeed);
				playerDir.y = oldDirX * sin(-rotSpeed) + playerDir.y * cos(-rotSpeed);
				double oldPlaneX = camPlane.x;
				camPlane.x = camPlane.x * cos(-rotSpeed) - camPlane.y * sin(-rotSpeed);
				camPlane.y = oldPlaneX * sin(-rotSpeed) + camPlane.y * cos(-rotSpeed);
				rotIntensity = lerp(rotIntensity, 1, deltaTime * 7);
			}
			// Key D is down
			if (keys[0x41].held) {
				double oldDirX = playerDir.x;
				playerDir.x = playerDir.x * cos(rotSpeed) - playerDir.y * sin(rotSpeed);
				playerDir.y = oldDirX * sin(rotSpeed) + playerDir.y * cos(rotSpeed);
				double oldPlaneX = camPlane.x;
				camPlane.x = camPlane.x * cos(rotSpeed) - camPlane.y * sin(rotSpeed);
				camPlane.y = oldPlaneX * sin(rotSpeed) + camPlane.y * cos(rotSpeed);
				rotIntensity = lerp(rotIntensity, 1, deltaTime * 7);
			}
			// Key E was pressed
			if (keys[0x45].pressed) {
				
			}
			if (!keys[0x44].held && !keys[0x41].held) rotIntensity = 0;

			if (keys[VK_F1].pressed) {
				saveSaveFile();
			}
			if (keys[VK_F2].pressed) {
				loadSaveFile();
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

			// --Draw UI--
			// Draw crosshair
			drawSprite(SCREEN_WIDTH / 2 - 8, SCREEN_HEIGHT / 2 - 8, getUiSprite("crosshair"));

			// Draw viewmodel
			//drawSprite(SCREEN_WIDTH - 54 + int(sin((double)frame / 8) * 10 * bobIntensity), SCREEN_HEIGHT - 54 + int(sin((double)frame / 4) * 5 * bobIntensity), getUiSprite("gun1"));

			if (getFlag("getitem")) {
				drawRectUI(0, 5, SCREEN_WIDTH, 17, PIXEL_SHADE0, FOREGROUND_BLUE);
				sprintf(dbg, "%s", messageString.c_str());
				printText(dbg, 6, 9, UPPER);
			}

			if (DISPLAY_FPS) {
				sprintf(dbg, "%d", int(1 / deltaTime));
				printText(dbg, 1, 1, UPPER);
			}

			//sprintf(dbg, "%f, %f", playerPos.x, playerPos.y);
			//printText(dbg, 1, 1, LOWER);

			// Key Space was pressed
			if (keys[VK_SPACE].pressed) {
				// If sprite that the player is directly looking at is enemy, start battle
				if (lookSprite >= 0 && sprites3d[lookSprite].type == 1)
					battleTransition();
				// If sprite that the player is facing (doesn't have to be directly looking at) is a chest, open it
				if (frontSprite >= 0 && sprites3d[frontSprite].name == "chest" && sprites3d[frontSprite].type == 2) {
					sprites3d[frontSprite].active = false;
					if (sprites3d[frontSprite].obtain.oAttack != NULL) {
						pStats.attacks.push_back(*(sprites3d[frontSprite].obtain.oAttack));
						sprintf(dbg, "Received %s.", (*(sprites3d[frontSprite].obtain.oAttack)).name);
						messageString = dbg;
					}
					else if (sprites3d[frontSprite].obtain.oItem != NULL) {
						pStats.items.push_back(*(sprites3d[frontSprite].obtain.oItem));
						sprintf(dbg, "Received %s.", (*(sprites3d[frontSprite].obtain.oItem)).name);
						messageString = dbg;
					}
					setFlag("getitem", 75);
				}
			}
		}

		if (keys[0x1B].pressed) {
			// Key ESC was pressed
			pauseGame();
		}

		if (transFlag) {
			readyForClrTrans();
		}

		break;
	}
	// Title screen
	case 1: {
		paused = false;

		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		fillScreen(PIXEL_SHADE2, FOREGROUND_RED);
		clearUI();
		maxMenuItems = 3;

		drawSpriteTransparent(0, 0, getUiSprite("blast"));

		// Logo
		drawSpriteTransparent(SCREEN_WIDTH / 2 - uiSprites[4].width / 2, 10, uiSprites[4]);

		// Start button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[1].width / 2, SCREEN_HEIGHT / 2 - uiSprites[1].height / 2 - 5, uiSprites[1]);
		// Options button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[5].width / 2, SCREEN_HEIGHT / 2 - uiSprites[5].height / 2 + 13, uiSprites[5]);
		// Quit button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[2].width / 2, SCREEN_HEIGHT / 2 - uiSprites[2].height / 2 + 31, uiSprites[2]);

		// Draw selection arrow and border
		char arrowFrame[7];
		sprintf(arrowFrame, "arrow%d", ((frame / 4) % 8));
		int buttonLoc = SCREEN_HEIGHT / 2 - uiSprites[3].height / 2 + (18 * selectedButton) - 5;
		drawSpriteTransparent(SCREEN_WIDTH / 2 - uiSprites[3].width / 2, buttonLoc, uiSprites[3]);
		drawSpriteTransparent(30, buttonLoc, getSprite(arrowFrame));

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > maxMenuItems - 1) selectedButton = 0;
		if (selectedButton < 0) selectedButton = maxMenuItems - 1;

		drawRectUI(SCREEN_WIDTH - uiSprites[6].width + 1, SCREEN_HEIGHT - uiSprites[6].height - 1, SCREEN_WIDTH - uiSprites[6].width - 1, SCREEN_HEIGHT - uiSprites[6].height + 1, PIXEL_SHADE0, 1);

		// Created by text
		drawSpriteTransparent(SCREEN_WIDTH - uiSprites[6].width - 1, SCREEN_HEIGHT - uiSprites[6].height - 1, uiSprites[6]);

		// Capture enter key to select button
		if (action) {
			if (selectedButton == 0) {
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 9;
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
			readyForClrTrans();
		}

		break;
	}
	// Options menu
	case 2: {
		bool transFlag = false;
		if (buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		maxMenuItems = 2;

		fillScreen(PIXEL_SHADE2, FOREGROUND_RED);
		printText("Noise Reduction", 5, 8, UPPER);

		drawSprite(SCREEN_WIDTH - 37, 5, (getConfigValue("NOISE_REDUCTION") == "1") ? uiSprites[8] : uiSprites[7]);

		// Back button
		drawSprite(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[10]);

		if (selectedButton == 0)
			drawSpriteTransparent(SCREEN_WIDTH - 37, 5, uiSprites[9]);
		if (selectedButton == 1)
			drawSpriteTransparent(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[3]);

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > maxMenuItems - 1) selectedButton = 0;
		if (selectedButton < 0) selectedButton = maxMenuItems - 1;

		// Capture enter key to select button
		if (action) {
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
			readyForClrTrans();
		}

		break;
	}
	// Transition from menu to black screen
	case 3: {
		srand(3);
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			int rnd = rand();
			int speed = (rnd % 8 + 8);
			for (int j = 0; j < speed; j++) {
				draw(SCREEN_WIDTH + 32 + j - (selectedButton * speed) - (rnd % 32 + 1), i, PIXEL_SHADE0, 0);
			}
		}

		selectedButton++;

		if (selectedButton >= SCREEN_WIDTH / 2)
			menu = nextMenu;

		break;
	}
	// Transition from black to new menu
	case 4: {
		srand(4);
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			int rnd = rand();
			int speed = (rnd % 8 + 8);
			for (int j = 0; j < speed; j++) {
				int x = SCREEN_WIDTH + 32 + j - (selectedButton * speed) - (rnd % 32 + 1);
				int index = i * SCREEN_WIDTH + x;
				if (index > -1 && index < BUFFER_SIZE)
					draw(x, i, transBuffer[index].Char.UnicodeChar, transBuffer[index].Attributes);
			}
		}

		selectedButton++;

		if (selectedButton >= SCREEN_WIDTH / 2) {
			menu = nextMenu;
			delete[] transBuffer;
			selectedButton = 0;
			srand(time(0));
		}

		break;
	}
	// Pause screen
	case 5: {
		maxMenuItems = 3;

		// Resume button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[11].width / 2, SCREEN_HEIGHT / 2 - uiSprites[11].height / 2 - 18, uiSprites[11]);
		// Stats button
		drawSprite(SCREEN_WIDTH / 2 - getUiSprite("button_stats").width / 2, SCREEN_HEIGHT / 2 - getUiSprite("button_stats").height / 2, getUiSprite("button_stats"));
		// Quit button
		drawSprite(SCREEN_WIDTH / 2 - uiSprites[2].width / 2, SCREEN_HEIGHT / 2 - uiSprites[2].height / 2 + 18, uiSprites[2]);

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > maxMenuItems - 1) selectedButton = 0;
		if (selectedButton < 0) selectedButton = maxMenuItems - 1;

		// Draw selection border
		int buttonLoc = SCREEN_HEIGHT / 2 - uiSprites[3].height / 2 + (18 * selectedButton) - 18;
		drawSpriteTransparent(SCREEN_WIDTH / 2 - uiSprites[3].width / 2, buttonLoc, uiSprites[3]);

		// Capture enter key to select button
		if (action) {
			if (selectedButton == 0)
				pauseGame();
			if (selectedButton == 1) {
				selectedButton = 0;
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 7;
			}
			if (selectedButton == 2) {
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

		break;
	}
	// Battle screen
	case 6: {
		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		if(battleData.eStats.name == "LowEye")
			fillScreen(PIXEL_SHADE1, FOREGROUND_RED);
		else
			fillScreen(PIXEL_SHADE1, FOREGROUND_RED | FOREGROUND_BLUE);
		clearUI();

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) {
			if (battleData.scrollPos - selectedButton == 0 && selectedButton > 0)
				battleData.scrollPos--;
			selectedButton--;
		}
		if (keys[0x53].pressed) {
			if (selectedButton - battleData.scrollPos == 2 && selectedButton < maxMenuItems - 1)
				battleData.scrollPos++;
			selectedButton++;
		}
		if (battleData.battleMenu > 0) {
			if (selectedButton > maxMenuItems - 1) {
				selectedButton = 0;
				battleData.scrollPos = 0;
			}
			if (selectedButton < 0) {
				selectedButton = maxMenuItems - 1;
				battleData.scrollPos = maxMenuItems - 3;
			}
		}
		else {
			if (selectedButton > maxMenuItems - 1) selectedButton = maxMenuItems - 1;
			if (selectedButton < 0) selectedButton = 0;
		}

		int sprWidth = sprites3d[lookSprite].sprites[0].width;
		int sprHeight = sprites3d[lookSprite].sprites[0].height;
		double sprScale = 64.0 / sprHeight;


		enemyTick(battleData.prevEHp, SCREEN_WIDTH / 2 - sprWidth * sprScale / 2, SCREEN_HEIGHT / 2 - sprHeight * sprScale / 2 - 10, sprScale, sprites3d[lookSprite].sprites[0]);


		// ---Begin drawing battle menu---
		vec2Int menuPos = { 2, SCREEN_HEIGHT - 32 };

		if (getFlag("attackPAnim")) {
			menuPos.x += rand() % 3 - 1;
			menuPos.y += rand() % 3 - 1;
			if (getFlagFramesLeft("attackPAnim") <= 1) battleData.turn = 0;
		}

		drawSprite(menuPos.x, menuPos.y, getUiSprite("battle_menu"));
		drawSpriteTransparent(menuPos.x + 4 + (selectedButton == 0 && battleData.battleMenu == 0 && battleData.turn == 0 ? 6 : 0), menuPos.y + 4, getUiSprite("battle_menu_attack"));
		drawSpriteTransparent(menuPos.x + 4 + (selectedButton == 1 && battleData.battleMenu == 0 && battleData.turn == 0 ? 6 : 0), menuPos.y + 12, getUiSprite("battle_menu_abilities"));
		drawSpriteTransparent(menuPos.x + 4 + (selectedButton == 2 && battleData.battleMenu == 0 && battleData.turn == 0 ? 6 : 0), menuPos.y + 20, getUiSprite("battle_menu_items"));

		sprintf(dbg, "HP: %d/%d", pStats.hp, pStats.maxHp);
		printText(dbg, menuPos.x + 64, menuPos.y + 4, DEFAULT, "chars_small");

		sprintf(dbg, "AP: %d/%d", pStats.ap, pStats.maxAp);
		printText(dbg, menuPos.x + 64, menuPos.y + 12, DEFAULT, "chars_small");


		// Player health bar
		drawHealthBar(menuPos.x + 118, menuPos.y + 4, 32, pStats.hp, pStats.maxHp, (FOREGROUND_GREEN | FOREGROUND_INTENSITY) + 1);
		// Player AP bar
		drawHealthBar(menuPos.x + 118, menuPos.y + 12, 32, pStats.ap, pStats.maxAp, (FOREGROUND_RED | FOREGROUND_INTENSITY) + 1);

		if (getFlag("potionFx")) {
			for (int y = -1; y < 6; y++) {
				for (int x = -1; x < 35; x++) {
					int rnd = rand() % (int((100 - (double)getFlagFramesLeft("potionFx")) / 10) + 1);
					if (rnd == 0) drawUI(x + menuPos.x + 118, y + menuPos.y + 4, PIXEL_SHADE0, (FOREGROUND_GREEN | FOREGROUND_INTENSITY) + 1);
				}
			}
		}
		if (getFlag("energyFx")) {
			for (int y = -1; y < 6; y++) {
				for (int x = -1; x < 35; x++) {
					int rnd = rand() % (int((100 - (double)getFlagFramesLeft("energyFx")) / 10) + 1);
					if (rnd == 0) drawUI(x + menuPos.x + 118, y + menuPos.y + 12, PIXEL_SHADE0, (FOREGROUND_RED | FOREGROUND_INTENSITY) + 1);
				}
			}
		}

		if (battleData.battleMenu == 1) {
			drawSpriteTransparent(menuPos.x + 16, menuPos.y - 8, getUiSprite("battle_menu_half"));

			int sz = 3;
			if (maxMenuItems < sz) sz = maxMenuItems;
			for (int i = battleData.scrollPos; i < battleData.scrollPos + sz; i++) {
				printText(pStats.attacks[i].name, menuPos.x + 20 + (selectedButton == i ? 6 : 0), menuPos.y - 4 + 8 * (i - battleData.scrollPos), DEFAULT, "chars_small");
			}

			if (keys[VK_BACK].pressed) {
				battleData.battleMenu = 0;
				selectedButton = 1;
				maxMenuItems = 3;
				battleData.scrollPos = 0;
			}
		}
		else if (battleData.battleMenu == 2) {
			drawSpriteTransparent(menuPos.x + 16, menuPos.y - 8, getUiSprite("battle_menu_half"));
			vector<item> stacked = getStackedItems();

			int sz = 3;
			if (stacked.size() < sz) sz = stacked.size();
			for (int i = battleData.scrollPos; i < battleData.scrollPos + sz; i++) {
				rect itemTextMask = {
					menuPos.x + 20,
					menuPos.y - 4 + 8 * (i - battleData.scrollPos),
					50,
					7
				};
				printText(stacked[i].name, menuPos.x + 20 + (selectedButton == i ? 6 : 0), menuPos.y - 4 + 8 * (i - battleData.scrollPos), DEFAULT, "chars_small", &itemTextMask);
				sprintf(dbg, "%d", getNumItems(stacked[i].name));
				printText(dbg, menuPos.x + 76, menuPos.y - 4 + 8 * (i - battleData.scrollPos), DEFAULT, "chars_small");
			}

			if (keys[VK_BACK].pressed) {
				battleData.battleMenu = 0;
				selectedButton = 2;
				maxMenuItems = 3;
				battleData.scrollPos = 0;
			}
		}

		// Draw selection arrow
		if (battleData.turn == 0) {
			char arrowFrame[7];
			sprintf(arrowFrame, "arrow%d", ((frame / 4) % 8));
			int buttonLoc = menuPos.y + 2 + ((selectedButton - battleData.scrollPos) * 8);
			if (battleData.battleMenu > 0)
				drawSpriteScaledTransparent(menuPos.x + 16, buttonLoc - 8, 0.5, getSprite(arrowFrame));
			else
				drawSpriteScaledTransparent(menuPos.x, buttonLoc, 0.5, getSprite(arrowFrame));
		}


		// ---End drawing battle menu---

		printText(battleData.eStats.name, 1, 1, DEFAULT, "chars_small");
		// Enemy health bar
		drawHealthBar(1, 8, 64, battleData.eStats.hp, battleData.eStats.maxHp, (FOREGROUND_GREEN | FOREGROUND_INTENSITY) + 1);
		sprintf(dbg, "HP: %d/%d", battleData.eStats.hp, battleData.eStats.maxHp);
		printText(dbg, 1, 15, DEFAULT, "chars_small");


		// Capture enter key to select action
		if (action) {
			if (battleData.battleMenu == 0) {
				if (selectedButton == 0 && battleData.turn == 0) {
					battleData.timerFrame = frame;
					if (selectedButton == 0) {
						battleData.turn = 1;
						battleData.prevEHp -= ceil((double)pStats.strength / battleData.eStats.defense) * (8 + (rand() % 2)) * ceil((double)pStats.level / 10);
					}
				}
				if (selectedButton == 1) {
					battleData.battleMenu = 1;
					selectedButton = 0;
					maxMenuItems = pStats.attacks.size();
					battleData.scrollPos = 0;
				}
				if (selectedButton == 2) {
					battleData.battleMenu = 2;
					selectedButton = 0;
					maxMenuItems = getNumStackedItems();
					battleData.scrollPos = 0;
				}
			}
			else if (battleData.battleMenu == 1) {
				if (pStats.attacks.size() > 0) {
					attack pAttack = pStats.attacks[selectedButton];
					if (pStats.ap >= pAttack.apCost) {
						battleData.prevEHp -= ceil((double)pStats.strength / battleData.eStats.defense) * (pAttack.power + (rand() % (pAttack.randomness + 1)));
						battleData.battleMenu = 0;
						selectedButton = 0;
						maxMenuItems = 3;
						battleData.turn = 1;
						battleData.timerFrame = frame;
						pStats.ap -= pAttack.apCost;
					}
					else {
						battleData.timerFrame = frame;
						setFlag("msgShown", 240);
					}
				}
				battleData.scrollPos = 0;
			}
			else if (battleData.battleMenu == 2) {
				if (pStats.items.size() > 0) {
					vector<item> stacked = getStackedItems();
					item pItem = stacked[selectedButton];
					pItem.action();
					battleData.battleMenu = 0;
					selectedButton = 0;
					maxMenuItems = 3;
					battleData.turn = 1;
					battleData.timerFrame = frame;
					removeItem(pItem.name);
				}
				battleData.scrollPos = 0;
			}
		}

		if (battleData.turn == 1) {
			if (battleData.eStats.hp - battleData.prevEHp > 0) battleData.timerFrame = frame;
			if (frame - battleData.timerFrame > 50) {
				int attackIndex = rand() % battleData.eStats.attacks.size();
				attack eAttack = battleData.eStats.attacks[attackIndex];
				pStats.hp -= ceil((double)battleData.eStats.strength / pStats.defense) * (eAttack.power + (rand() % eAttack.randomness));
				battleData.timerFrame = frame;
				setFlag("attackPAnim", 30);
			}
			removeFlag("msgShown");
		}
		else {
			if (!transFlag && getFlag("msgShown")) {
				printText("Not enough AP.", menuPos.x, menuPos.y - 16, DEFAULT, "chars_small");
			}
		}

		// If enemy HP is zero, end the battle and reward player
		if (battleData.eStats.hp <= 0) {
			selectedButton = 0;
			sprites3d[lookSprite].active = false;
			pStats.xp += battleData.eStats.xp;
			if (battleData.eStats.name == "LowEye") {
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 11;
			} else {
				menu = 3;
				mergeBuffers();
				clearUI();
				nextMenu = 0;
			}
			while (pStats.xp >= pStats.maxXp) {
				pStats.level++;
				pStats.xp = pStats.xp - pStats.maxXp;
				pStats.maxXp *= 1.5;

				sprintf(dbg, "Leveled up to LVL %d", pStats.level);
				messageString = dbg;
				setFlag("getitem", 125);
			}

			pStats.strength += floor(pStats.level / 2);
			pStats.defense += floor(pStats.level / 2);
		}

		//If player HP is zero, game over :(
		if (pStats.hp <= 0) {
			gameOver();
		}

		if (transFlag) {
			readyForClrTrans();
		}

		break;
	}
	// Player stats
	case 7: {
		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		fillScreen(PIXEL_SHADE2, FOREGROUND_RED);

		printText("Player Statistics", 4, 4, UPPER);

		sprintf(dbg, "HP: %d/%d", pStats.hp, pStats.maxHp);
		printText(dbg, 4, 20, DEFAULT, "chars_small");
		sprintf(dbg, "AP: %d/%d", pStats.ap, pStats.maxAp);
		printText(dbg, 4, 28, DEFAULT, "chars_small");
		sprintf(dbg, "XP: %d/%d", pStats.xp, pStats.maxXp);
		printText(dbg, 4, 36, DEFAULT, "chars_small");
		sprintf(dbg, "LVL: %d", pStats.level);
		printText(dbg, 4, 44, DEFAULT, "chars_small");

		drawSprite(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[10]);
		drawSpriteTransparent(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[3]);

		if (action) {
			menu = 0;
			clearUI();
			paused = false;
		}

		if (transFlag) {
			readyForClrTrans();
		}

		break;
	}
	// Game over screen
	case 8: {
		clearScreen();
		clearUI();

		int tx = 47, ty = SCREEN_HEIGHT / 2 - 3 + battleData.scrollPos;
		int dframe = frame - battleData.timerFrame;

		if (dframe > 300 && battleData.scrollPos > -40) battleData.scrollPos--;

		printText("Game Over", tx, ty, UPPER);

		// Fade in text animation
		int twidth = 70, theight = 10;
		for (int y = ty; y < ty + theight; y++) {
			for (int x = tx; x < tx + twidth; x++) {
				int index = y * SCREEN_WIDTH + x;
				if (uiBuffer[index].Char.UnicodeChar > 0) {
					if (dframe < 51) uiBuffer[index].Char.UnicodeChar = 0;
					if (dframe > 50 && dframe < 101) uiBuffer[index].Char.UnicodeChar = PIXEL_SHADE3;
					if (dframe > 100 && dframe < 151) uiBuffer[index].Char.UnicodeChar = PIXEL_SHADE2;
					if (dframe > 150 && dframe < 201) {
						uiBuffer[index].Char.UnicodeChar = PIXEL_SHADE1;
						uiBuffer[index].Attributes ^= FOREGROUND_INTENSITY;
					}
					if (dframe > 250) uiBuffer[index].Char.UnicodeChar = PIXEL_SHADE0;
				}
			}
		}

		// When Game Over text has reached the top
		if (battleData.scrollPos <= -40) {
			// Capture key presses for selection (W and S)
			if (keys[0x57].pressed) selectedButton--;
			if (keys[0x53].pressed) selectedButton++;
			if (selectedButton > maxMenuItems - 1) selectedButton = 0;
			if (selectedButton < 0) selectedButton = maxMenuItems - 1;

			drawSprite(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 17, getUiSprite("button_continue"));
			drawSprite(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 1, getUiSprite("button_quitgame"));

			// Draw selection border
			int buttonLoc = SCREEN_HEIGHT / 2 + (18 * selectedButton) - 17;
			drawSpriteTransparent(SCREEN_WIDTH / 2 - 50, buttonLoc, getUiSprite("button_border_large"));

			if (selectedButton == 0) {
				printText("Continue from last save.", 29, 100, DEFAULT, "chars_small");
				if (action) {
					menu = 0;
					loadSaveFile();
				}
			}
			else if (selectedButton == 1) {
				printText("Close the game.", 48, 100, DEFAULT, "chars_small");
				if (action) {
					exit(0);
				}
			}
		}
		else {
			if (action) {
				battleData.scrollPos = -40;
			}
		}

		break;
	}
	// New/Load screen
	case 9: {
		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		fillScreen(PIXEL_SHADE2, FOREGROUND_RED);
		clearUI();
		maxMenuItems = 3;

		// New game button
		drawSprite(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 17, getUiSprite("button_newgame"));
		// Continue button
		drawSprite(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 1, getUiSprite("button_continue"));
		// Back button
		drawSprite(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[10]);

		// Draw selection arrow and border
		char arrowFrame[7];
		sprintf(arrowFrame, "arrow%d", ((frame / 4) % 8));

		// Draw selection border
		if (selectedButton == 2) {
			drawSprite(73, SCREEN_HEIGHT - 19, getSprite(arrowFrame));
			drawSpriteTransparent(SCREEN_WIDTH - 69, SCREEN_HEIGHT - 19, uiSprites[3]);
		}
		else {
			int buttonLoc = SCREEN_HEIGHT / 2 + (18 * selectedButton) - 17;
			drawSpriteTransparent(SCREEN_WIDTH / 2 - 50, buttonLoc, getUiSprite("button_border_large"));
			drawSprite(12, buttonLoc, getSprite(arrowFrame));
		}

		// Capture key presses for selection (W and S)
		if (keys[0x57].pressed) selectedButton--;
		if (keys[0x53].pressed) selectedButton++;
		if (selectedButton > maxMenuItems - 1) selectedButton = 0;
		if (selectedButton < 0) selectedButton = maxMenuItems - 1;

		// Capture enter key to select button
		if (action) {
			if (selectedButton == 0) {
				menu = 3;
				mergeBuffers();
				nextMenu = 10;
				battleData.timerFrame = frame;
				clearUI();
			}
			if (selectedButton == 1) {
				menu = 0;
				clearUI();
				loadSaveFile();
			}
			if (selectedButton == 2) menu = 1;
			selectedButton = 0;
		}

		if (transFlag) {
			readyForClrTrans();
		}

		break;
	}
	// Story intro screen
	case 10: {
		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		clearScreen();
		clearUI();

		draw(0, 0, PIXEL_SHADE3, FOREGROUND_RED);

		int dframe = frame - battleData.timerFrame;
		vec2 textPos = { 10, 20 };

		printText("Spaceship 451", textPos.x, textPos.y + 0, DEFAULT, "chars_small");
		printText("Hey so the aliens on our ship", textPos.x, textPos.y + 16, DEFAULT, "chars_small");
		printText("have taken over and we'd like", textPos.x, textPos.y + 24, DEFAULT, "chars_small");
		printText("to reposess custody of it. If", textPos.x, textPos.y + 32, DEFAULT, "chars_small");
		printText("you could take care of that,", textPos.x, textPos.y + 40, DEFAULT, "chars_small");
		printText("that'd be great.", textPos.x, textPos.y + 48, DEFAULT, "chars_small");

		printText("Thanks,", textPos.x, textPos.y + 64, DEFAULT, "chars_small");
		printText("Management", textPos.x, textPos.y + 72, DEFAULT, "chars_small");

		if (dframe > 700) {
			menu = 3;
			mergeBuffers();
			clearUI();
			nextMenu = 0;
		}

		if (transFlag) {
			readyForClrTrans();
		}

		break;
	}
	// Endgame text menu
	case 11: {
		bool transFlag = false;
		if (frame > 0 && buffer[0].Attributes == 0 && buffer[BUFFER_SIZE - 1].Attributes == 0)
			transFlag = true;

		clearScreen();
		clearUI();

		draw(0, 0, PIXEL_SHADE3, FOREGROUND_RED);

		int dframe = frame - battleData.timerFrame;
		vec2 textPos = { 10, 20 };

		printText("Congratulations on completing", textPos.x, textPos.y + 8, DEFAULT, "chars_small");
		printText("Spaceship 451. You have passed", textPos.x, textPos.y + 16, DEFAULT, "chars_small");
		printText("our test. Now begins the real", textPos.x, textPos.y + 24, DEFAULT, "chars_small");
		printText("journey.", textPos.x, textPos.y + 32, DEFAULT, "chars_small");

		printText("From,", textPos.x, textPos.y + 48, DEFAULT, "chars_small");
		printText("Unknown", textPos.x, textPos.y + 56, DEFAULT, "chars_small");

		if (dframe > 800) {
			menu = 3;
			mergeBuffers();
			clearUI();
			nextMenu = 1;
		}

		if (transFlag) {
			readyForClrTrans();
		}

		break;
	}
	}

	// Update flags
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i].endFrame <= frame) {
			flags.erase(flags.begin() + i);
		}
	}
}