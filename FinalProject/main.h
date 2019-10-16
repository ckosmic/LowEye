#include "gameEngine.h"

struct attack {
	int power;
	int type;					// (0 - ability)
	int randomness;
	char* name;
};

struct enemy {
	int maxHp;
	int strength;
	int defense;
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
	int strength;
	int defense;
	vector<attack> attacks;
};

struct enemyStats {
	int hp;
	int maxHp;
	int strength;
	int defense;
	vector<attack> attacks;
	char* name;
};

const attack BASIC = {
	3,									// Power
	0,									// Type of attack
	1,									// Randomness addition to power
	"Attack"							// Name of attack
};
const attack FLAME = {
	5,									// Power
	0,									// Type of attack
	2,									// Randomness addition to power
	"Flame"								// Name of attack
};
const attack LASER_GUN = {
	4,
	0,
	2,
	"Laser Gun"
};