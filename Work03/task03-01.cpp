#include <bits/stdc++.h>
using namespace std;

int ReadFile(string Filename, vector<string> &Words){
    ifstream file(Filename);
    if(!file.is_open()){
        cerr << "Error opening file: " << Filename << endl;
        return -1;
    }
    string word;
    while(file >> word){
        Words.push_back(word);
    }
    file.close();
    if(Words.empty()){
        cerr << "No words found in file: " << Filename << endl;
        return -1;
    }else{
        cout << "Read from file: " << Filename << endl;
        cout << "Total words: " << Words.size() << endl;
        return Words.size();
    }    
}

void PreProcess(vector<string> &Words){ 
    for(auto &word : Words){
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word.erase(remove_if(word.begin(),word.end(), ::ispunct), word.end());
    }
    return;
}


int main(){
    vector<string> Words;
    string Filename = "D:\\Program Files\\Github Storage\\22CS21007\\Work03\\file.txt";
    int count = ReadFile(Filename, Words);
    PreProcess(Words);
    
    for(auto word : Words){
        cout << word << " ";
    }cout << endl;

    system("pause");
    return 0;
}