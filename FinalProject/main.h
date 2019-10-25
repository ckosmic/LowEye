#include "gameEngine.h"

struct attack {
	int power;
	int type;					// (0 - ability)
	int randomness;
	int apCost;
	char* name;
};

class item {
private:
	void(*fptr_void)() = NULL;
	void(*fptr_int)(int) = NULL;
	int val = -69420;
public:
	char name[MAX_PATH];
	int quantity;

	item(const char* itemName, void(*func)(int), int v) {
		strcpy(name, itemName);
		fptr_int = func;
		val = v;
	}

	item(const char* itemName, void(*func)()) {
		strcpy(name, itemName);
		fptr_void = func;
	}

	void action() {
		if(val == -69420)
			(*fptr_void)();
		else
			(*fptr_int)(val);
	}
};

struct enemy {
	int maxHp;
	int strength;
	int defense;
	int xp;
	vector<attack> attacks;
	char* name;
};

struct obtainable {
	attack* oAttack;
	item* oItem;
};

struct sprite3d {
	vec2 position;				// Position on the map
	int graphic;				// Which graphic to use when rotating
	sprite sprites[4];			// The rotation sprites
	double rotation;			// Rotation relative to the sprite
	double size;				// The scale of the sprite
	int offset;					// The Y offset of the sprite
	double collisionRadius;		// The radius of player-sprite collision
	char* name;					// Name of sprite3d
	int type;					// Type of sprite (0 - default, 1 - enemy, 2 - chest)
	enemy enemyType;			// Type of enemy (used if type == 1)
	obtainable obtain;			// Obtainable item or attack (used if type == 2)
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
	int ap;
	int maxAp;
	int strength;
	int defense;
	int xp;
	int maxXp;
	int level;
	vector<attack> attacks;
	vector<item> items;
};

struct enemyStats {
	int hp;
	int maxHp;
	int strength;
	int defense;
	vector<attack> attacks;
	char* name;
	int xp;
};

struct flag {
	string name;
	int endFrame;
};

struct battle_data {
	enemyStats eStats;
	int prevEHp;
	int turn;
	int timerFrame;
	int battleMenu;
	int scrollPos;
} battleData;

extern playerStats pStats;
extern vector<flag> flags;

void setFlag(string name, int length);
void removeFlag(string name);
bool getFlag(string name);
int getFlagFramesLeft(string name);

#pragma region Attacks

attack A_BASIC = {
	3,									// Power
	0,									// Type of attack
	1,									// Randomness addition to power
	0,									// AP cost
	"Attack"							// Name of attack
};
attack A_FLAME = {
	10,									// Power
	0,									// Type of attack
	2,									// Randomness addition to power
	5,
	"Flame"								// Name of attack
};
attack A_FREEZE = {
	10,
	0,
	2,
	5,
	"Freeze"
};
attack A_LASER_GUN = {
	9,
	0,
	2,
	4,
	"Laser Gun"
};
attack A_SUPER = {
	100,
	0,
	0,
	0,
	"Super"
};
attack A_BITE = {
	5,
	0,
	3,
	100,
	"Bite"
};

#pragma endregion

#pragma region Items

void modifyPlayerHp(int amount) {
	setFlag("potionFx", 100);
	pStats.hp += amount;
	if (pStats.hp > pStats.maxXp) pStats.hp = pStats.maxHp;
}

item I_POTION("Potion", modifyPlayerHp, 50);
item I_TEST("Test", modifyPlayerHp, 0);

#pragma endregion

