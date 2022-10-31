;�ļ�����Define.inc
;���������ֺ�����PROTO���壬���ֳ����Ķ���

;���ø���ͷ�ļ�
include windows.inc 
include user32.inc 
include kernel32.inc 
include gdi32.inc
include msvcrt.inc
include comdlg32.inc
include comctl32.inc
include msvcrt.inc

includelib user32.lib 
includelib kernel32.lib 
includelib gdi32.lib
includelib msvcrt.lib
includelib comdlg32.lib
includelib comctl32.lib
includelib msvcrt.lib

;����ԭ��

;����WndProc����������¼��ĺ�����λ��
IHandleModeChange			PROTO :HWND,:WPARAM,:LPARAM ;����WM_COMMAND(�˵����)���µ�ģʽ�仯
IHandleMouseMove			PROTO :HWND,:WPARAM,:LPARAM ;��������ƶ��¼�(������д��)
IHandleButtonDown			PROTO :HWND,:WPARAM,:LPARAM ;������갴���¼�(������д��)
IHandleButtonUp				PROTO :HWND,:WPARAM,:LPARAM	;��������������¼�(������д�룬����)
IHandleCursor				PROTO :HWND,:WPARAM,:LPARAM ;��������¼�
IHandlePaint				PROTO :HWND,:WPARAM,:LPARAM,:PAINTSTRUCT ;������ͼ�¼�

;���ֻ�ͼ����
IPaint						PROTO :HDC
IErase						PROTO :HDC
IText						PROTO :HDC,:HWND
IPaintLine					PROTO :HDC
IPaintRectangleFrame		PROTO :HDC
IPaintTriangle0Frame		PROTO :HDC
IPaintTriangle1Frame		PROTO :HDC
IPaintPolygonFrame			PROTO :HDC
IPaintRectangle				PROTO :HDC
IPaintTriangle0				PROTO :HDC
IPaintTriangle1				PROTO :HDC
IPaintEllipse				PROTO :HDC
IPaintPolygon				PROTO :HDC

;��ͼ��������
IGetCurrentPoint		PROTO :DWORD ;��õ�ǰ��
;�ж϶�����Ƿ�������--�Ѿ������������ϣ����һ����͵�һ����ӽ��غ�.
;�ӽ���eax = 1 ����eax = 0
IJudgePolygonEnd			PROTO 
IAddGraphPoint				PROTO ;����ǰ��洢������
IGetPolygonPointList		PROTO ;���ݵ��еõ����ƶ���εĴ洢����
IGetTriangle0PointList		PROTO ;���ݵ��еõ�����ֱ��������(�Ϸ�)�Ĵ洢����
IGetTriangle1PointList		PROTO ;���ݵ��еõ�����ֱ��������(�·�)�Ĵ洢����
IGetRectanglePointList		PROTO ;���ݵ��еõ����ƾ��εĴ洢����
IIncreasePolygonLine		PROTO :HDC;�ڶ���λ���ʱ����һ����
IIncreasePolygonLastLine	PROTO, :HDC;�ڶ���λ���ʱ�������һ���ߣ����ӿ�ʼ�ͽ���
ICallTextDialog				PROTO :HWND,:UINT,:WPARAM,:LPARAM ;�����Ի�����������
IHandleTextDialog			PROTO :HWND,:WPARAM,:LPARAM ;���Ի����������ִ洢�����ڻ���

;�˵������ƵȺ���
ICreateMenu					PROTO ;�����˵�

IHandleColor				PROTO :HWND, :DWORD
IHandleFont					PROTO :HWND
IHandleEraserSize			PROTO :HWND
ICallEraserDialog			PROTO :HWND, :UINT, :WPARAM, :LPARAM
IHandleEraserDialog			PROTO :HWND, :WPARAM, :LPARAM
IHandlePainterSize			PROTO :HWND
ICallPainterDialog			PROTO :HWND, :UINT, :WPARAM, :LPARAM
IHandlePainterDialog		PROTO :HWND, :WPARAM, :LPARAM

