#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "inc/mainwindow.h"

namespace Ui {
class LoginWidget;
}

class MainWThread : public QThread
{
public:
    void showUi();

    void run() override;
};


class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

signals:
    void logining();

private slots:
    void toLogin(const bool &flag);
    void readSettings();
    void updateSettings();
    void getValueFromObj(const QJsonObject &obj);

private:
    Ui::LoginWidget *ui;
    string *id;
    int *pre_size;
    QTcpSocket *socket;
    QFile *settings;

    //json option
    bool auto_login;
    bool remember_password;
};

#endif // LOGINWIDGET_H
