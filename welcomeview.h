#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WelcomeView; }
QT_END_NAMESPACE

namespace Hospital {

class WelcomeView : public QWidget
{
    Q_OBJECT
public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();
signals:
    void gotoPatient();
    void gotoDoctor();
    void gotoDept();
private:
    Ui::WelcomeView *ui;
};

} // namespace Hospital
