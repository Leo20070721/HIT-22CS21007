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




int main()
{
	init();

	cleardevice();
	
	Button btn1 = { 10, 10, 100, 30, _T("Button1"), RGB(255, 0, 0), RGB(0, 0, 255) };
	btn1.create();
	TextBox txt1 = { 10, 50, 200, 30, _T("TextBox1"), RGB(255, 255, 0), RGB(0, 255, 0) };
	txt1.create();

	wait_for_escape();
	closegraph();			// 关闭绘图窗口
	return 0;
}
