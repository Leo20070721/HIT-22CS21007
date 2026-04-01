#include <math.h>
#include <iostream>

// 当前 EasyX 库是 ANSI 版本，避免 UNICODE 宏导致接口类型不匹配
#ifdef UNICODE
#undef UNICODE
#endif
#ifdef _UNICODE
#undef _UNICODE
#endif

#include <graphics.h>		// 引用图形库头文件
#include <conio.h>

using namespace std;

typedef struct{
	int x, y, w, h; 
	TCHAR text[16]; 
	COLORREF color; 
	void create(){ // 绘制按钮 
		setfillcolor(color); 
		solidrectangle(x, y, x + w, y + h); 
		setlinecolor(BLACK); 
		rectangle(x, y, x + w, y + h);
		outtextxy(x + w / 2 - 20, y + h / 2 - 10, text);
	}
}Button;

typedef struct{
	int x, y, w, h; 
	TCHAR text[16]; 
	COLORREF color; 
	void create(){ // 绘制按钮 
		setfillcolor(color); 
		solidrectangle(x, y, x + w, y + h); 
		setlinecolor(BLACK); 
		rectangle(x, y, x + w, y + h);
		outtextxy(x + 10 , y + h / 2 - 10, text);
	}
}TextBox;
void init(){
initgraph(540, 220);
	setbkcolor(WHITE);
	setlinecolor(BLUE);		// 设置线条颜色为蓝色
	settextcolor(BLUE);		// 设置文字颜色为蓝色
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, "宋体");	// 设置文字样式，大小为 20，字体为宋体
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
int main()
{
	init();

	static Button btn1 = {50, 50, 100, 50, "TEST", LIGHTGRAY};
	btn1.create();

	static TextBox txt1 = {50, 150, 150, 50, "Hello, EasyX!", WHITE};
	txt1.create();

	wait_for_escape();
	//_getch();				// 按任意键继续
	closegraph();			// 关闭绘图窗口
	return 0;
}
