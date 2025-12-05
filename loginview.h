#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginView; }
QT_END_NAMESPACE

namespace Hospital {

class LoginView : public QWidget
{
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();
signals:
    void loginSuccess(const QString &user);
private slots:
    void onLogin();
    void onRegister();
private:
    Ui::LoginView *ui;
};

} // namespace Hospital
