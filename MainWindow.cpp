#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    if(ui->display->displayText().isEmpty()){
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

