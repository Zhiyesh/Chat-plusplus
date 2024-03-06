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
    pre_size(new int(0)),
    socket(new QTcpSocket()),
    read_account(new QFile("./history")),
    isnot_first(true),
    ndg(nullptr),
    add(nullptr),
    flt(nullptr),
    FriendTab(new QTabWidget(this))
{
    ui->setupUi(this);
    ui->InputBox->setPlaceholderText("说点什么~");
    ui->TheOtherChatting->setAlignment(Qt::AlignLeft);
    ui->MyChatting->setAlignment(Qt::AlignRight);
    ui->EnableEnterSend->setCheckState(Qt::Unchecked);
    ui->SendMessage->setEnabled(false);

    FriendTab->resize(517, 571);
    FriendTab->move(27, 48);
    FriendTab->setStyleSheet("background-color: rgb(255, 255, 255);"
                             "border: 6px solid rgb(255, 255, 255);"
                             "border-radius: 12px;");

    if (read_account->open(QIODevice::ReadOnly)) {
        phone = read_account->readAll();
        read_account->close();
    }

    //Connect to Server
    socket->connectToHost(ADDRESS, PORT);

    connect(socket, &QTcpSocket::connected, [&](){
        //已连接
        socket->write(QString("logining#%1").arg(phone).toStdString().c_str());
        socket->waitForBytesWritten();
        connect(socket, &QTcpSocket::readyRead, [&](){
            //接受到信息
            Zy::mSleep(500);
            QString ret = socket->readAll();
            QStringList rec = ret.split("#");
            if (rec[0] == "msg") {
                if (isnot_first && ui->MyChatting->toPlainText() != "") {
                    ui->TheOtherChatting->append("\n\n");
                    isnot_first = false;
                }
                ui->TheOtherChatting->append(rec[2] + "\n\n");
            }
            else if (rec[0] == "recmyfri") {
                friends = QString(rec[1]).split("z");
            }
        });
        Zy::mSleep(800);
        //Init my friends
        socket->write("getmyfri#");
        socket->waitForBytesWritten();
        socket->write(phone.toStdString().c_str());
        socket->waitForBytesWritten();
    });

    //发送消息
    connect(ui->SendMessage, &QPushButton::clicked, [&](){
        Zy::mSleep(100);
        QString content = ui->InputBox->toPlainText();
        if (content != "") {
            socket->write(QString("send#%1#%2#%3")
                          .arg(phone)
                          .arg(FriendTab->tabText(FriendTab->currentIndex()))
                          .arg(content).toStdString().c_str());
            ui->MyChatting->append(content + "\n\n");
            ui->InputBox->setText("");
        }
        ui->InputBox->setFocus();
    });

    //回车键发送启用
    connect(ui->EnableEnterSend, &QCheckBox::stateChanged, [&](){
        if (ui->EnableEnterSend->checkState() == Qt::Checked) {
            enabled_entertosend = true;
        }
        else enabled_entertosend = false;
    });

    //回车发送功能实现
    connect(ui->InputBox, &QTextEdit::textChanged, [&](){
        QString content = ui->InputBox->toPlainText();
        if (content == "\n") {
            ui->InputBox->setText("");
            return;
        }
        if (!(enabled_entertosend)) goto END;
        //检测回车
        if (content.size() > *pre_size) {
            if (*(content.end() - 1) == '\n') {
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
        if (ndg != nullptr) {
            delete ndg;
            ndg = new FriendListWidget(friends, true);
        }
        else {
            ndg = new FriendListWidget(friends, true);
        }
        ndg->show();
        connect(ndg, SIGNAL(seletedFriend(QString)), this, SLOT(addDialogTab(QString)));
    });

    //关闭会话
    connect(ui->close_dialog, &QAction::triggered, [&](){
        FriendTab->removeTab(FriendTab->currentIndex());
        if (FriendTab->currentIndex() == -1) {
            ui->SendMessage->setEnabled(false);
        }
    });

    //关闭所有会话
    connect(ui->close_every_dialog, &QAction::triggered, [&](){
        while (FriendTab->currentIndex() != -1) FriendTab->removeTab(0);
    });

//好友
    //添加好友
    connect(ui->add_friend, &QAction::triggered, [&](){
        if (add != nullptr) {
            delete add;
            add = new AddFriendDialog();
        }
        else {
            add = new AddFriendDialog();
        }
        add->show();
    });

    //好友列表
    connect(ui->friend_list, &QAction::triggered, [&](){
        if (flt != nullptr) {
            delete flt;
            flt = new FriendListWidget(friends);
        }
        else {
            flt = new FriendListWidget(friends);
        }
        flt->show();
    });


//选项
    //退出登录
    connect(ui->exit_login, &QAction::triggered, [&](){
        emit ui->exit->triggered();
        emit ui->add_new_id->triggered();
    });

    //添加新号码
    connect(ui->add_new_id, &QAction::triggered, [&](){
        LoginWThread *lt = new LoginWThread();
        lt->showUi();
    });

    //设置
    connect(ui->settings, &QAction::triggered, [&](){

    });

    //退出应用(同时退出登录)
    connect(ui->exit, &QAction::triggered, [&](){
        this->close();
        socket->write(QString("exit#%1").arg(phone).toStdString().c_str());
        socket->waitForBytesWritten();
    });

}

void MainWindow::addDialogTab(QString phone)
{
    QWidget *tmp = new QWidget();
    FriendTab->addTab(tmp, phone);
    ui->SendMessage->setEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit ui->exit->triggered();
    socket->close();
    event->accept();
}

MainWindow::~MainWindow()
{
#define DELETE(mem)  \
    if (mem != nullptr) { \
        delete mem;       \
        mem = nullptr;    \
    }

    DELETE(ui);
    DELETE(pre_size);
    DELETE(socket);
    DELETE(read_account);
    DELETE(ndg);
    DELETE(add);
    DELETE(flt);
    DELETE(FriendTab);
}

