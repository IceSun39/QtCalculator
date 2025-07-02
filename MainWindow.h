#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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
    void on_deleteAll_clicked();
    void on_equal_clicked();
    void on_changeSign_clicked();
    void on_reverseNumber_clicked();
    void on_squareNumber_clicked();
    void on_squareRoot_clicked();
    void on_getPercent_clicked();

    void setStandardMode();
    void setEngineeringMode();



    void on_piButton_clicked();

    void on_eButton_clicked();

private:
    Ui::MainWindow *ui;
    bool displayIsEmpty = true;
    QString lastEntered;

    // Current active widgets and buttons
    QLabel* currentDisplay;
    QLabel* currentUpperDisplay;
    QList<QPushButton*> currentDigitButtons;
    QList<QPushButton*> currentOperatorButtons;
    QPushButton* currentEqualButton;
    QPushButton* currentDeleteButton;
    QPushButton* currentDeleteAllButton;
    QPushButton* currentDeleteLineButton;
    QPushButton* currentChangeSignButton;
    QPushButton* currentReverseButton;
    QPushButton* currentSquareButton;
    QPushButton* currentSqrtButton;
    QPushButton* currentPercentButton;
};

#endif // MAINWINDOW_H
