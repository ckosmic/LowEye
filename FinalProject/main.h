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
public:
	char name[MAX_PATH];

	item(const char* itemName, void(*func)(int)) {
		strcpy(name, itemName);
		fptr_int = func;
	}

	item(const char* itemName, void(*func)()) {
		strcpy(name, itemName);
		fptr_void = func;
	}

	void action(int val) {
		(*fptr_int)(val);
	}

	void action() {
		(*fptr_void)();
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

struct sprite3d {
	vec2 position;				// Position on the map
	int graphic;				// Which graphic to use when rotating
	sprite sprites[4];			// The rotation sprites
	double rotation;			// Rotation relative to the sprite
	double size;				// The scale of the sprite
	int offset;					// The Y offset of the sprite
	double collisionRadius;		// The radius of player-sprite collision
	char* name;					// Name of sprite3d
	int type;					// Type of sprite (0 - default, 1 - enemy)
	enemy enemyType;			// Type of enemy (used if type == 1)
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

struct obtainable {
	attack oAttack;

};

extern playerStats pStats;

#pragma region Attacks

const attack A_BASIC = {
	3,									// Power
	0,									// Type of attack
	1,									// Randomness addition to power
	0,									// AP cost
	"Attack"							// Name of attack
};
const attack A_FLAME = {
	10,									// Power
	0,									// Type of attack
	2,									// Randomness addition to power
	5,
	"Flame"								// Name of attack
};
const attack A_FREEZE = {
	10,
	0,
	2,
	5,
	"Freeze"
};
const attack A_LASER_GUN = {
	9,
	0,
	2,
	4,
	"Laser Gun"
};
const attack A_SUPER = {
	100,
	0,
	0,
	0,
	"Super"
};
const attack A_BITE = {
	5,
	0,
	3,
	100,
	"Bite"
};

#pragma endregion

#pragma region Items

void modifyPlayerHp(int amount) {
	pStats.hp += amount;
}

item I_POTION("Potion", modifyPlayerHp);

#pragma endregion

