#include "./GUIBasicTypes.h"

#include <time.h>


WindowsControl mainWindows;

class Windows_Final: public WindowsType{
    int mode;
    bool result;
    int rightAnswer;
    int guessedTimes;
    public:
        Windows_Final(int mode, bool result, int rightAnswer, int guessedTimes){
            this->mode = mode;
            this->result = result;
            this->rightAnswer = rightAnswer;
            this->guessedTimes = guessedTimes;
            
            int x_mid = 400, y_mid = 300;
            texts.push_back(Text(x_mid, 100, (result ? _T("Congratulations!") : _T("Game Over!")), 40, BLACK, _T("宋体"),std::make_pair (0,0)));
            {
                TCHAR buffer[64];
                _stprintf_s(buffer, _countof(buffer), _T("Right Answer: %d"), rightAnswer);
                texts.push_back(Text(x_mid, 150, buffer, 20, BLACK, _T("宋体"), std::make_pair (0,0)));
                if(result){
                    _stprintf_s(buffer, _countof(buffer), _T("Your totally guessed times: %d"), guessedTimes);
                }else{
                    if(mode == 1){
                        _stprintf_s(buffer, _countof(buffer), _T("You didn't guess the right answer in 1 try."), guessedTimes);
                    }else if (mode == 3){
                        _stprintf_s(buffer, _countof(buffer), _T("You have wasted your 10 chances."), mode);
                    }else{
                        _stprintf_s(buffer, _countof(buffer), _T("You are loser, YOU DAMN HACKER."), mode);
                    }
                    texts.push_back(Text(x_mid, 180, buffer, 20, BLACK, _T("宋体"), std::make_pair (0,0)));
                }
            }
            buttons.push_back(Button(x_mid,400,150,50,Text(_T("Play Again"), 20, WHITE, _T("宋体"),std::make_pair (0,0)), GREEN, BLACK, std::make_pair (0,0), std::make_pair (0,0)));
            buttons.push_back(Button(x_mid,480,150,50,Text(_T("Main Menu"), 20, WHITE, _T("宋体"),std::make_pair (0,0)), BROWN, BLACK, std::make_pair (0,0), std::make_pair (0,0)));
        }

