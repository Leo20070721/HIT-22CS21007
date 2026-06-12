#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <queue>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define _TEXT_MAX_TXT_SIZE 256
#define _TXTBOX_MAX_TXT_SIZE 5

void StringToTCharVector(const std::string& src, TCHAR* _temp) {
    _temp[0]='\0';
    if (src.empty()) {
        return ;// 返回一个只包含字符串结束符的空数组
    }

#ifdef UNICODE
    // Unicode 构建：将 char 转为 wchar_t
    int wideLen = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, nullptr, 0);
    if (wideLen == 0) {
        // 转换失败处理
        return ;
    }

    std::vector<TCHAR> tcharVec(wideLen);
    MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, tcharVec.data(), wideLen);
    int pos=0;
    for(auto ch:tcharVec){
        _temp[pos++]=ch;
    }
    return ;
#else
    // ANSI 构建：直接复制
    std::vector<TCHAR> tcharVec(src.begin(), src.end());
    tcharVec.push_back(_T('\0'));
    int pos=0;
    for(auto ch:tcharVec){
        _temp[pos++]=ch;
    }
    return ;
#endif
}

class Text{
    protected:
        int height;
        TCHAR font[15];
        COLORREF color;
        int x, y; 
        std::pair <int,int> writeMode;
        TCHAR text[_TEXT_MAX_TXT_SIZE];
    public:
        Text(){
            this->x = 0;
            this->y = 0;
            this->writeMode = std::make_pair(0, 0);
            _tcscpy_s(this->text, _T("Hello World!"));
            this->height = 20;
            this->color = BLACK;
            _tcscpy_s(this->font, _T("宋体"));
        }
        Text(const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), std::pair <int,int> writeMode=std::make_pair(0,0)){
            this->x = 0;
            this->y = 0;
            this->writeMode = writeMode;
            _tcscpy_s(this->text, text);
            this->height = height;
            this->color = color;
            _tcscpy_s(this->font, font);
            
        }
        Text(int x, int y, const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), std::pair <int,int> writeMode=std::make_pair(0,0)){
            this->x = x;
            this->y = y;
            this->writeMode = writeMode;
            _tcscpy_s(this->text, text);
            this->height = height;
            this->color = color;
            _tcscpy_s(this->font, font);
            
        }

        void setParams(int x, int y, const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), std::pair <int,int> writeMode=std::make_pair(0,0)){
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
        std::pair<int,int> makeFont(){
            settextstyle(height, 0, font);
            setcolor(color);
            return std::make_pair(textwidth(text), textheight(text));
        }

        void create(){ 
            std::pair<int,int> font_size = makeFont();
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
        std::pair <int,int> writeMode;
        std::pair <int,int> textWriteMode;

    public:
        Button(int x, int y, int w, int h,const TCHAR* text, COLORREF FillColor, COLORREF LineColor, std::pair <int,int> writeMode=std::make_pair(0,0), std::pair <int,int> txtMode=std::make_pair(0,0)){
            this->setParams(x, y, w, h, text, FillColor, LineColor, writeMode, txtMode);
        }
        Button(int x, int y, int w, int h,const Text txt, COLORREF FillColor, COLORREF LineColor, std::pair <int,int> writeMode=std::make_pair(0,0), std::pair <int,int> textWriteMode=std::make_pair(0,0)){
            this->setParams(x, y, w, h, txt, FillColor, LineColor, writeMode, textWriteMode);  
        }

        void setParams(int x, int y, int w, int h,TCHAR* text, COLORREF FillColor, COLORREF LineColor, std::pair <int,int> writeMode=std::make_pair(0,0), std::pair <int,int> textWriteMode=std::make_pair(0,0)){
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
        void setParams(int x, int y, int w, int h,Text txt, COLORREF FillColor, COLORREF LineColor, std::pair <int,int> writeMode=std::make_pair(0,0), std::pair <int,int> textWriteMode=std::make_pair(0,0)){
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

        void setTextParams(int x, int y, const TCHAR* text, int height=20, COLORREF color=BLACK, const TCHAR* font=_T("宋体"), std::pair <int,int> writeMode=std::make_pair(0,0)){ 
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
class TXTInputBox{
    protected:
        int x, y, w, h; 
        TCHAR txt[_TXTBOX_MAX_TXT_SIZE];
        const int maxTxtSize = _TXTBOX_MAX_TXT_SIZE;
        COLORREF FillColor,LineColor[3]; 
        bool isActive=false; // 是否处于编辑状态
        bool isReadOnly=false; // 是否只读
        std::pair <int,int> writeMode;
    public:        
        TXTInputBox(int x, int y, int w, int h, COLORREF FillColor, std::pair <int,int> writeMode=std::make_pair(0,0)){
            this->LineColor[0]=LIGHTGRAY; 
            this->LineColor[1]=BLACK; 
            this->LineColor[2]=BLUE;
            this->x=x; this->y=y; this->w=w; this->h=h;
            this->FillColor=FillColor;
            _tcscpy_s(this->txt, _TXTBOX_MAX_TXT_SIZE, _T(""));
            this->writeMode = writeMode;
        }

        void setValue(TCHAR *txt){ 
            _tcscpy_s(this->txt, _TXTBOX_MAX_TXT_SIZE, txt);
        }

        void setParams(int x, int y, int w, int h, COLORREF FillColor, std::pair <int,int> writeMode=std::make_pair(0,0)){
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
            if(isReadOnly){
                setlinecolor(LineColor[0]);
            }else if(isActive){
                setlinecolor(LineColor[2]);			
            }else{
                setlinecolor(LineColor[1]); 
            }
            solidrectangle(draw_x, draw_y, draw_x + w, draw_y + h);
            rectangle(draw_x, draw_y, draw_x + w, draw_y + h);  

            setcolor(BLACK);
            settextstyle(this->h - 15, 0, _T("宋体"));       
            outtextxy(draw_x + 10, draw_y + 10, txt);      

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

        bool popChar(){ 
            if(isActive && !isReadOnly){
                int len = _tcslen(this->txt);
                if(len > 0){
                    this->txt[len - 1] = _T('\0');
                }
                return true;
            }
            return false;
        }

        const TCHAR* getText(){
            return txt;
        }
};
class BarChart{ 
    protected:
        int x, y, w, h;
        int wide_percent=90;
        std::map< std::string, int> data;
        const COLORREF BarColor[15]={
            RGB(0xA8, 0xDA, 0xDC),//浅湖蓝
            RGB(0xF4, 0xA2, 0x61),//暖杏橙
            RGB(0xE7, 0x6F, 0x51),//珊瑚粉
            RGB(0x2A, 0x9D, 0x8F),//薄荷绿
            RGB(0xE9, 0xC4, 0x6A),//淡鹅黄
            RGB(0x90, 0xBE, 0x6D),//草绿
            RGB(0xFA, 0xB2, 0xB2),//浅豆沙粉
            RGB(0x9B, 0x5D, 0xE5),//薰衣草紫
            RGB(0xf9, 0xC7, 0x7F),//柔和金
            RGB(0x43, 0xAA, 0x8B),//森林绿
            RGB(0x57, 0x75, 0x90),//灰蓝
            RGB(0xF9, 0x84, 0x4A),//暖橘
            RGB(0xB5, 0x83, 0x8D),//灰粉
            RGB(0x7D, 0xCF, 0xB6)//浅薄荷
        };
    public:
        BarChart(int x, int y, int w, int h){
            this->setParams(x, y, w, h);
        }
        void setParams(int x, int y, int w, int h){ 
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }

        void setData(std::map< std::string, int> data){ 
            this->data = data;
        }

        void create(int mode=0){ 
            int bottom = y + h;            
            
            setcolor(BLACK);
            line(x, bottom, x + w, bottom);
            line(x, bottom, x, y);

            std::vector <std::pair<std::string, int>> bar_data;
            switch (mode)
            {
            case 1:{
                std::priority_queue<std::pair<int, std::string>> _mid;  
                for(auto i : data){
                    _mid.push(std::make_pair(i.second, i.first));
                }
                while(!_mid.empty()){
                    bar_data.push_back(std::make_pair(_mid.top().second, _mid.top().first));
                    _mid.pop();
                }
                break;
            }
                
            case 0:
            default:{
                for(auto i : data){
                    bar_data.push_back(std::make_pair(i.first, i.second));
                }
                break;
            }                
            } 

            write_BarChart(bar_data);
        }

        void write_BarChart(std::vector<std::pair<std::string, int>> data){
            int per_wide = w / data.size();
            int max_num=-1;
            for(const auto& it : this->data){ 
                if(it.second > max_num){
                    max_num = it.second;
                }
            }
            int per_height = h / max_num;

            for(int i=0; i<data.size(); i++){
                int real_w = 0.01 * this->wide_percent * per_wide;
                int gap_w = 0.01 / 2 * (100-this->wide_percent) * per_wide;
                int x = i * per_wide;
                int y = h - data[i].second * per_height;
                int w = per_wide;
                
                int h = data[i].second * per_height;
                std::string text = data[i].first;
                setfillcolor(this->BarColor[i]);
                fillrectangle(x+gap_w, y, real_w, h);
                settextcolor(BLACK);
                TCHAR temp[20];
                StringToTCharVector(data[i].first, temp);
                outtextxy(x+gap_w, y-(textheight(temp)*11/10), temp);
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
        std::vector<Text> texts;
        std::vector<Button> buttons;
        std::vector<TXTInputBox> txtInputBoxes;
        bool RUNNING = false;        
    public:
        virtual ~WindowsType() = default;
        bool isRunning(){ return RUNNING; }
        virtual void close(){ 
            aboutToDisappear();
            RUNNING = false;
            cleardevice();
        }
        virtual void run(){ 
            RUNNING = true;
            cleardevice();
            fresh();
            aboutToAppear();
        }

        virtual void fresh(){
            cleardevice();
            for(auto text : texts){text.create();}
            for(auto button : buttons){button.create();}
            for(auto txtInputBox : txtInputBoxes){txtInputBox.create();}
        }

        virtual void reInit(){};
        virtual void aboutToAppear(){};
        virtual void aboutToDisappear(){};

        virtual void mouseDown(int x, int y){printf("Empty mouse down handler:x=%d,y=%d\n", x, y);}
        virtual void charInput(char ch){printf("Empty char input handler: ch=%c\n", ch);}
        virtual void keyDown(int keyCode){printf("Empty key down handler: keyCode=%d\n", keyCode);}
};

class WindowsControl{
    private:
        std::stack<WindowsType*> windows;
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
        void backWithRebuilt(){
            if(!windows.empty()){
                windows.top()->close();
                delete windows.top();
                windows.pop();
                if(!windows.empty()){
                    windows.top()->reInit();
                    windows.top()->run();
                }
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
        void keyDown(int keyCode){ 
            printf("pass key down msg\n");
            if(!windows.empty()){
                windows.top()->keyDown(keyCode);
            }
        }
};

class MsgProcessor{
    public:
        bool checkMsg(ExMessage *msg=NULL, WindowsControl* mainWindows=NULL){ 
            bool fresh_flag = false;
            if(msg->message == WM_KEYDOWN && msg->vkcode == VK_ESCAPE){
                mainWindows->closeWindow();
            }else if(msg->message == WM_KEYDOWN && msg->vkcode == VK_BACK){
                mainWindows->keyDown(msg->vkcode);
                fresh_flag = true;
            }else if(msg->message == WM_LBUTTONDOWN){
                printf("mouse down: x=%d,y=%d\n", msg->x, msg->y);
                mainWindows->mouseDown(msg->x, msg->y);
                fresh_flag = true;
            }else if(msg->message == WM_CHAR){
                printf("char input: char=%c\n", msg->ch);
                mainWindows->charInput(msg->ch);
                fresh_flag = true;
            }
            return fresh_flag;
        }
};
