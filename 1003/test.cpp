#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cmath> // 用于幂运算

// 函数用于执行四则运算，包括幂运算
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

// 函数用于计算表达式字符串
double evaluateExpression(const std::string &expression) {
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
            while (!opStack.empty() && opStack.top() != '(' &&
                   ((ch == '+' || ch == '-') && (opStack.top() == '*' || opStack.top() == '/')) ||
                   ((ch == '^') && (opStack.top() == '*' || opStack.top() == '/' || opStack.top() == '+' || opStack.top() == '-'))) {
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

    if (numStack.size() == 1) {
        return numStack.top();
    } else {
        std::cerr << "Error: Invalid expression." << std::endl;
        exit(1);
    }
}

int main() {
    std::string s = "( 3 + 1 ) ^ 3";
    double result = evaluateExpression(s);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
