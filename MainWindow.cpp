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
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    //підключення операторів
    for (auto *btn : currentOperatorButtons)
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    //підлючення функціональних кнопок
    connect(currentEqualButton, &QPushButton::clicked, this, &MainWindow::on_equal_clicked);
    connect(currentDeleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked);
    connect(currentDeleteLineButton, &QPushButton::clicked, this, &MainWindow::on_deleteLine_clicked);
    connect(currentDeleteAllButton, &QPushButton::clicked, this, &MainWindow::on_deleteAll_clicked);
    connect(currentChangeSignButton, &QPushButton::clicked, this, &MainWindow::on_changeSign_clicked);
    connect(currentReverseButton, &QPushButton::clicked, this, &MainWindow::on_reverseNumber_clicked);
    connect(currentSquareButton, &QPushButton::clicked, this, &MainWindow::on_squareNumber_clicked);
    connect(currentSqrtButton, &QPushButton::clicked, this, &MainWindow::on_squareRoot_clicked);
    connect(currentPercentButton, &QPushButton::clicked, this, &MainWindow::on_getPercent_clicked);
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
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    //підключення операторів
    for (auto *btn : currentOperatorButtons)
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    //підлючення функціональних кнопок
    connect(currentEqualButton, &QPushButton::clicked, this, &MainWindow::on_equal_clicked);
    connect(currentDeleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked);
    connect(currentDeleteLineButton, &QPushButton::clicked, this, &MainWindow::on_deleteLine_clicked);
    connect(currentDeleteAllButton, &QPushButton::clicked, this, &MainWindow::on_deleteAll_clicked);
    connect(currentChangeSignButton, &QPushButton::clicked, this, &MainWindow::on_changeSign_clicked);
    connect(currentReverseButton, &QPushButton::clicked, this, &MainWindow::on_reverseNumber_clicked);
    connect(currentSquareButton, &QPushButton::clicked, this, &MainWindow::on_squareNumber_clicked);
    connect(currentSqrtButton, &QPushButton::clicked, this, &MainWindow::on_squareRoot_clicked);
    connect(currentPercentButton, &QPushButton::clicked, this, &MainWindow::on_getPercent_clicked);
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
        if (buttonValue == "*" || buttonValue == "/") return;

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
    text = CalculatorEngine::squareNumber(text);
    currentUpperDisplay->setText(currentDisplay->text() + "^2");
    currentDisplay->setText(text);
}

//шукає квадратний корінь
void MainWindow::on_squareRoot_clicked()
{
    int posOfSqrt;
    QString text = currentDisplay->text();
    QString oldText = text;
    text = CalculatorEngine::squareRootNumber(text, posOfSqrt);
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

void MainWindow::on_piButton_clicked()
{
    if(QString("+-*/").contains(lastEntered)){
        if(displayIsEmpty){
            QString text = currentDisplay->text();
            text = "3.1415926";
            lastEntered = text;
            displayIsEmpty = false;
            currentDisplay->setText(text);
        }
        else{
            QString text = currentDisplay->text();
            currentDisplay->setText(text + "3.1415926");
            lastEntered = "3.1415926";
        }
    }
}


void MainWindow::on_eButton_clicked()
{
    if(QString("+-*/").contains(lastEntered)){
        if(displayIsEmpty){
            QString text = currentDisplay->text();
            text = "2.7182818";
            lastEntered = text;
            displayIsEmpty = false;
            currentDisplay->setText(text);
        }
        else{
            QString text = currentDisplay->text();
            currentDisplay->setText(text + "2.7182818");
            lastEntered = "2.7182818";
        }
    }
}
