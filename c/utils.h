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


#define SLOT_X              100
#define SLOT_X_OFFSET       20
#define SLOT_Y              809
#define SLOT_SIZE           7
#define MATCH_COUNT         3

#define SLOT_MOVE_TIME      0.2

int slot_count = 0;
int stage = 1;
int total=0;

int index_insert = -1;
struct Button* slot[SLOT_SIZE] = {0};

HBITMAP bmp_bg;
void paint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc_window = BeginPaint(hWnd, &ps);
    HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
    HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);
    HBITMAP blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
    SelectObject(hdc_memBuffer, blankBmp);
    SelectObject(hdc_loadBmp, bmp_bg);
    BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
    BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);
    DeleteObject(blankBmp);
    DeleteDC(hdc_memBuffer);
    DeleteDC(hdc_loadBmp);
    EndPaint(hWnd, &ps);
}




BOOL overlap(struct Button* a, struct Button* b) {
    /* returns TRUE if a overlaps b, FALSE otherwise */
    if (a->k <= b->k) {
        return FALSE;
    }
    if (a->in_slot) {
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
    return SLOT_X + slot_index * (TILE_WIDTH + SLOT_X_OFFSET);
}

void match_slot() {
    // deletes any matched tiles and shifts the remaining tiles left
    printf("match slot\n");
    int count = 0;
    int prev_type = -1;
    printf("slot_count matchslot  %d \n",slot_count);

    for (int i = 0; i < slot_count +1 ; i++) {
        printf("i = %d count = %d\n",i,count);
        if (slot[i]->type == prev_type) {
            count++;
            printf("if i = %d count = %d\n",i,count);
            if (count >= MATCH_COUNT) {
                slot_count -= count;
                last_button = 0;
                index_insert = i;
                printf("index insert %d count %d\n",index_insert,count);
                return ;
            }
        } else {
            count = 1;
            prev_type = slot[i]->type;
        }
    } 
   //update();
   index_insert = -1;
}
void move_slot_buttons(){
    if(index_insert == -1) return;
    printf("index insert %d\n",index_insert);
    for (int i = 0; i < 3; i++) {
        delete_button_struct(slot[index_insert - i]);
    }
    for (int i = index_insert + 1; i < slot_count+3; i++) {
        move_button(slot[i], slot_x(i - 3), SLOT_Y, SLOT_MOVE_TIME);
        slot[i - 3] = slot[i];
    }
    index_insert = -1;
}
void insert_slot(struct Button* btn, int index) {
    // moves tile at and to the right of index one slot to the right
    // moves btn to the newly freed slot
    for (int i = slot_count - 1; i >= index; i--) {
        move_button(slot[i], slot_x(i + 1), SLOT_Y, SLOT_MOVE_TIME);
        slot[i + 1] = slot[i];
    }
    slot[index] = btn;
    btn->in_slot = TRUE;
    match_slot();
    btn->callback = move_slot_buttons;
    slot_count++;
    move_button(btn, slot_x(index), SLOT_Y, SLOT_MOVE_TIME);

}




#endif