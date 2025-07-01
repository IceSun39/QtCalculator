#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onButtonClicked();

    void on_deleteButton1_clicked();

    void on_deleteLine1_clicked();

    void on_equal1_clicked();

    void on_changeSign1_clicked();

    void on_deleteAll1_clicked();

    void on_reverseNumber1_clicked();

    void on_squareNumber1_clicked();

    void on_squareRoot1_clicked();

    void on_getPercent1_clicked();

    void setStandardMode();

    void setEngineeringMode();


private:
    bool displayIsEmpty = true;
    QString lastEntered;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
