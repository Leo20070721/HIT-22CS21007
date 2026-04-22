#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>

#include "./GUIWindows.cpp"

using namespace std;

int main(){      
    init(800,600);
    ExMessage msg;

    mainWindows.openWindow(new Windows_Index());
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
