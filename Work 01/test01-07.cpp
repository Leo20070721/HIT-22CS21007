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
	TCHAR text[15]; 
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
	TCHAR text[32], MAX_LENGTH=12; 
	COLORREF FillColor,LineColor[3]; 
	bool isActive=false; // 是否处于编辑状态
	bool isReadOnly=false; // 是否只读

	void create(){ // 绘制按钮 		
		setfillcolor(FillColor); 		
		if(isReadOnly){
			setlinecolor(LineColor[0]);
		}else if(isActive){
			setlinecolor(LineColor[2]);			
		}else{
			setlinecolor(LineColor[1]);
		}

		solidrectangle(x, y, x + w, y + h); 
		rectangle(x, y, x + w, y + h);
		outtextxy(x + 10 , y + h / 2 - 10, text);

		if(isActive && !isReadOnly){
			setlinecolor(LineColor[2]);
			int cursor_x = x + 10 + textwidth(text); // 光标紧跟文本后
			int cursor_y1 = y + 10;
			int cursor_y2 = y + h - 10;
			line(cursor_x, cursor_y1, cursor_x, cursor_y2);
		}
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
	int txt_MAX_LENGTH[3] = {12, 12, 24};
	COLORREF txt_fill_color[3] = {WHITE, WHITE, WHITE};
	COLORREF txt_line_color[3] = {LIGHTGRAY, BLACK, BLUE};//ReadOnly, Normal, Active

};
struct MainWindowType{
	bool RUNNING = true;
	Button btns[5];
	TextBox txts[3];
	
	int isActive; // -1: none, 0: Data1, 1: Data2, 2: Result

	MainWindowType(){ 
		MainWindowInit();
	}

	void MainWindowInit(){
		MainWindowData MainWindowData;
		RUNNING = true;
		for(int i = 0; i < 5; i++){
			btns[i].x = MainWindowData.startX + i * MainWindowData.btn_gap;
			btns[i].y = MainWindowData.startY;
			btns[i].w = MainWindowData.btn_w;
			btns[i].h = MainWindowData.btn_h;
			_tcscpy_s(btns[i].text, 15, MainWindowData.btns_text[i]);
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
			_tcscpy_s(txts[i].text, 32, _T(""));
			txts[i].FillColor = MainWindowData.txt_fill_color[i];
			txts[i].LineColor[0] = MainWindowData.txt_line_color[0];
			txts[i].LineColor[1] = MainWindowData.txt_line_color[1];
			txts[i].LineColor[2] = MainWindowData.txt_line_color[2];
			txts[i].MAX_LENGTH = MainWindowData.txt_MAX_LENGTH[i];
		}
	}

	void MainWindowDraw(){ 
		MainWindowData MainWindowData;
		cleardevice();
		for(int i = 0; i < 5; i++){btns[i].create();}
		for(int i = 0; i < 3; i++){outtextxy(MainWindowData.startX + i * MainWindowData.txt_gap, MainWindowData.startY + MainWindowData.btn_h + 10, MainWindowData.const_txts_hint[i]);}
		for(int i = 0; i < 3; i++){txts[i].create();}
	}
};

struct Calculator{	
	void Calculate(MainWindowType &MainWindow, char op){
		double data1 = _tstof(MainWindow.txts[0].text);
		double data2 = _tstof(MainWindow.txts[1].text);
		double result = 0;
		switch(op){
			case '+':
				result = data1 + data2;
				break;
			case '-':
				result = data1 - data2;
				break;
			case '*':
				result = data1 * data2;
				break;
			case '/':				
				result = data1 / data2;
				break;
		}
		_stprintf_s(MainWindow.txts[2].text, MainWindow.txts[2].MAX_LENGTH, _T("%g"), result);
		MainWindow.MainWindowDraw();
	}
};
struct MessageWorker{
	void HandleMouseLButtonDown(ExMessage msg, MainWindowType &MainWindow){
		bool isTextBoxActivedChanged=false;
		bool isCalculated=false;

		for(int i=0;i<3;i++){
			if(msg.x >= MainWindow.txts[i].x && msg.x <= MainWindow.txts[i].x + MainWindow.txts[i].w && msg.y >= MainWindow.txts[i].y && msg.y <= MainWindow.txts[i].y + MainWindow.txts[i].h){
				if(!MainWindow.txts[i].isReadOnly && MainWindow.isActive != i){
					MainWindow.txts[i].isActive = true;
					MainWindow.isActive = i;
					isTextBoxActivedChanged = true;
				}
			}else{
				MainWindow.txts[i].isActive = false;
			}
		}
		if(!isTextBoxActivedChanged){MainWindow.isActive = -1;}


		for(int i = 0; i < 5; i++){
			if(msg.x >= MainWindow.btns[i].x && msg.x <= MainWindow.btns[i].x + MainWindow.btns[i].w && msg.y >= MainWindow.btns[i].y && msg.y <= MainWindow.btns[i].y + MainWindow.btns[i].h){
				isCalculated = true;
				switch(i){
					case 0: 
						Calculator().Calculate(MainWindow, '+');
						break;
					case 1:
						Calculator().Calculate(MainWindow, '-');
						break;
					case 2: 
						Calculator().Calculate(MainWindow, '*');
						break;
					case 3: 
						Calculator().Calculate(MainWindow, '/');
						break;
					case 4: 
						MainWindow.RUNNING = false;
						return;
				}
			}
		}


		if(isTextBoxActivedChanged || isCalculated){MainWindow.MainWindowDraw();}
	}

	void HandleKeyboardCharInput(ExMessage msg, MainWindowType &MainWindow){
		if(MainWindow.isActive == -1) return;
		if(MainWindow.txts[MainWindow.isActive].isReadOnly) return;
		
		if(msg.ch == '\b'){ // 处理退格键
			int len = _tcslen(MainWindow.txts[MainWindow.isActive].text);
			if(len > 0){
				MainWindow.txts[MainWindow.isActive].text[len - 1] = '\0';
			}			
		}else if((msg.ch >= '0' && msg.ch <= '9') || msg.ch == '.' || msg.ch == '-'){ // 只允许输入数字、小数点和负号
			int len = _tcslen(MainWindow.txts[MainWindow.isActive].text);
			if(len <= MainWindow.txts[MainWindow.isActive].MAX_LENGTH){ // 确保不超过文本框限制
				MainWindow.txts[MainWindow.isActive].text[len] = msg.ch;
				MainWindow.txts[MainWindow.isActive].text[len + 1] = '\0';
			}
		}
		MainWindow.MainWindowDraw();		
	}
	
};


int main()
{
	init();

	//SetFocus(GetHWnd()); // 确保窗口获得焦点

	MainWindowType MainWindow;
	MainWindow.MainWindowDraw();

	ExMessage msg;	
	while(MainWindow.RUNNING){
		while (peekmessage(&msg, EM_MOUSE | EM_KEY | EM_CHAR))
		{
			if(msg.message == WM_LBUTTONDOWN){
				MessageWorker().HandleMouseLButtonDown(msg, MainWindow);
			}else if(msg.message == WM_CHAR){
				MessageWorker().HandleKeyboardCharInput(msg, MainWindow);
			}else if(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE){
				MainWindow.RUNNING = false;
				break;
			}
		}		
		Sleep(10);
	}

	//wait_for_escape();
	closegraph();			// 关闭绘图窗口
	return 0;
}
