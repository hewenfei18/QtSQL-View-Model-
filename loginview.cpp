#include "loginview.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent), m_db(nullptr)
{
    label = new QLabel("欢迎使用诊疗测试系统");
    usernameEdit = new QLineEdit("admin");
    passwordEdit = new QLineEdit("123456");
    passwordEdit->setEchoMode(QLineEdit::Password);
    loginBtn = new QPushButton("登录");
    registerBtn = new QPushButton("注册");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->addSpacing(20);

    QHBoxLayout *userLayout = new QHBoxLayout;
    userLayout->addWidget(new QLabel("用户名："), 0, Qt::AlignRight);
    userLayout->addWidget(usernameEdit);
    layout->addLayout(userLayout);

    QHBoxLayout *pwdLayout = new QHBoxLayout;
    pwdLayout->addWidget(new QLabel("密码："), 0, Qt::AlignRight);
    pwdLayout->addWidget(passwordEdit);
    layout->addLayout(pwdLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(loginBtn);
    btnLayout->addWidget(registerBtn);
    layout->addLayout(btnLayout);

    connect(loginBtn, &QPushButton::clicked, this, &LoginView::onLoginClicked);
    connect(registerBtn, &QPushButton::clicked, this, &LoginView::onRegisterClicked);
}

void LoginView::setDatabase(idatabase* db) {
    m_db = db;
}

void LoginView::onLoginClicked() {
    if (!m_db) {
        QMessageBox::critical(this, "错误", "数据库未初始化！");
        return;
    }

    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();

    auto user = m_db->fetchOne(
        "SELECT * FROM User WHERE USERNAME = ? AND PASSWORD = ?",
        {username, password}
        );

    if (user.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名或密码错误！");
        return;
    }

    emit loggedIn(username);
}

void LoginView::onRegisterClicked() {
    if (!m_db) {
        QMessageBox::critical(this, "错误", "数据库未初始化！");
        return;
    }

    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入用户名和密码！");
        return;
    }

    auto result = m_db->exec(
        "SELECT ID FROM User WHERE USERNAME = ?",
        {username}
        );

    if (!result.next()) {
        bool success = m_db->insert("User", {
                                                {"USERNAME", username},
                                                {"PASSWORD", password},
                                                {"FULLNAME", "未知用户"}
                                            });

        if (success) {
            QMessageBox::information(this, "成功", "注册成功！");
            emit registered();
        } else {
            QMessageBox::critical(this, "错误", "注册失败！");
        }
    } else {
        QMessageBox::warning(this, "错误", "用户名已存在！");
    }
}
