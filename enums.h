#ifndef ENUMS
#define ENUMS

typedef enum {
	STR,
	AGI,
	INT,
	END,
	LCK,
	NUM_STATS
} StatID;

typedef enum {
	WARRIOR,
	WIZARD,
	NUM_CLASSES
} ClassEnum;

typedef enum {
	NONE,
	BROKEN_SWORD,
	SHORT_SWORD,
	NUM_ITEMS
} ItemID;

#endif