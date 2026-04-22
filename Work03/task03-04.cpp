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

map<string, int> TaskWordCount(const vector<string> &Words, const vector<string> &Tasks){
    map<string, int> Result;
    vector<string> SortedWords = Words;
    sort(SortedWords.begin(), SortedWords.end());
    for(const auto &task : Tasks){
        bool found = false;
        int l=0, r=SortedWords.size()-1;
        while(l<=r){
            int mid = l + (r-l)/2;
            if(SortedWords[mid] == task){
                l=mid;
                found = true;
                break;
            }else if(l>=r && SortedWords[mid] != task){
                break;
            }else if(SortedWords[mid] < task){
                l = mid + 1;
            }else{
                r = mid - 1;
            }
        }
        if(found){
            while(l>=0 && SortedWords[l] == task){l--;}
            r=l;
            while(r+1<SortedWords.size() && SortedWords[r+1] == task){r++;}
            Result[task] = r-l;
        }else{
            Result[task] = 0;
        }        
    }
    return Result;
}

pair<int, int> Statistics(const vector<string> &Words, const vector<string> &Tasks, vector< pair<string, int> > &Result){
    map<string, int> WordCounts;
    map<int, int> CountFrequency;
    vector<int> Counts;
    for(const auto &word : Words){
        WordCounts[word]++;
    }
    for(const auto &task : Tasks){
        Result.push_back(make_pair(task, WordCounts[task]));
        CountFrequency[WordCounts[task]]++;
    }
    for(const auto &pair : CountFrequency){
        Counts.push_back(pair.first);
    }
    sort(Result.begin(), Result.end(), [](const pair<string, int> &a, const pair<string, int> &b){
        return a.second > b.second;
    });
    sort(Counts.begin(), Counts.end());
    return make_pair(Counts.back(), Result[Result.size()/2].second);

}

int main(){
    vector<string> Words, Tasks;
    map<string, int> Result;
    vector< pair<string, int> > StatisticsResult;
    string Filename;

    Filename = "D:\\Program Files\\Github Storage\\22CS21007\\Work03\\tasks.txt";
    ReadFile(Filename, Tasks);
    PreProcess(Tasks);

    Filename = "D:\\Program Files\\Github Storage\\22CS21007\\Work03\\file.txt";
    int count = ReadFile(Filename, Words);
    PreProcess(Words);
    //-------------------------------------------------
    
    Result = TaskWordCount(Words, Tasks);
    WriteResult("D:\\Program Files\\Github Storage\\22CS21007\\Work03", Result);

    pair<int, int> Stats = Statistics(Words, Tasks, StatisticsResult);
    for(const auto &pair : StatisticsResult){
        cout << pair.first << ": " << pair.second << endl;
    }
    cout << "众数（目前为最高频）: " << Stats.first << endl;
    cout << "中位数: " << Stats.second << endl;

    system("pause");
    return 0;
}