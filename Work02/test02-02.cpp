#include <math.h>
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <stack>
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
        const TCHAR* getText(){
            return text;
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

        int getTextWidth(){
            makeFont();
            return textwidth(text);
        }

        int getTextLength(){
            return _tcslen(text);
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
        
        bool isBeingClicked(int x, int y){
            int draw_x = this->x - this->w * (this->writeMode.first+1)/2, 
                draw_y = this->y - this->h * (this->writeMode.second+1)/2;
            return (x >= draw_x && x <= draw_x + this->w 
                &&  y >= draw_y && y <= draw_y + this->h);
        }
};
#define _TXTBOX_MAX_TXT_SIZE 5
class TXTInputBox{
    protected:
        int x, y, w, h; 
        TCHAR txt[_TXTBOX_MAX_TXT_SIZE];
        const int maxTxtSize = _TXTBOX_MAX_TXT_SIZE;
        COLORREF FillColor,LineColor[3]; 
        bool isActive=false; // 是否处于编辑状态
        bool isReadOnly=false; // 是否只读
        pair <int,int> writeMode;
    public:        
        TXTInputBox(int x, int y, int w, int h, COLORREF FillColor, pair <int,int> writeMode=make_pair(0,0)){
            this->LineColor[0]=LIGHTGRAY; 
            this->LineColor[1]=BLACK; 
            this->LineColor[2]=BLUE;
            this->x=x; this->y=y; this->w=w; this->h=h;
            this->FillColor=FillColor;
            _tcscpy_s(this->txt, _TXTBOX_MAX_TXT_SIZE, _T(""));
            this->writeMode = writeMode;
        }

        void setParams(int x, int y, int w, int h, COLORREF FillColor, pair <int,int> writeMode=make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->FillColor = FillColor;
            for(int i=0;i<3;i++){
                this->LineColor[i] = LineColor[i];
            }
            this->isReadOnly = isReadOnly;
        }

        void create(){ // 绘制按钮 	
            int draw_x = x - w * (writeMode.first+1)/2, 
                draw_y = y - h * (writeMode.second+1)/2;

            setfillcolor(FillColor); 
            setbkmode(OPAQUE);
            solidrectangle(draw_x, draw_y, draw_x + w, draw_y + h); 
            if(isReadOnly){
                setlinecolor(LineColor[0]);
            }else if(isActive){
                setlinecolor(LineColor[2]);			
            }else{
                setlinecolor(LineColor[1]); 
            }
            settextstyle(this->h - 15, 0, _T("宋体"));
            setcolor(BLACK);
            outtextxy(draw_x + 10, draw_y + 10, txt);
            rectangle(draw_x, draw_y, draw_x + w, draw_y + h);

            if(isActive && !isReadOnly){
                setlinecolor(LineColor[2]);
                int cursor_x = draw_x + 10 + textwidth(this->txt); // 光标紧跟文本后
                int cursor_y1 = draw_y + 10;
                int cursor_y2 = draw_y + h - 10;
                line(cursor_x, cursor_y1, cursor_x, cursor_y2);
            }
        }

        bool isBeingClicked(int x, int y){
            int draw_x = this->x - this->w * (this->writeMode.first+1)/2, 
                draw_y = this->y - this->h * (this->writeMode.second+1)/2;
            bool ret =  (x >= draw_x && x <= draw_x + this->w 
                &&  y >= draw_y && y <= draw_y + this->h);
            if(ret && !isReadOnly){
                isActive = true;
            }else{
                isActive = false;
            }
            return ret;
        }

        bool pushChar(char ch){
            if(isActive && !isReadOnly){
                int len = _tcslen(this->txt);
                if(len < maxTxtSize - 1){
                    this->txt[len] = ch;
                    this->txt[len + 1] = _T('\0');
                }
                return true;
            }
            return false;
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
        virtual ~WindowsType() = default;
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

        virtual void mouseDown(int x, int y){printf("Empty mouse down handler:x=%d,y=%d\n", x, y);}
        virtual void charInput(char ch){printf("Empty char input handler: ch=%c\n", ch);}
};

class WindowsControl{
    private:
        stack<WindowsType*> windows;
    public:
        WindowsType* getWindow(){ return windows.top(); }
        bool isEmpty(){ return windows.empty(); }
        void openWindow(WindowsType* win){
            if(!windows.empty()){
                windows.top()->close();
            }
            windows.push(win);
            windows.top()->run();
        }
        void closeWindow(){ 
            if(!windows.empty()){
                windows.top()->close();
                delete windows.top();
                windows.pop();
            }
            if(!windows.empty()){
                windows.top()->run();
            }
        }
        void refresh(){
            if(!windows.empty()){
                windows.top()->fresh();
            }
        }
        void mouseDown(int x, int y){
            printf("pass mouse down msg\n");
            if(!windows.empty()){
                windows.top()->mouseDown(x, y);
            }
        }
        void charInput(char ch){
            printf("pass char input msg\n");
            if(!windows.empty()){
                windows.top()->charInput(ch);
            }
        }
};
WindowsControl mainWindows;

class Windows_Game: public WindowsType{
    private:
        int mode;
        TCHAR modeTitleText[3][_TEXT_MAX_TXT_SIZE]={
            _T("Mode 1: Just guess for 1 try."),
            _T("Mode 2: Guess until right."),
            _T("Mode 3: Guess for 10 tries.")
        };
        stack <TCHAR*> guessHistory;
        int guessedTimes = 0;
    public:
        Windows_Game(int mode){
            this->mode = mode;
            int x_mid = 400, y_mid = 300;
            texts.push_back(Text(x_mid, 50, modeTitleText[mode-1], 40, BLACK, _T("宋体"),make_pair(0,0)));
            texts.push_back(Text(x_mid, 100, _T("You have tried for: 0 times"), 20, BLACK, _T("宋体"),make_pair(0,0)));
            texts.push_back(Text(100, 150, _T("Please input your guess:"), 20, BLACK, _T("宋体"),make_pair(-1,0)));
            txtInputBoxes.push_back(TXTInputBox(x_mid,200,350,50, WHITE));
            texts.push_back(Text(100, 260, _T("Guess History:"), 20, BLACK, _T("宋体"),make_pair(0,0)));
            buttons.push_back(Button(760,410,100,50,Text(_T("Confirm"), 20, WHITE, _T("宋体"),make_pair(0,0)), GREEN, BLACK, make_pair(1,0), make_pair(0,0)));
            buttons.push_back(Button(760,480,100,50,Text(_T("Reset"), 20, WHITE, _T("宋体"),make_pair(0,0)), BROWN, BLACK, make_pair(1,0), make_pair(0,0)));
            buttons.push_back(Button(760,550,100,50,Text(_T("Exit"), 20, WHITE, _T("宋体"),make_pair(0,0)), RED, BLACK, make_pair(1,0), make_pair(0,0)));
        }

        void buttonConfirmHandler(){
            ;
        }

        void mouseDown(int x, int y) override{
            printf("get mouse down msg in game window\n");
            if(txtInputBoxes[0].isBeingClicked(x, y)){
                printf("click input box\n");
            }else{
                if(buttons[0].isBeingClicked(x, y)){
                    printf("click confirm button\n");
                    buttonConfirmHandler();
                }else if(buttons[1].isBeingClicked(x, y)){
                    printf("click reset button\n");
                }else if(buttons[2].isBeingClicked(x, y)){
                    printf("click exit button\n");
                    mainWindows.closeWindow();
                }
            }
        }

        void charInput(char chget) override{
            if('0' <= chget && chget <= '9'){
                txtInputBoxes[0].pushChar(chget);
            }
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

        void mouseDown(int x, int y) override{
            printf("get mouse down msg\n");
            for(int i = 0; i < buttons.size(); i++){
                if(buttons[i].isBeingClicked(x, y)){
                    printf("click button %d\n", i);
                    mainWindows.openWindow(new Windows_Game(i+1));
                    break;
                }
            }
        }
};



int main(){      
    init(800,600);
    ExMessage msg;

    mainWindows.openWindow(new Windows_Index());
    
	while(!mainWindows.isEmpty()){
        bool fresh_flag = false;

		while (peekmessage(&msg, EM_KEY | EM_MOUSE | EM_CHAR, true))
		{
			if(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE){
                mainWindows.closeWindow();
                break;
            }else if(msg.message == WM_LBUTTONDOWN){
                printf("mouse down: x=%d,y=%d\n", msg.x, msg.y);
                mainWindows.mouseDown(msg.x, msg.y);
                fresh_flag = true;
            }else if(msg.message == WM_CHAR){
                printf("char input: char=%c\n", msg.ch);
                mainWindows.charInput(msg.ch);
                fresh_flag = true;
            }
		}

        if(fresh_flag)mainWindows.refresh();
		Sleep(10);
	}
    

    closegraph();
    return 0;
}
