#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include "./GUIBasicTypes.h"


WindowsControl mainWindows;

class Windows_Main: public WindowsType{
    
    public:
        Windows_Main(){        
            setbkcolor(BROWN);    
            cleardevice();
            buttons.push_back(Button(1300, 180, 200, 50, _T("Load Image"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 360, 200, 50, _T("Trans to Gray"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 540, 200, 50, _T("Edge detection"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 720, 200, 50, _T("Sharpening"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
        }

        void mouseDown(int x, int y) override{
            printf("get mouse down msg in final window\n");
            if(buttons[0].isBeingClicked(x, y)){
                printf("click button 0\n");
            }else if(buttons[1].isBeingClicked(x, y)){
                printf("click button 1\n");
            }else if(buttons[2].isBeingClicked(x, y)){
                printf("click button 2\n");
            }else if(buttons[3].isBeingClicked(x, y)){
                printf("click button 3\n");
            }
        }
};

using namespace std;

int main(){      
    init(1600,900);
    ExMessage msg;

    mainWindows.openWindow(new Windows_Main());
    MsgProcessor msgProcessor;

	while(!mainWindows.isEmpty()){
        bool fresh_flag = false;

		if(peekmessage(&msg, EM_KEY | EM_MOUSE | EM_CHAR, true))
		{
			fresh_flag = msgProcessor.checkMsg(&msg, &mainWindows);
		}

        if(fresh_flag)mainWindows.refresh();
		Sleep(10);
	}
    

    closegraph();
    return 0;
}
