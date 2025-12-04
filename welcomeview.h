#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);

signals:
    void toLoginView();

private slots:
    void onEnterClicked();
};

#endif // WELCOMEVIEW_H
