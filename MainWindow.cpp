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

    if((lastEntered == '+' && buttonValue == '-') || (lastEntered == '-' && buttonValue == '+')){
        QString text = ui->display->text();
        text.chop(1);
        ui->display->setText(text);
    }

    // Додаємо цифру до дисплея коли він пустий
    if(displayIsEmpty){
        //знак ділення та множення не можуть бути першими знаками
        if(buttonValue != '/' && buttonValue != '*'){
            ui->display->clear();
            ui->display->setText(ui->display->text() + buttonValue);
            displayIsEmpty = false;
        }
    }
    //додаємо цийру до дисплея коли не пустий
    else{
        ui->display->setText(ui->display->text() + buttonValue);
    }
    lastEntered = buttonValue;
}

void MainWindow::on_deleteButton_clicked()
{
    if(!displayIsEmpty){
        QString text = ui->display->text();
        text.chop(1); // видаляє останній символ
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
    int lastIndex;
    QString text = ui->display->text();
    double number = CalculatorEngine::getLastNumber(text, lastIndex);
    lastIndex = lastIndex == 0 ? 0 : --lastIndex;
    if(text.at(lastIndex) == '+'){
        text.replace(lastIndex, 1, "-");
    }
    else if(text.at(lastIndex) == '-'){
        text.replace(lastIndex, 1, "+");
    }
    else if(QString("*/()").contains(text.at(lastIndex))){
        text.insert(++lastIndex, "(-");
        text.push_back(')');
    }
    else if(text.at(lastIndex).isNumber()){
        if(lastIndex != 0){
            text.insert(lastIndex, "(-");
            text.push_back(')');
        }
        //якщо останній індекс 0, то у нас тільки одне число в рядку
        else{
            text.push_front("(-");
            text.push_back(')');
        }
    }
    ui->display->setText(text);
}

