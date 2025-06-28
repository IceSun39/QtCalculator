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
            tokens.push_back({Operator, 0, text.at(i)});
            i++;
        }
    }
    return tokens;
}

int CalculatorEngine::getPriority(QChar oper){
    if(oper == '+' || oper == '-') return 1;
    else return 2;
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
        else{
            if(!st.isEmpty()){
                while(getPriority(st.top().op) >= getPriority(curr->op)){
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

double CalculatorEngine::evaluate(const QVector<Token>& tokens){

}
