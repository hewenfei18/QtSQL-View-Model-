#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QSqlQuery>
namespace Hospital {

LoginView::LoginView(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginView)
{
    ui->setupUi(this);
    connect(ui->pushButton_login, &QPushButton::clicked, this, &LoginView::onLogin);
    connect(ui->pushButton_register, &QPushButton::clicked, this, &LoginView::onRegister);
}

LoginView::~LoginView() { delete ui; }

void LoginView::onLogin()
{
    QString u = ui->lineEdit_username->text().trimmed();
    QString p = ui->lineEdit_password->text();
    if (IDatabase::instance()->validateUser(u, p)) {
        IDatabase::instance()->addUserLog(u, "登录");
        emit loginSuccess(u);
    } else {
        QMessageBox::warning(this, "提示", "用户名或密码错误！");
    }
}

void LoginView::onRegister()
{
    QString u = ui->lineEdit_username->text().trimmed();
    QString p = ui->lineEdit_password->text();
    if (u.isEmpty() || p.isEmpty()) return;
    QSqlQuery q;
    q.prepare("INSERT OR IGNORE INTO users(username,password) VALUES(?,?)");
    q.addBindValue(u);
    q.addBindValue(p);
    if (q.exec() && q.numRowsAffected() > 0) {
        QMessageBox::information(this, "提示", "注册成功，请登录");
    } else {
        QMessageBox::warning(this, "提示", "用户名已存在");
    }
}

} // namespace Hospital
