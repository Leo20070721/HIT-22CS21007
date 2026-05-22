#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;

#define ques_N 8
int board[ques_N+2];
int ans_cnt=0;
int SoluteMode=0;

void init(int width=500, int height=600){
    initgraph(width, height);
	setbkcolor(WHITE);
	setlinecolor(BLUE);		// 设置线条颜色为蓝色
	settextcolor(BLUE);		// 设置文字颜色为蓝色
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("宋体"));	// 设置文字样式，大小为 20，字体为宋体
	cleardevice();
}

void GUIDrawBoard(){
    cleardevice();
    for(int i=1;i<=ques_N;i++){
        for(int j=1;j<=ques_N;j++){
            if(board[i]==j){
                setfillcolor(RED);
                fillrectangle(j*50,i*50,j*50+50,i*50+50);
            }
            else if((i+j)%2==0){
                setfillcolor(WHITE);
                fillrectangle(j*50,i*50,j*50+50,i*50+50);
            }else{
                setfillcolor(LIGHTGRAY);
                fillrectangle(j*50,i*50,j*50+50,i*50+50);
            }
        }
    }
}

bool isLegal(int row,int col){

    for(int i=1;i<row;i++){
        if(board[i]==col){return false;}
    }

    for(int i=1;i<row;i++){
        if(abs(board[i]-col)==abs(i-row)){return false;}
    }    

    return true;
}

void printAns(){
    cout<<"Find a possible answer:\n";
    for(int i=1;i<=ques_N;i++){
        for(int j=1;j<=ques_N;j++){
            if(board[i]==j){cout<<"Q ";}
            else{cout<<". ";}
        }cout<<"\n";
    }
    cout<<"\n";

    GUIDrawBoard();
    Sleep(200);

    return;
}

void BrouteSolute(){
    int row=1;
    while(row>=1){
        board[row]++;
        if(board[row]>ques_N){
            board[row]=0;
            row--;
        }
        else if(isLegal(row,board[row])){
            if(row==ques_N){
                printAns();
                ans_cnt++;
                return;
            }
            else{row++;}
        }
    }
}

bool DFSSolute(int row){
    if(row>ques_N){
        printAns();
        ans_cnt++;
        return true;
    }

    for(int col=1;col<=ques_N;col++){
        if(isLegal(row,col)){
            board[row]=col;
            if(DFSSolute(row+1)){
                if(SoluteMode==0){return true;}
            }
            board[row]=0;
        }
    }
    return false;
}

signed main() {
    
    
    cout<<"Welcome to the "<<ques_N<<"-queens problem solver!\n";
    cout<<"Please choose the solute mode:\n";
    cout<<"0: Find one ans\n";
    cout<<"1: Find all ans\n";
    cin>>SoluteMode;
    if(SoluteMode!=0 && SoluteMode!=1){
        cout<<"Invalid Input!\n";
        MessageBoxW(NULL, L"Invalid Input!", L"Error", MB_OK);
        system("pause");
        return 0;
    }

    init(500,500);
    cout<<"The ans of "<<ques_N<<"-queens problem is:\n";
    DFSSolute(1);
    cout<<"Total ans: "<<ans_cnt<<"\n";
    if(ans_cnt==0){
        MessageBoxW(NULL, L"No ans!", L"Total ans", MB_OK);
    }else{
        wchar_t message[256];
        swprintf(message,L"Total ans: %d",ans_cnt);
        MessageBoxW(NULL, message, L"Total ans", MB_OK);
    }
    

    system("pause");    
    closegraph();
    return 0;
}