#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CalculatorEngine.h"
#include <QMenu>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Створення меню режимів
    QMenu *menu = new QMenu(this);
    menu->addAction("Звичайний", this, &MainWindow::setStandardMode);
    menu->addAction("Інженерний", this, &MainWindow::setEngineeringMode);
    ui->toolButtonMenu1->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonMenu1->setMenu(menu);
    ui->toolButtonMenu1_2->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonMenu1_2->setMenu(menu);

    // Ініціалізація режиму
    setStandardMode();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//ставить стандартний режим
void MainWindow::setStandardMode()
{
    // Перехід на сторінку стандартного режиму (індекс 0)
    ui->stackedWidget->setCurrentIndex(0);

    // Присвоєння вказівників на віджети стандартного режиму
    QWidget* page = ui->stackedWidget->currentWidget();
    currentDisplay = page->findChild<QLabel*>("display1_2");
    currentUpperDisplay = page->findChild<QLabel*>("upperDisplay1_2");

    currentDisplay->setStyleSheet("color: white; font-size: 24px;");
    currentDisplay->setAlignment(Qt::AlignRight);
    currentUpperDisplay->setStyleSheet("color: gray; font-size: 14px;");
    currentUpperDisplay->setAlignment(Qt::AlignRight);

    // Цифрові кнопки
    currentDigitButtons = {
        page->findChild<QPushButton*>("oneNum1_2"),
        page->findChild<QPushButton*>("twoNum1_2"),
        page->findChild<QPushButton*>("threeNum1_2"),
        page->findChild<QPushButton*>("fourNum1_2"),
        page->findChild<QPushButton*>("fiveNum1_2"),
        page->findChild<QPushButton*>("sixNum1_2"),
        page->findChild<QPushButton*>("sevenNum1_2"),
        page->findChild<QPushButton*>("eightNum1_2"),
        page->findChild<QPushButton*>("nineNum1_2"),
        page->findChild<QPushButton*>("zeroNum1_2")
    };
    // Операторські кнопки
    currentOperatorButtons = {
        page->findChild<QPushButton*>("plus1_2"),
        page->findChild<QPushButton*>("minus1_2"),
        page->findChild<QPushButton*>("multiply1_2"),
        page->findChild<QPushButton*>("division1_2"),
        page->findChild<QPushButton*>("dot1_2")
    };

    // Інші функціональні кнопки
    currentEqualButton = page->findChild<QPushButton*>("equal1_2");
    currentDeleteButton = page->findChild<QPushButton*>("deleteButton1_2");
    currentDeleteLineButton = page->findChild<QPushButton*>("deleteLine1_2");
    currentDeleteAllButton = page->findChild<QPushButton*>("deleteAll1_2");
    currentChangeSignButton = page->findChild<QPushButton*>("changeSign1_2");
    currentReverseButton = page->findChild<QPushButton*>("reverseNumber1_2");
    currentSquareButton = page->findChild<QPushButton*>("squareNumber1_2");
    currentSqrtButton = page->findChild<QPushButton*>("squareRoot1_2");
    currentPercentButton = page->findChild<QPushButton*>("getPercent1_2");

    // Підключення слотів
    for (auto *btn : currentDigitButtons)
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked, Qt::UniqueConnection);
    //підключення операторів
    for (auto *btn : currentOperatorButtons)
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked, Qt::UniqueConnection);
    //підлючення функціональних кнопок
    connect(currentEqualButton, &QPushButton::clicked, this, &MainWindow::on_equal_clicked, Qt::UniqueConnection);
    connect(currentDeleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked, Qt::UniqueConnection);
    connect(currentDeleteLineButton, &QPushButton::clicked, this, &MainWindow::on_deleteLine_clicked, Qt::UniqueConnection);
    connect(currentDeleteAllButton, &QPushButton::clicked, this, &MainWindow::on_deleteAll_clicked, Qt::UniqueConnection);
    connect(currentChangeSignButton, &QPushButton::clicked, this, &MainWindow::on_changeSign_clicked, Qt::UniqueConnection);
    connect(currentReverseButton, &QPushButton::clicked, this, &MainWindow::on_reverseNumber_clicked, Qt::UniqueConnection);
    connect(currentSquareButton, &QPushButton::clicked, this, &MainWindow::on_squareNumber_clicked, Qt::UniqueConnection);
    connect(currentSqrtButton, &QPushButton::clicked, this, &MainWindow::on_squareRoot_clicked, Qt::UniqueConnection);
    connect(currentPercentButton, &QPushButton::clicked, this, &MainWindow::on_getPercent_clicked, Qt::UniqueConnection);
}

