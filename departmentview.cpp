#include "departmentview.h"
#include "ui_departmentview.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>        // 调试
#include <QSqlError>     // 取错误
#include <QSqlQuery>     // 立刻再查库
#include <QFileInfo>     // 绝对路径
#include <QDir>          // 路径格式化

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

DepartmentView::DepartmentView(QWidget *parent)
    : QWidget(parent), ui(new Ui::DepartmentView)
{
    ui->setupUi(this);
    refresh();
    connect(ui->pushButton_search, &QPushButton::clicked, this, &DepartmentView::onSearch);
    connect(ui->pushButton_add,    &QPushButton::clicked, this, &DepartmentView::onAdd);
    connect(ui->pushButton_edit,   &QPushButton::clicked, this, &DepartmentView::onEdit);
    connect(ui->pushButton_delete, &QPushButton::clicked, this, &DepartmentView::onDelete);
    connect(ui->pushButton_back,   &QPushButton::clicked, this, &DepartmentView::onBack);
}

DepartmentView::~DepartmentView() { delete ui; }

void DepartmentView::refresh()
{
    auto *m = IDatabase::instance()->departmentModel(ui->lineEdit_search->text());

    // ===== 1. 打印实际数据库绝对路径 =====
    qDebug() << "【当前 db 绝对路径】"
             << QDir::toNativeSeparators(QFileInfo("Qt_Lab3a.db").absoluteFilePath());

    // ===== 2. 立刻再查一次库内真实数据 =====
    QSqlQuery q("SELECT id, name FROM departments ORDER BY id");
    qDebug() << "【库内实时数据】";
    while (q.next())
        qDebug() << q.value(0).toInt() << q.value(1).toString();
    qDebug() << "【库内总行数】" << q.size();

    if (m->lastError().isValid())
        qDebug() << "【model 错误】" << m->lastError().text();

    fillTable(ui->tableWidget_department, m);
    delete m;
}

void DepartmentView::onSearch() { refresh(); }

void DepartmentView::onAdd()
{
    QString name = QInputDialog::getText(this, "添加", "科室名称");
    if (name.isEmpty()) return;

    if (IDatabase::instance()->addDepartment(name)) {
        qDebug() << "【新增科室成功】";
        IDatabase::instance()->addUserLog("admin", "新增科室");
        refresh();
    } else {
        qDebug() << "【新增失败】" << QSqlDatabase::database().lastError().text();
    }
}

void DepartmentView::onEdit()
{
    int r = ui->tableWidget_department->currentRow();
    if (r < 0) return;
    int   id   = ui->tableWidget_department->item(r, 0)->text().toInt();
    QString oldName = ui->tableWidget_department->item(r, 1)->text();
    QString newName = QInputDialog::getText(this, "修改", "新名称", QLineEdit::Normal, oldName);
    if (newName.isEmpty() || oldName == newName) return;

    if (IDatabase::instance()->updateDepartment(id, newName)) {
        qDebug() << "【修改科室成功】";
        IDatabase::instance()->addUserLog("admin", "修改科室");
        refresh();
    } else {
        qDebug() << "【修改失败】" << QSqlDatabase::database().lastError().text();
    }
}

void DepartmentView::onDelete()
{
    int r = ui->tableWidget_department->currentRow();
    if (r < 0) return;
    int id = ui->tableWidget_department->item(r, 0)->text().toInt();
    if (QMessageBox::question(this, "确认", "确定删除？") != QMessageBox::Yes) return;

    if (IDatabase::instance()->deleteDepartment(id)) {
        qDebug() << "【删除科室成功】";
        IDatabase::instance()->addUserLog("admin", "删除科室");
        refresh();
    } else {
        qDebug() << "【删除失败】" << QSqlDatabase::database().lastError().text();
    }
}

void DepartmentView::onBack() { emit back(); }

} // namespace Hospital
