#include "CalculatorEngine.h"

// Токенізуємо вхідний текстовий рядок у список токенів
// додати випадки для abs, exp, mod, log, ln, yroot, ^
QVector<Token> CalculatorEngine::tokenize(const QString& text) {
    QVector<Token> tokens;
    int i = 0;

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
            //обробка унарного та бінарного мінуса
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

// Повертає останнє число у тексті виразу та індекс першого символу цього числа
double CalculatorEngine::getLastNumber(const QString& text, int& numberStartIndex)
{
    if (text.isEmpty()) {
        numberStartIndex = 0;
        return 0.0;
    }

    int i = text.length() - 1;
    QString numberStr;

    // Ігноруємо закриваючі дужки у випадку обгорнутого числа: (-12)
    if (text.at(i) == ')') {
        int openParenIndex = text.lastIndexOf("(-", i);
        if (openParenIndex != -1) {
            numberStr = text.mid(openParenIndex + 2, i - openParenIndex - 2); // витягаємо число між дужками
            numberStartIndex = openParenIndex + 2;
            return numberStr.toDouble();
        }
    }

    // Проходимо з кінця до першого символу числа
    while (i >= 0 && (text.at(i).isDigit() || text.at(i) == '.')) {
        numberStr.prepend(text.at(i));
        i--;
    }

    // Запам'ятовуємо індекс першого символу числа
    numberStartIndex = i + 1;

    // Пробуємо перетворити в число
    return numberStr.toDouble();
}

//перевіряє на (-
bool CalculatorEngine::isNegativeWrapped(const QString& text, int startIndex, double number)
{
    // Генеруємо рядок типу "(-12)" або "(-12.5)"
    QString wrapped = "(-" + QString::number(number) + ")";

    // Порівнюємо частину тексту з цією обгорткою
    return text.mid(startIndex, wrapped.length()) == wrapped;
}

//повертає 1/(число)
QString CalculatorEngine::wrapAsReciprocal(double number)
{
    return "1/" + QString::number(number, 'g', 15);
}

QString CalculatorEngine::wrapAsReciprocal(const QString &expression)
{
    return "1/" + expression;
}

//видаляє останнє число
QString CalculatorEngine::removeLastNumber(const QString &text, int startIndex)
{
    QString expr = text;
    expr.chop(expr.length() - startIndex);
    return expr;
}

//перевіряє чи число загорнуте в дужки
bool CalculatorEngine::isWrappedInParentheses(const QString& text, int numberStartIndex)
{
    //якщо розмір менше двох, то дужок ніяк там бути не може
    if (text.isEmpty() || numberStartIndex < 2)
        return false;

    //якщо нема ), то дужки не закриті
    int closeParenIndex = text.indexOf(')', numberStartIndex);
    if (closeParenIndex == -1)
        return false;

    // Шукаємо відкриваючу дужку лівіше
    int openParenIndex = text.lastIndexOf('(', numberStartIndex);
    if (openParenIndex == -1)
        return false;

    // Витягуємо рядок усередині дужок
    QString inside = text.mid(openParenIndex + 1, closeParenIndex - openParenIndex - 1);

    // Перевіряємо, чи це дійсно число (може з мінусом)
    bool ok = false;
    inside.toDouble(&ok);
    return ok;
}

QString CalculatorEngine::extractExpressionInParentheses(const QString &text, int &startIndex) {
    int end = text.length() - 1;
    if (text[end] != ')') return "";

    int balance = 0;
    for (int i = end; i >= 0; --i) {
        if (text[i] == ')') balance++;
        else if (text[i] == '(') balance--;

        if (balance == 0) {
            startIndex = i;
            return text.mid(i, end - i + 1); // включно з дужками
        }
    }
    return "";
}


//функція для 1/число
QString CalculatorEngine::reverseNumber(const QString &expression)
{
    // Копіюємо вхідний вираз, щоб не змінювати оригінальний
    QString text = expression;

    // Знаходимо останнє число у виразі та індекс, з якого воно починається
    int numberStartIndex;
    double number = CalculatorEngine::getLastNumber(text, numberStartIndex);

    // Отримуємо символ перед числом, якщо такий існує
    QChar prevChar = numberStartIndex > 0 ? text.at(numberStartIndex - 1) : QChar();

    // Випадок 1: якщо останнє число вже загорнуте в дужки (наприклад: (6), (-3.2), тощо)
    if (CalculatorEngine::isWrappedInParentheses(text, numberStartIndex)) {
        // Знаходимо позицію відкриваючої та закриваючої дужки
        int openIndex = text.lastIndexOf('(', numberStartIndex);
        int closeIndex = text.indexOf(')', numberStartIndex);

        // Витягуємо повністю вираз у дужках (наприклад: "(6)", "(-3.2)")
        QString wrapped = text.mid(openIndex, closeIndex - openIndex + 1);

        // Видаляємо його з виразу
        text = CalculatorEngine::removeLastNumber(text, openIndex);

        // Додаємо до виразу обернене значення: 1/(...)
        text += CalculatorEngine::wrapAsReciprocal(wrapped);
    }
    // Випадок 2: перед числом стоїть оператор (+, -, *, /) — загортаємо обернення у дужки
    else if (QString("+-*/").contains(prevChar)) {
        text = removeLastNumber(text, numberStartIndex);
        text += '(' + wrapAsReciprocal(number) + ')';
    }
    // Випадок 3: просто число (без знаку перед ним) — додаємо обернене як є
    else {
        text = removeLastNumber(text, numberStartIndex);
        text += wrapAsReciprocal(number);
    }

    // Повертаємо оновлений вираз
    return text;
}

//функція для піднесення до квадрату
QString CalculatorEngine::squareNumber(const QString &expression){
    QString text = expression;

    int numberStartIndex;
    QString subExpr;

    // Перевірка чи в кінці стоїть дужка
    if (text.endsWith(')')) {
        subExpr = CalculatorEngine::extractExpressionInParentheses(text, numberStartIndex);
        if (!subExpr.isEmpty()) {
            QVector<Token> tokens = CalculatorEngine::tokenize(subExpr.mid(1, subExpr.length() - 2)); // без дужок
            QQueue<Token> rpn = CalculatorEngine::parsing(tokens);
            double value = CalculatorEngine::evaluate(rpn);
            value *= value;

            // видалити підвираз із text
            text = text.left(numberStartIndex);
            text += QString::number(value, 'g', 15);
            return text;
        }
    }

    // Інакше працюємо по-старому
    double number = CalculatorEngine::getLastNumber(text, numberStartIndex);
    number *= number;

    if(CalculatorEngine::isWrappedInParentheses(text, numberStartIndex)){
        numberStartIndex = text.lastIndexOf('(', numberStartIndex);
    }

    text = CalculatorEngine::removeLastNumber(text, numberStartIndex);
    text += QString::number(number, 'g', 15);

    return text;
}


//функція для знаходження квадратного кореня
QString CalculatorEngine::squareRootNumber(const QString &expression, int& posOfSqrt)
{
    QString text = expression;
    int numberStartIndex;
    double number = CalculatorEngine::getLastNumber(text, numberStartIndex);

    // Якщо число обгорнуте в дужки (наприклад (-6))
    if (CalculatorEngine::isWrappedInParentheses(text, numberStartIndex)) {
        // Знаходимо позицію відкриваючої дужки
        numberStartIndex = text.lastIndexOf('(', numberStartIndex);

        // Перевіряємо, чи число від'ємне
        if (text.at(numberStartIndex + 1) == '-')
            return "Error"; // Корінь із від’ємного числа не обробляється
    }

    // Обчислюємо квадратний корінь
    number = std::sqrt(number);


    //зберігаємо індекс куди вставити значок кореня
    posOfSqrt = numberStartIndex;

    // Видаляємо останнє число (з дужками, якщо були)
    text = CalculatorEngine::removeLastNumber(text, numberStartIndex);

    // Додаємо результат
    text += QString::number(number, 'g', 15);

    return text;
}

//функція яка повертає один процент від числа
QString CalculatorEngine::getPercent(const QString &expression)
{
    QString text = expression;
    int lastIndex;

    // Отримуємо останнє число у виразі
    double number = getLastNumber(text, lastIndex);
    int numberStart = lastIndex;

    //беремо з нього відсоток
    number /= 100;
    //якщо число в дужках
    if(CalculatorEngine::isWrappedInParentheses(text, lastIndex)){
        text = CalculatorEngine::removeLastNumber(text, numberStart);
        text = text + QString::number(number, 'g', 15) + ')';
    }
    //якщо число без дужок
    else{
        text = CalculatorEngine::removeLastNumber(text, lastIndex);
        text += QString::number(number, 'g', 15);
    }

    return text;
}

//функція для зміни знака числа
QString CalculatorEngine::toggleLastNumberSign(const QString& expression)
{
    QString text = expression;
    int lastIndex;

    // Отримуємо останнє число у виразі
    double number = getLastNumber(text, lastIndex);
    int numberStart = lastIndex;

    // Перевіряємо, чи це число вже записане у вигляді (-...)
    if (isNegativeWrapped(text, numberStart, number)) {
        // Якщо так — прибираємо "(-" і ")"
        text.remove(numberStart, 2); // видаляємо "(-"
        text.remove(text.indexOf(")", numberStart), 1); // видаляємо ")"
        return text;
    }

    // Позиція символу перед числом
    int signPos = numberStart > 0 ? numberStart - 1 : 0;

    // Якщо перед числом стоїть + або -
    if (text.at(signPos) == '+') {
        text.replace(signPos, 1, "-");
    }
    else if (text.at(signPos) == '-') {
        text.replace(signPos, 1, "+");
    }
    // Якщо перед числом * або /
    else if (QString("*/").contains(text.at(signPos))) {
        text.insert(numberStart, "(-");
        text.append(")");
    }
    // Якщо перед числом нічого або це просто окреме число
    else if (text.at(signPos).isDigit()) {
        if (numberStart != 0) {
            text.insert(numberStart, "(-");
            text.append(")");
        } else {
            text.prepend("(-");
            text.append(")");
        }
    }

    return text;
}

