#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QObject>


enum TokenType { Number, Operator };

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

    double evaluate(QVector<Token> tokens);
signals:
};

#endif // CALCULATORENGINE_H
