#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inc/addfrienddialog.h"
#include "inc/hostsocket.h"
#include "inc/sleep.h"
#include "inc/friendlistwidget.h"
#include <QString>
#include <QThread>
#include <QTcpSocket>
#include <QCloseEvent>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTabWidget>

#include <QDebug>

#define JSON_PATH  "./settings.json"
#define ACT_PATH   "./account.dat"

using std::string;

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

private slots:
    void addDialogTab(QString phone);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    bool enabled_entertosend;
    int *pre_size;
    QTcpSocket *socket;
    QFile *read_account;
    QString phone;
    bool isnot_first;
    FriendListWidget *ndg;  //新建会话窗口
    AddFriendDialog *add;  //添加好友窗口
    FriendListWidget *flt;  //好友列表窗口
    QStringList friends;
    QTabWidget *FriendTab;
};

#endif // MAINWINDOW_H
