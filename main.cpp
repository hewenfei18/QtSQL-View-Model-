#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include "loginview.h"
#include "masterview.h"
#include "idatabase.h"   // 注意头文件

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("HospitalDemo");

    // ---- 自检：驱动 + 数据库连接 ----
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
    qDebug() << "DB open:" << Hospital::IDatabase::instance()->db().isOpen();

    auto *login = new Hospital::LoginView;
    auto *mainW = new Hospital::MasterView;
    login->show();

    QObject::connect(login, &Hospital::LoginView::loginSuccess,
                     mainW, [=](const QString &u){
                         mainW->setUser(u);
                         mainW->show();
                         login->close();
                     });
    return a.exec();
}