;������ֳ���
.data

	;��Ļ�͹�������Ϣ
	ScreenLength			WORD 541
	ScreenWidth				WORD 784
	WorkRegion				RECT <0, 0, 800, 600>

	;�ַ�����
	MAX_LENGTH				WORD 100

	;��Դ�ļ�
	IDD_DIALOG1				WORD 104
	IDD_DIALOG2				WORD 106
	IDD_DIALOG3				WORD 107
	IDC_EDIT1				WORD 1001
	IDC_EDIT2				WORD 1003
	IDC_EDIT3				WORD 1004
	IDC_TEXTCURSOR			DWORD 103
	IDC_ERASERCURSOR		DWORD 101
	IDC_PAINTCURSOR			DWORD 102


	;�Ϸ��˵������ֱ��
	IDM_FILE_MENU			WORD 301
	IDM_DRAW_MENU			WORD 302
	IDM_GRAPH_TOOL_MENU		WORD 303
	IDM_GRAPH_FRAME_MENU	WORD 304
	IDM_SETTINGS_MENU		WORD 305

	;�˵�������ѡ��ı��, word
	IDM_DRAW				WORD 400
	IDM_ERASE				WORD 401
	IDM_TEXT				WORD 402
	IDM_LINE				WORD 403
	IDM_SOLID_LINE			WORD 4031
	IDM_DASH_LINE			WORD 4032
	IDM_DOT_LINE			WORD 4033
	IDM_DASHDOT_LINE		WORD 4034
	IDM_DASHDOT2_LINE		WORD 4035
	IDM_INSIDEFRAME_LINE	WORD 4036
	IDM_TRIANGLE0			WORD 4040
	IDM_TRIANGLE1			WORD 4041
	IDM_RECTANGLE			WORD 405
	IDM_ELLIPSE				WORD 406
	IDM_POLYGON				WORD 407
	IDM_RECTANGLE_FRAME		WORD 408
	IDM_TRIANGLE0_FRAME		WORD 4090
	IDM_TRIANGLE1_FRAME		WORD 4091
	IDM_POLYGON_FRAME		WORD 410
	;File
	IDM_SAVE				WORD 411
	IDM_LOAD				WORD 412
	;Color
	IDM_BRUSH_COLOR			WORD 413
	IDM_PEN_COLOR			WORD 414
	IDM_FONT				WORD 415

	IDM_BRUSH 				WORD 416
	IDM_SOLID_BRUSH			WORD 4160
	IDM_BDIAG_BRUSH			WORD 4161
	IDM_FDIAG_BRUSH			WORD 4162
	IDM_DCROSS_BRUSH		WORD 4163
	IDM_CROSS_BRUSH 		WORD 4164
	IDM_HORIZ_BRUSH			WORD 4165
	IDM_VERTI_BRUSH			WORD 4166

	;�����߿�����Ƥ��С
	IDM_LINE_SIZE			WORD 417
 	IDM_ERASER_SIZE         WORD 418		

	;��ǰģʽ�ı��
	IDM_MODE_DRAW				DWORD 500
	IDM_MODE_ERASE				DWORD 501
	IDM_MODE_TEXT				DWORD 502
	IDM_MODE_LINE				DWORD 503
	IDM_MODE_TRIANGLE0			DWORD 5040
	IDM_MODE_TRIANGLE1			DWORD 5041
	IDM_MODE_RECTANGLE			DWORD 505
	IDM_MODE_ELLIPSE			DWORD 506
	IDM_MODE_POLYGON			DWORD 507
	IDM_MODE_RECTANGLE_FRAME	DWORD 508
	IDM_MODE_TRIANGLE0_FRAME	DWORD 5090
	IDM_MODE_TRIANGLE1_FRAME	DWORD 5091
	IDM_MODE_POLYGON_FRAME		DWORD 510
	IDM_MODE_BACKGROUBD_COLOR	DWORD 511
	IDM_MODE_FRAME_COLOR		DWORD 512
	IDM_MODE_FONT				DWORD 513


	; �����Լ��������ַ���
	ClassName				db "DrawingWinClass", 0
	AppName					db "��ͼ", 0

	; �˵��ַ���
	FileMenuString			BYTE "�ļ�", 0
		LoadMenuString			BYTE "��", 0
		SaveMenuString			BYTE "����", 0

	DrawMenuString			BYTE "��ͼ", 0
		PaintMenuString			BYTE "��ͼ", 0
		EraseMenuString			BYTE "����", 0
		TextMenuString			BYTE "����", 0
		LineSizeString			BYTE "���ʿ���", 0
		EraserSizeString		BYTE "��Ƥ����С", 0

	FrameMenuString			BYTE "��ܻ��ƹ���", 0
		LineMenuString			BYTE "ֱ��", 0
		RectangleFrameMenuString BYTE "���ο�", 0
		Triangle0FrameMenuString BYTE "ֱ�������ο��Ϸ���", 0
		Triangle1FrameMenuString BYTE "ֱ�������ο��·���", 0
		PolygonFrameMenuString	BYTE "����ο�", 0
		BrushMenuString			BYTE "��䷽ʽ", 0

			SolidLineMenuString		BYTE "ʵ��", 0
			DashLineMenuString		BYTE "����", 0
			DotLineMenuString		BYTE "����", 0
			DashDotLineMenuString	BYTE "�ߡ���",0
			DashDot2LineMenuString	BYTE "�ߡ��㡢��", 0
			InsideFrameLineMenuString BYTE "���߿�", 0
			SolidBrushMenuString	BYTE "ʵ��", 0
			BDiagonalBrushMenuString  BYTE "��б��", 0
			FDiagonalBrushMenuString  BYTE "��б��", 0
			DiagCrossBrushMenuString  BYTE "б����", 0
			CrossBrushMenuString	BYTE "����", 0
			HorizontalBrushMenuString BYTE "ƽ��", 0
			VerticalBrushMenuString	  BYTE "��ֱ", 0


	ColorMenuString			BYTE "��ɫ", 0
		ColorBrushMenuString	BYTE "���", 0
		ColorPenMenuString		BYTE "����", 0
		
	
	FontMenuString			BYTE "����", 0
		FontChooseMenuString	BYTE "ѡ������", 0 

	ToolMenuString			BYTE "ͼ�λ��ƹ���", 0
		RectangleMenuString		BYTE "����", 0
		Triangle0MenuString		BYTE "ֱ�������Σ��Ϸ���", 0
		Triangle1MenuString		BYTE "ֱ�������Σ��·���", 0
		EllipseMenuString		BYTE "��Բ", 0
		PolygonMenuString		BYTE "�����", 0

	SettingsMenuString		BYTE "����", 0

	PenColor 				DWORD 0ffh
	PenStyle				DWORD PS_SOLID ;PS_SOLID == 0
	BrushColor 				DWORD 0ffffffh ;Fill Rect with BrushColor
	BrushMode				DWORD 0
		SOLID_BRUSH 		DWORD 0
		HATCH_BRUSH			DWORD 1
		HatchStyle			DWORD HS_BDIAGONAL
	ArrayCustom_Color		DWORD 16 DUP(0)
	
	LogicFont				LOGFONT <> 
    CurrentFont				HFONT	0

	
	; �����й�
	fileType byte "BMP(*.bmp)", 0, 0
	fileType2 byte "BMP(*.bmp)", 0 ,"*.bmp", 0, 0
	extenName byte "bmp", 0


	rsFileName	db	1000 DUP (?)
	rsTitleName	db	1000 DUP (?)
	rsFile OPENFILENAME <>



