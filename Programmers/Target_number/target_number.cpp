#include <string>
#include <vector>
using namespace std;

vector<char> op_const = {'+', '-'};
void Backtracking(vector<int>& numbers, vector<char>& op, int target, int& cnt){
    if(op.size() == numbers.size()){
        int res = numbers[0];   //초기화
        if (op[0] == '-'){//음수면 -1 곱해주기
            res *= -1;
        }
        for(int i = 1; i < numbers.size(); i++){
            if(op[i] == '+'){//+이면 더해주고
                res += numbers[i];
            }
            else {//-이면 빼주고
                res -= numbers[i];
            }
        }
        if(res == target) cnt++;    //target과 같으면 answer++
        return;
    }
    for(int i = 0; i < 2; i++){
        op.push_back(op_const[i]);
        Backtracking(numbers, op, target, cnt);
        op.pop_back();
    }
}

int solution(vector<int> numbers, int target) {
    int answer = 0;
    vector<char> op;
    Backtracking(numbers, op, target, answer);
    
    return answer;
}
