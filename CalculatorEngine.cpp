#include "CalculatorEngine.h"

// Токенізуємо вхідний текстовий рядок у список токенів
QVector<Token> CalculatorEngine::tokenize(const QString& text) {
    QVector<Token> tokens;
    int i = 0;

    TokenType lastTokenType = Operator; // умовно, на старті вважаємо попередній токен "не числом"

    while (i < text.length()) {
        QChar ch = text.at(i);

        if (ch.isSpace()) {
            ++i;
            continue;
        }

        // Обробка чисел, включаючи ті, що починаються з крапки
        if (ch.isDigit() || ch == '.') {
            QString res;
            if (ch == '.') res = "0";
            while (i < text.length() && (text.at(i).isDigit() || text.at(i) == '.')) {
                res += text.at(i++);
            }
            double num = res.toDouble();
            tokens.push_back({ Number, num, ' ' });
            lastTokenType = Number;
            continue;
        }

        // Обробка функцій (abs, ln, log, тощо)
        if (ch.isLetter()) {
            QString word;
            while (i < text.length() && text.at(i).isLetter()) {
                word += text.at(i++);
            }

            if (word == "mod" || word == "yroot" || word == "exp") {
                tokens.push_back({ Operator, 0, ' ', word });  // ← оператор з назвою
            }
            else {
                tokens.push_back({ Function, 0, ' ', word }); // інші — функції
            }
            lastTokenType = Operator;
            continue;
        }

        // Обробка дужок
        if (ch == '(') {
            tokens.push_back({ LeftParen, 0, '(' });
            ++i;
            lastTokenType = LeftParen;
            continue;
        }
        if (ch == ')') {
            tokens.push_back({ RightParen, 0, ')' });
            ++i;
            lastTokenType = RightParen;
            continue;
        }

        // Обробка мінуса: унарний чи бінарний?
        if (ch == '-') {
            bool isUnary = (lastTokenType == Operator || lastTokenType == LeftParen || lastTokenType == Function);
            ++i;
            if (isUnary) {
                // Очікуємо число одразу після унарного мінуса
                QString res;
                if (i < text.length() && text.at(i) == '.') res = "0";
                while (i < text.length() && (text.at(i).isDigit() || text.at(i) == '.')) {
                    res += text.at(i++);
                }
                double num = res.toDouble();
                tokens.push_back({ Number, -num, ' ' });
                lastTokenType = Number;
            } else {
                tokens.push_back({ Operator, 0, '-' });
                lastTokenType = Operator;
            }
            continue;
        }

        // Інші оператори
        if (QString("*/+^,").contains(ch)) {
            tokens.push_back({ Operator, 0, ch });
            ++i;
            lastTokenType = Operator;
            continue;
        }

        // Якщо дійшли сюди — щось пішло не так
        qWarning() << "Невідомий символ у виразі:" << ch;
        ++i;
    }

    return tokens;
}


// Визначення пріоритету оператора
int CalculatorEngine::getPriority(const Token& token) {
    if (token.type == Operator) {
        if (token.funcName == "mod" || token.funcName == "yroot" || token.funcName == "exp") return 2;
        if (token.op == '+' || token.op == '-') return 1;
        if (token.op == '*' || token.op == '/') return 2;
        if (token.op == '^') return 3;
    }
    return 0;
}

// Перетворення інфіксного запису на зворотний польський (постфіксний)
QQueue<Token> CalculatorEngine::parsing(const QVector<Token>& tokens) {
    QQueue<Token> output;
    QStack<Token> operators;

    for (const Token& token : tokens) {
        switch (token.type) {
        case Number:
            output.enqueue(token);
            break;

        case Function:
            operators.push(token); // функції мають вищий пріоритет
            break;

        case Operator:
            while (!operators.isEmpty() &&
                   ((operators.top().type == Operator && getPriority(operators.top()) >= getPriority(token)) ||
                    (operators.top().type == Function))) {
                output.enqueue(operators.pop());
            }
            operators.push(token);
            break;

        case LeftParen:
            operators.push(token);
            break;

        case RightParen:
            while (!operators.isEmpty() && operators.top().type != LeftParen) {
                output.enqueue(operators.pop());
            }
            if (!operators.isEmpty() && operators.top().type == LeftParen) {
                operators.pop(); // викинути ліву дужку
            }
            // Якщо після закриваючої дужки йде функція — додаємо її
            if (!operators.isEmpty() && operators.top().type == Function) {
                output.enqueue(operators.pop());
            }
            break;
        }
    }

    while (!operators.isEmpty()) {
        output.enqueue(operators.pop());
    }

    return output;
}


