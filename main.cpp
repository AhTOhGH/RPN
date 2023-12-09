//
//  main.cpp
//  RPN
//
//  Created by Антон Нечаев on 10.12.2022.
//

#include <iostream>
#include <vector>
#include <variant>
#include <stack>
#include "assert.h"

using TToken = std::variant<double, char>;
using VTokens = std::vector<TToken>;
using TTokenStack = std::stack<TToken>;

inline VTokens Tokenize(const std::string& expr) 
{
    VTokens result;
    const char *current = expr.c_str();
    while(*current) 
    {
        if(isdigit(*current)) 
        {
            char *end = nullptr;
            result.push_back(strtod(current, &end));
            current = end;
        }
        else 
        {
            result.push_back(*current);
            ++current;
        }
    }
    return result;
}


struct SOutVisitor
{
    void operator()(double a) {std::cout<<a;}
    void operator()(char a) {std::cout<<a;}
};

struct SRPN_maker
{
    void operator()(double a)
    {
        RPN.push_back(a);
    }
    
    void operator()(char a)
    {
        if (isOperation(a))
        {
            // 2 a
            if (operations.empty())
            {
                operations.push(a);
            }
            else if(int currentPriority = priority(a); priority(std::get<char>(operations.top())) < currentPriority)
            {
                operations.push(a);
            }
            // 2 b
            else
            {
                while (priority(std::get<char>(operations.top())) >= currentPriority)
                {
                    RPN.push_back(operations.top());
                    operations.pop();
                }
            }
        }
        else if(a == '(')
        {
            operations.push(a);
        }
        else if(a==')')
        {
            while (std::get<char>(operations.top()) != '(')
            {
                RPN.push_back(operations.top());
                operations.pop();
            }
            operations.pop();
        }
    }
    
    bool isOperation(char c_)
    {
        switch (c_)
        {
            case '+':
                return true;
            case '-':
                return true;
            case '*':
                return true;
            case '/':
                return true;
            
            default:
                return false;
        }
    }
    
    int priority(char c_)
    {
        switch (c_)
        {
            case '+':
                return 3;
            case '-':
                return 3;
            case '*':
                return 4;
            case '/':
                return 4;
            
            default:
                return 0;
        }
    }
    
    bool isSkobka(char c_)
    {
        return c_ == ')' || c_ == '(';
    }
    
    VTokens RPN;
    TTokenStack operations;
};

struct RPN_Calculator
{
    void operator()(double a)
    {
        numbers.push(a);
    }
    
    void operator()(char a)
    {
        double dFirst = numbers.top();
        numbers.pop();
        double dSecond = numbers.top();
        numbers.pop();
        
        numbers.push(evalute(a, dFirst, dSecond));
    }
    
    double evalute(char o_, double l_, double r_)
    {
        switch (o_)
        {
            case '+':
                return l_ + r_;
            case '-':
                return l_ - r_;
            case '*':
                return l_ * r_;
            case '/':
                return l_ / r_;
            
            default:
                return 0;
        }
    }
    
    std::stack<double> numbers;
};

double calculate(const std::string& expression)
{
    const auto tokens = Tokenize(expression);
    
    SRPN_maker maker;
    for (const auto& token: tokens)
    {
        std::visit(maker, token);
    }
    
    while (!maker.operations.empty())
    {
        maker.RPN.push_back(maker.operations.top());
        maker.operations.pop();
    }
    
    RPN_Calculator calc;
    for (const auto& token : maker.RPN) 
    {
        std::visit(calc, token);
    }
    
    assert(calc.numbers.size() == 1);
    
    return calc.numbers.top();
}

int main(int argc, const char * argv[])
{
    std::string expression("5+4");
    std::cout << expression << " = " <<calculate(expression) << std::endl;
    
    expression = "(5+4)*2";
    std::cout << expression << " = " <<calculate(expression)<< std::endl;
    
    expression = "5.8+4";
    std::cout << expression << " = " <<calculate(expression)<< std::endl;

    return 0;
}
