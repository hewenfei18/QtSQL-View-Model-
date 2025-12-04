#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "idatabase.h"

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    void setDatabase(idatabase* db);

signals:
    void loggedIn(const QString& username);
    void registered();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QLabel *label;
    QLineEdit *usernameEdit, *passwordEdit;
    QPushButton *loginBtn, *registerBtn;
    idatabase* m_db;
};

#endif // LOGINVIEW_H
