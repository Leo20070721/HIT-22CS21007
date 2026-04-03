#include <math.h>
#include <iostream>

#include <graphics.h>		// 引用图形库头文件
#include <conio.h>

using namespace std;


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

	outtextxy(10, 10, _T("Hello, World!"));

	wait_for_escape();
	//_getch();				// 按任意键继续
	closegraph();			// 关闭绘图窗口
	return 0;
}
