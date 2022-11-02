#pragma once

#pragma region ͷ�ļ�����

// Windows ͷ�ļ�
#include <windows.h>
#include <windowsx.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <random>
#include <mmsystem.h>

// ��Դͷ�ļ�
#include "resource.h"
#pragma comment(lib, "Msimg32.lib") //����ͼ������̬�⣬��ʹ�ÿ���λͼ�Ļ���TransparentBlt�Ⱥ���

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <vector>
#include <math.h>


#pragma endregion

#pragma region �궨��

#define WINDOW_TITLEBARHEIGHT	32			//�������߶�
#define WINDOW_WIDTH			768		    //��Ϸ���ڿ���
#define WINDOW_HEIGHT			1024		//��Ϸ���ڸ߶�


#define STAGE_STARTMENU			0		//��ʼ�����ID
#define STAGE_1					1		//��һ����Ϸ������ID


#define UNIT_SIZE_X				80		//��λ�Ŀ���
#define UNIT_SIZE_Y				73		//��λ�ĸ߶�
#define UNIT_LAST_FRAME			11		//��λ�Ķ���֡���ֵ


//����
#define BG_NEW_COLUMNS          32      //��������
#define BG_NEW_ROWS             24      //��������
#define BG_NEW_CELL_WIDTH		32		
#define BG_NEW_CELL_HEIGHT		32		


//��������
#define BUTTON_STARTGAME			1001	//��ʼ��Ϸ��ťID
#define BUTTON_STARTGAME_WIDTH		212		//��ʼ��Ϸ��ť����
#define BUTTON_STARTGAME_HEIGHT		76		//��ʼ��Ϸ��ť�߶�

#define TIMER_GAMETIMER				1		//��Ϸ��Ĭ�ϼ�ʱ��ID
#define TIMER_GAMETIMER_ELAPSE		30		//Ĭ�ϼ�ʱ��ˢ�¼���ĺ�����
#define UNIT_SPEED					3.0		//��λ�����ٶ�	

#define MAP_WIDTH			8
#define MAP_LENGTH			8
#define MAP_HEIGHT			2
#define NUM_GRASS			1
#define NUM_CARROT			2
#define NUM_CORN			3
#define NUM					18
#define FPS					24
#define N_TILE_TYPES        3
#pragma endregion

#pragma region �ṹ������
// �����ṹ��
struct Stage {
	int stageID;		//�������
	HBITMAP bg;			//����ͼƬ
	int timeCountDown;	//��Ϸʱ�䵹��ʱ
	bool timerOn;		//��ʱ���Ƿ����У���Ϸ�Ƿ���ͣ��

};

// �ƶ��ṹ��
struct MoveArgs {
	HWND btn;
	int x;
	int y;
	double speed;
	BOOL threaded;
};

// ��ť�ṹ��
struct Button {
	int buttonID;	//��ť���
	bool visible;	//��ť�Ƿ�ɼ�
	HBITMAP img;	//ͼƬ
	int x;			//����x
	int y;			//����y
	int width;		//����
	int height;		//�߶�
};
#pragma endregion

#pragma region �¼�������������


// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ���̰����¼���������
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����ɿ��¼���������
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ����ƶ��¼���������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����������¼���������
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �������ɿ��¼���������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ��ʱ���¼���������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion

#pragma region ������Ϸ״̬������������
// ���Ӱ�ť����
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// ��ʼ����������
void InitStage(HWND hWnd, int stageID,LPARAM lParam);

// ˢ�µ�λ״̬����
void UpdateUnits(HWND hWnd);

void update(HWND hwnd);

POINT win_pos(HWND hwnd);

int inslot(HWND hwnd);

void updateslot();
int slot_sort(int type);

int get_button_type(HWND hwnd);

int tile_type(HWND btn);
#pragma endregion

#pragma region ��ͼ��������

// ��ͼ����
void Paint(HWND hWnd);

// ��ͼ��ƺ���
int** GetMap(int k);

// ��ʼ����������
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src);

#pragma endregion