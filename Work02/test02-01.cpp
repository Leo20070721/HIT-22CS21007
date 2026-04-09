#include <math.h>
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <vector>
using namespace std;

#define _TEXT_MAX_TXT_SIZE 256
class Text{
    protected:
        int height;
        TCHAR font[15];
        COLORREF color;
        int x, y; 
        pair <int,int> writeMode;
        TCHAR text[_TEXT_MAX_TXT_SIZE];
    public:
        Text(){
            this->x = 0;
            this->y = 0;
            this->writeMode = make_pair(0, 0);
            _tcscpy_s(this->text, _T("Hello World!"));
            this->height = 20;
            this->color = BLACK;
            _tcscpy_s(this->font, _T("宋体"));
        }
        Text(const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), pair <int,int> writeMode=make_pair(0,0)){
            this->x = 0;
            this->y = 0;
            this->writeMode = writeMode;
            _tcscpy_s(this->text, text);
            this->height = height;
            this->color = color;
            _tcscpy_s(this->font, font);
            
        }
        Text(int x, int y, const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), pair <int,int> writeMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->writeMode = writeMode;
            _tcscpy_s(this->text, text);
            this->height = height;
            this->color = color;
            _tcscpy_s(this->font, font);
            
        }

        void setParams(int x, int y, const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), pair <int,int> writeMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->writeMode = writeMode;
            _tcscpy_s(this->text, text);
            this->height = height;
            this->color = color;
            _tcscpy_s(this->font, font);            
        }
        void setText(const TCHAR* text){
            _tcscpy_s(this->text, text);
        }
        void setPos(int x, int y){
            this->x = x;
            this->y = y;
        }
        pair<int,int> makeFont(){
            settextstyle(height, 0, font);
            setcolor(color);
            return make_pair(textwidth(text), textheight(text));
        }

        void create(){ 
            pair<int,int> font_size = makeFont();
            int text_w = font_size.first, text_h = font_size.second;
            int draw_x = x - text_w * (writeMode.first+1)/2, 
                draw_y = y - text_h * (writeMode.second+1)/2;
            outtextxy(draw_x, draw_y, text);
        }
};
class Button{
    protected:
        int x, y, w, h; 
        Text txt;
        COLORREF FillColor,LineColor; 
        pair <int,int> writeMode;
        pair <int,int> textWriteMode;

    public:
        Button(int x, int y, int w, int h,const TCHAR* text, COLORREF FillColor, COLORREF LineColor, pair <int,int> writeMode=make_pair(0,0), pair <int,int> txtMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->txt.setText(text);
            this->FillColor = FillColor;
            this->LineColor = LineColor;
            this->writeMode = writeMode;
            this->textWriteMode = textWriteMode;
        }
        Button(int x, int y, int w, int h,const Text txt, COLORREF FillColor, COLORREF LineColor, pair <int,int> writeMode=make_pair(0,0), pair <int,int> textWriteMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->txt = txt;
            this->FillColor = FillColor;
            this->LineColor = LineColor;
            this->writeMode = writeMode;
            this->textWriteMode = textWriteMode;
        }

        void setParams(int x, int y, int w, int h,TCHAR* text, COLORREF FillColor, COLORREF LineColor, pair <int,int> writeMode=make_pair(0,0), pair <int,int> textWriteMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->txt.setText(text);
            this->FillColor = FillColor;
            this->LineColor = LineColor;
            this->writeMode = writeMode;
            this->textWriteMode = textWriteMode;
        }
        void setParams(int x, int y, int w, int h,Text txt, COLORREF FillColor, COLORREF LineColor, pair <int,int> writeMode=make_pair(0,0), pair <int,int> textWriteMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->txt = txt;
            this->FillColor = FillColor;
            this->LineColor = LineColor;
            this->writeMode = writeMode;
            this->textWriteMode = textWriteMode;
        }

