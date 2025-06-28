#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QString>
#include <QVector>
#include <QQueue>
#include <QStack>
#include <QDebug>

enum TokenType { Number, Operator, LeftParen, RightParen };

struct Token {
    TokenType type;
    double value;
    QChar op;
};

class CalculatorEngine
{
public:
    static QVector<Token> tokenize(const QString& text);
    static int getPriority(QChar oper);
    static QQueue<Token> parsing(const QVector<Token>& tokens);
    static double evaluate(QQueue<Token>& tokens);
};

#endif // CALCULATORENGINE_H
