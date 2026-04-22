#include <math.h>
#include <iostream>
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include <vector>
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
#define _TXTBOX_MAX_SIZE 256
struct TextBox{
	int x, y, w, h; 
	TCHAR text[_TXTBOX_MAX_SIZE];
	int MAX_LENGTH=12; 
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
	int btn_startX, btn_startY ;
	int btn_size,btn_lines;
	vector <int> btn_num_per_line;
	int btn_w, btn_h, btn_gap_w, btn_gap_h;
	vector <COLORREF> btn_fill_color;
	vector <TCHAR*> btns_text;

	int hint_startY;
	vector <TCHAR*> const_txts_hint;
	
	int txt_startX, txt_startY;
	int txt_size;
	int txt_w, txt_h, txt_gap;
	vector <int> txt_MAX_LENGTH;
	vector <bool> txt_isReadOnly;
	vector <COLORREF> txt_fill_color;
	vector <COLORREF> txt_line_color;

};

MainWindowData MainWindowData01={
	20, 20, 
	5, 1,
	{5},
	80, 50, 20, 0,
	{RGB(173, 216, 230), RGB(173, 216, 230), RGB(173, 216, 230), RGB(173, 216, 230), WHITE},
	{_T("+"), _T("-"), _T("*"), _T("/"), _T("ESC")},

	120, 
	{_T("Data1"), _T("Data2"), _T("Result")},

	20, 140,
	3,
	140, 50, 20,
	{12, 12, 24},
	{false, false, true},
	{WHITE, WHITE, WHITE},
	{LIGHTGRAY, BLACK, BLUE}//ReadOnly, Normal, Active
};

struct MainWindowType{
	bool RUNNING = true;
	vector <Button> btns;
	vector <TextBox> txts;
	MainWindowData MainWindowData;
	int isActive; // -1: none, 0: Data1, 1: Data2, 2: Result

	void MainWindowInit(){
		RUNNING = true;
		btns.resize(MainWindowData.btn_size);
		txts.resize(MainWindowData.txt_size);
		
		int idx = 0;
		for(int i = 0; i < MainWindowData.btn_lines; i++){
			for(int j = 0; j < MainWindowData.btn_num_per_line[i]; j++){
				btns[idx].x = MainWindowData.btn_startX + j * (MainWindowData.btn_w + MainWindowData.btn_gap_w);
				btns[idx].y = MainWindowData.btn_startY + i * (MainWindowData.btn_h + MainWindowData.btn_gap_h);
				btns[idx].w = MainWindowData.btn_w;
				btns[idx].h = MainWindowData.btn_h;
				_tcscpy_s(btns[idx].text, 15, MainWindowData.btns_text[idx]);
				btns[idx].FillColor = MainWindowData.btn_fill_color[idx];
				btns[idx].LineColor = BLACK;
				idx++;
			}
		}
		
		isActive = -1;
		for(int i = 0; i < MainWindowData.txt_size; i++){
			txts[i].isReadOnly = MainWindowData.txt_isReadOnly[i];
		}
		
		for(int i = 0; i < MainWindowData.txt_size; i++){
			txts[i].x = MainWindowData.txt_startX + i * (MainWindowData.txt_w + MainWindowData.txt_gap);
			txts[i].y = MainWindowData.txt_startY;
			txts[i].w = MainWindowData.txt_w;
			txts[i].h = MainWindowData.txt_h;
			_tcscpy_s(txts[i].text, _TXTBOX_MAX_SIZE, _T(""));
			txts[i].FillColor = MainWindowData.txt_fill_color[i];
			txts[i].LineColor[0] = MainWindowData.txt_line_color[0];
			txts[i].LineColor[1] = MainWindowData.txt_line_color[1];
			txts[i].LineColor[2] = MainWindowData.txt_line_color[2];
			txts[i].MAX_LENGTH = MainWindowData.txt_MAX_LENGTH[i];
		}
	}

	void MainWindowDraw(){ 
		cleardevice();
		for(int i = 0; i < MainWindowData.btn_size; i++){btns[i].create();}
		for(int i = 0; i < MainWindowData.const_txts_hint.size(); i++){
			outtextxy(MainWindowData.txt_startX + i*(MainWindowData.txt_w+MainWindowData.txt_gap), MainWindowData.hint_startY, MainWindowData.const_txts_hint[i]);
		}
		for(int i = 0; i < MainWindowData.txt_size; i++){txts[i].create();}
	}
};

