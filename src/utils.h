#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include "button.h"


#define WINDOW_WIDTH            768
#define WINDOW_HEIGHT           1024


#define STAGE_STARTMENU            0
#define STAGE_1                    1


#define UNIT_SIZE_X                80
#define UNIT_SIZE_Y                73  


#define BUTTON_STARTGAME            1001
#define BUTTON_STARTGAME_WIDTH        212
#define BUTTON_STARTGAME_HEIGHT        76

#define MAP_WIDTH            8
#define MAP_LENGTH            8
#define MAP_HEIGHT            2
#define NUM_GRASS            1
#define NUM_CARROT            2
#define NUM_CORN            3
#define NUM                    18


#define SLOT_X              100
#define SLOT_Y              809
#define SLOT_SIZE           7
#define MATCH_COUNT         3

int slot_count = 0;
struct Button* slot[SLOT_SIZE] = {0};

struct Stage {
    int id;
    int timeCountDown;
    HBITMAP bg;
};



BOOL overlap(struct Button* a, struct Button* b) {
    /* returns TRUE if a overlaps b, FALSE otherwise */
    if (a->k <= b->k) {
        return FALSE;
    }
    POINT pt_a, pt_b;
    win_pos(a->hWnd, &pt_a);
    win_pos(b->hWnd, &pt_b);

    if (pt_b.x >= pt_a.x + TILE_WIDTH ||
        pt_b.x <= pt_a.x - TILE_WIDTH ||
        pt_b.y >= pt_a.y + TILE_HEIGHT ||
        pt_b.y <= pt_a.y - TILE_HEIGHT) {

        return FALSE;
    }

    return TRUE;
}

void shuffle(int arr[], int n) {
    /* shuffles (randomizes) arr of length n */
    int temp;
    while (--n) {
        int i = rand() % n;
        temp = arr[n];
        arr[n] = arr[i];
        arr[i] = temp;
    }
}

int slot_x(int slot_index) {
    return SLOT_X + slot_index * TILE_WIDTH;
}

void insert_slot(struct Button* btn, int index) {
    // moves tile at and to the right of index one slot to the right
    // moves btn to the newly freed slot
    for (int i = slot_count - 1; i >= index; i--) {
        move_button(slot[i], slot_x(i), SLOT_Y, 1);
    }
    btn->in_slot = TRUE;
    slot_count++;
    move_button(btn, slot_x(index), SLOT_Y, 1);
}

void match_slot(int index) {
    // index: index of the last matched tile in `slot`
    // deletes the matched tile and shifts the remaining tiles left
    for (int i = 0; i < MATCH_COUNT; i++) {
        delete_button_struct(slot[index - i]);
    }
    for (int i = index + 1; i < slot_count; i++) {
        move_button(slot[i], slot_x(i - MATCH_COUNT), SLOT_Y, 1);
    }
    slot_count -= MATCH_COUNT;
}

#endif