//ставить інженерний режим
void MainWindow::setEngineeringMode()
{
    // Перехід на сторінку інженерного режиму (індекс 1)
    ui->stackedWidget->setCurrentIndex(1);

    // Присвоєння вказівників на віджети інженерного режиму
    QWidget* page = ui->stackedWidget->currentWidget();
    currentDisplay = page->findChild<QLabel*>("display1");
    currentUpperDisplay = page->findChild<QLabel*>("upperDisplay1");

    currentDisplay->setStyleSheet("color: white; font-size: 24px;");
    currentDisplay->setAlignment(Qt::AlignRight);
    currentUpperDisplay->setStyleSheet("color: gray; font-size: 14px;");
    currentUpperDisplay->setAlignment(Qt::AlignRight);

    // Цифрові кнопки
    currentDigitButtons = {
        page->findChild<QPushButton*>("oneNum1"),
        page->findChild<QPushButton*>("twoNum1"),
        page->findChild<QPushButton*>("threeNum1"),
        page->findChild<QPushButton*>("fourNum1"),
        page->findChild<QPushButton*>("fiveNum1"),
        page->findChild<QPushButton*>("sixNum1"),
        page->findChild<QPushButton*>("sevenNum1"),
        page->findChild<QPushButton*>("eightNum1"),
        page->findChild<QPushButton*>("nineNum1"),
        page->findChild<QPushButton*>("zeroNum1")
    };
    // Операторські кнопки
    currentOperatorButtons = {
        page->findChild<QPushButton*>("plus1"),
        page->findChild<QPushButton*>("minus1"),
        page->findChild<QPushButton*>("multiply1"),
        page->findChild<QPushButton*>("division1"),
        page->findChild<QPushButton*>("dot1")
    };

    // Інші функціональні кнопки
    currentEqualButton = page->findChild<QPushButton*>("equal1");
    currentDeleteButton = page->findChild<QPushButton*>("deleteButton1");
    currentDeleteLineButton = page->findChild<QPushButton*>("deleteLine1");
    currentDeleteAllButton = page->findChild<QPushButton*>("deleteAll1");
    currentChangeSignButton = page->findChild<QPushButton*>("changeSign1");
    currentReverseButton = page->findChild<QPushButton*>("reverseNumber1");
    currentSquareButton = page->findChild<QPushButton*>("squareNumber1");
    currentSqrtButton = page->findChild<QPushButton*>("squareRoot1");
    currentPercentButton = page->findChild<QPushButton*>("getPercent1");

    // Підключення слотів
    for (auto *btn : currentDigitButtons)
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked, Qt::UniqueConnection);
    //підключення операторів
    for (auto *btn : currentOperatorButtons)
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked, Qt::UniqueConnection);
    //підлючення функціональних кнопок
    connect(currentEqualButton, &QPushButton::clicked, this, &MainWindow::on_equal_clicked, Qt::UniqueConnection);
    connect(currentDeleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked, Qt::UniqueConnection);
    connect(currentDeleteLineButton, &QPushButton::clicked, this, &MainWindow::on_deleteLine_clicked, Qt::UniqueConnection);
    connect(currentDeleteAllButton, &QPushButton::clicked, this, &MainWindow::on_deleteAll_clicked, Qt::UniqueConnection);
    connect(currentChangeSignButton, &QPushButton::clicked, this, &MainWindow::on_changeSign_clicked, Qt::UniqueConnection);
    connect(currentReverseButton, &QPushButton::clicked, this, &MainWindow::on_reverseNumber_clicked, Qt::UniqueConnection);
    connect(currentSquareButton, &QPushButton::clicked, this, &MainWindow::on_squareNumber_clicked, Qt::UniqueConnection);
    connect(currentSqrtButton, &QPushButton::clicked, this, &MainWindow::on_squareRoot_clicked, Qt::UniqueConnection);
    connect(currentPercentButton, &QPushButton::clicked, this, &MainWindow::on_getPercent_clicked, Qt::UniqueConnection);

}

