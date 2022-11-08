#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#define MAX_FILENAME_LEN 255
#define MAX_SIZE 64

int build_map(HWND hwnd, const char name[]) {
	int i = 0, j = 0, k = 0;
	FILE* fp = NULL;
	char filename[MAX_FILENAME_LEN];
	strcpy(filename, "../res/");
	strcat(filename, name);
	fp = fopen(filename, "r");
    int offset_x, offset_y;
    fscanf(fp, "%d %d", &offset_x, &offset_y);
    /* int tiles_num[N_TILE_TYPES];
    
    for (int i = 0; i < N_TILE_TYPES; i++){
        fscanf(fp, "%d", &tiles_num[i]);
    }
    int total = 0;
    
    for (int type = 0; type < N_TILE_TYPES; type++) {
        for (int j = 0; j < tiles_num[type] * 3; j++) {
            item[total++] = type;
        }
    } */

    int *item = (int*)malloc(sizeof(int) * N_BUTTONS);
    int total = 0;
    for (int i = 0; i < N_TILE_TYPES; i++) {
        int type_count;
        fscanf(fp, "%d", &type_count);
        printf("%d ", type_count);
        for (int j = 0; j < type_count * MATCH_COUNT; j++) {
            item[total++] = i;
        }
    }

    shuffle(item, total);
	int map_width = 0, map_length = 0, map_height = 0;
	fscanf(fp, "%d %d %d", &map_width, &map_length, &map_height);
	/* total = 0;
	while(fscanf(fp, "%d", &flag)!=EOF) {
        //printf("i=%d j=%d k=%d, flag=%d num=%d\n", i, j, k, flag,total);
		if (flag) {
            //printf("%d\n",item[total]);
			
		}
		i++;
		if (i == map_width) {
			i = 0;
			j++;
		}
		if (j == map_length) {
			j = 0;
			k++;
		}
	} */


    int flag;

    int count = 0;
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            for (int k = 0; k < map_length; k++) {
                fscanf(fp, "%d", &flag);
                if (flag) {
                    create_button(hwnd, item[count++], offset_x + k * TILE_WIDTH / 2, offset_y + j * TILE_HEIGHT / 2, i, TILE_WIDTH, TILE_HEIGHT);
                    printf("found\n");
                }
            }
        }
    }
    fclose(fp);
    return total;
}