        void setTextParams(int x, int y, const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), pair <int,int> writeMode=make_pair(0,0)){ 
            this->txt.setParams(x, y, text, height, color, font, writeMode);        
        }
        void setText(const TCHAR* text){
            this->txt.setText(text);
        }

        void create(){
            int draw_x = x - w * (writeMode.first+1)/2, 
                draw_y = y - h * (writeMode.second+1)/2;
            setfillcolor(FillColor); 
            setbkmode(OPAQUE);
            solidrectangle(draw_x, draw_y, draw_x + w, draw_y + h); 
            setlinecolor(LineColor); 
            rectangle(draw_x, draw_y, draw_x + w, draw_y + h);
            
            int text_x = draw_x + w * (textWriteMode.first+1)/2, 
                text_y = draw_y + h * (textWriteMode.second+1)/2;
            setbkmode(TRANSPARENT);
            txt.setPos(text_x, text_y);
            txt.makeFont();
            this->txt.create();
            setbkmode(OPAQUE);
        }        
};
#define _TXTBOX_MAX_TXT_SIZE 256
class TXTInputBox{
    protected:
        int x, y, w, h; 
        TCHAR text[_TXTBOX_MAX_TXT_SIZE];
        int MAX_LENGTH=12; 
        COLORREF FillColor,LineColor[3]; 
        bool isActive=false; // 是否处于编辑状态
        bool isReadOnly=false; // 是否只读

    public:
        TXTInputBox(int x, int y, int w, int h, COLORREF FillColor, COLORREF LineColor[3]){
            this->x=x; this->y=y; this->w=w; this->h=h;
            this->FillColor=FillColor; this->LineColor[0]=LineColor[0]; this->LineColor[1]=LineColor[1]; this->LineColor[2]=LineColor[2];
        }

        void setParams(int x, int y, int w, int h,const TCHAR* text, COLORREF FillColor, COLORREF LineColor[3], bool isReadOnly=false){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            _tcscpy_s(this->text, text);
            this->FillColor = FillColor;
            for(int i=0;i<3;i++){
                this->LineColor[i] = LineColor[i];
            }
            this->isReadOnly = isReadOnly;
        }

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

void init(int width=500, int height=600){
    initgraph(width, height);
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

class WindowsType{
    protected:
        vector<Text> texts;
        vector<Button> buttons;
        vector<TXTInputBox> txtInputBoxes;
        bool RUNNING = false;
    public:
        bool isRunning(){ return RUNNING; }
        void close(){ 
            RUNNING = false;
            cleardevice();
        }
        void run(){ 
            RUNNING = true;
            cleardevice();
            fresh();
        }

        void fresh(){
            cleardevice();
            for(auto text : texts){text.create();}
            for(auto button : buttons){button.create();}
            for(auto txtInputBox : txtInputBoxes){txtInputBox.create();}
        }
};

class Windows_Index: public WindowsType{
    public:
        Windows_Index(){
            int x_mid = 400, y_mid = 300;
            texts.push_back(Text(x_mid, 50, _T("Number Guess Games"), 40, BLACK, _T("宋体"),make_pair(0,0)));
            texts.push_back(Text(100, 100, _T("Please choose the game mode:"), 20, BLACK, _T("宋体"),make_pair(-1,0)));
            buttons.push_back(Button(x_mid,150,350,50,Text(_T("Mode 1: Just guess for 1 try."), 20, WHITE, _T("宋体"),make_pair(0,0)), GREEN, WHITE, make_pair(0,0), make_pair(0,0)));
            buttons.push_back(Button(x_mid,225,350,50,Text(_T("Mode 2: Guess until right."), 20, WHITE, _T("宋体"),make_pair(0,0)), BLUE, WHITE, make_pair(0,0), make_pair(0,0)));            
            buttons.push_back(Button(x_mid,300,350,50,Text(_T("Mode 3: Guess for 10 tries."), 20, WHITE, _T("宋体"),make_pair(0,0)), RED, WHITE, make_pair(0,0), make_pair(0,0)));
            {
                int lineGap = 20;int y_start=360;int _i=0;
                texts.push_back(Text(25, y_start, _T("Game rules:"), 20, BLACK, _T("宋体"),make_pair(-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("1. This program will generate a random number."), 20, BLACK, _T("宋体"),make_pair(-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("2. You need to guess the number in limited attempts."), 20, BLACK, _T("宋体"),make_pair(-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("3. Program will provide feedback on your guess is bigger or smaller."), 20, BLACK, _T("宋体"),make_pair(-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("4. Different modes have different numbers of attempts."), 20, BLACK, _T("宋体"),make_pair(-1,0)));
            }
            
        }
};

int main(){ 
    WindowsType* windows[3] = {new Windows_Index()};

    init(800,600);

    ExMessage msg;	
    int WindowsIndex = 0;
    windows[WindowsIndex]->run();
	while(windows[WindowsIndex]->isRunning()){
        bool fresh_flag = false;
		while (peekmessage(&msg, EM_KEY))
		{
			if(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE){
                windows[WindowsIndex]->close();
                break;
            }
		}
        if(fresh_flag)windows[WindowsIndex]->fresh();
		Sleep(10);
	}
    

    wait_for_escape();

    return 0;
}
