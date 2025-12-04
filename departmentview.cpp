#include "departmentview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>

DepartmentView::DepartmentView(QWidget *parent)
    : QWidget(parent), m_db(nullptr)
{
    searchEdit = new QLineEdit;
    searchBtn = new QPushButton("搜索");
    addBtn = new QPushButton("添加科室");
    editBtn = new QPushButton("修改科室");
    deleteBtn = new QPushButton("删除科室");
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels({"科室ID", "科室名称"});
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("科室名称："));
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

    connect(searchBtn, &QPushButton::clicked, this, &DepartmentView::onSearchClicked);
    connect(addBtn, &QPushButton::clicked, this, &DepartmentView::onAddClicked);
    connect(editBtn, &QPushButton::clicked, this, &DepartmentView::onEditClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &DepartmentView::onDeleteClicked);
}

void DepartmentView::setDatabase(idatabase* db)
{
    m_db = db;
    refreshTable();
}

void DepartmentView::refreshTable(const QString& filter)
{
    if (!m_db) return;
    tableWidget->setRowCount(0);

    QString sql = "SELECT * FROM Department";
    QVariantList params;
    if (!filter.isEmpty()) {
        sql += " WHERE NAME LIKE ?";
        params << "%" + filter + "%";
    }

    QSqlQuery query = m_db->exec(sql, params);
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("NAME").toString()));
        row++;
    }
}

void DepartmentView::onSearchClicked()
{
    refreshTable(searchEdit->text().trimmed());
}

void DepartmentView::onAddClicked()
{
    if (!m_db) return;
    bool ok;
    QString name = QInputDialog::getText(this, "添加科室", "科室名称：", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        if (m_db->insert("Department", {{"NAME", name}})) {
            QMessageBox::information(this, "成功", "科室添加成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "科室名称已存在");
        }
    }
}

void DepartmentView::onEditClicked()
{
    if (!m_db) return;
    QTableWidgetItem *item = tableWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请选择要修改的科室");
        return;
    }
    int id = tableWidget->item(item->row(), 0)->text().toInt();
    QString oldName = tableWidget->item(item->row(), 1)->text();
    bool ok;
    QString newName = QInputDialog::getText(this, "修改科室", "科室名称：", QLineEdit::Normal, oldName, &ok);
    if (ok && !newName.isEmpty() && newName != oldName) {
        if (m_db->update("Department", {{"NAME", newName}}, "ID = ?", {id})) {
            QMessageBox::information(this, "成功", "科室修改成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "科室名称已存在");
        }
    }
}

void DepartmentView::onDeleteClicked()
{
    if (!m_db) return;
    QTableWidgetItem *item = tableWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请选择要删除的科室");
        return;
    }
    int id = tableWidget->item(item->row(), 0)->text().toInt();
    if (QMessageBox::question(this, "确认", "确定删除该科室？") == QMessageBox::Yes) {
        if (m_db->remove("Department", "ID = ?", {id})) {
            QMessageBox::information(this, "成功", "科室删除成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "该科室已关联医生，无法删除");
        }
    }
}
