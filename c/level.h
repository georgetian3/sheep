#ifndef LEVEL_H
#define LEVEL_H

#include <windows.h>
#include <stdio.h>

#define N_MAPS 2

char filename[] = " .map";

int map_index(int i, int j, int k, int wd, int ht, int dp) {
    return k * wd * ht + j * wd + i;
}

void read_map(BOOL* map, int level, int* wd, int* ht, int* dp) {
    if (level < 1 || level > N_MAPS) {
        printf("Wrong level\n");
        exit(1);
    }
    filename[0] = '0' + level;
    FILE* map_file = fopen(filename, "r");

    map = (BOOL***)malloc(sizeof(BOOL***) * *dp);
    for (int i = 0; i < *dp; i++) {
        map[i] = (BOOL**)malloc(sizeof(BOOL**) * *ht)
        for (int j = 0; j < *wd; j++) {
            ;
        }
    }
    scanf("%d %d %d ", wd, ht, dp);
    for (int i = 0; i < *dp; i++) {
        ;
    }

}


#endif