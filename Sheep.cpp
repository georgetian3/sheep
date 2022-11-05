
//

#include "Sheep.h"
#include "animation.h"
#include <stdio.h>
using namespace std;



#pragma region 全局变量

#define MAX_LOADSTRING 100



// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名



HBITMAP bmp_Map;			//地图图像资源
HBITMAP bmp_StartButton;	//开始按钮图像资源
HBITMAP bmp_grass;
HBITMAP bmp_carrot;
HBITMAP bmp_corn;
HBITMAP bmp_bgd;
HBITMAP bmp_tree; //bitmap here
HBITMAP bmp_Background;		//生成的背景图像
int now_num = 0;
int remain_num = NUM;
HWND main_win;
HWND slot[7] = {};
int slot_type[7] = {};
int button_type[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT] = {};
int is_buttoned[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT] = {};
HWND all_button[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT] = {};
int now_buttom_num = 0;
Stage* currentStage = NULL; //当前场景状态
vector<Button*> buttons;	//按钮


int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
int map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];


double const PI = acos(double(-1));


#pragma endregion


#pragma region Win32程序框架



// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TOWERDEFENSE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOWERDEFENSE));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TOWERDEFENSE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
	   WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT, 
	   WINDOW_WIDTH, 
	   WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT, 
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND btn = GetDlgItem(hWnd, LOWORD(wParam));
	int posi;
	main_win = hWnd;
	switch (message) {
	case WM_COMMAND:
		
		posi=slot_sort(get_button_type(btn));
		update_buttoned(btn);
		now_num++;
		move_button(btn, 112+80*posi, 809, 0.5);
		slot[posi] = btn;
		updateslot();
		update(hWnd);
		remain_num--;
		if (remain_num == 0) {
			exit(0);
		}
		if (now_num == 7) {
			exit(1);
		}
		break;
	case WM_CREATE:
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// 定时器事件
		if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		// 绘图
		Paint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion



#pragma region 事件处理函数

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//加载图像资源
	bmp_Map = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_MAP));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_grass = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GRASS));
	bmp_corn = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CORN));
	bmp_carrot = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_CARROT));
	bmp_bgd = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BGD));
	bmp_tree = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_TREE));
	//添加按钮

	//初始化背景
	bmp_Background = InitBackGround(hWnd, bmp_bgd);

	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU,lParam);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_UP:
		keyUpDown = true;
		break;
	case VK_DOWN:
		keyDownDown = true;
		break;
	case VK_LEFT:
		keyLeftDown = true;
		break;
	case VK_RIGHT:
		keyRightDown = true;
		break;
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_UP:
		keyUpDown = false;
		break;
	case VK_DOWN:
		keyDownDown = false;
		break;
	case VK_LEFT:
		keyLeftDown = false;
		break;
	case VK_RIGHT:
		keyRightDown = false;
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	// 按钮处理
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->visible) {
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY) {
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
				{
					InitStage(hWnd, STAGE_1,lParam);
				}
				break;
				}
			}
		}
	}
}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	UpdateUnits(hWnd);

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion



#pragma region 其它游戏状态处理函数

int get_button_type(HWND hwnd) {
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_LENGTH; j++) {
			for (int k = 0; k < MAP_HEIGHT; k++) {
				if (all_button[i][j][k] == hwnd) {
					return button_type[i][j][k];
				}
			}
		}
	}
	return - 1;
}
int update_buttoned(HWND hwnd) {
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_LENGTH; j++) {
			for (int k = 0; k < MAP_HEIGHT; k++) {
				if (all_button[i][j][k] == hwnd) {
					if (map[i][j][k] != -1) {
						int sign1 = (i + 1) < MAP_WIDTH ? i + 1 : MAP_WIDTH - 1;
						int sign2 = (j + 1) < MAP_LENGTH ? j + 1 : MAP_LENGTH - 1;
						is_buttoned[i][j][k] = 0;
						is_buttoned[sign1][j][k] = 0;
						is_buttoned[i][sign2][k] = 0;
						is_buttoned[sign1][sign2][k] = 0;
					}
				}
			}
		}
	}
	return -1;
}
int slot_sort(int type) {
	for (int i = 0; i < 7; i++) {
		if (i == 6) {
			slot_type[i] = type;
			return i;
		}
		if (type > slot_type[i] ) {
			for (int j = 6; j > i; j--) {
				slot_type[j] = slot_type[j - 1];
				if (slot[j - 1] != NULL) {
					move_button(slot[j - 1], 112 + 80 * j, 809, 0);
					slot[j] = slot[j - 1];
				}
			}
			slot_type[i] = type;
			return i;
		}
	}

}

