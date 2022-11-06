#pragma once
#include <windows.h>
#include <stdio.h>
#include "utils.h"
#define MAX_FILENAME_LEN 255
#define MAX_SIZE 64
void build_map(HWND hwnd, const char name[],int item[]) {
	int i = 0, j = 0, k = 0;
	FILE* fp = NULL;
	char filename[MAX_FILENAME_LEN]={0};
	strcat(filename, "../res/");
	strcat(filename, name);
	fp = fopen(filename, "r");

	int flag;
	int map_width = 0, map_length = 0, map_height = 0;
	fscanf(fp, "%d %d %d", &map_width, &map_length, &map_height);
	int num = 0;
	while(fscanf(fp, "%d", &flag)!=EOF) {
        printf("i=%d j=%d k=%d, flag=%d num=%d\n", i, j, k, flag,num);
		if (flag) {
			create_button(hwnd, STATE_ENABLED, item[num++], TILE_WIDTH / 2 + i * TILE_WIDTH / 2, TILE_HEIGHT / 2 + j * TILE_HEIGHT/ 2, k, TILE_WIDTH, TILE_HEIGHT);
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
	}
}