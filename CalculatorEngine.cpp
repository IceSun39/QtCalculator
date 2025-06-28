#include "CalculatorEngine.h"

CalculatorEngine::CalculatorEngine(QObject *parent)
    : QObject{parent}
{}

QVector<Token> CalculatorEngine::tokenize(const QString& text){
    QVector<Token> tokens;
    size_t i = 0;
    while(i < text.length()){
        if(text.at(i).isSpace()) {i++; continue;}

        if(text.at(i).isDigit()){
            QString res;
            while(i < text.length() && (text.at(i).isDigit() || text.at(i) == '.')){
                res += text.at(i);
                i++;
            }
            double num = res.toDouble();
            tokens.push_back({Number, num, ' '});
        }

        if(text.at(i) == '.'){
            QString res = "0,";
            while(i < text.length() && text.at(i).isDigit()){
                res += text.at(i);
                i++;
            }
            double num = res.toDouble();
            tokens.push_back({Number, num, ' '});
        }

        if(text.at(i).isSymbol()){
            if(text.at(i) == '('){
                tokens.push_back({LeftParen, 0, text.at(i)});
                i++;
            }
            else if(text.at(i) == ')'){
                tokens.push_back({RightParen, 0, text.at(i)});
                i++;
            }
            else{
                tokens.push_back({Operator, 0, text.at(i)});
                i++;
            }
        }
    }
    return tokens;
}

int CalculatorEngine::getPriority(QChar oper){
    if(oper == '+' || oper == '-') return 1;
    else if(oper == '*' || oper == '/') return 2;
    return -1;
}

QQueue<Token> CalculatorEngine::parsing(const QVector<Token>& tokens){
    QQueue<Token> resQueue;
    QStack<Token> st;
    auto curr = tokens.begin();
    while(curr != tokens.end()){
        if(curr->type == Number){
            resQueue.push_back(*curr);
            curr++;
        }
        else if(curr->type == LeftParen){
            st.push(*curr);
            curr++;
        }
        else if(curr->type == RightParen){
            while(!st.isEmpty() && st.top().op != '('){
                resQueue.push_back(st.top());
                st.pop();
            }
            if(!st.isEmpty()) st.pop();//видаляємо '('
            else qDebug() << "Error: unmatched closing parenthesis ')'";
        }
        else{
            if(!st.isEmpty()){
                while(!st.isEmpty() && getPriority(st.top().op) >= getPriority(curr->op)){
                    resQueue.push_back(st.top());
                    st.pop();
                }
                st.push(*curr);
                curr++;
            }
            else{
                st.push(*curr);
                curr++;
            }
        }
    }
    while(!st.isEmpty()){
        resQueue.push_back(st.top());
        st.pop();
    }

    return resQueue;
}

double CalculatorEngine::evaluate(QQueue<Token>& tokens){
    QStack<double> st;
    for (const auto& token : tokens) {
        if (token.type == Number) {
            st.push(token.value);
        }
        else if (token.type == Operator) {
            if (st.size() < 2) {
                qDebug() << "Error: not enough operands";
                return -1;
            }

            double right = st.top(); st.pop(); // правий операнд
            double left = st.top(); st.pop();  // лівий операнд

            if (token.op == '+') st.push(left + right);
            else if (token.op == '-') st.push(left - right);
            else if (token.op == '*') st.push(left * right);
            else if (token.op == '/') {
                if (right != 0.0) st.push(left / right);
                else {
                    qDebug() << "Error: division by zero";
                    return -1;
                }
            }
        }
    }

    if (st.size() == 1) {
        return st.top();
    } else {
        qDebug() << "Error: invalid expression";
        return -1;
    }
}

