#include "inc/loginwidget.h"
#include "ui_loginwidget.h"

void MainWThread::showUi()
{
    this->run();
}

void MainWThread::run()
{
    MainWindow *mw = new MainWindow();
    mw->show();
}

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget),
    id(new string()),
    pre_size(new int(0)),
    socket(new QTcpSocket())
{
    ui->setupUi(this);

    //widget init
    ui->Account->setEditable(true);
    ui->Account->lineEdit()->setPlaceholderText("号码");
    ui->Account->setContextMenuPolicy(Qt::NoContextMenu);
    ui->Account->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->Account->lineEdit()->setFocus();

    ui->Password->setPlaceholderText("密码");
    ui->Password->setContextMenuPolicy(Qt::NoContextMenu);
    ui->Password->setEchoMode(QLineEdit::Password);
    ui->Password->setAttribute(Qt::WA_InputMethodEnabled, false);

    ui->FormatError->hide();
    ui->PasswordError->hide();

    //号码输入
    connect(ui->Account->lineEdit(), &QLineEdit::textEdited, [&](){
        ui->Password->setText("");

        //增加字
        if (ui->Account->lineEdit()->text().size() > *pre_size)
        {
            QString end = *(ui->Account->lineEdit()->text().end() - 1);
            if (end <= 57 && end >= 48)  //输入的是数字
            {
                *id += end.toStdString();
            }
            ui->Account->lineEdit()->setText(QString::fromStdString(*id));
        }
        //删除字
        else
        {
            *id = ui->Account->lineEdit()->text().toStdString();
        }
        *pre_size = ui->Account->lineEdit()->text().size();
    });

    //回车跳转密码输入
    connect(ui->Account->lineEdit(), &QLineEdit::returnPressed, [&](){
        if (ui->Account->lineEdit()->text() != "")
        {
            ui->Password->setFocus();
        }
        else
        {
            ui->Account->lineEdit()->setPlaceholderText("请输入号码");
        }
    });

    //密码输入
    connect(ui->Password, &QLineEdit::textEdited, [&](){

    });

    //回车登录
    connect(ui->Password, &QLineEdit::returnPressed, [&](){
        ui->ToLogin->clicked();
    });

    //点击登录按钮
    connect(ui->ToLogin, &QPushButton::clicked, [&](){
        ui->FormatError->hide();
        ui->PasswordError->hide();
        ui->ToLogin->setText("登录中...");
        ui->Account->lineEdit()->setPlaceholderText("号码");
        mySleep(100);
        if (ui->Account->currentText().size() >= 10 && ui->Password->text().size() >= 8)
        {
            emit logining();
        }
        else
        {
            ui->ToLogin->setText("登录");
            ui->FormatError->show();
        }
    });

    //登录操作
    connect(this, &LoginWidget::logining, [&](){
        socket->connectToHost(ADDRESS, PORT);
    });

    //校验
    connect(socket, &QTcpSocket::connected, [&](){
        //send phone
        socket->write(ui->Account->lineEdit()->text().toStdString().c_str());
        socket->waitForBytesWritten();
        //send flag
        socket->write("z");
        socket->waitForBytesWritten();
        //send password
        socket->write(ui->Password->text().toStdString().c_str());
        socket->waitForBytesWritten();
    });

    //检查
    connect(socket, &QTcpSocket::readyRead, [&](){
        mySleep(500);
        QString ret = socket->readAll();
        bool flag = ret == "yes" ? true : false;
        toLogin(flag);
    });
}

void LoginWidget::toLogin(bool flag)
{
    //登录成功
    if (flag)
    {
        this->close();
        MainWThread *mt = new MainWThread();
        mt->showUi();
        ui->Password->setText("");
        ui->FormatError->hide();
        ui->PasswordError->hide();
        ui->ToLogin->setText("登录");
        return;
    }

    //密码错误
    else
    {
        ui->ToLogin->setText("登录");
        ui->PasswordError->show();
    }
}

LoginWidget::~LoginWidget()
{
    if (ui != NULL) delete ui; ui = NULL;
    if (id != NULL) delete id; id = NULL;
    if (pre_size != NULL) delete pre_size; pre_size = NULL;
    delete socket;
}
