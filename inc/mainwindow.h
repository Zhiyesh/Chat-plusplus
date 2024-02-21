#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inc/addfrienddialog.h"
#include "inc/hostip.h"
#include "inc/sleep.h"
#include <QString>
#include <QThread>
#include <QTcpSocket>
#include <QTime>

using std::string;
using std::fstream;
using std::ios_base;

namespace Ui {
class MainWindow;
}

class LoginWThread : public QThread
{
public:
    void showUi();

    void run() override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool *enabled_entertosend;
    int *pre_size;
};

#endif // MAINWINDOW_H
