#include <math.h>

#include "map.h"
#include "types.h"
#include "array.h"
#include "main.h"
#include "animate.h"

void create_map(string location) {
    FILE* map_file = fopen("data/map1.data", "rb");

    Header header;
    fread(&header, sizeof(Header), 1, map_file);
    assert(header.magic == 0x9125);
    assert(header.version >= 1);

    map = malloc(sizeof(Map) + sizeof(MapTile) * (header.size_x * header.size_y));
    map->size_x = header.size_x;
    map->size_y = header.size_y;

    map->start_x = header.start_x; // ADD TO FILE
    map->start_y = header.start_y;
    
    map->tiles = ((void *) map + sizeof(Map));

    for(int x = 0; x < map->size_x; x++) {
        for(int y = 0; y < map->size_y; y++) {
            fread(&map->tiles[y * map->size_x + x], sizeof(MapTile), 1, map_file);
        }
    }
}

void destroy_map(Map* map) {
    free(map);
}

typedef struct {
    SDL_FPoint key;
    SDL_FPoint value;
} FPointMap;

typedef struct {
    SDL_FPoint key;
    u32 value;
} ScoreMap;


float get_distance(SDL_FPoint start, SDL_FPoint end) {
    return sqrtf(powf(end.x - start.x, 2) + powf(end.y - start.y, 2));
}

float get_square_distance(SDL_FPoint start, SDL_FPoint end) {
    return fabs(end.x - start.x) + fabs(end.y - start.y);
}

int compareFPoint(const void * a, const void * b) {
    return memcmp(a, b, sizeof(SDL_FPoint));
}

int compareScore(const void * a, const void * b) {
    if (((ScoreMap*)a)->value < ((ScoreMap*)b)->value) return  1;
    if (((ScoreMap*)a)->value > ((ScoreMap*)b)->value) return -1;
    return 0;
}

// void* find(Array* arr, void* key, __compar_fn_t compar, void* default_value) {
//     void* item = bsearch(key, arr->memory, arr->length, arr->t_size, compar);
//     if(item == NULL) {
//         return default_value;
//     }

//     return item;
// }

#define get_neighour(current, i) \
    (i == 0 ? (SDL_FPoint) {current.x - 1, current.y} : \
     i == 1 ? (SDL_FPoint) {current.x + 1, current.y} : \
     i == 2 ? (SDL_FPoint) {current.x, current.y - 1} : \
              (SDL_FPoint) {current.x, current.y + 1})

Array* reconstruct_path(Array *cameFrom, SDL_FPoint* current) {
    Array* total_path = create_array(sizeof(SDL_FPoint), 50, 20, NULL);
    while(current != ((void*)8)) { // 8 is the location that -> value points to on a NULL return
        add(&total_path, current);
        current = &(((FPointMap*) find(cameFrom, current, compareFPoint, NULL))->value);
    }

    return total_path;
}

Array* a_star(SDL_FPoint start, SDL_FPoint goal) {
    Array* openSet = create_array(sizeof(SDL_FPoint), 100, 100, NULL);
    Array* cameFrom = create_array(sizeof(FPointMap), 100, 100, NULL);
    Array* gScore = create_array(sizeof(ScoreMap), 100, 100, NULL);
    Array* fScore = create_array(sizeof(ScoreMap), 100, 100, NULL);

    add(&gScore, &(ScoreMap) {start, 0});
    add(&fScore, &(ScoreMap) {start, get_square_distance(start, goal)});
    add(&openSet, &start);

    SDL_FPoint current;
    SDL_FPoint neighour;
    u64 tentative_gScore;
    ScoreMap default_value = {{0,0}, UINT32_MAX};
    while (openSet->length > 0) {
        current = *(SDL_FPoint*) get(openSet, openSet->length - 1);
        if(current.x == goal.x && current.y == goal.y){
            free(openSet);
            free(gScore);
            free(fScore);
            return reconstruct_path(cameFrom, &current);
        }

        delete(openSet, openSet->length - 1);
        for(int i = 0; i < 4; i++){
            neighour = get_neighour(current, i); // MAKE SURE THIS ONE IS ALLOWED
            if(neighour.x < 0 || neighour.x >= map->size_x || neighour.y < 0 || neighour.y >= map->size_y || map->tiles[(int) round(neighour.y * map->size_x + neighour.x)].id == 1) {
                continue;
            }

            tentative_gScore = ((ScoreMap*) find(gScore, &current, compareFPoint, &default_value))->value + get_square_distance(current, neighour);
            if(tentative_gScore < ((ScoreMap*) find(gScore, &neighour, compareFPoint, &default_value))->value) {
                add(&cameFrom, &(FPointMap) {neighour, current});
                add(&gScore, &(ScoreMap) {neighour, tentative_gScore});
                tentative_gScore += get_square_distance(neighour, goal);
                add(&fScore, &(ScoreMap) {neighour, tentative_gScore});

                if(((ScoreMap*) find(openSet, &neighour, compareFPoint, &default_value))->value == UINT32_MAX) { // Not found
                    insert(&openSet, &neighour, compareScore);
                }
            }
        }
    }

    free(openSet);
    free(gScore);
    free(fScore);
    free(cameFrom);

    printf("FAIL\n");
    return NULL; // No path found
}