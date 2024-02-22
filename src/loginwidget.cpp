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
    socket(new QTcpSocket()),
    auto_login(false),
    remember_password(false),
    settings(new QFile(JSON_PATH))
{
    ui->setupUi(this);
    setFixedSize(776, 626);

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
    ui->NetworkDisconnect->hide();

    //Settings init
    readSettings();

    //Set CheckBox initial value
    ui->AutoLogin->setCheckState(auto_login ? Qt::Checked : Qt::Unchecked);
    ui->RememberPassword->setCheckState(remember_password ? Qt::Checked : Qt::Unchecked);

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

    //回车跳转密码框
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

    //回车登录
    connect(ui->Password, &QLineEdit::returnPressed, [&](){
        ui->ToLogin->clicked();
    });

    //点击登录按钮
    connect(ui->ToLogin, &QPushButton::clicked, [&](){
        ui->FormatError->hide();
        ui->PasswordError->hide();
        ui->NetworkDisconnect->hide();
        ui->ToLogin->setText("登录中...");
        ui->Account->lineEdit()->setPlaceholderText("号码");
        Zy::mSleep(100);
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
        if (!socket->waitForConnected(5000))
        {
            ui->ToLogin->setText("登录");
            ui->NetworkDisconnect->show();
        }
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
        Zy::mSleep(500);
        QString ret = socket->readAll();
        bool flag = ret == "yes" ? true : false;
        toLogin(flag);
    });

    //自动登录开启
    connect(ui->AutoLogin, &QCheckBox::clicked, [&](){
        if (ui->AutoLogin->checkState() == Qt::Checked)
        {
            auto_login = true;
        }
        else auto_login = false;
        updateSettings();
    });

    //自动登录实现
    if (auto_login && (ui->Account->lineEdit()->text() != "") && (ui->Password->text() != ""))
    {
        emit ui->ToLogin->clicked();
    }

    //记住密码开启
    connect(ui->RememberPassword, &QCheckBox::clicked, [&](){
        if (ui->RememberPassword->checkState() == Qt::Checked)
        {
            remember_password = true;
        }
        else remember_password = false;
        updateSettings();
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

void LoginWidget::readSettings()
{
    QByteArray jsondata;
    if (settings->open(QIODevice::ReadOnly))
    {
        jsondata = settings->readAll();
        settings->close();
    }
    else
    {
        updateSettings();
        return;
    }

    QJsonDocument jsondoc = QJsonDocument::fromJson(jsondata);

    if (jsondoc.isObject())
    {
        QJsonObject root = jsondoc.object();
        getValueFromObj(root);
    }
}

void LoginWidget::updateSettings()
{
    //create json
    QJsonObject root;

    QJsonObject login_settings;
    login_settings.insert("auto_login", auto_login);
    login_settings.insert("remember_password", remember_password);

    root.insert("login_settings", login_settings);

    QJsonDocument jsondoc(root);
    QByteArray jsondata = jsondoc.toJson();

    if (settings->open(QIODevice::WriteOnly))
    {
        //覆盖写
        settings->write(jsondata);
        settings->close();
    }
}

void LoginWidget::getValueFromObj(QJsonObject obj)
{
    QStringList keys = obj.keys();
    for (QString key : keys)
    {
        QJsonValue value = obj.value(key);
        if (value.isBool())
        {
            if (key == "auto_login")
                auto_login = value.toBool();
            else if (key == "remember_password")
                remember_password = value.toBool();
        }
        else if (value.isObject())
        {
            getValueFromObj(value.toObject());
        }
    }
}

LoginWidget::~LoginWidget()
{
    if (ui != NULL) delete ui; ui = NULL;
    if (id != NULL) delete id; id = NULL;
    if (pre_size != NULL) delete pre_size; pre_size = NULL;
    delete socket;
    delete settings;
}
