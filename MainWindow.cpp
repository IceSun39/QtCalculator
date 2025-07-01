#include "MainWindow.h"
#include "CalculatorEngine.h"
#include "./ui_MainWindow.h"
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->display1->setStyleSheet("color: white; font-size: 24px;");
    ui->display1->setAlignment(Qt::AlignRight);
    ui->upperDisplay1->setStyleSheet("color: gray; font-size: 14px;");
    ui->upperDisplay1->setAlignment(Qt::AlignRight);


    QList<QPushButton*> printButtons = {
        ui->oneNum1, ui->twoNum1, ui->threeNum1,
        ui->fourNum1, ui->fiveNum1, ui->sixNum1,
        ui->sevenNum1, ui->eightNum1, ui->nineNum1,
        ui->zeroNum1, ui->plus1, ui->minus1, ui->multiply1,
        ui->division1, ui->dot1
    };

    for (QPushButton *button : printButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    }

    // Створюємо меню
    QMenu *menu = new QMenu(this);
    menu->addAction("Звичайний", this, &MainWindow::setStandardMode);
    menu->addAction("Інженерний", this, &MainWindow::setEngineeringMode);

    // Прив'язуємо меню до кнопки
    ui->toolButtonMenu1->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonMenu1->setMenu(menu);

    // Стандартний калькулятор
    ui->stackedWidget->setCurrentIndex(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString buttonValue = clickedButton->text();
    QString currentText = ui->display1->text();

    // Якщо дисплей пустий
    if (displayIsEmpty) {
        // Першим не можуть бути * або /
        if (buttonValue == "*" || buttonValue == "/") return;

        // Якщо це крапка — додаємо "0."
        if (buttonValue == ".") {
            ui->display1->setText("0.");
            lastEntered = "0.";
        }
        else {
            ui->display1->setText(buttonValue);
            lastEntered = buttonValue;
        }
        displayIsEmpty = false;
        return;
    }

    // Якщо останній введений — оператор
    bool lastWasNumber;
    lastEntered.toDouble(&lastWasNumber);

    //заміна операторів
    if(QString("+-*/").contains(buttonValue) && QString("+-*/").contains(lastEntered)){
        currentText.chop(1);
        currentText += buttonValue;
        ui->display1->setText(currentText);
        return;
    }

    // Заборона двох операторів поспіль
    //if (!lastWasNumber && QString("+-*/").contains(buttonValue)) {
    //    return;
    //}

    // Заборона двох крапок в одному числі
    if (lastWasNumber && buttonValue == "." && lastEntered.contains(".")) {
        return;
    }

    // Обмеження довжини числа
    if (lastWasNumber && lastEntered.length() >= 9 && QString("0123456789").contains(buttonValue)) {
        return;
    }

    // Дозволене натискання — додаємо
    ui->display1->setText(currentText + buttonValue);

    // Оновлюємо lastEntered
    if (QString("+-*/()").contains(buttonValue)) {
        lastEntered = buttonValue;
    } else {
        if(!lastWasNumber) lastEntered = buttonValue;
        else lastEntered += buttonValue;
    }
}

void MainWindow::on_deleteButton1_clicked()
{
    if(!displayIsEmpty){
        QString text = ui->display1->text();
        text.chop(1); // видаляє останній символ
        lastEntered.chop(1);
        ui->display1->setText(text);
    }
    if(ui->display1->text().isEmpty()){
        displayIsEmpty = true;
        ui->display1->setText(ui->display1->text() + '0');
    }
}

void MainWindow::on_deleteLine1_clicked()
{
    ui->display1->clear();
    lastEntered.clear();
    displayIsEmpty = true;
    ui->display1->setText(ui->display1->text() + '0');
}

void MainWindow::on_equal1_clicked()
{
    QString expr = ui->display1->text();
    QVector<Token> tokens = CalculatorEngine::tokenize(expr);
    QQueue<Token> rnp = CalculatorEngine::parsing(tokens);
    double result = CalculatorEngine::evaluate(rnp);

    // Показуємо попередній вираз (меншим, сірим)
    ui->upperDisplay1->setText(expr);

    // Показуємо результат більшим шрифтом
    ui->display1->setText(QString::number(result));

}

void MainWindow::on_changeSign1_clicked()
{
    QString text = ui->display1->text();
    text = CalculatorEngine::toggleLastNumberSign(text);
    ui->display1->setText(text);
}

void MainWindow::on_deleteAll1_clicked()
{
    ui->display1->clear();
    ui->upperDisplay1->clear();
    lastEntered.clear();
    displayIsEmpty = true;
    ui->display1->setText(ui->display1->text() + '0');
}

void MainWindow::on_reverseNumber1_clicked()
{
    QString text = ui->display1->text();
    text = CalculatorEngine::reverseNumber(text);
    ui->display1->setText(text);
}

void MainWindow::on_squareNumber1_clicked()
{
    QString text = ui->display1->text();
    text = CalculatorEngine::squareNumber(text);
    ui->upperDisplay1->setText(ui->display1->text() + "^2");
    ui->display1->setText(text);
}

void MainWindow::on_squareRoot1_clicked()
{
    int posOfSqrt;
    QString text = ui->display1->text();
    QString oldText = text;
    text = CalculatorEngine::squareRootNumber(text, posOfSqrt);
    oldText.insert(posOfSqrt, "√");
    ui->upperDisplay1->setText(oldText);
    ui->display1->setText(text);
}

void MainWindow::on_getPercent1_clicked()
{
    QString text = ui->display1->text();
    text = CalculatorEngine::getPercent(text);
    ui->display1->setText(text);
}

void MainWindow::setStandardMode()
{
    // стандартний калькулятор
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::setEngineeringMode()
{
    // інженерний калькулятор
    ui->stackedWidget->setCurrentIndex(0);
}




