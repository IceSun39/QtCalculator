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
    QString digitValue = clickedButton->text();

    // Додаємо цифру до дисплея
    if(displayIsEmpty){
        ui->display->clear();
        ui->display->setText(ui->display->text() + digitValue);
        displayIsEmpty = false;
    }
    else{
        ui->display->setText(ui->display->text() + digitValue);
    }
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

