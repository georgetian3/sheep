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
	char filename[MAX_FILENAME_LEN]={0};
	strcat(filename, "../res/");
	strcat(filename, name);
	fp = fopen(filename, "r");
    int total = 0 ; 
    int tiles_num[N_TILE_TYPES]={0};
    fscanf(fp,"%d",&total);
    for (int i=0;i<N_TILE_TYPES;i++){
        fscanf(fp,"%d",&tiles_num[i]);
    }
    int *item = (int*)malloc(sizeof(int)*total);
    for (int i = 0; i < total;) {
        for(int type=0;type<N_TILE_TYPES;type++){
            for(int j=0;j<tiles_num[type]*3;j++,i++){
                item[i]=type;
            }
        }
    }

    shuffle(item,total);
	int flag;
	int map_width = 0, map_length = 0, map_height = 0;
	fscanf(fp, "%d %d %d", &map_width, &map_length, &map_height);
	total = 0;
	while(fscanf(fp, "%d", &flag)!=EOF) {
        //printf("i=%d j=%d k=%d, flag=%d num=%d\n", i, j, k, flag,total);
		if (flag) {
            //printf("%d\n",item[total]);
			create_button(hwnd,item[total++], TILE_WIDTH / 2 + i * TILE_WIDTH / 2, TILE_HEIGHT / 2 + j * TILE_HEIGHT/ 2, k, TILE_WIDTH, TILE_HEIGHT);
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
    fclose(fp);
    return total;
}