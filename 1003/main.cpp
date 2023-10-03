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
    } else if ((top == '-' || top == '+') && (now == '*' || now == '/')) {
        return false;
    } else if ((top == '-' || top == '+' || top == '*' || top == '/') && now == '^'){
        return false;
    }
    return true;
}

void perform(stack< double> &number_stack, stack<char> &operator_stack) {
    double second = number_stack.top();
    number_stack.pop();
    double first = number_stack.top();
    number_stack.pop();
    char op = operator_stack.top();
    operator_stack.pop();
     double result;
    if (op == '+') result = first + second;
    else if (op == '-') {
        result = first - second;
    }
    else if (op == '*') result = first * second;
    else if (op == '/') {
        result = first / second;
    }
    else if (op == '^') result = pow(first, second);
    number_stack.push(result);
    return;
}

 double analyze(const string &exp) {
    
    stack< double> number_stack;
    stack<char> operator_stack;
    operator_stack.push('(');
    stack<char>sss = operator_stack;
    string exp_ = exp + ')';
    bool ifnext = false;
    for (int i = 0; i < exp_.size(); ++i) {
        if(exp_[i] == ' '){
            continue;
        }
        else if (exp_[i] == '(') {
            operator_stack.push('(');
        } else if (exp_[i] == ')') {
            while (operator_stack.top() != '('){
                perform(number_stack, operator_stack);                
            }
            operator_stack.pop();
        } else if (ifnext) {
            while (priority(operator_stack.top(), exp_[i])) {
                perform(number_stack, operator_stack);
            }
            operator_stack.push(exp_[i]);
            ifnext = false;
        } else {
            int j = i;
            if (exp_[j] == '-'){
                if(exp_[j+1] < '0' || exp_[j+1] > '9'){
                    number_stack.push(-1);
                    operator_stack.push('*');
                    ifnext = false;
                }
                else{
                    ++i;
                    while (i+1 != exp_.size() && exp_[i] >= '0' && exp_[i] <= '9') ++i;
                    number_stack.push(( double)stoi(exp_.substr(j, i - j)));
                    ifnext = true;
                    --i;
                }
            }
            else{
                ++i;
                while (exp_[i] >= '0' && exp_[i] <= '9') ++i;
                number_stack.push((double)stoi(exp_.substr(j, i - j)));
                ifnext = true;
                --i;
            }
        }
    }
    return number_stack.top();
}

vector< double> vec_calculate(vector<vector< double>> table, string vec_postfix){
    stack<int> s;
    for(int cnt = 0; cnt < vec_postfix.size(); cnt++){
        int tmp = 0;
        if(digits.find(vec_postfix[cnt]) != digits.npos){
            int pos = cnt;
            while(vec_postfix[cnt] >= '0' && vec_postfix[cnt] <= '9'){
                ++cnt;
            }
            s.push(stoi(vec_postfix.substr(pos, cnt - pos)));
            --cnt;
        }
        else if(vec_postfix[cnt] == '+' || vec_postfix[cnt] == '-' || vec_postfix[cnt] == '*' || vec_postfix[cnt] == '/'){
            vector< double>& first = table[s.top()];
            s.pop();
            vector< double>& second = table[s.top()];
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
            }
            first.clear();
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
    
    string buf;
    while(getline(fin, buf)){   
        vector<vector<double>>table;
        stack<char> vec_proc;
        string vec_postfix = "";
        int vec_cnt = 0;
        for(int cnt = 0; cnt < buf.size(); cnt++){
            vector<double>element;
            if(buf[cnt] == '['){
                cnt++;
                string res = "";
                while(buf[cnt] != ']'){
                    if(buf[cnt] == ','){
                        double tmp = analyze(res);
                        element.push_back(tmp);
                        res = "";
                        cnt++;
                    }
                    else if(buf[cnt] == ' '){
                        cnt++;
                    }
                    else{
                        res += buf[cnt];
                        cnt++;
                    }
                }
                element.push_back(analyze(res));
                vec_postfix += to_string(vec_cnt);
                vec_postfix += " ";
                table.push_back(element);
                vec_cnt++;
            }
            else if(buf[cnt] == ' '){
                continue;
            }
            else{
                while(!vec_proc.empty() && priority(vec_proc.top(), buf[cnt])){
                    vec_postfix += vec_proc.top();
                    vec_postfix += " ";
                    vec_proc.pop();
                }
                vec_proc.push(buf[cnt]);
            }
            element.clear();
        }

        while(!vec_proc.empty()){
            vec_postfix += vec_proc.top();
            vec_postfix += " ";
            vec_proc.pop();
        }

        cout << "[";
        vector<double>result = vec_calculate(table, vec_postfix);
        for(int res_cnt = 0; res_cnt < result.size(); res_cnt++){
            result[res_cnt] = round(result[res_cnt] * 100) / 100.0;
            if(res_cnt != result.size() - 1){
                if(result[res_cnt] == trunc(result[res_cnt])){
                    cout << (int)result[res_cnt] << ",";
                }
                else if(result[res_cnt] * 10 == trunc(result[res_cnt] * 10)){
                    cout << fixed << setprecision(1) << result[res_cnt] << ",";
                }
                else{
                    cout << fixed << setprecision(2) << result[res_cnt] << ",";
                }
            }
            else{
                if(result[res_cnt] == trunc(result[res_cnt])){
                    cout << (int)result[res_cnt];
                }
                else if(result[res_cnt] * 10 == trunc(result[res_cnt] * 10)){
                    cout << fixed << setprecision(1) << result[res_cnt];
                }
                else{
                    cout << fixed << setprecision(2) << result[res_cnt];
                }
            }
        }
        cout << "]" << endl;
        result.clear();
        table.clear();
        vec_postfix.clear();
    }
    fin.close();
    return 0;
}