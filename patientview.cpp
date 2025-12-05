#include "patientview.h"
#include "ui_patientview.h"
#include "patienteditview.h"
#include "idatabase.h"
#include <QMessageBox>

static void fillTable(QTableWidget *tw, QSqlQueryModel *m)
{
    tw->setRowCount(0);
    for (int i = 0; i < m->rowCount(); ++i) {
        tw->insertRow(i);
        for (int j = 0; j < m->columnCount(); ++j)
            tw->setItem(i, j, new QTableWidgetItem(m->data(m->index(i, j)).toString()));
    }
}

namespace Hospital {

PatientView::PatientView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PatientView)
{
    ui->setupUi(this);

    ui->tableWidget_patient->setColumnCount(8);
    ui->tableWidget_patient->setHorizontalHeaderLabels(
        {"患者ID", "身份证号", "姓名", "性别", "出生日期", "身高(m)", "体重(kg)", "手机号"});

    refresh();

    connect(ui->pushButton_search, &QPushButton::clicked, this, &PatientView::onSearch);
    connect(ui->pushButton_add,    &QPushButton::clicked, this, &PatientView::onAdd);
    connect(ui->pushButton_edit,   &QPushButton::clicked, this, &PatientView::onEdit);
    connect(ui->pushButton_delete, &QPushButton::clicked, this, &PatientView::onDelete);
    connect(ui->pushButton_back,   &QPushButton::clicked, this, &PatientView::onBack);
}

PatientView::~PatientView() { delete ui; }

void PatientView::refresh()
{
    auto *m = IDatabase::instance()->patientModel(ui->lineEdit_search->text());
    fillTable(ui->tableWidget_patient, m);
    delete m;
}

void PatientView::onSearch() { refresh(); }

void PatientView::onAdd()
{
    if (!m_edit) m_edit = new PatientEditView(this);
    if (m_edit->exec() == QDialog::Accepted) {
        IDatabase::instance()->addPatient(m_edit->data());
        IDatabase::instance()->addUserLog("admin", "新增患者");
        refresh();
    }
}

void PatientView::onEdit()
{
    int r = ui->tableWidget_patient->currentRow();
    if (r < 0) return;
    QString id = ui->tableWidget_patient->item(r, 0)->text();
    if (!m_edit) m_edit = new PatientEditView(this);
    m_edit->load(id);
    if (m_edit->exec() == QDialog::Accepted) {
        IDatabase::instance()->updatePatient(id, m_edit->data());
        IDatabase::instance()->addUserLog("admin", "修改患者");
        refresh();
    }
}

void PatientView::onDelete()
{
    int r = ui->tableWidget_patient->currentRow();
    if (r < 0) return;
    QString id = ui->tableWidget_patient->item(r, 0)->text();
    if (QMessageBox::question(this, "确认", "确定删除？") != QMessageBox::Yes) return;
    IDatabase::instance()->deletePatient(id);
        IDatabase::instance()->addUserLog("admin", "删除患者");
    refresh();
}

void PatientView::onBack() { emit back(); }   // 关键：发信号
} // namespace Hospital
