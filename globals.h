#ifndef GLOBALS
#define GLOBALS

#include "types.h"

Stat STATS[] = {
	(Stat) {.name="Strength"},
	(Stat) {.name="Agility"},
	(Stat) {.name="Intelligence"},
	(Stat) {.name="Endurance"},
	(Stat) {.name="Luck"},
};

Class CLASSES[] = {
	(Class) {.name="Warrior", .base_hp=40},
	(Class) {.name="Wizard", .base_hp=25},
};

Item ITEMS[] = {
	(Item) {.name="NONE", .level_requirement=0, .durability=-1},
	(Item) {.name="Broken Sword", .level_requirement=0, .durability=100},
	(Item) {.name="Short Sword", .level_requirement=5, .durability=500},
};

#endif