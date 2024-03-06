#include "inc/friendlistwidget.h"
#include "ui_friendlistwidget.h"

FriendListWidget::FriendListWidget(QStringList friends, bool selectable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendListWidget)
{
    ui->setupUi(this);
    setFixedSize(454, 564);

    if (selectable) {
        ui->Ok->show();
        ui->Cancel->show();
    }
    else {
        ui->Ok->hide();
        ui->Cancel->hide();
    }

    if (!friends.isEmpty()) {
        ui->IsNoFriend->hide();
        ui->FriendShow->addItems(friends);
    }
    else ui->IsNoFriend->show();

    connect(ui->Ok, &QPushButton::clicked, [&](){
        emit seletedFriend(ui->FriendShow->currentItem()->text());
        this->close();
    });

    connect(ui->Cancel, &QPushButton::clicked, this, &FriendListWidget::close);
}

FriendListWidget::~FriendListWidget()
{
    delete ui;
}