//функція для додавання символа на екран
void MainWindow::onButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton || !currentDisplay)
        return;

    QString buttonValue = clickedButton->text();
    QString currentText = currentDisplay->text();

    // Якщо дисплей пустий
    if (displayIsEmpty) {
        if (buttonValue == "*" || buttonValue == "/" || buttonValue == ")") return;

        if (buttonValue == ".") {
            currentDisplay->setText("0.");
            lastEntered = "0.";
        } else {
            currentDisplay->setText(buttonValue);
            lastEntered = buttonValue;
        }
        displayIsEmpty = false;
        return;
    }

    // Перевірка чи минулий символ був числом
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);

    // Змінини оператор, якщо був введений інший оператор
    if (QString("+-*/").contains(buttonValue) && QString("+-*/").contains(lastEntered)) {
        currentText.chop(1);
        currentText += buttonValue;
        currentDisplay->setText(currentText);
        lastEntered = buttonValue;
        return;
    }

    // Запобігання багатьом точкам в числі
    if (lastWasNumber && buttonValue == "." && lastEntered.contains(".")) {
        return;
    }

    // Обмеження довжини числа
    if (lastWasNumber && lastEntered.length() >= 9 && QString("0123456789").contains(buttonValue)) {
        return;
    }

    // Додавання до екрану
    currentDisplay->setText(currentText + buttonValue);

    // Оновлення lastEntered
    if (QString("+-*/()").contains(buttonValue)) {
        lastEntered = buttonValue;
    } else {
        if (!lastWasNumber)
            lastEntered = buttonValue;
        else
            lastEntered += buttonValue;
    }
}

//видалення останнього символу
void MainWindow::on_deleteButton_clicked()
{
    if(!displayIsEmpty){
        QString text = currentDisplay->text();
        text.chop(1); // видаляє останній символ
        lastEntered.chop(1);
        currentDisplay->setText(text);
    }
    if(currentDisplay->text().isEmpty()){
        displayIsEmpty = true;
        currentDisplay->setText(currentDisplay->text() + '0');
    }
}

//видалення рядка
void MainWindow::on_deleteLine_clicked()
{
    currentDisplay->clear();
    lastEntered.clear();
    displayIsEmpty = true;
    currentDisplay->setText(currentDisplay->text() + '0');
}

//видалення рядка та сірого рядка зверху
void MainWindow::on_deleteAll_clicked()
{
    currentDisplay->clear();
    currentUpperDisplay->clear();
    lastEntered.clear();
    displayIsEmpty = true;
    currentDisplay->setText(currentDisplay->text() + '0');
}

//кнопка дорівнює
void MainWindow::on_equal_clicked()
{
    QString expr = currentDisplay->text();
    //додаємо дужки
    int missedParens = leftParenCount - rightParenCount;
    while(missedParens-- > 0){
        expr += ')';
    }
    QVector<Token> tokens = CalculatorEngine::tokenize(expr);
    QQueue<Token> rnp = CalculatorEngine::parsing(tokens);
    double result = CalculatorEngine::evaluate(rnp);

    // Показуємо попередній вираз (меншим, сірим)
    currentUpperDisplay->setText(expr);

    // Показуємо результат більшим шрифтом
    currentDisplay->setText(QString::number(result));

}

//міняє знак останнього числа
void MainWindow::on_changeSign_clicked()
{
    QString text = currentDisplay->text();
    text = CalculatorEngine::toggleLastNumberSign(text);
    currentDisplay->setText(text);
}

//обертає число (1/число)
void MainWindow::on_reverseNumber_clicked()
{
    QString text = currentDisplay->text();
    text = CalculatorEngine::reverseNumber(text);
    currentDisplay->setText(text);
}

//підносить до квадрату
void MainWindow::on_squareNumber_clicked()
{
    QString text = currentDisplay->text();
    text = CalculatorEngine::numberToPower(text,2);
    currentUpperDisplay->setText(currentDisplay->text() + "^2");
    currentDisplay->setText(text);
}

//шукає квадратний корінь
void MainWindow::on_squareRoot_clicked()
{
    int posOfSqrt;
    QString text = currentDisplay->text();
    QString oldText = text;
    text = CalculatorEngine::rootNumber(text, 2, posOfSqrt);
    oldText.insert(posOfSqrt, "√");
    currentUpperDisplay->setText(oldText);
    currentDisplay->setText(text);
}

//находить один відсоток від числа
void MainWindow::on_getPercent_clicked()
{
    QString text = currentDisplay->text();
    text = CalculatorEngine::getPercent(text);
    currentDisplay->setText(text);
}

//кнопка для пі
void MainWindow::on_piButton_clicked()
{
    QString text = currentDisplay->text();
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);
    if(displayIsEmpty){
        text = PI;
        lastEntered = PI;
        displayIsEmpty = false;
    }
    else if(QString("+-*/(").contains(lastEntered)){
        text += PI;
    }
    else if(lastEntered == ')' || lastWasNumber){
        text = text + '*' + PI;
    }
    currentDisplay->setText(text);
    lastEntered = PI;
}

