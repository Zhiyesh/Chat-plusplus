#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include "inc/loginwidget.h"

void LoginWThread::showUi()
{
    this->run();
}

void LoginWThread::run()
{
    LoginWidget *lw = new LoginWidget();
    lw->show();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    enabled_entertosend(new bool(false)),
    pre_size(new int(0))

{
    ui->setupUi(this);
    ui->InputBox->setPlaceholderText("说点什么~");
    ui->TheOtherChatting->setAlignment(Qt::AlignLeft);
    ui->MyChatting->setAlignment(Qt::AlignRight);
    ui->EnableEnterSend->setCheckState(Qt::Unchecked);

    //发送消息
    connect(ui->SendMessage, &QPushButton::clicked, [&](){
        Zy::mSleep(100);
        QString content = ui->InputBox->toPlainText();
        if (content != "")
        {
            ui->MyChatting->append(content + "\n\n");
            ui->InputBox->setText("");
        }
        ui->InputBox->setFocus();
    });

    //接收消息
    //ui->MyChatting->append("\n");

    //回车键发送启用
    connect(ui->EnableEnterSend, &QCheckBox::stateChanged, [&](){

        if (ui->EnableEnterSend->checkState() == Qt::Checked)
            *enabled_entertosend = true;
        else
            *enabled_entertosend = false;
    });

    //回车发送功能实现
    connect(ui->InputBox, &QTextEdit::textChanged, [&](){
        QString content = ui->InputBox->toPlainText();
        if (content == "\n")
        {
            ui->InputBox->setText("");
            return;
        }
        if (!(*enabled_entertosend)) goto END;
        //检测回车
        if (content.size() > *pre_size)
        {
            if (*(content.end() - 1) == '\n')
            {
                std::string tmp = content.toStdString();
                tmp.erase(tmp.end() - 1);
                ui->InputBox->setText(QString::fromStdString(tmp));

                emit ui->SendMessage->clicked();
                *pre_size = 0;
            }
        }
        END: *pre_size = content.size();
    });

//会话
    //新建会话
    connect(ui->new_dialog, &QAction::triggered, [&](){
        ui->TheOtherChatting->setText("");
        ui->MyChatting->setText("");
        ui->InputBox->setText("");
        ui->TheOtherChatting->setAlignment(Qt::AlignLeft);
        ui->MyChatting->setAlignment(Qt::AlignRight);
    });

    //关闭会话
    connect(ui->close_dialog, &QAction::triggered, [&](){
        emit ui->new_dialog->triggered();
    });

//好友
    //添加好友
    connect(ui->add_friend, &QAction::triggered, [&](){
        AddFriendDialog *add = new AddFriendDialog();
        add->show();
    });


//选项
    //退出登录
    connect(ui->exit_login, &QAction::triggered, [&](){
        this->close();
        LoginWThread *lt = new LoginWThread();
        lt->showUi();
    });

    //添加新号码
    connect(ui->add_new_id, &QAction::triggered, [&](){
        LoginWThread *lt01 = new LoginWThread();
        lt01->showUi();
    });

    //设置
    connect(ui->settings, &QAction::triggered, [&](){

    });

    //退出应用(同时退出登录)
    connect(ui->exit, &QAction::triggered, [&](){
        this->close();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
    delete enabled_entertosend;
    delete pre_size;
}

