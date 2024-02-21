#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "inc/mainwindow.h"


namespace Ui {
class LoginWidget;
}


class LoginWidget;

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
    void toLogin(bool flag);

private:
    Ui::LoginWidget *ui;
    string *id;
    int *pre_size;
    QTcpSocket *socket;
};

#endif // LOGINWIDGET_H
