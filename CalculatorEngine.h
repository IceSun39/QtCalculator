#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QObject>
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

class CalculatorEngine : public QObject
{
    Q_OBJECT
public:
    explicit CalculatorEngine(QObject *parent = nullptr);
private:
    QVector<Token> tokenize(const QString& text);

    int getPriority(QChar oper);

    QQueue<Token> parsing(const QVector<Token>& tokens);

    double evaluate(const QVector<Token>& tokens);
signals:
};

#endif // CALCULATORENGINE_H