        void aboutToAppear() override{
            if(!result){
                PlaySound(_T("D:\\Program Files\\Github Storage\\22CS21007\\Work02\\Task06\\Sounds\\Defeat.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
            }
        }
        void aboutToDisappear() override{
            PlaySound(NULL, NULL, 0);
        }

        void mouseDown(int x, int y) override{
            printf("get mouse down msg in final window\n");
            if(buttons[0].isBeingClicked(x, y)){
                printf("click play again button\n");
                mainWindows.backWithRebuilt();
            }else if(buttons[1].isBeingClicked(x, y)){
                printf("click back to main menu button\n");
                mainWindows.closeWindow();
                mainWindows.closeWindow();
            }
        }
};

class Windows_Game: public WindowsType{
    private:
        int mode;
        TCHAR modeTitleText[3][_TEXT_MAX_TXT_SIZE]={
            _T("Mode 1: Just guess for 1 try."),
            _T("Mode 2: Guess until right."),
            _T("Mode 3: Guess for 10 tries.")
        };
        std::vector<Text> guessHistory;
        int guessedTimes = 0,MAXguessedTimes = 1;
        int rightAnswer;
    public:
        Windows_Game(int mode){            
            srand((unsigned)time(NULL));
            this->rightAnswer = rand() % 100 + 1;
            this->mode = mode;
            setMAXguessedTimes(mode);
            this->guessedTimes = 0;

            int x_mid = 400, y_mid = 300;
            texts.push_back(Text(x_mid, 50, modeTitleText[mode-1], 40, BLACK, _T("宋体"),std::make_pair (0,0)));
            texts.push_back(Text(x_mid, 100, _T("You have tried for: 0 times"), 20, BLACK, _T("宋体"),std::make_pair (0,0)));
            texts.push_back(Text(100, 150, _T("Please input your guess:"), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
            txtInputBoxes.push_back(TXTInputBox(x_mid,200,350,50, WHITE));
            texts.push_back(Text(100, 260, _T("Guess History:"), 20, BLACK, _T("宋体"),std::make_pair (0,0)));
            buttons.push_back(Button(760,410,100,50,Text(_T("Confirm"), 20, WHITE, _T("宋体"),std::make_pair (0,0)), GREEN, BLACK, std::make_pair (1,0), std::make_pair (0,0)));
            buttons.push_back(Button(760,480,100,50,Text(_T("Reset"), 20, WHITE, _T("宋体"),std::make_pair (0,0)), BROWN, BLACK, std::make_pair (1,0), std::make_pair (0,0)));
            buttons.push_back(Button(760,550,100,50,Text(_T("Exit"), 20, WHITE, _T("宋体"),std::make_pair (0,0)), RED, BLACK, std::make_pair (1,0), std::make_pair (0,0)));
        }
        void aboutToAppear() override{
            PlaySound(_T("D:\\Program Files\\Github Storage\\22CS21007\\Work02\\Task06\\Sounds\\Background.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
        }
        void aboutToDisappear() override{
            PlaySound(NULL, NULL, 0);
        }
        void fresh() override{ 
            cleardevice();
            for(auto text : texts){text.create();}
            for(auto button : buttons){button.create();}
            for(auto txtInputBox : txtInputBoxes){txtInputBox.create();}
            for(auto guessHistoryItem : guessHistory){guessHistoryItem.create();}
            
        }

        void reInit() override{
            setMAXguessedTimes(this->mode);
            while(!guessHistory.empty()) guessHistory.pop_back();
            srand((unsigned)time(NULL));
            this->rightAnswer = rand() % 100 + 1;
            this->guessedTimes = 0;
            while(!guessHistory.empty()) guessHistory.pop_back();
            txtInputBoxes[0].setValue(_T(" "));
            texts[1].setText(_T("You have tried for: 0 times"));
        }

        void setMAXguessedTimes(int mode){
            switch(mode){
                case 1:
                    this->MAXguessedTimes = 1;
                    break;
                case 2:
                    this->MAXguessedTimes = INT_MAX-1;
                    break;
                case 3:
                    this->MAXguessedTimes = 10;
                    break;
            }
        }

        void buttonConfirmHandler(){
            int guessNum = _ttoi(txtInputBoxes[0].getText());
            guessedTimes++;       
            
            if(guessNum == rightAnswer){
                mainWindows.openWindow(new Windows_Final(this->mode, true, rightAnswer, guessedTimes));
            }else{
                if(guessedTimes >= MAXguessedTimes){
                    mainWindows.openWindow(new Windows_Final(this->mode, false, rightAnswer, guessedTimes));
                }
            }
            printf("guessNum=%d, rightAnswer=%d, guessedTimes=%d\n", guessNum, rightAnswer, guessedTimes);
            {
                TCHAR str[64];
                if(guessNum > rightAnswer){
                    _stprintf_s(str, _countof(str), _T("%d is too big."), guessNum);
                }else{
                    _stprintf_s(str, _countof(str), _T("%d is too small."), guessNum);
                }
                guessHistory.push_back(Text(120, 260 + 30 * guessedTimes, str, 20, BLACK, _T("宋体"), std::make_pair (-1,0)));
                texts[1].setText((_stprintf_s(str, _countof(str), _T("You have tried for: %d times"), guessedTimes), str));
            }
            
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
                    reInit();
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

        void keyDown(int keyCode) override{
            if(keyCode == VK_BACK){
                txtInputBoxes[0].popChar();
            }
        }
};

class Windows_Index: public WindowsType{
    public:
        Windows_Index(){
            int x_mid = 400, y_mid = 300;
            texts.push_back(Text(x_mid, 50, _T("Number Guess Games"), 40, BLACK, _T("宋体"),std::make_pair (0,0)));
            texts.push_back(Text(100, 100, _T("Please choose the game mode:"), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
            buttons.push_back(Button(x_mid,150,350,50,Text(_T("Mode 1: Just guess for 1 try."), 20, WHITE, _T("宋体"),std::make_pair (0,0)), GREEN, WHITE, std::make_pair (0,0), std::make_pair (0,0)));
            buttons.push_back(Button(x_mid,225,350,50,Text(_T("Mode 2: Guess until right."), 20, WHITE, _T("宋体"),std::make_pair (0,0)), BLUE, WHITE, std::make_pair (0,0), std::make_pair (0,0)));            
            buttons.push_back(Button(x_mid,300,350,50,Text(_T("Mode 3: Guess for 10 tries."), 20, WHITE, _T("宋体"),std::make_pair (0,0)), RED, WHITE, std::make_pair (0,0), std::make_pair (0,0)));
            {
                int lineGap = 20;int y_start=360;int _i=0;
                texts.push_back(Text(25, y_start, _T("Game rules:"), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("1. This program will generate a random number."), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("2. You need to guess the number in limited attempts."), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("3. Program will provide feedback on your guess is bigger or smaller."), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
                texts.push_back(Text(50, y_start + (++_i) * lineGap, _T("4. Different modes have different numbers of attempts."), 20, BLACK, _T("宋体"),std::make_pair (-1,0)));
            }            
        }

        void aboutToAppear() override{
            //PlaySound(_T("D:\\Program Files\\Github Storage\\22CS21007\\Work02\\Task06\\Sounds\\MainMenu.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
        }
        void aboutToDisappear() override{
            PlaySound(NULL, NULL, 0);
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