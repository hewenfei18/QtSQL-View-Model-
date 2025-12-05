#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MasterView; }
QT_END_NAMESPACE

namespace Hospital {

class WelcomeView;
class PatientView;
class DoctorView;
class DepartmentView;

class MasterView : public QMainWindow
{
    Q_OBJECT
public:
    explicit MasterView(QWidget *parent = nullptr);
    ~MasterView();
    void setUser(const QString &u);
private slots:
    void switchTo(QWidget *w);
private:
    Ui::MasterView *ui;
    QString m_user;
    WelcomeView *m_welcome = nullptr;
    PatientView *m_patient = nullptr;
    DoctorView *m_doctor = nullptr;
    DepartmentView *m_dept = nullptr;
};

} // namespace Hospital
