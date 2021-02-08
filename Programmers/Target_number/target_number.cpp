#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> citations) {
    int answer = 0;
    sort(citations.begin(), citations.end(), greater<>());
    int h = citations[0]; 
    while(h >= 0){
        int cnt = 0;
        for(int i = 0; i < citations.size(); i++){
            if(citations[i] < h) break;
            else cnt++;
        }
        if(cnt >= h){
            answer = h; break;
        }
        else h--;
    }
    
    return answer;
}
