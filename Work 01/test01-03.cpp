#include <math.h>
#include <iostream>
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
using namespace std;

// 当前 EasyX 库是 ANSI 版本，避免 UNICODE 宏导致接口类型不匹配
/*#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif*/
//-------------------------------------
struct Button{
	int x, y, w, h; 
	TCHAR text[16]; 
	COLORREF FillColor,LineColor; 
	
	void create(){ // 绘制按钮 
		setfillcolor(FillColor); 
		solidrectangle(x, y, x + w, y + h); 
		setlinecolor(LineColor); 
		rectangle(x, y, x + w, y + h);
		outtextxy(x + w / 2 - 20, y + h / 2 - 10, text);
	}
};

struct TextBox{
	int x, y, w, h; 
	TCHAR text[16]; 
	COLORREF FillColor,LineColor; 
	bool isActive=false; // 是否处于编辑状态
	bool isReadOnly=false; // 是否只读

	void create(){ // 绘制按钮 
		setfillcolor(FillColor); 
		solidrectangle(x, y, x + w, y + h); 
		setlinecolor(LineColor); 
		rectangle(x, y, x + w, y + h);
		outtextxy(x + 10 , y + h / 2 - 10, text);
	}
};
void init(){
initgraph(540, 220);
	setbkcolor(WHITE);
	setlinecolor(BLUE);		// 设置线条颜色为蓝色
	settextcolor(BLUE);		// 设置文字颜色为蓝色
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("宋体"));	// 设置文字样式，大小为 20，字体为宋体
	cleardevice();
}
void wait_for_escape(){
	while(true){
		if(GetAsyncKeyState(VK_ESCAPE) & 0x8000){	// 按下 ESC 键退出循环
			break;
		}
		Sleep(10);
	}
}

struct MainWindowData
{	
	int startX = 20, startY = 20;

	int btn_w = 80, btn_h = 50, btn_gap = 100;
	COLORREF btn_fill_color[5] = {RGB(173, 216, 230), RGB(173, 216, 230), RGB(173, 216, 230), RGB(173, 216, 230), WHITE};
	TCHAR *btns_text[5] = {_T("+"), _T("-"), _T("*"), _T("/"), _T("ESC")};
	
	TCHAR *const_txts_hint[3] = {_T("Data1"), _T("Data2"), _T("Result")};
	
	int txt_w = 140, txt_h = 50, txt_gap = 160;
};


struct MainWindowType{
	Button btns[5];
	TextBox txts[3];
	
	int isActive; // -1: none, 0: Data1, 1: Data2, 2: Result

	MainWindowType(){ 
		MainWindowInit();
	}

	void MainWindowInit(){
		MainWindowData MainWindowData;
		for(int i = 0; i < 5; i++){
			btns[i].x = MainWindowData.startX + i * MainWindowData.btn_gap;
			btns[i].y = MainWindowData.startY;
			btns[i].w = MainWindowData.btn_w;
			btns[i].h = MainWindowData.btn_h;
			_tcscpy_s(btns[i].text, 16, MainWindowData.btns_text[i]);
			btns[i].FillColor = MainWindowData.btn_fill_color[i];
			btns[i].LineColor = BLACK;
		}
		
		isActive = -1;
		txts[2].isReadOnly = true;
		for(int i = 0; i < 3; i++){
			txts[i].x = MainWindowData.startX + i * MainWindowData.txt_gap;
			txts[i].y = MainWindowData.startY + MainWindowData.btn_h + 40;
			txts[i].w = MainWindowData.txt_w;
			txts[i].h = MainWindowData.txt_h;
			_tcscpy_s(txts[i].text, 16, _T(""));
			txts[i].FillColor = WHITE;
			if(isActive == i && txts[i].isActive && !txts[i].isReadOnly){
				txts[i].LineColor = BLUE;
			}
			else if (txts[i].isReadOnly){
				txts[i].LineColor = LIGHTGRAY; // 只读状态用浅灰色边框表示
			}
			else {
				txts[i].LineColor = BLACK;
			}
		}
	}

	void MainWindowDraw(){ 
		MainWindowData MainWindowData;
		
	}
};

int main()
{
	init();

	MainWindowData MainWindowData;
	MainWindowType MainWindow;
	
	cleardevice();
	for(int i = 0; i < 5; i++){MainWindow.btns[i].create();}
	for(int i = 0; i < 3; i++){outtextxy(MainWindowData.startX + i * MainWindowData.txt_gap, MainWindowData.startY + MainWindowData.btn_h + 10, MainWindowData.const_txts_hint[i]);}
	for(int i = 0; i < 3; i++){MainWindow.txts[i].create();}

	wait_for_escape();
	closegraph();			// 关闭绘图窗口
	return 0;
}
