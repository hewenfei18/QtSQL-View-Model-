#include "doctorview.h"
#include "ui_doctorview.h"
#include "idatabase.h"
#include <QInputDialog>
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

DoctorView::DoctorView(QWidget *parent)
    : QWidget(parent), ui(new Ui::DoctorView)
{
    ui->setupUi(this);

    ui->tableWidget_doctor->setColumnCount(4);
    ui->tableWidget_doctor->setHorizontalHeaderLabels(
        {"医生ID", "工号", "姓名", "所属科室"});

    refresh();

    connect(ui->pushButton_search, &QPushButton::clicked, this, &DoctorView::onSearch);
    connect(ui->pushButton_add,    &QPushButton::clicked, this, &DoctorView::onAdd);
    connect(ui->pushButton_edit,   &QPushButton::clicked, this, &DoctorView::onEdit);
    connect(ui->pushButton_delete, &QPushButton::clicked, this, &DoctorView::onDelete);
    connect(ui->pushButton_back,   &QPushButton::clicked, this, &DoctorView::onBack);
}

DoctorView::~DoctorView() { delete ui; }

void DoctorView::refresh()
{
    auto *m = IDatabase::instance()->doctorModel(ui->lineEdit_search->text());
    fillTable(ui->tableWidget_doctor, m);
    delete m;
}

void DoctorView::onSearch() { refresh(); }

void DoctorView::onAdd()
{
    QString id     = QInputDialog::getText(this, "添加", "医生ID");
    QString workNo = QInputDialog::getText(this, "添加", "工号");
    QString name   = QInputDialog::getText(this, "添加", "姓名");
    QString dept   = QInputDialog::getText(this, "添加", "科室");
    if (id.isEmpty() || name.isEmpty()) return;
    QVariantMap d;
    d["id"] = id; d["workNo"] = workNo; d["name"] = name; d["dept"] = dept;
    if (IDatabase::instance()->addDoctor(d)) {
        IDatabase::instance()->addUserLog("admin", "新增医生");
        refresh();
    }
}

void DoctorView::onEdit()
{
    int r = ui->tableWidget_doctor->currentRow();
    if (r < 0) return;
    QString id     = ui->tableWidget_doctor->item(r, 0)->text();
    QString workNo = QInputDialog::getText(this, "修改", "新工号", QLineEdit::Normal,
                                          ui->tableWidget_doctor->item(r, 1)->text());
    QString name   = QInputDialog::getText(this, "修改", "新姓名", QLineEdit::Normal,
                                          ui->tableWidget_doctor->item(r, 2)->text());
    QString dept   = QInputDialog::getText(this, "修改", "新科室", QLineEdit::Normal,
                                          ui->tableWidget_doctor->item(r, 3)->text());
    QVariantMap d;
    d["workNo"] = workNo; d["name"] = name; d["dept"] = dept;
    if (IDatabase::instance()->updateDoctor(id, d)) {
        IDatabase::instance()->addUserLog("admin", "修改医生");
        refresh();
    }
}

void DoctorView::onDelete()
{
    int r = ui->tableWidget_doctor->currentRow();
    if (r < 0) return;
    QString id = ui->tableWidget_doctor->item(r, 0)->text();
    if (QMessageBox::question(this, "确认", "确定删除？") != QMessageBox::Yes) return;
    IDatabase::instance()->deleteDoctor(id);
    IDatabase::instance()->addUserLog("admin", "删除医生");
    refresh();
}

void DoctorView::onBack() { emit back(); }   // 关键：发信号
} // namespace Hospital
