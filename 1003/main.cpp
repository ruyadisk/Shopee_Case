#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <math.h>


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
    return 0;
}

bool isdigit(char c){
    return (c >='0' && c <= '9');
}

float calculate(std::string postfix){
	int pos = 0;
    std::stack<float>s;
	for(int cnt = 0; cnt < postfix.size(); cnt++){
        int tmp = 0;
        if(isdigit(postfix[cnt])){
            while(isdigit(postfix[cnt])){
                tmp = tmp * 10 + (postfix[cnt] - '0');
                cnt++;
            }
            s.push(tmp);
        }
        else{
            float first = s.top();
            s.pop();
            float second = s.top();
            s.pop();
            float ret = 0;
            switch(postfix[cnt]){
                case '+':
                ret = first + second;
                break;
                case '-':
                ret = second - first;
                break;
                case '*':
                ret = first * second;
                break;
                case '/':
                ret = second / first;
                break;
                case '^':
                ret = pow(second, first);
                break;
            }
            s.push(ret);
            cnt++;
        }
    }
    return s.top();
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
        std::vector<std::vector<float>>table;
        std::stack<char> vec_proc;
        std::string vec_postfix;
        int vec_cnt = 0;
        for(int cnt = 0; cnt < it.size(); cnt++){
            if(it[cnt] == '['){
                std::vector<float>tmp_table;
                std::stack<char>element_proc;
                std::string element_postfix;
                cnt++;
                while(it[cnt] != ']'){
                    if(it[cnt] == ','){
                        while(!element_proc.empty()){
                            element_postfix += element_proc.top();
                            element_postfix += " ";
                            element_proc.pop(); 
                        }
                        std::cout << "Post : " << element_postfix << std::endl;
                        std::cout << "Result : " << calculate(element_postfix) << 
                        std::endl;
                        tmp_table.push_back(calculate(element_postfix));
                        // vec_postfix += std::to_string(vec_cnt);
                        // vec_postfix += " ";
                        element_postfix.clear();
                        // vec_cnt++;
                        cnt++;
                    }
                    else if(it[cnt] == ' '){
                        cnt++;
                    }
                    else if(it[cnt] >= '0' && it[cnt] <= '9'){
                        while(it[cnt] >= '0' && it[cnt] <= '9'){
                            element_postfix += it[cnt];
                            cnt++;    
                        }
                        element_postfix += ' ';
                    }
                    else if(it[cnt] == '('){
                        element_proc.push(it[cnt]);
                        cnt++;
                    }
                    else if(it[cnt] == ')'){
                        while(element_proc.top() != '('){
                            element_postfix += element_proc.top();
                            element_postfix += " ";
                            element_proc.pop();
                        }
                        element_proc.pop();
                        cnt++;
                    }
                    else{
                        while(!element_proc.empty() && priority(it[cnt]) <= priority(element_proc.top())){
                            element_postfix += element_proc.top();
                            element_postfix += " ";
                            element_proc.pop();
                        }
                        element_proc.push(it[cnt]);
                        cnt++;
                    }
                }
                while(!element_proc.empty()){
                    element_postfix += element_proc.top();
                    element_proc.pop();
                }
                std::cout << "Post : " << element_postfix << std::endl; 
                std::cout << "Result : " << calculate(element_postfix) << std::endl;
                tmp_table.push_back(calculate(element_postfix));
                table.push_back(tmp_table);
                vec_postfix += std::to_string(vec_cnt);
                vec_postfix += " ";
                element_postfix.clear();
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
    
    
    }
}