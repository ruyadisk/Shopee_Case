#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;

string digits = "0123456789";

bool priority(char top, char now) {
    if (top == '(') {
        return false;
    } else if ((top == '-' || top == '+' || top == '^') && (now == '*' || now == '/')) {
        return false;
    }
    return true;
}

void perform(stack<double> &number_stack, stack<char> &operator_stack) {
    double second = number_stack.top();
    number_stack.pop();
    double first = number_stack.top();
    number_stack.pop();
    char op = operator_stack.top();
    operator_stack.pop();
    double result;
    if (op == '+') result = first + second;
    else if (op == '-') result = first - second;
    else if (op == '*') result = first * second;
    else if (op == '/') result = first / second;
    else if (op == '^') result = pow(first, second);
    number_stack.push(result);
    return;
}

double analyze(const string &exp) {
    stack<double> number_stack;
    stack<char> operator_stack;
    operator_stack.push('(');
    string exp_ = exp + ')';
    bool ifnext = false;
    for (int i = 0; i < exp_.size(); ++i) {
        if(exp_[i] == ' '){
            continue;
        }
        else if (exp_[i] == '(' || exp_[i] == '[' || exp_[i] == '{') {
            operator_stack.push('(');
        } else if (exp_[i] == ')' || exp_[i] == ']' || exp_[i] == '}') {
            while (operator_stack.top() != '(')
                perform(number_stack, operator_stack);
            operator_stack.pop();
        } else if (ifnext) {
            while (priority(operator_stack.top(), exp_[i])) {
                perform(number_stack, operator_stack);
            }
            operator_stack.push(exp_[i]);
            ifnext = false;
        } else {
            int j = i;
            if (exp_[j] == '+' || exp_[j] == '-' || exp_[j] == '*' || exp_[j] == '/' || exp_[j] == '^') ++i;
            while (digits.find(exp_[i]) != digits.npos) ++i;
            number_stack.push((double)stoi(exp_.substr(j, i - j)));
            --i;
            ifnext = true;
        }
    }
    return number_stack.top();
}

vector<double> vec_calculate(vector<vector<double>> table, string vec_postfix){
    int pos = 0;
    stack<int> s;
    for(int cnt = 0; cnt < vec_postfix.size(); cnt++){
        int tmp = 0;
        if(digits.find(vec_postfix[cnt]) != digits.npos){
            pos = cnt;
            while(digits.find(vec_postfix[cnt]) != digits.npos){
                ++cnt;
            }
            s.push(stod(vec_postfix.substr(pos, cnt - pos)));
            --cnt;
        }
        else if(vec_postfix[cnt] == '+' || vec_postfix[cnt] == '-' || vec_postfix[cnt] == '*' || vec_postfix[cnt] == '/' || vec_postfix[cnt] == '^'){
            vector<double>& first = table[s.top()];
            s.pop();
            vector<double>& second = table[s.top()];
            int flg = s.top();
            s.pop();
            int size_first = first.size();
            int size_second = second.size();
            if(size_first > size_second){
                int size_dif = size_first - size_second;
                for(int cnt_ = 0; size_dif > 0; size_dif--){
                    second.push_back(second[cnt_]);
                    cnt_++;
                }
            }
            else if(size_first < size_second){
                int size_dif = size_second - size_first;
                for(int cnt_ = 0; size_dif > 0; size_dif--){
                    first.push_back(first[cnt_]);
                    cnt_++;
                }
            }
            switch(vec_postfix[cnt]){
                case '+':
                for(int vec_cnt = 0; vec_cnt < second.size(); vec_cnt++){
                    second[vec_cnt] += first[vec_cnt];
                }
                break;
                case '-':
                for(int vec_cnt = 0; vec_cnt < second.size(); vec_cnt++){
                    second[vec_cnt] -= first[vec_cnt];
                }
                break;
                case '*':
                for(int vec_cnt = 0; vec_cnt < second.size(); vec_cnt++){
                    second[vec_cnt] *= first[vec_cnt];
                }
                break;
                case '/':
                for(int vec_cnt = 0; vec_cnt < second.size(); vec_cnt++){
                    second[vec_cnt] /= first[vec_cnt];
                }
                break;
                case '^':
                for(int vec_cnt = 0; vec_cnt < second.size(); vec_cnt++){
                    second[vec_cnt] = pow(second[vec_cnt],first[vec_cnt]);
                }
                break;
            }
            s.push(flg);
        }
        else{
            continue;
        }
    }
    return table[s.top()];
}


int main(int argc, char* argv[]){
    ifstream fin;
    fin.open(argv[1]);
    
    vector<string>line_table;
    string buf;
    while(getline(fin, buf)){    
        line_table.push_back(buf);
    }
    
    for(auto &it : line_table){
        vector<vector<double>>table;
        stack<char> vec_proc;
        string vec_postfix;
        int vec_cnt = 0;
        for(int cnt = 0; cnt < it.size(); cnt++){
            vector<double>element;
            if(it[cnt] == '['){
                cnt++;
                string res = "";
                while(it[cnt] != ']'){
                    if(it[cnt] == ','){
                        double tmp = analyze(res);
                        element.push_back(tmp);
                        res = "";
                        cnt++;
                    }
                    else if(it[cnt] == ' '){
                        cnt++;
                    }
                    else{
                        res += it[cnt];
                        cnt++;
                    }
                }
                element.push_back(analyze(res));
                vec_postfix += to_string(vec_cnt);
                vec_postfix += " ";
                table.push_back(element);
                vec_postfix += vec_cnt;
                vec_postfix += " ";
                vec_cnt++;
            }
            else if(it[cnt] == ' ' || (int)it[cnt] == 13){
                continue;
            }
            else{
                while(!vec_proc.empty() && priority(vec_proc.top(), it[cnt])){
                    vec_postfix += vec_proc.top();
                    vec_postfix += " ";
                    vec_proc.pop();
                }
                vec_proc.push(it[cnt]);
            }
        }
        while(!vec_proc.empty()){
            vec_postfix += vec_proc.top();
            vec_proc.pop();
        }
        for(auto &it : vec_calculate(table, vec_postfix)){
            cout << fixed << setprecision(2) << it << " ";
        }
        cout << endl;
    
    }
}