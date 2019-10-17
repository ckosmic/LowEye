#include "gameEngine.h"

struct attack {
	int power;
	int type;					// (0 - ability)
	int randomness;
	int apCost;
	char* name;
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
	vector<attack> attacks;
	int xp;
	int maxXp;
	int level;
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

const attack BASIC = {
	3,									// Power
	0,									// Type of attack
	1,									// Randomness addition to power
	0,									// AP cost
	"Attack"							// Name of attack
};
const attack FLAME = {
	5,									// Power
	0,									// Type of attack
	2,									// Randomness addition to power
	5,
	"Flame"								// Name of attack
};
const attack LASER_GUN = {
	4,
	0,
	2,
	4,
	"Laser Gun"
};
const attack SUPER = {
	100,
	0,
	0,
	0,
	"Super"
};