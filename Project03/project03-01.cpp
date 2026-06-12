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
