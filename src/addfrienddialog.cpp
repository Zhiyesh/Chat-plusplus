#include "inc/addfrienddialog.h"
#include "ui_addfrienddialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriendDialog)
{
    ui->setupUi(this);

    ui->SearchResult->hide();
    ui->AddFromResult->hide();
    ui->PhoneInput->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->PhoneInput->setContextMenuPolicy(Qt::NoContextMenu);

    //取消按钮
    connect(ui->Cancel, &QPushButton::clicked, [&](){
        this->close();
    });

    //查找按钮
    connect(ui->Search, &QPushButton::clicked, [&](){
        ui->SearchResult->setText("正在查找...");
        ui->SearchResult->setStyleSheet("font: 11pt \"微软雅黑\";");
        ui->SearchResult->show();
        ui->AddFromResult->hide();

        QString phone = ui->PhoneInput->text();
        QString name = QString();
        //查找成功判定
        if (checkExist(phone, name))
        {
            ui->SearchResult->setText(name);
            ui->SearchResult->show();

            ui->AddFromResult->show();
        }
        else
        {
            ui->SearchResult->setText("找不到该号码！");
            ui->SearchResult->setStyleSheet("font: 11pt \"微软雅黑\";color: rgb(149, 20, 65);");
            ui->SearchResult->show();
        }
    });

    //回车查找
    connect(ui->PhoneInput, &QLineEdit::returnPressed, [&](){
        emit ui->Search->clicked();
    });

    //添加按钮
    connect(ui->AddFromResult, &QPushButton::clicked, [&](){

    });
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

bool AddFriendDialog::checkExist(const QString &phone, QString &name)
{
    if (phone != "")
    {
        name = "好友1123";
        return true;
    }
    return false;
}
