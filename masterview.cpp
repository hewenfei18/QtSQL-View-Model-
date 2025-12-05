#include "masterview.h"
#include "ui_masterview.h"
#include "welcomeview.h"
#include "patientview.h"
#include "doctorview.h"
#include "departmentview.h"
#include "idatabase.h"
#include <QDebug>

namespace Hospital {

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MasterView)
{
    ui->setupUi(this);

    // 创建子页面
    m_welcome = new WelcomeView(this);
    m_patient = new PatientView(this);
    m_doctor  = new DoctorView(this);
    m_dept    = new DepartmentView(this);

    ui->stackedWidget->addWidget(m_welcome);
    ui->stackedWidget->addWidget(m_patient);
    ui->stackedWidget->addWidget(m_doctor);
    ui->stackedWidget->addWidget(m_dept);

    // 欢迎页 → 各模块
    connect(m_welcome, &WelcomeView::gotoPatient, this, [this](){ switchTo(m_patient); });
    connect(m_welcome, &WelcomeView::gotoDoctor,  this, [this](){ switchTo(m_doctor); });
    connect(m_welcome, &WelcomeView::gotoDept,     this, [this](){ switchTo(m_dept); });

    // 各模块 → 欢迎页
    connect(m_patient, &PatientView::back, this, [this](){ switchTo(m_welcome); });
    connect(m_doctor,  &DoctorView::back,  this, [this](){ switchTo(m_welcome); });
    connect(m_dept,    &DepartmentView::back, this, [this](){ switchTo(m_welcome); });
}

MasterView::~MasterView() { delete ui; }

void MasterView::setUser(const QString &u)
{
    m_user = u;
    ui->label_welcome->setText("当前用户：" + u);
    IDatabase::instance()->addUserLog(u, "进入主界面");
    switchTo(m_welcome);   // ← 关键：先显示欢迎页
}

void MasterView::switchTo(QWidget *w)
{
    ui->stackedWidget->setCurrentWidget(w);
}

} // namespace Hospital
