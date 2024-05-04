// typedef enum {
//     WARRIOR,
//     WIZARD
// } Class;

// typedef enum {
//     STR,
//     END,
//     NUM_STATS // Number of stats
// } StatID;

// Item ITEMS[] = {
//     (Item) {.name="NULL", .level_requirement=0, .durability=-1},
//     (Item) {.name="Broken Sword", .level_requirement=0, .durability=100},
//     (Item) {.name="Short Sword", .level_requirement=5, .durability=500},
// }; // Change to be in a file later

// typedef enum {
//     WARRIOR,
//     WIZARD
// } Class;

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
    FILE* enums = fopen("enums.h", "w");
    FILE* globals = fopen("globals.h", "w");

    char input[200];
    char enum_name[100];
    char name[100];

    FILE* stats = fopen("data/stats.data", "r");
    fputs("#ifndef ENUMS\n#define ENUMS\n\ntypedef enum {\n", enums);
    fputs("#ifndef GLOBALS\n#define GLOBALS\n\n#include \"types.h\"\n\nStat STATS[] = {\n", globals);
    while (fgets(input, 200, stats) != NULL) {
        if (strncmp(input, "//", 2) == 0) {
            continue;
        } else {
            sscanf(input, "%[^,],%[^,\n]", enum_name, name);
            fprintf(enums, "\t%s,\n", enum_name);
            fprintf(globals, "\t(Stat) {.name=\"%s\"},\n", name);
        }
    }
    fputs("};\n\n", globals);
    fputs("\tNUM_STATS\n} StatID;\n\n", enums);
    fclose(stats);

    FILE* classes = fopen("data/classes.data", "r");
    fputs("typedef enum {\n", enums);
    fprintf(globals, "Class CLASSES[] = {\n");
    int base_hp;
    while (fgets(input, sizeof(input), classes) != NULL) {
        if (strncmp(input, "//", 2) == 0) {
            continue;
        } else {
            sscanf(input, "%[^,],%[^,\n],%d", enum_name, name, &base_hp);
            fprintf(enums, "\t%s,\n", enum_name);
            fprintf(globals, "\t(Class) {.name=\"%s\", .base_hp=%d},\n", name, base_hp);
        }
    }
    fputs("};\n\n", globals);
    fputs("\tNUM_CLASSES\n} ClassEnum;\n\n", enums);
    fclose(classes);

    FILE* items = fopen("data/items.data", "r");
    fputs("typedef enum {\n", enums);
    int level_requirement, durability;
    fprintf(globals, "Item ITEMS[] = {\n");
    while (fgets(input, sizeof(input), items) != NULL) {
        if (strncmp(input, "//", 2) == 0) {
            continue;
        } else {
            sscanf(input, "%[^,],%[^,],%d,%d", enum_name, name, &level_requirement, &durability);
            fprintf(enums, "\t%s,\n", enum_name);
            fprintf(globals, "\t(Item) {.name=\"%s\", .level_requirement=%d, .durability=%d},\n", name, level_requirement, durability);
        }
    }
    fputs("};\n\n#endif", globals);
    fputs("\tNUM_ITEMS\n} ItemID;\n\n#endif", enums);
    fclose(items);

    fclose(globals);
    fclose(enums);

    return 0;
};