#ifndef CALCULATORENGINE_H
#define CALCULATORENGINE_H

#include <QObject>

class CalculatorEngine : public QObject
{
    Q_OBJECT
public:
    explicit CalculatorEngine(QObject *parent = nullptr);

signals:
};

#endif // CALCULATORENGINE_H