struct Calculator{
	bool CommonLegalCheck(MainWindowType &MainWindow){
		for(int i = 0; i < 2; i++){
			if(_tcslen(MainWindow.txts[i].text) == 0){
				TCHAR buf[64];
				_stprintf_s(buf, 64, _T("Data%d can't be empty!"), i + 1);
				MessageBox(GetHWnd(), buf, _T("Error"), MB_OK | MB_ICONERROR);
				return false;
			}
			if(_tcslen(MainWindow.txts[i].text) == 1 && (MainWindow.txts[i].text[0] == '.' || MainWindow.txts[i].text[0] == '-')){
				TCHAR buf[64];
				_stprintf_s(buf, 64, _T("Data%d can't be empty!"), i + 1);
				MessageBox(GetHWnd(), buf, _T("Error"), MB_OK | MB_ICONERROR);
				return false;
			}
			if(_tcslen(MainWindow.txts[i].text) == 2 && MainWindow.txts[i].text[0] == '-' && MainWindow.txts[i].text[1] == '.'){
				TCHAR buf[64];
				_stprintf_s(buf, 64, _T("Data%d can't be empty!"), i + 1);
				MessageBox(GetHWnd(), buf, _T("Error"), MB_OK | MB_ICONERROR);
				return false;
			}

			bool haveDot = false;
			for(int j = 0; j < _tcslen(MainWindow.txts[i].text); j++){
				if(MainWindow.txts[i].text[j] == '.'){
					if(haveDot){
						TCHAR buf[64];
						_stprintf_s(buf, 64, _T("Data%d has too many dots!"), i + 1);
						MessageBox(GetHWnd(), buf, _T("Error"), MB_OK | MB_ICONERROR);
						return false;
					}
					haveDot = true;
				}
				else if(MainWindow.txts[i].text[j] == '-'){
					if(j != 0){
						TCHAR buf[64];
						_stprintf_s(buf, 64, _T("Data%d has misplaced minus sign!"), i + 1);
						MessageBox(GetHWnd(), buf, _T("Error"), MB_OK | MB_ICONERROR);
						return false;
					}
				}
				//防CTFer打穿我的内存
				else if(!((MainWindow.txts[i].text[j] >= '0' && MainWindow.txts[i].text[j] <= '9') || MainWindow.txts[i].text[j] == '.' || MainWindow.txts[i].text[j] == '-')){
					TCHAR buf[64];
					_stprintf_s(buf, 128, _T("Data%d has illegal character!\nU R JUST A DIRTY HACKER, AREN'T YOU?"), i + 1);
					MessageBox(GetHWnd(), buf, _T("Fatal Error"), MB_OK | MB_ICONERROR);
					exit(-1);
					return false;
				}
			}

		}
		return true;
	}

	void Calculate(MainWindowType &MainWindow, char op){
		if(!CommonLegalCheck(MainWindow)) return;
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
				if(data2 == 0){
					TCHAR buf[64];
					_stprintf_s(buf, 64, _T("Data2 cannot be zero!"));
					MessageBox(GetHWnd(), buf, _T("Error"), MB_OK | MB_ICONERROR);
					return;
				}
				result = data1 / data2;
				break;
		}
		//cout<<"data1: "<<data1<<" data2: "<<data2<<" result: "<<result<<endl;
		_stprintf_s(MainWindow.txts[2].text, MainWindow.txts[2].MAX_LENGTH, _T("%g"), result);
		//double的数据范围太大了，完全够这几位霍霍的，就不写数据范围检查了

		//Easter Egg
		if(data1 == 114 && data2 == 514 && op == '+'){
			MessageBox(GetHWnd(), _T("114+514=1919810"), _T("Deep Dark Fantasy"), MB_OK | MB_ICONINFORMATION);
		}

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
		}else if((msg.ch >= '0' && msg.ch <= '9') || msg.ch == '.' || msg.ch == '-'){ // 只允许输入数字、小数点和负号 //
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

	MainWindowType MainWindow01;
	MainWindow01.MainWindowData = MainWindowData01;
	MainWindow01.MainWindowInit();
	MainWindow01.MainWindowDraw();

	ExMessage msg;	
	while(MainWindow01.RUNNING){
		while (peekmessage(&msg, EM_MOUSE | EM_KEY | EM_CHAR))
		{
			if(msg.message == WM_LBUTTONDOWN){
				MessageWorker().HandleMouseLButtonDown(msg, MainWindow01);
			}else if(msg.message == WM_CHAR){
				MessageWorker().HandleKeyboardCharInput(msg, MainWindow01);
			}else if(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE){
				MainWindow01.RUNNING = false;
				break;
			}
		}		
		Sleep(10);
	}

	//wait_for_escape();
	closegraph();			// 关闭绘图窗口
	return 0;
}
