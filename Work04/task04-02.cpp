#include <bits/stdc++.h>
using namespace std;

#define ques_N 8
int board[ques_N+2];
int ans_cnt=0;

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
                return true;
            }
            board[row]=0;
        }
    }
    return false;
}

signed main() {
    
    cout<<"The ans of "<<ques_N<<"-queens problem is:\n";
    DFSSolute(1);
    cout<<"Total ans: "<<ans_cnt<<"\n";
    
    system("pause");   

    return 0;
}