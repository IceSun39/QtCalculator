#include "MainWindow.h"
#include "CalculatorEngine.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->display->setStyleSheet("color: white; font-size: 24px;");
    ui->display->setAlignment(Qt::AlignRight);

    QList<QPushButton*> printButtons = {
        ui->oneNum, ui->twoNum, ui->threeNum,
        ui->fourNum, ui->fiveNum, ui->sixNum,
        ui->sevenNum, ui->eightNum, ui->nineNum,
        ui->zeroNum, ui->plus, ui->minus, ui->multiply,
        ui->division, ui->dot
    };

    for (QPushButton *button : printButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString buttonValue = clickedButton->text();
    QString currentText = ui->display->text();

    // Якщо дисплей пустий
    if (displayIsEmpty) {
        // Першим не можуть бути * або /
        if (buttonValue == "*" || buttonValue == "/") return;

        // Якщо це крапка — додаємо "0."
        if (buttonValue == ".") {
            ui->display->setText("0.");
            lastEntered = "0.";
        }
        else {
            ui->display->setText(buttonValue);
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
        ui->display->setText(currentText);
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
    ui->display->setText(currentText + buttonValue);

    // Оновлюємо lastEntered
    if (QString("+-*/()").contains(buttonValue)) {
        lastEntered = buttonValue;
    } else {
        if(!lastWasNumber) lastEntered = buttonValue;
        else lastEntered += buttonValue;
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if(!displayIsEmpty){
        QString text = ui->display->text();
        text.chop(1); // видаляє останній символ
        lastEntered.chop(1);
        ui->display->setText(text);
    }
    if(ui->display->text().isEmpty()){
        displayIsEmpty = true;
        ui->display->setText(ui->display->text() + '0');
    }
}


void MainWindow::on_deleteLine_clicked()
{
    ui->display->clear();
    lastEntered.clear();
    displayIsEmpty = true;
    ui->display->setText(ui->display->text() + '0');
}


void MainWindow::on_equal_clicked()
{
    QString expr = ui->display->text();
    QVector<Token> tokens = CalculatorEngine::tokenize(expr);
    QQueue<Token> rnp = CalculatorEngine::parsing(tokens);
    double result = CalculatorEngine::evaluate(rnp);

    // Показуємо попередній вираз (меншим, сірим)
    ui->upperDisplay->setText(expr);
    ui->upperDisplay->setStyleSheet("color: gray; font-size: 14px;");
    ui->upperDisplay->setAlignment(Qt::AlignRight);

    // Показуємо результат більшим шрифтом
    ui->display->setText(QString::number(result));
    ui->display->setStyleSheet("color: white; font-size: 24px;");
    ui->display->setAlignment(Qt::AlignRight);

}

void MainWindow::on_changeSign_clicked()
{
    QString text = ui->display->text();
    text = CalculatorEngine::toggleLastNumberSign(text);
    ui->display->setText(text);
}

void MainWindow::on_deleteAll_clicked()
{
    ui->display->clear();
    ui->upperDisplay->clear();
    lastEntered.clear();
    displayIsEmpty = true;
    ui->display->setText(ui->display->text() + '0');
}


void MainWindow::on_reverseNumber_clicked()
{
    int lastIndex;
    QString text = ui->display->text();
    double number = CalculatorEngine::getLastNumber(text, lastIndex);
    number = 1 / number;
    QString toAdd = QString::number(number);
    text.erase(text.cbegin() + lastIndex, text.cend());
    text += toAdd;
    ui->display->setText(text);
}

