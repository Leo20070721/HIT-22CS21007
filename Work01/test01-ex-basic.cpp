#include <math.h>
#include <iostream>
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include <vector>
#include <stack>
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
initgraph(520, 400);
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
MainWindowData MainWindowDataBasic = {
	20, 120, 
	20, 4,
	{5, 5, 5, 5},
	80, 50, 20, 10,
	{	
		WHITE, WHITE, WHITE, LIGHTGRAY, LIGHTGRAY, 
		WHITE, WHITE, WHITE, LIGHTGRAY, LIGHTGRAY, 
		WHITE, WHITE, WHITE, LIGHTGRAY, LIGHTCYAN, 
		WHITE, WHITE, LIGHTCYAN, LIGHTGRAY, LIGHTCYAN, 
	},
	{	
		_T("7"), _T("8"), _T("9"), _T("+"), _T("("), 
		_T("4"), _T("5"), _T("6"), _T("-"), _T(")"), 
		_T("1"), _T("2"), _T("3"), _T("*"), _T("Back"),
		_T("0"), _T("."), _T("="), _T("/"), _T("ESC"),
	},

	20,
	{_T("Calculator")},

	20,40,
	1,
	480, 50, 20,
	{36},
	{false},
	{WHITE},
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
		
		isActive = 0;
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
	void ErrorMessageBox(const TCHAR* err, const TCHAR* msg){
		MessageBox(NULL, msg, err, MB_OK | MB_ICONERROR);		
	}

	bool ParenthesesLegalCheck(const TCHAR* txt){
		int balance = 0;
		for(int i = 0; txt[i] != '\0'; i++){
			if(txt[i] == '(') balance++;
			else if(txt[i] == ')') balance--;
			if(balance < 0){
				ErrorMessageBox(_T("Syntax Error"), _T("Parentheses are not balanced."));
				return false;
			} 
		}
		return true;
	}

	//注：算式解析由GPT-5.4 mini生成
	void Calculate(MainWindowType &MainWindow){
		if(!ParenthesesLegalCheck(MainWindow.txts[0].text)) return;
		stack<double> numStack;
		stack<char> opStack;

		TCHAR* expr = MainWindow.txts[0].text;
		bool ok = true;

		auto precedence = [](char op)->int{
			if(op == '+' || op == '-') return 1;
			if(op == '*' || op == '/') return 2;
			return 0;
		};

		auto applyTop = [&]()->bool{
			if(numStack.size() < 2 || opStack.empty()){
				ErrorMessageBox(_T("Syntax Error"), _T("Invalid expression."));
				return false;
			}

			double right = numStack.top();
			numStack.pop();
			double left = numStack.top();
			numStack.pop();
			char op = opStack.top();
			opStack.pop();
			double result = 0.0;

			switch(op){
			case '+': result = left + right; break;
			case '-': result = left - right; break;
			case '*': result = left * right; break;
			case '/':
				if(right == 0.0){
					ErrorMessageBox(_T("Math Error"), _T("Division by zero."));
					return false;
				}
				result = left / right;
				break;
			default:
				ErrorMessageBox(_T("Syntax Error"), _T("Invalid operator."));
				return false;
			}

			numStack.push(result);
			return true;
		};

		for(int i = 0; expr[i] != '\0' && ok; i++){
			TCHAR ch = expr[i];

			if(ch == ' ' || ch == '\t') continue;

			if((ch >= '0' && ch <= '9') || ch == '.' || ((ch == '+' || ch == '-') && (i == 0 || expr[i - 1] == '(' || expr[i - 1] == '+' || expr[i - 1] == '-' || expr[i - 1] == '*' || expr[i - 1] == '/'))){
				int j = i;
				bool negative = false;
				bool hasSign = false;

				if(expr[j] == '+' || expr[j] == '-'){
					hasSign = true;
					negative = (expr[j] == '-');
					j++;

					if(expr[j] == '('){
						if(negative){
							numStack.push(0.0);
							while(!opStack.empty() && opStack.top() != '(' && precedence(opStack.top()) >= precedence('-')){
								if(!applyTop()){ ok = false; break; }
							}
							if(!ok) break;
							opStack.push('-');
						}
						i = j - 1;
						continue;
					}
				}

				double value = 0.0;
				double fraction = 0.1;
				bool seenDigit = false;
				bool seenDot = false;

				for(; expr[j] != '\0'; j++){
					TCHAR numCh = expr[j];
					if(numCh >= '0' && numCh <= '9'){
						seenDigit = true;
						if(!seenDot){
							value = value * 10 + (numCh - '0');
						}else{
							value += (numCh - '0') * fraction;
							fraction *= 0.1;
						}
					}else if(numCh == '.'){
						if(seenDot){
							ErrorMessageBox(_T("Syntax Error"), _T("Invalid number format."));
							ok = false;
							break;
						}
						seenDot = true;
					}else{
						break;
					}
				}

				if(!ok) break;
				if(!seenDigit){
					ErrorMessageBox(_T("Syntax Error"), _T("Invalid number format."));
					ok = false;
					break;
				}

				if(hasSign && negative) value = -value;
				numStack.push(value);
				i = j - 1;
				continue;
			}

			if(ch == '('){
				opStack.push(ch);
				continue;
			}

			if(ch == ')'){
				while(!opStack.empty() && opStack.top() != '('){
					if(!applyTop()){ ok = false; break; }
				}
				if(!ok) break;
				if(opStack.empty()){
					ErrorMessageBox(_T("Syntax Error"), _T("Parentheses are not balanced."));
					ok = false;
					break;
				}
				opStack.pop();
				continue;
			}

			if(ch == '+' || ch == '-' || ch == '*' || ch == '/'){
				while(!opStack.empty() && opStack.top() != '(' && precedence(opStack.top()) >= precedence(ch)){
					if(!applyTop()){ ok = false; break; }
				}
				if(!ok) break;
				opStack.push(ch);
				continue;
			}

			ErrorMessageBox(_T("Syntax Error"), _T("Illegal character found."));
			ok = false;
		}

		while(ok && !opStack.empty()){
			if(opStack.top() == '('){
				ErrorMessageBox(_T("Syntax Error"), _T("Parentheses are not balanced."));
				ok = false;
				break;
			}
			if(!applyTop()){
				ok = false;
				break;
			}
		}

		if(!ok || numStack.size() != 1) return;

		double result = numStack.top();
		_stprintf_s(MainWindow.txts[0].text, _TXTBOX_MAX_SIZE, _T("%.10g"), result);
	}	
};
struct MessageWorker{
	bool CharLegalCheck(TCHAR ch){
		if(ch >= '0' && ch <= '9') return true;
		if(ch == '.' || ch == '-') return true;
		if(ch == '+' || ch == '-' || ch == '*' || ch == '/') return true;
		if(ch == '(' || ch == ')') return true;
		return false;
		
	}