void updateslot() {
	for (int i = 0; i < 5; i++) {
		if (slot_type[i] == slot_type[i + 1] && slot_type[i + 1] == slot_type[i + 2] && slot_type[i + 2] != -1) {
			DestroyWindow(slot[i]);
			DestroyWindow(slot[i + 1]);
			DestroyWindow(slot[i + 2]);
			slot_type[i] = -1;
			slot_type[i + 1] = -1;
			slot_type[i + 2] = -1;
			now_num -= 3;
			slot[i] = NULL;
			slot[i + 1] = NULL;
			slot[i + 2] = NULL;
			for (int j = i + 3; j < 7; j++) {
				if (slot[j] != NULL) {
					move_button(slot[j], 112 + 80 * (j - 3), 809, 0);
					slot_type[j - 3] = slot_type[j];
					slot_type[j] = -1;
					slot[j - 3] = slot[j];
					slot[j] = NULL;
				}
			}
		}
	}
}

int inslot(HWND hwnd) {
	for (int i = 0; i < 7; i++) {
		if (hwnd == slot[i]) {
			return 1;
		}
	}
	return 0;
}
// 添加按钮函数
void update(HWND hwnd) {
	for (int i = 0; i < MAP_WIDTH; i ++) {
		for (int j = 0; j < MAP_LENGTH; j ++) {
			for (int k = 0; k < MAP_HEIGHT; k++) {
				HWND btn = all_button[i][j][k];
				if (btn == NULL) {
					continue;
				}
				int flag = detect_overlap(i,j,k);
				if (flag == 0) {
					EnableWindow(btn, TRUE);
				}
				else {
					EnableWindow(btn, FALSE);
				}
			}
		}
	}
}
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y) {
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;
	button->visible = false;
	return button;
}
HWND createButton(HWND hwnd, LPARAM lParam, int id, int x, int y,int type){
	HWND btn=NULL; 
	switch (type)
	{
		LRESULT res;
	case 0:
		btn = (HWND)CreateWindow(TEXT("button"), 0, WS_VISIBLE | WS_CHILD | BS_BITMAP , x, y, UNIT_SIZE_X, UNIT_SIZE_Y, hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		res = SendMessage(
			btn,
			(UINT)BM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP,
			(LPARAM)bmp_grass);
		break;
	case 1:
		btn = (HWND)CreateWindow(TEXT("button"), 0, WS_VISIBLE | WS_CHILD | BS_BITMAP , x, y, UNIT_SIZE_X, UNIT_SIZE_Y, hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		res = SendMessage(
			btn,
			(UINT)BM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP,
			(LPARAM)bmp_corn);
		break;
	case 2:
		btn = (HWND)CreateWindow(TEXT("button"), 0, WS_VISIBLE | WS_CHILD | BS_BITMAP , x, y, UNIT_SIZE_X, UNIT_SIZE_Y, hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		res = SendMessage(
			btn,
			(UINT)BM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP,
			(LPARAM)bmp_carrot);
		break;
	case 3:
		btn = (HWND)CreateWindow(TEXT("button"), 0, WS_VISIBLE | WS_CHILD | BS_BITMAP, x, y, UNIT_SIZE_X, UNIT_SIZE_Y, hwnd, (HMENU)id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		res = SendMessage(
			btn,
			(UINT)BM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP,
			(LPARAM)bmp_tree);
		break;
	default:
		
		break;
	}
	
	return btn;
}
int detect_overlap(int i, int j, int k) {
	int sign1 = (i + 1) < MAP_WIDTH ? i + 1 : MAP_WIDTH - 1;
	int sign2 = (j + 1) < MAP_LENGTH ? j + 1 : MAP_LENGTH - 1;
	for (int l = k + 1; l < MAP_HEIGHT; l++) {
		if (is_buttoned[i][j][l] != 0 || is_buttoned[sign1][j][l] != 0 || is_buttoned[i][sign2][l] != 0 || is_buttoned[sign1][sign2][l] != 0) {
			return 1;
		}
	}
	return 0;
}
void MakeRand(int arr[], int count)
{
	srand((unsigned int)time(NULL)); //随机数种子;
	for (int i = 0; i < count - 1; i++)
	{
		int num = i + rand() % (count - 1 - i); // 取随机数
		int temp = arr[i];
		arr[i] = arr[num];
		arr[num] = temp; //交换
	}
}

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID,LPARAM lParam) {
	int item[NUM] = {};
	memset(slot_type, -1,sizeof(slot_type));
	for (int i = 0; i < NUM;) {
		for (int j = 0; j < NUM_CORN*3; j++, i++) {
			item[i] = 0;
		}
		for (int j = 0; j < NUM_GRASS*3; j++, i++) {
			item[i] = 1;
		}
		for (int j = 0; j < NUM_CARROT*3; j++, i++) {
			item[i] = 2;
		}
		for (int j = 0; j < NUM_TREE * 3; j++, i++) {
			item[i] = 3;
		}
	}
	MakeRand(item, NUM);
	memset(map, -1, sizeof(map));
	int flag = 0;
	for (int i = 0; i < MAP_WIDTH; i+=3) {
		for (int j = 0; j < MAP_LENGTH; j+=3) {
			for (int k = 0; k < MAP_HEIGHT; k++) {
				if (k == 2) {
					map[i+1][j][k] = item[flag];
				}
				else {
					map[i][j][k] = item[flag];
				}
				flag++;
			}
		}
	}

	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_LENGTH; j++) {
			for (int k = 0; k < MAP_HEIGHT; k++) { 
				HWND btn = createButton(hWnd, lParam, 100 * k + i  * 10 + j,  UNIT_SIZE_X/2 + i * UNIT_SIZE_X/2, UNIT_SIZE_Y/2 + j * UNIT_SIZE_Y/2, map[i][j][k]);
				button_type[i][j][k] = map[i][j][k];
				all_button[i][j][k] = btn;
				if (map[i][j][k] != -1) {
					int sign1 = (i + 1) < MAP_WIDTH ? i + 1 : MAP_WIDTH - 1;
					int sign2 = (j + 1) < MAP_LENGTH ? j + 1 : MAP_LENGTH - 1;
					is_buttoned[i][j][k] = 1;
					is_buttoned[sign1][j][k] = 1;
					is_buttoned[i][sign2][k] = 1;
					is_buttoned[sign1][sign2][k] = 1;

				}
			}
			
		}
	}
	update(hWnd);
	//createButton(hWnd, lParam, 0, 64, 64);

	if (stageID == STAGE_STARTMENU) {
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//显示开始界面的按钮
		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_STARTGAME) {
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}


	}
	else if (stageID >= STAGE_1 && stageID <= STAGE_1)
	{
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//显示游戏界面的按钮
		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (false) 
			{
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}

		// 按场景初始化单位
		switch (stageID) {
		case STAGE_1:
			break;
		default:
			break;
		}


	}

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


// 刷新单位状态函数
void UpdateUnits(HWND hWnd) {


}



#pragma endregion



#pragma region 绘图函数
// 绘图函数
void Paint(HWND hWnd) {

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, currentStage->bg);

	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_STARTMENU) {

	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_1) 
	{


	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}



// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src) {

	srand(time(NULL));
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	//加载资源
	SelectObject(hdc_loadBmp, bmp_src);
	TransparentBlt(
		hdc_memBuffer,0, 0,
		768, 1024,
		hdc_loadBmp, 0,0,
		768, 1024,
		RGB(255, 255, 255));
	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}

#pragma endregion