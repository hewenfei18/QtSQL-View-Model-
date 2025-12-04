#include "doctorview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialog>

DoctorView::DoctorView(QWidget *parent)
    : QWidget(parent), m_db(nullptr)
{
    searchEdit = new QLineEdit;
    searchBtn = new QPushButton("搜索");
    addBtn = new QPushButton("添加医生");
    editBtn = new QPushButton("修改医生");
    deleteBtn = new QPushButton("删除医生");
    deptCombo = new QComboBox;
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"医生ID", "工号", "姓名", "所属科室"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("医生姓名："));
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchBtn);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(btnLayout);

    connect(searchBtn, &QPushButton::clicked, this, &DoctorView::onSearchClicked);
    connect(addBtn, &QPushButton::clicked, this, &DoctorView::onAddClicked);
    connect(editBtn, &QPushButton::clicked, this, &DoctorView::onEditClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &DoctorView::onDeleteClicked);
}

void DoctorView::setDatabase(idatabase* db)
{
    m_db = db;
    loadDepartments();
    refreshTable();
}

void DoctorView::loadDepartments()
{
    if (!m_db) return;
    deptCombo->clear();
    QSqlQuery query = m_db->exec("SELECT ID, NAME FROM Department");
    while (query.next()) {
        deptCombo->addItem(query.value("NAME").toString(), query.value("ID"));
    }
}

void DoctorView::refreshTable(const QString& filter)
{
    if (!m_db) return;
    tableWidget->setRowCount(0);

    QString sql = R"(
        SELECT d.ID, d.WORK_NO, d.NAME, dept.NAME
        FROM Doctor d LEFT JOIN Department dept ON d.DEPARTMENT_ID = dept.ID
    )";
    QVariantList params;
    if (!filter.isEmpty()) {
        sql += " WHERE d.NAME LIKE ?";
        params << "%" + filter + "%";
    }

    QSqlQuery query = m_db->exec(sql, params);
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        row++;
    }
}

void DoctorView::onSearchClicked()
{
    refreshTable(searchEdit->text().trimmed());
}

void DoctorView::onAddClicked()
{
    if (!m_db || deptCombo->count() == 0) return;
    QDialog dlg(this);
    dlg.setWindowTitle("添加医生");
    QLineEdit *workNoEdit = new QLineEdit;
    QLineEdit *nameEdit = new QLineEdit;
    QComboBox *deptComboDlg = new QComboBox;
    loadDepartments();
    for (int i = 0; i < deptCombo->count(); i++) {
        deptComboDlg->addItem(deptCombo->itemText(i), deptCombo->itemData(i));
    }

    QFormLayout *form = new QFormLayout(&dlg);
    form->addRow("工号：", workNoEdit);
    form->addRow("姓名：", nameEdit);
    form->addRow("所属科室：", deptComboDlg);
    QPushButton *okBtn = new QPushButton("确定");
    QPushButton *cancelBtn = new QPushButton("取消");
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    form->addRow(btnLayout);

    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString workNo = workNoEdit->text().trimmed();
        QString name = nameEdit->text().trimmed();
        if (workNo.isEmpty() || name.isEmpty()) {
            QMessageBox::warning(this, "提示", "工号和姓名不能为空");
            return;
        }
        if (m_db->insert("Doctor", {
                                       {"WORK_NO", workNo},
                                       {"NAME", name},
                                       {"DEPARTMENT_ID", deptComboDlg->currentData()}
                                   })) {
            QMessageBox::information(this, "成功", "医生添加成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "工号已存在");
        }
    }
}

void DoctorView::onEditClicked()
{
    if (!m_db) return;
    QTableWidgetItem *item = tableWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请选择要修改的医生");
        return;
    }
    int id = tableWidget->item(item->row(), 0)->text().toInt();
    QString oldWorkNo = tableWidget->item(item->row(), 1)->text();
    QString oldName = tableWidget->item(item->row(), 2)->text();

    QDialog dlg(this);
    dlg.setWindowTitle("修改医生");
    QLineEdit *workNoEdit = new QLineEdit(oldWorkNo);
    QLineEdit *nameEdit = new QLineEdit(oldName);
    QComboBox *deptComboDlg = new QComboBox;
    loadDepartments();
    for (int i = 0; i < deptCombo->count(); i++) {
        deptComboDlg->addItem(deptCombo->itemText(i), deptCombo->itemData(i));
    }

    QFormLayout *form = new QFormLayout(&dlg);
    form->addRow("工号：", workNoEdit);
    form->addRow("姓名：", nameEdit);
    form->addRow("所属科室：", deptComboDlg);
    QPushButton *okBtn = new QPushButton("确定");
    QPushButton *cancelBtn = new QPushButton("取消");
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    form->addRow(btnLayout);

    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString workNo = workNoEdit->text().trimmed();
        QString name = nameEdit->text().trimmed();
        if (workNo.isEmpty() || name.isEmpty()) {
            QMessageBox::warning(this, "提示", "工号和姓名不能为空");
            return;
        }
        if (m_db->update("Doctor", {
                                       {"WORK_NO", workNo},
                                       {"NAME", name},
                                       {"DEPARTMENT_ID", deptComboDlg->currentData()}
                                   }, "ID = ?", {id})) {
            QMessageBox::information(this, "成功", "医生修改成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "工号已存在");
        }
    }
}

void DoctorView::onDeleteClicked()
{
    if (!m_db) return;
    QTableWidgetItem *item = tableWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请选择要删除的医生");
        return;
    }
    int id = tableWidget->item(item->row(), 0)->text().toInt();
    if (QMessageBox::question(this, "确认", "确定删除该医生？") == QMessageBox::Yes) {
        if (m_db->remove("Doctor", "ID = ?", {id})) {
            QMessageBox::information(this, "成功", "医生删除成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "删除失败");
        }
    }
}