	void HandleMouseLButtonDown(ExMessage msg, MainWindowType &MainWindow){
		bool isCalculated=false;

		for(int i = 0; i < MainWindow.btns.size(); i++){
			if(msg.x >= MainWindow.btns[i].x && msg.x <= MainWindow.btns[i].x + MainWindow.btns[i].w && msg.y >= MainWindow.btns[i].y && msg.y <= MainWindow.btns[i].y + MainWindow.btns[i].h){
				isCalculated = true;
				if(_tcscmp(MainWindow.btns[i].text, _T("ESC")) == 0){
					MainWindow.RUNNING = false;
					return;
				}else if(_tcscmp(MainWindow.btns[i].text, _T("Back")) == 0){					
					int len = _tcslen(MainWindow.txts[MainWindow.isActive].text);
					if(len > 0){
						MainWindow.txts[MainWindow.isActive].text[len - 1] = '\0';
					}					
				}else if(_tcscmp(MainWindow.btns[i].text, _T("=")) == 0){ 
					Calculator().Calculate(MainWindow);
				}else{
					_tcscat_s(MainWindow.txts[0].text, _TXTBOX_MAX_SIZE, MainWindow.btns[i].text);
				}				
								
			}
		}

		if(isCalculated){MainWindow.MainWindowDraw();}
	}

	void HandleKeyboardCharInput(ExMessage msg, MainWindowType &MainWindow){
		if(MainWindow.isActive == -1) return;
		if(MainWindow.txts[MainWindow.isActive].isReadOnly) return;
		
		if(msg.ch == '\b'){ // 处理退格键
			int len = _tcslen(MainWindow.txts[MainWindow.isActive].text);
			if(len > 0){
				MainWindow.txts[MainWindow.isActive].text[len - 1] = '\0';
			}			
		}else if(msg.ch == '='){
			Calculator().Calculate(MainWindow);
		}else if(CharLegalCheck(msg.ch)){
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
	SetFocus(GetHWnd());

	MainWindowType MainWindow01;
	MainWindow01.MainWindowData = MainWindowDataBasic;
	MainWindow01.MainWindowInit();
	MainWindow01.MainWindowDraw();

	ExMessage msg;	
	while(MainWindow01.RUNNING){
		while (peekmessage(&msg, EM_MOUSE | EM_KEY | EM_CHAR))
		{
			if(msg.message == WM_LBUTTONDOWN){
				MessageWorker().HandleMouseLButtonDown(msg, MainWindow01);
			}else if(msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE){
				MainWindow01.RUNNING = false;
				break;
			}else if(msg.message == WM_CHAR){
				MessageWorker().HandleKeyboardCharInput(msg, MainWindow01);
			}
		}		
		Sleep(10);
	}

	//wait_for_escape();
	closegraph();			// 关闭绘图窗口
	return 0;
}
