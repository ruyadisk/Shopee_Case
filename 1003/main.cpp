#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <math.h>


double performOperation(double num1, double num2, char op) {
    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0.0) {
                return num1 / num2;
            } else {
                std::cerr << "Error: Division by zero!" << std::endl;
                exit(1);
            }
        case '^':
            return std::pow(num1, num2);
        default:
            std::cerr << "Error: Invalid operator '" << op << "'" << std::endl;
            exit(1);
    }
}

int priority(char c){
    if(c == '+' || c == '-'){
        return 1;
    }
    if(c == '*' || c == '/'){
        return 2;
    }
    if(c == '^'){
        return 3;
    }
    return -1;
}

bool isdigit(char c){
    return (c >='0' && c <= '9');
}

double evaluateExpression(const std::string &expression) {
    std::cout << expression << std::endl;
    std::istringstream iss(expression);
    std::stack<double> numStack;
    std::stack<char> opStack;
    char ch;

    while (iss >> ch) {
        if (isdigit(ch) || ch == '.') {
            iss.putback(ch);
            double num;
            iss >> num;
            numStack.push(num);
        } else if (ch == '(') {
            opStack.push(ch);
        } else if (ch == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                char op = opStack.top();
                opStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                numStack.push(performOperation(num1, num2, op));
            }
            if (!opStack.empty() && opStack.top() == '(') {
                opStack.pop(); // 弹出左括号
            } else {
                std::cerr << "Error: Unmatched ')' in expression." << std::endl;
                exit(1);
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            while (!opStack.empty() && priority(ch) <= priority(opStack.top())) {
                char op = opStack.top();
                opStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                numStack.push(performOperation(num1, num2, op));
            }
            opStack.push(ch);
        }
    }
    while (!opStack.empty()) {
        char op = opStack.top();
        opStack.pop();
        double num2 = numStack.top();
        numStack.pop();
        double num1 = numStack.top();
        numStack.pop();
        numStack.push(performOperation(num1, num2, op));
    }
    return numStack.top();
}

std::vector<double> vec_calculate(std::vector<std::vector<float>>table, std::string vec_postfix){
    
}


int main(int argc, char* argv[]){
    std::ifstream fin;
    fin.open(argv[1]);
    
    std::vector<std::string>line_table;
    std::string buf;
    while(std::getline(fin, buf)){    
        line_table.push_back(buf);
    }
    
    for(auto &it : line_table){
        std::vector<std::vector<double>>table;
        std::stack<char> vec_proc;
        std::string vec_postfix;
        int vec_cnt = 0;
        for(int cnt = 0; cnt < it.size(); cnt++){
            std::vector<double>element;
            if(it[cnt] == '['){
                cnt++;
                std::string res = "";
                while(it[cnt] != ']'){
                    if(it[cnt] == ','){
                        std::cout << res << std::endl;
                        double tmp = evaluateExpression(res);
                        element.push_back(tmp);
                        res = "";
                        cnt++;
                    }
                    else if(it[cnt] == ' '){
                        cnt++;
                    }
                    else{
                        res += it[cnt];
                        res += " ";
                        std::cout << res << std::endl;
                        cnt++;
                    }
                }
                element.push_back(evaluateExpression(res));
                vec_postfix += std::to_string(vec_cnt);
                vec_postfix += " ";
                table.push_back(element);
                vec_postfix += vec_cnt;
                vec_postfix += " ";
                vec_cnt++;
            }
            else if(it[cnt] == ' ' || (int)it[cnt] == 13){
                continue;
            }
            else{ // +,-,*,/ ...
                while(!vec_proc.empty() && (priority(it[cnt])) <= priority(vec_proc.top())){
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
        for(auto& tblit : table){
            for(auto& tblit_ : tblit){
                std::cout << tblit_ << " ";
            }
            std::cout << std::endl;
        }
        std::cout << vec_postfix << std::endl;
        std::cout << calculate
    
    }
}