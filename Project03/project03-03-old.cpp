#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <tchar.h>
#include "./GUIBasicTypes.h"

class pixel{
    public:
        int r, g, b;
        pixel(int r=0, int g=0, int b=0): r(r), g(g), b(b) {}
};

class bmpStruct
{
    private:
        BITMAPFILEHEADER header;
        BITMAPINFOHEADER info;
        int width, height, biBitCount;
        unsigned char* paletteData;
        unsigned char* data;
    public:
        bmpStruct(){
            header = {0};
            width = 0;
            height = 0;
            biBitCount = 0;
            paletteData = NULL;
            data = NULL;
        }
        ~bmpStruct(){
            if(data) delete[] data;
            if(paletteData) delete[] paletteData;
        }

        bool openImage(LPCTSTR filename){
            if(data){
                delete[] data;
                data = NULL;
            }
            if(paletteData){
                delete[] paletteData;
                paletteData = NULL;
            }

            FILE* fp = _tfopen(filename, _T("rb"));
            if(fp == NULL){
                _tprintf(_T("Failed to open file: %s\n"), filename);
                return false;
            }
            
            fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);
            fread(&info, sizeof(BITMAPINFOHEADER), 1, fp);
            width = info.biWidth;
            height = info.biHeight;
            biBitCount = info.biBitCount;
            int lineByte = (width * biBitCount / 8 + 3) / 4 * 4; // 每行字节数（4字节对齐）
            
            if(biBitCount == 8){
                // 8位图像需要读取调色板
                int paletteSize = 256 * sizeof(RGBQUAD);
                paletteData = new unsigned char[paletteSize];
                fread(paletteData, sizeof(unsigned char), paletteSize, fp);
            }
            
            data = new unsigned char[lineByte * height];
            fread(data, sizeof(unsigned char), lineByte * height, fp);
            fclose(fp);
            return true;
        }

        pixel getPixel(int x, int y){
            if(x < 0 || x >= width || y < 0 || y >= height){
                printf("Pixel coordinates out of bounds: (%d, %d)\n", x, y);
                return pixel(-1, -1, -1); // 返回无效坐标
            }
            int lineByte = (width * biBitCount / 8 + 3) / 4 * 4; // 每行字节数（4字节对齐）
            unsigned char* pixelData = data + (height - 1 - y) * lineByte + x * (biBitCount / 8);
            if(biBitCount == 24){
                // 24位图像，直接返回RGB值
                return pixel(pixelData[2], pixelData[1], pixelData[0]); // RGB顺序
            } else if(biBitCount == 8){
                // 8位图像，返回调色板中的颜色索引
                unsigned char colorIndex = pixelData[0];
                return pixel(paletteData[colorIndex * 4 + 2], paletteData[colorIndex * 4 + 1], paletteData[colorIndex * 4]); // RGB顺序
            } else {
                printf("Unsupported bit count: %d\n", biBitCount);
                return pixel(-1, -1, -1); // 返回无效坐标
            }
        }
};


WindowsControl mainWindows;

class Windows_Main: public WindowsType{
    private:
        bmpStruct img;
        IMAGE imgForGUI;
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
                putimage(10, 10, &imgForGUI);
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
                int loadResult = loadimage(&imgForGUI, _fileAddress, 0, 0, true);
                if(loadResult == 2){
                    _tprintf(_T("Failed to find image: %s\n"), _fileAddress);
                    hasLoadedImage = false;
                }else if(loadResult == 5007){
                    _tprintf(_T("Failed to load image: %s\n"), _fileAddress);
                    hasLoadedImage = false;
                }else {
                    hasLoadedImage = true;
                    putimage(10, 10, &imgForGUI);
                    img.openImage(_fileAddress);
                    _tprintf(_T("Image loaded %d: %s\n"), loadResult, _fileAddress);
                }
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
