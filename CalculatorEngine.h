#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QString>
#include <QVector>
#include <QQueue>
#include <QStack>
#include <QDebug>
#include <cmath>

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
    static QQueue<Token> parsing(const QVector<Token>& tokens);
    static double evaluate(QQueue<Token>& tokens);
    static double getLastNumber(const QString& text, int& lastIndex);
    static QString toggleLastNumberSign(const QString& expression);
    static QString reverseNumber(const QString& expression);
    static QString squareNumber(const QString& expression);
    static QString squareRootNumber(const QString& expression, int& posOfSqrt);
    static QString getPercent(const QString& expression);
private:
    static int getPriority(QChar oper);
    static bool isNegativeWrapped(const QString& text, int startIndex, double number);
    static QString wrapAsReciprocal(double number);
    static QString wrapAsReciprocal(const QString& expression); // для "1/(...)"
    static QString removeLastNumber(const QString& text, int startIndex);
    static bool isWrappedInParentheses(const QString& text, int numberStartIndex);
    static QString extractExpressionInParentheses(const QString &text, int &startIndex);

};

#endif // CALCULATORENGINE_H
