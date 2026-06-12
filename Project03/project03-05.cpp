#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <tchar.h>
#include "./GUIBasicTypes.h"

WindowsControl mainWindows;

class imageEdit{
    public:
        IMAGE transToGray(const IMAGE img_src){
            IMAGE img = img_src;
            int width = img.getwidth();
            int height = img.getheight();
            DWORD* pBuf = GetImageBuffer(&img);

            for (int i = 0; i < width * height; ++i) {
                unsigned char b = (pBuf[i] >> 16) & 0xFF;
                unsigned char g = (pBuf[i] >> 8)  & 0xFF;
                unsigned char r = pBuf[i] & 0xFF;
                unsigned char gray = (unsigned char)(r * 0.299 + g * 0.587 + b * 0.114);
                pBuf[i] = BGR(RGB(gray, gray, gray));
            }
            return img;
        }

        IMAGE edgeDetection(const IMAGE& img_src) {//Sobel
            int w = img_src.getwidth();
            int h = img_src.getheight();
            IMAGE dst;
            dst.Resize(w, h);

            DWORD* pSrc = GetImageBuffer(const_cast<IMAGE*>(&img_src));
            DWORD* pDst = GetImageBuffer(&dst);

            if (!pSrc || !pDst) return dst;

            const int Gx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
            const int Gy[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

            for (int y = 1 ; y < h - 1; y++) {
                for (int x = 1; x < w - 1; x++) {
                    int sumX = 0, sumY = 0;
                    for (int j = -1; j <= 1; j++) {
                        for (int i = -1; i <= 1; i++) {
                            int gray = pSrc[(y + j) * w + (x + i)] & 0xFF;
                            sumX += gray * Gx[j + 1][i + 1];
                            sumY += gray * Gy[j + 1][i + 1];
                        }
                    }
                    int mag = (int)sqrt((double)(sumX * sumX + sumY * sumY));
                    if (mag > 255) mag = 255;
                    pDst[y * w + x] = BGR(RGB(mag, mag, mag));
                }
            }

            for (int y = 0; y < h; y++) {
                pDst[y * w] = pDst[y * w + 1];
                pDst[y * w + w - 1] = pDst[y * w + w - 2];
            }
            for (int x = 1; x < w - 1; x++) {
                pDst[x] = pDst[w + x];
                pDst[(h-1)*w + x] = pDst[(h-2)*w + x];
            }

            return dst;
        }
};

class Windows_Main: public WindowsType{
    private:
        IMAGE img;
        bool hasLoadedImage;
    
    public:
        Windows_Main(){        
            hasLoadedImage = false;
            setbkcolor(BROWN);    
            cleardevice();
            buttons.push_back(Button(1300, 180, 200, 50, _T("Load Image"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 360, 200, 50, _T("Trans to Gray"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 540, 200, 50, _T("Edge detection"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 720, 200, 50, _T("Sharpening"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
        }

        void fresh() override{
            cleardevice();
            if(hasLoadedImage){
                putimage(10, 10, &img);
            }
            for(auto text : texts){text.create();}
            for(auto button : buttons){button.create();}
            for(auto txtInputBox : txtInputBoxes){txtInputBox.create();}
        }

        void mouseDown(int x, int y) override{
            printf("get mouse down msg in final window\n");
            if(buttons[0].isBeingClicked(x, y)){
                printf("Tried to load image\n");
                TCHAR _fileAddress[256];
                InputBox(_fileAddress, 256, _T("Please input the image address:"));
                int loadResult = loadimage(&img, _fileAddress, 0, 0, true);
                if(loadResult == 2){
                    _tprintf(_T("Failed to find image: %s\n"), _fileAddress);
                    hasLoadedImage = false;
                }else if(loadResult == 5007){
                    _tprintf(_T("Failed to load image: %s\n"), _fileAddress);
                    hasLoadedImage = false;
                }else {
                    hasLoadedImage = true;
                    putimage(10, 10, &img);
                    _tprintf(_T("Image loaded %d: %s\n"), loadResult, _fileAddress);
                }
            }else if(buttons[1].isBeingClicked(x, y)){                  
                printf("Tried to convert to gray\n");
                if(hasLoadedImage){
                    imageEdit imgEdit;
                    img = imgEdit.transToGray(img);
                }
            }else if(buttons[2].isBeingClicked(x, y)){
                printf("Tried to detect edges\n");
                if(hasLoadedImage){
                    imageEdit imgEdit;
                    img = imgEdit.edgeDetection(img);
                }
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
