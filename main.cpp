#include <QApplication>
#include "welcomeview.h"
#include "loginview.h"
#include "masterview.h"
#include "idatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    idatabase db;
    if (!db.connect("hospital.db")) {
        return -1;
    }

    WelcomeView welcomeView;
    LoginView loginView;
    MasterView masterView;
    masterView.setDatabase(&db);

    QObject::connect(&welcomeView, &WelcomeView::toLoginView, [&]() {
        welcomeView.hide();
        loginView.setDatabase(&db);
        loginView.show();
    });

    QObject::connect(&loginView, &LoginView::loggedIn, [&](const QString&) {
        loginView.hide();
        masterView.show();
    });

    welcomeView.show();
    return a.exec();
}
