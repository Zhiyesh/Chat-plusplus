#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QWidget>
#include "inc/sleep.h"

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget *parent = 0);
    ~AddFriendDialog();

public slots:
    bool checkExist(const QString& phone, QString& name);

private:
    Ui::AddFriendDialog *ui;
};

#endif // ADDFRIENDDIALOG_H
