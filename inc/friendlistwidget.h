#ifndef FRIENDLISTWIDGET_H
#define FRIENDLISTWIDGET_H

#include <QWidget>

namespace Ui {
class FriendListWidget;
}

class FriendListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListWidget(QStringList friends, bool selectable = false, QWidget *parent = 0);
    ~FriendListWidget();

signals:
    void seletedFriend(QString);

private:
    Ui::FriendListWidget *ui;
};

#endif // FRIENDLISTWIDGET_H
