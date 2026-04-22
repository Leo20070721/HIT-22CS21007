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
        for(int i=0; i<word.length(); i++){
            if(word[i] < 'a' || word[i] > 'z'){
                word.erase(i, 1);
                i--;
            }
        }
        if(word.length()>30){cout<<"Warning: Word '" << word << "' is too long and may cause issues." << endl;}
    }
    return;
}

bool WriteResult(string Filepath, map<string, int> &Words){ 
    string Filename = Filepath + "\\result.txt";
    ofstream file(Filename);
    if(!file.is_open()){
        cerr << "Error opening file for writing: " << Filename << endl;
        return false;
    }
    for(const auto &pair : Words){
        file << pair.first << ": " << pair.second << endl;
    }
    file.close();
    cout << "Successfully wrote to file: " << Filename << endl;
    return true;
}

int main(){
    vector<string> Words, Tasks;
    map<string, int> Result;
    string Filename;

    Filename = "D:\\Program Files\\Github Storage\\22CS21007\\Work03\\tasks.txt";
    ReadFile(Filename, Tasks);
    PreProcess(Tasks);

    Filename = "D:\\Program Files\\Github Storage\\22CS21007\\Work03\\file.txt";
    int count = ReadFile(Filename, Words);
    PreProcess(Words);
    
    /*for(auto word : Words){
        cout << word << " ";
    }cout << endl;
    for(auto word : Tasks){
        cout << word << " ";
    }cout << endl;*/

    for(auto word : Words){
        if(find(Tasks.begin(), Tasks.end(), word) != Tasks.end()){
            Result[word]++;
        }
    }

    WriteResult("D:\\Program Files\\Github Storage\\22CS21007\\Work03", Result);

    system("pause");
    return 0;
}