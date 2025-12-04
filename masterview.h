#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include "departmentview.h"
#include "doctorview.h"
#include "patientview.h"

class MasterView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MasterView(QWidget *parent = nullptr);
    void setDatabase(idatabase* db);

private:
    QStackedWidget *stackedWidget;
    DepartmentView *deptView;
    DoctorView *doctorView;
    PatientView *patientView;
    QLabel *welcomeLabel;
};

#endif // MASTERVIEW_H
