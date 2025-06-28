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

    void on_deleteButton_clicked();

    void on_deleteLine_clicked();

    void on_equal_clicked();

private:
    bool displayIsEmpty = true;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