// Обчислення результату зі зворотного польського запису
double CalculatorEngine::evaluate(QQueue<Token>& rpn) {
    QStack<double> stack;

    while (!rpn.isEmpty()) {
        Token token = rpn.dequeue();

        switch (token.type) {
        case Number:
            stack.push(token.value);
            break;

        case Operator: {
            if (token.funcName == "mod" || token.funcName == "yroot" || token.funcName == "exp") {
                if (stack.size() < 2) throw std::runtime_error("Недостатньо аргументів для оператора");

                double b = stack.pop();
                double a = stack.pop();

                if (token.funcName == "mod")
                    stack.push(fmod(a, b));
                else if (token.funcName == "yroot") {
                    if (a < 0 && ((int)b) % 2 == 0)
                        throw std::runtime_error("Корінь парного степеня з від’ємного");
                    stack.push(pow(a, 1.0 / b));
                }
                else if (token.funcName == "exp") {
                    stack.push(a * pow(10, b));  // ось тут!
                }
            } else {
                // звичайні оператори
                if (stack.size() < 2) throw std::runtime_error("Недостатньо операндів");

                double b = stack.pop();
                double a = stack.pop();

                switch (token.op.toLatin1()) {
                case '+': stack.push(a + b); break;
                case '-': stack.push(a - b); break;
                case '*': stack.push(a * b); break;
                case '/': stack.push(a / b); break;
                case '^': stack.push(pow(a, b)); break;
                default: throw std::runtime_error("Невідомий оператор");
                }
            }
            break;
        }

        case Function: {
            if (stack.isEmpty()) throw std::runtime_error("Недостатньо аргументів для функції");

            // Для yroot і mod потрібно два аргументи
            if (token.funcName == "mod" || token.funcName == "yroot") {
                if (stack.size() < 2) throw std::runtime_error("Недостатньо аргументів для функції");

                double b = stack.pop();
                double a = stack.pop();

                if (token.funcName == "mod")
                    stack.push(fmod(a, b));
                else if (token.funcName == "yroot")
                    stack.push(pow(b, 1.0 / a));
            } else {
                double a = stack.pop();

                if (token.funcName == "abs")
                    stack.push(std::abs(a));
                else if (token.funcName == "log")
                    stack.push(log10(a));
                else if (token.funcName == "ln")
                    stack.push(log(a));
                else
                    throw std::runtime_error("Невідома функція: " + token.funcName.toStdString());
            }
            break;
        }

        default:
            throw std::runtime_error("Невідомий токен у evaluate()");
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Помилка обчислення: зайві значення у стеку");

    return stack.top();
}

double CalculatorEngine::getLastNumber(const QString &text)
{

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
    //if (text[end] != ')') return "";

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
QString CalculatorEngine::numberToPower(const QString& expression, int power)
{
    QString text = expression;

    int numberStartIndex;
    QString subExpr;

    // Перевірка чи в кінці стоїть дужка
    if (text.endsWith(')')) {
        subExpr = CalculatorEngine::extractExpressionInParentheses(text, numberStartIndex);
        if (!subExpr.isEmpty()) {
            //обчислюємо значення в дужках
            QVector<Token> tokens = CalculatorEngine::tokenize(subExpr.mid(1, subExpr.length() - 2)); // без дужок
            QQueue<Token> rpn = CalculatorEngine::parsing(tokens);
            double value = CalculatorEngine::evaluate(rpn);
            std::pow(value, power);

            // видалити підвираз із text
            text = text.left(numberStartIndex);
            text += QString::number(value, 'g', 15);
            return text;
        }
    }

    double number = CalculatorEngine::getLastNumber(text, numberStartIndex);
    std::pow(number, power);

    if(CalculatorEngine::isWrappedInParentheses(text, numberStartIndex)){
        numberStartIndex = text.lastIndexOf('(', numberStartIndex);
    }

    text = CalculatorEngine::removeLastNumber(text, numberStartIndex);
    text += QString::number(number, 'g', 15);

    return text;
}


//функція для знаходження квадратного кореня
QString CalculatorEngine::rootNumber(const QString& expression, int powerOfRoot, int& posOfSqrt)
{
    QString text = expression;
    QString subExpr;
    int numberStartIndex;
    double number = CalculatorEngine::getLastNumber(text, numberStartIndex);

    // Якщо число обгорнуте в дужки (наприклад (-6))
    if (text.endsWith(')')) {
        subExpr = CalculatorEngine::extractExpressionInParentheses(text, numberStartIndex);
        if (!subExpr.isEmpty()) {
            QVector<Token> tokens = CalculatorEngine::tokenize(subExpr.mid(1, subExpr.length() - 2));
            QQueue<Token> rpn = CalculatorEngine::parsing(tokens);
            double value = CalculatorEngine::evaluate(rpn);

            if (value < 0 && powerOfRoot % 2 == 0) {
                return "Error, finding root from negative value";
            }

            value = std::pow(value, 1.0 / powerOfRoot);

            // Видаляємо підвираз
            text = text.left(numberStartIndex);
            text += QString::number(value, 'g', 15);
            return text;
        }
    }

    // Якщо просто число без дужок
    if (number < 0 && powerOfRoot % 2 == 0) {
        return "Error, finding root from negative value";
    }

    number = std::pow(number, 1.0 / powerOfRoot);

    posOfSqrt = numberStartIndex;
    text = CalculatorEngine::removeLastNumber(text, numberStartIndex);
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

