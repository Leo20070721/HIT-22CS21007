#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <tchar.h>
#include "./GUIBasicTypes.h"//自定义头文件

WindowsControl mainWindows;

class imageEdit{//图片操作类
    public:
        IMAGE transToGray(const IMAGE& img_src){//转为灰度图
            int width = img_src.getwidth();
            int height = img_src.getheight();
            IMAGE img;
            img.Resize(width, height);
            DWORD* pBuf_src = GetImageBuffer(&img_src);
            DWORD* pBuf = GetImageBuffer(&img);

            for (int i = 0; i < width * height; ++i) {//二维遍历，转化算法
                unsigned char b = (pBuf_src[i] >> 16) & 0xFF;
                unsigned char g = (pBuf_src[i] >> 8)  & 0xFF;
                unsigned char r = pBuf_src[i] & 0xFF;
                unsigned char gray = (unsigned char)(r * 0.299 + g * 0.587 + b * 0.114);
                pBuf[i] = BGR(RGB(gray, gray, gray));
            }
            return img;
        }

        IMAGE edgeDetection(const IMAGE& img_src) {//边缘检测算法，采用Sobel算子
            int w = img_src.getwidth();
            int h = img_src.getheight();
            IMAGE dst;
            dst.Resize(w, h);

            DWORD* pSrc = GetImageBuffer(const_cast<IMAGE*>(&img_src));
            DWORD* pDst = GetImageBuffer(&dst);

            if (!pSrc || !pDst) return dst;

            const int Gx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
            const int Gy[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

            for (int y = 1 ; y < h - 1; y++) {//二维迭代，注意边界
                for (int x = 1; x < w - 1; x++) {
                    int sumX = 0, sumY = 0;//卷积计算
                    for (int j = -1; j <= 1; j++) {
                        for (int i = -1; i <= 1; i++) {
                            int gray = pSrc[(y + j) * w + (x + i)] & 0xFF;
                            sumX += gray * Gx[j + 1][i + 1];
                            sumY += gray * Gy[j + 1][i + 1];
                        }
                    }
                    int mag = (int)sqrt((double)(sumX * sumX + sumY * sumY));//梯度计算
                    if (mag > 255) mag = 255;//上限截断
                    pDst[y * w + x] = BGR(RGB(mag, mag, mag));
                }
            }

            //卷积计算
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

        IMAGE sharpenColor(const IMAGE& src, int strength = 1) {//图片锐化，采用Laplace算子
            int w = src.getwidth();
            int h = src.getheight();
            IMAGE dst;
            dst.Resize(w, h);

            DWORD* pSrc = GetImageBuffer(const_cast<IMAGE*>(&src));
            DWORD* pDst = GetImageBuffer(&dst);

            const int kernel[3][3] = {
                { 0, -1*strength,  0},
                {-1*strength,  1+4*strength, -1*strength},
                { 0, -1*strength,  0}
            };

            for (int y = 1; y < h - 1; y++) {//二维迭代，注意边界
                for (int x = 1; x < w - 1; x++) {
                    int sumB = 0, sumG = 0, sumR = 0;//卷积计算

                    for (int ky = -1; ky <= 1; ky++) {
                        for (int kx = -1; kx <= 1; kx++) {
                            DWORD color = pSrc[(y +ky) * w + (x + kx)];
                            int b =  color        & 0xFF;
                            int g = (color >> 8)  & 0xFF;
                            int r = (color >> 16) & 0xFF;

                            int k = kernel[ky + 1][kx + 1];
                            sumB += b * k;
                            sumG += g * k;
                            sumR += r * k;
                        }
                    }

                    //上限截断
                    sumB = std::min(std::max(sumB, 0), 255);
                    sumG = std::min(std::max(sumG, 0), 255);
                    sumR = std::min(std::max(sumR, 0), 255);

                    pDst[y * w + x] = BGR(RGB(sumR, sumG, sumB));
                }
            }

            //边界处理
            for (int y = 0; y < h; y++) {
                pDst[y * w] = pSrc[y * w];
                pDst[y * w + w - 1] = pSrc[y * w + w - 1];
            }
            for (int x = 0; x < w; x++) {
                pDst[x] = pSrc[x];
                pDst[(h - 1) * w + x] = pSrc[(h - 1) * w + x];
            }

            return dst;
        }
};

class Windows_Main: public WindowsType{//主界面类，继承自页面类
    private:
        IMAGE img;//处理图片缓存
        bool hasLoadedImage;//是否已打开图片
    
    public:
        Windows_Main(){//主界面构造函数，储存GUI界面组件        
            hasLoadedImage = false;
            setbkcolor(BROWN);    
            cleardevice();
            buttons.push_back(Button(1300, 125, 200, 50, _T("Load Image"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 280, 200, 50, _T("Trans to Gray"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 435, 200, 50, _T("Edge Detection"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 590, 200, 50, _T("Sharpening"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
            buttons.push_back(Button(1300, 745, 200, 50, _T("Save Image"), LIGHTGRAY, BLACK, std::make_pair(-1,0), std::make_pair(0,0)));
        }

        void fresh() override{//重写页面刷新虚函数，显示缓存的图片
            cleardevice();
            if(hasLoadedImage){
                putimage(10, 10, &img);
            }
            for(auto text : texts){text.create();}
            for(auto button : buttons){button.create();}
            for(auto txtInputBox : txtInputBoxes){txtInputBox.create();}
        }

        void mouseDown(int x, int y) override{//重写鼠标点击操作虚函数，处理各个按钮被按下时的反应
            printf("get mouse down msg in final window\n");
            if(buttons[0].isBeingClicked(x, y)){//“Load Image”按钮被点击，读取地址，加载图像
                printf("Tried to load image\n");
                TCHAR _fileAddress[512];
                InputBox(_fileAddress, 512, _T("Please input the image address:"));
                
                //处理空地址与地址前后的双引号
                if(_fileAddress[0] == '\0'){
                    printf("Empty address, canceled loading image\n");
                    return;
                }else if(_fileAddress[0] == '\"' && _fileAddress[_tcslen(_fileAddress)-1] == '\"'){
                    _tcsncpy_s(_fileAddress, _fileAddress+1, _tcslen(_fileAddress)-2);
                }

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
            }else if(buttons[1].isBeingClicked(x, y)){//“Trans to Grey”按钮被点击，尝试将图像转为灰度图
                printf("Tried to convert to gray\n");
                if(hasLoadedImage){
                    imageEdit imgEdit;
                    img = imgEdit.transToGray(img);
                }
            }else if(buttons[2].isBeingClicked(x, y)){//“Edge Detection”按钮被点击，尝试对图像进行边缘检测
                printf("Tried to detect edges\n");
                if(hasLoadedImage){
                    imageEdit imgEdit;
                    img = imgEdit.edgeDetection(img);
                }
            }else if(buttons[3].isBeingClicked(x, y)){//“Sharpening”按钮被点击，尝试对图像进行锐化操作
                printf("Tried to sharpen\n");
                if(hasLoadedImage){
                    imageEdit imgEdit;
                    img = imgEdit.sharpenColor(img);
                }
            }else if(buttons[4].isBeingClicked(x, y)){//“Save Image”按钮被点击，读取地址，尝试将图片存储至指定地址
                printf("Tried to save image\n");
                if(hasLoadedImage){
                    TCHAR _fileAddress[512];
                    InputBox(_fileAddress, 512, _T("Please input the address to save:"));

                    //处理空地址与地址前后的双引号
                    if(_fileAddress[0] == '\0'){
                        printf("Empty address, canceled loading image\n");
                        return;
                    }else if(_fileAddress[0] == '\"' && _fileAddress[_tcslen(_fileAddress)-1] == '\"'){
                        _tcsncpy_s(_fileAddress, _fileAddress+1, _tcslen(_fileAddress)-2);
                    }

                    saveimage(_fileAddress, &img);                    
                }
            }
        }
};

using namespace std;

int main(){      
    init(1600,900);//初始化GUI窗口
    ExMessage msg;

    mainWindows.openWindow(new Windows_Main());//启动主界面
    MsgProcessor msgProcessor;//消息处理类

	while(!mainWindows.isEmpty()){//主循环
        bool fresh_flag = false;

        //读取输入
		if(peekmessage(&msg, EM_KEY | EM_MOUSE | EM_CHAR, true))
		{
			fresh_flag = msgProcessor.checkMsg(&msg, &mainWindows);
		}

        if(fresh_flag)mainWindows.refresh();//若产生页面更新，则刷新页面
		Sleep(10);
	}
    

    closegraph();//关闭GUI窗口
    return 0;
}