//кнопка для числа Ейлера
void MainWindow::on_eButton_clicked()
{
    QString text = currentDisplay->text();
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);
    if(displayIsEmpty){
        text = E;
        lastEntered = E;
        displayIsEmpty = false;
    }
    else if(QString("+-*/(").contains(lastEntered)){
        text += E;
    }
    else if(lastEntered == ')' || lastWasNumber){
        text = text + '*' + E;
    }
    currentDisplay->setText(text);
    lastEntered = E;
}

//ліва дужка
void MainWindow::on_LeftParenButton_clicked()
{
    QString text = currentDisplay->text();
    leftParenCount++;
    //якзо екран пустий
    if (displayIsEmpty) {
        text = "(";
        currentDisplay->setText(text);
        displayIsEmpty = false;
    //якщо останній раз був введений оператор
    } else if (QString("+-*/(").contains(lastEntered)) {
        currentDisplay->setText(text + "(");
    //
    } else {
        currentDisplay->setText(text + "*(");
    }
    lastEntered = "(";
}

//права дужка
void MainWindow::on_RightParenButton_clicked()
{
    if(leftParenCount > rightParenCount){
        rightParenCount++;
        QString text = currentDisplay->text();
        //якзо останній був ведений оператор, то дужки ще закривати не можна
        if(QString("+-*/").contains(lastEntered)) return;
        else{
            currentDisplay->setText(text + ')');
            lastEntered = ')';
        }
    }
}

void MainWindow::on_lnButton_clicked()
{
    QString text = currentDisplay->text();
    leftParenCount++;
    //якзо екран пустий
    if (displayIsEmpty) {
        text = "ln(";
        currentDisplay->setText(text);
        displayIsEmpty = false;
    //якщо останній раз був введений оператор
    } else if (QString("+-*/(").contains(lastEntered)) {
        currentDisplay->setText(text + "ln(");
        //
    } else {
        currentDisplay->setText(text + "*ln(");
    }
    lastEntered = "ln(";
}

void MainWindow::on_logButton_clicked()
{
    QString text = currentDisplay->text();
    leftParenCount++;
    //якзо екран пустий
    if (displayIsEmpty) {
        text = "log(";
        currentDisplay->setText(text);
        displayIsEmpty = false;
    //якщо останній раз був введений оператор
    } else if (QString("+-*/(").contains(lastEntered)) {
        currentDisplay->setText(text + "log(");
        //
    } else {
        currentDisplay->setText(text + "*log(");
    }
    lastEntered = "log(";
}

void MainWindow::on_tenToPowerButton_clicked()
{
    QString text = currentDisplay->text();
    //якзо екран пустий
    if (displayIsEmpty) {
        text = "10^";
        currentDisplay->setText(text);
        displayIsEmpty = false;
    //якщо останній раз був введений оператор
    } else if (QString("+-*/(").contains(lastEntered)) {
        currentDisplay->setText(text + "10^");
        //
    } else {
        currentDisplay->setText(text + "*10^");
    }
    lastEntered = "10^";
}

void MainWindow::on_xToPowerButton_clicked()
{
    QString text = currentDisplay->text();
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);

    //підносити в степінь можна тільки якщо останнє введене було число або ')'
    if(lastWasNumber || lastEntered == ')'){
        currentDisplay->setText(text + '^');
        lastEntered = "^";
    }
}

void MainWindow::on_sqrtRootNButton_clicked()
{
    QString text = currentDisplay->text();
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);

    //знаходити корінь можна тільки якщо останнє введене було число або ')'
    if(lastWasNumber || lastEntered == ')'){
        currentDisplay->setText(text + "ysqrt");
        lastEntered = "ysqrt";
    }
}


void MainWindow::on_absButton_clicked()
{
    QString text = currentDisplay->text();
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);
    leftParenCount++;

    // Знаходимо останнє число у виразі та індекс, з якого воно починається
    int numberStartIndex;
    double number = CalculatorEngine::getLastNumber(text, numberStartIndex);

    int numberPrevIndex = numberStartIndex == 0 ? 0 : numberStartIndex - 1;

    if(displayIsEmpty){
        text = "abs(";
        currentDisplay->setText(text);
    }
    else if(lastWasNumber){
        text.insert(numberPrevIndex, "abs(");
        currentDisplay->setText(text);
    }
    else{
        currentDisplay->setText(text + "*abs(");
    }
    lastEntered = "abs(";
}

