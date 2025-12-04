#include "masterview.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("医院诊疗系统-主界面");
    setFixedSize(800, 600);

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menu = menuBar->addMenu("功能模块");
    QAction *deptAct = menu->addAction("科室管理");
    QAction *doctorAct = menu->addAction("医生管理");
    QAction *patientAct = menu->addAction("患者管理");
    setMenuBar(menuBar);

    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);

    welcomeLabel = new QLabel("请选择功能模块");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size:14pt; font-weight:bold;");
    stackedWidget->addWidget(welcomeLabel);

    deptView = new DepartmentView;
    doctorView = new DoctorView;
    patientView = new PatientView;
    stackedWidget->addWidget(deptView);
    stackedWidget->addWidget(doctorView);
    stackedWidget->addWidget(patientView);

    connect(deptAct, &QAction::triggered, [=]() { stackedWidget->setCurrentWidget(deptView); });
    connect(doctorAct, &QAction::triggered, [=]() { stackedWidget->setCurrentWidget(doctorView); });
    connect(patientAct, &QAction::triggered, [=]() { stackedWidget->setCurrentWidget(patientView); });
}

void MasterView::setDatabase(idatabase* db)
{
    deptView->setDatabase(db);
    doctorView->setDatabase(db);
    patientView->setDatabase(db);
}
