#include "CalculatorEngine.h"

// Токенізуємо вхідний текстовий рядок у список токенів
QVector<Token> CalculatorEngine::tokenize(const QString& text) {
    QVector<Token> tokens;
    int i = 0;

    //обробка унарного мінуса
    if(text.at(0) == '-'){
        QString res = "-";
        ++i;
        while (i < text.length() && (text.at(i).isDigit() || text.at(i) == '.')) {
            res += text.at(i++);
        }
        double num = res.toDouble();
        tokens.push_back({ Number, num, ' ' });
    }

    //обробка унарного плюса
    if(text.at(0) == '+') i++;

    for (; i < text.length(); ) {
        // Обробка числа (ціле або з крапкою)
        if (text.at(i).isDigit()) {
            QString res;
            while (i < text.length() && (text.at(i).isDigit() || text.at(i) == '.')) {
                res += text.at(i++);
            }
            double num = res.toDouble();
            tokens.push_back({ Number, num, ' ' });
            continue;
        }

        // Обробка десяткових чисел, що починаються з крапки (наприклад: .5)
        if (text.at(i) == '.') {
            QString res = "0.";
            ++i;
            while (i < text.length() && text.at(i).isDigit()) {
                res += text.at(i++);
            }
            double num = res.toDouble();
            tokens.push_back({ Number, num, ' ' });
            continue;
        }

        // Обробка символів: дужки або оператори
        if (QString("+-*/()").contains(text.at(i))) {
            QChar ch = text.at(i++);
            if (ch == '(') {
                tokens.push_back({ LeftParen, 0, ch });
            }
            else if (ch == ')') {
                tokens.push_back({ RightParen, 0, ch });
            }
            else if(ch == '-'){
                //якщо мінус на початку або ситуація типу 5*(-3)
                if(i - 2 < 0 || text.at(i - 2) == '('){
                    QString res;
                    while (i < text.length() && (text.at(i).isDigit() || text.at(i) == '.')) {
                        res += text.at(i++);
                    }
                    double num = res.toDouble();
                    tokens.push_back({ Number, -num, ' ' });
                    continue;
                }
                //якщо бінарний мінус
                else{
                    tokens.push_back({Operator, 0, '-'});
                }
            }
            else {
                tokens.push_back({ Operator, 0, ch });
            }
        }

    }

    return tokens;
}

// Визначення пріоритету оператора
int CalculatorEngine::getPriority(QChar oper) {
    if (oper == '+' || oper == '-') {
        return 1;
    }
    else if (oper == '*' || oper == '/') {
        return 2;
    }
    return -1; // Невідомий оператор
}

// Перетворення інфіксного запису на зворотний польський (постфіксний)
QQueue<Token> CalculatorEngine::parsing(const QVector<Token>& tokens) {
    QQueue<Token> resQueue;  // результат (у RPN)
    QStack<Token> st;        // стек для операторів

    for (const auto& token : tokens) {
        if (token.type == Number) {
            resQueue.push_back(token); // Число — одразу в результат
        }
        else if (token.type == LeftParen) {
            st.push(token); // Відкрита дужка — в стек
        }
        else if (token.type == RightParen) {
            // Поки не знайдена відкрита дужка, переносимо оператори в результат
            while (!st.isEmpty() && st.top().op != '(') {
                resQueue.push_back(st.top());
                st.pop();
            }
            if (!st.isEmpty()) {
                st.pop(); // видаляємо відкриту дужку
            } else {
                qDebug() << "Error: unmatched closing parenthesis ')'";
            }
        }
        else if (token.type == Operator) {
            // Поки у стеку оператори з вищим або рівним пріоритетом — переміщаємо їх у результат
            while (!st.isEmpty() && getPriority(st.top().op) >= getPriority(token.op)) {
                resQueue.push_back(st.top());
                st.pop();
            }
            st.push(token);
        }
    }

    // Усі залишки в стеку перекидаємо в результат
    while (!st.isEmpty()) {
        resQueue.push_back(st.top());
        st.pop();
    }

    return resQueue;
}

// Обчислення результату зі зворотного польського запису
double CalculatorEngine::evaluate(QQueue<Token>& tokens) {
    QStack<double> st;

    for (const auto& token : tokens) {
        if (token.type == Number) {
            st.push(token.value); // Кладемо число в стек
        }
        else if (token.type == Operator) {
            // Має бути як мінімум 2 операнди
            if (st.size() < 2) {
                qDebug() << "Error: not enough operands";
                return -1;
            }

            double right = st.top(); st.pop(); // Правий операнд
            double left = st.top(); st.pop();  // Лівий операнд

            // Виконуємо відповідну операцію
            if (token.op == '+') {
                st.push(left + right);
            }
            else if (token.op == '-') {
                st.push(left - right);
            }
            else if (token.op == '*') {
                st.push(left * right);
            }
            else if (token.op == '/') {
                if (right != 0.0) {
                    st.push(left / right);
                } else {
                    qDebug() << "Error: division by zero";
                    return -1;
                }
            }
        }
    }

    // Після обчислення в стеку має бути рівно одне число — результат
    if (st.size() == 1) {
        return st.top();
    } else {
        qDebug() << "Error: invalid expression";
        return -1;
    }
}

//функція, що повертає останнє число
double CalculatorEngine::getLastNumber(const QString& text, int& lastIndex){
    int i = text.length() - 1;
    QString res;
    while(i >= 0 && (text.at(i).isNumber() || text.at(i) == '.' || text.at(i) == ')')){
        res.push_front(text.at(i--));
    }
    lastIndex = ++i;
    return res.toDouble();
}
