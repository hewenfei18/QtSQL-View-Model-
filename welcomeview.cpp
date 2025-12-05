#include "welcomeview.h"
#include "ui_welcomeview.h"

namespace Hospital {

WelcomeView::WelcomeView(QWidget *parent)
    : QWidget(parent), ui(new Ui::WelcomeView)
{
    ui->setupUi(this);
    connect(ui->pushButton_patient, &QPushButton::clicked, this, &WelcomeView::gotoPatient);
    connect(ui->pushButton_doctor,  &QPushButton::clicked, this, &WelcomeView::gotoDoctor);
    connect(ui->pushButton_department, &QPushButton::clicked, this, &WelcomeView::gotoDept);
}

WelcomeView::~WelcomeView() { delete ui; }

} // namespace Hospital
