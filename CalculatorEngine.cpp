#include "CalculatorEngine.h"

CalculatorEngine::CalculatorEngine(QObject *parent)
    : QObject{parent}
{}

QVector<Token> tokenize(const QString& text){
    QVector<Token> tokens;
    size_t i = 0;
    while(i < text.length()){
        if(text.at(i).isSpace()) {i++; continue;}

        if(text.at(i).isDigit()){
            QString res;
            while(text.at(i).isDigit() || text.at(i) == ','){
                res += text.at(i);
            }
            double num = res.toDouble();
            tokens.push_back({Number, num, ' '});
        }

        if(text.at(i).isSymbol()){
            tokens.push_back({Operator, 0, text.at(i)});
        }
    }
}
