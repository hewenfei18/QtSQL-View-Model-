#include "patientview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

PatientView::PatientView(QWidget *parent)
    : QWidget(parent), m_db(nullptr), editDialog(new PatientEditView(this))
{
    searchEdit = new QLineEdit;
    searchBtn = new QPushButton("查询");
    addBtn = new QPushButton("添加");
    editBtn = new QPushButton("修改");
    deleteBtn = new QPushButton("删除");
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(8);
    tableWidget->setHorizontalHeaderLabels({
        "患者ID", "身份证号", "姓名", "性别",
        "出生日期", "身高(m)", "体重(kg)", "手机号"
    });
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(new QLabel("搜索："));
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

    connect(searchBtn, &QPushButton::clicked, this, &PatientView::onSearchClicked);
    connect(addBtn, &QPushButton::clicked, this, &PatientView::onAddClicked);
    connect(editBtn, &QPushButton::clicked, this, &PatientView::onEditClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &PatientView::onDeleteClicked);
}

void PatientView::setDatabase(idatabase* db)
{
    m_db = db;
    refreshTable();
}

void PatientView::refreshTable(const QString& filter)
{
    if (!m_db) return;
    tableWidget->setRowCount(0);

    QString sql = "SELECT * FROM Patient";
    QVariantList params;
    if (!filter.isEmpty()) {
        sql += " WHERE NAME LIKE ? OR ID LIKE ?";
        params << "%" + filter + "%" << "%" + filter + "%";
    }

    QSqlQuery query = m_db->exec(sql, params);
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("ID_CARD").toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("NAME").toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("SEX").toString()));
        tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("DOB").toString()));
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(query.value("HEIGHT").toDouble(), 'f', 2)));
        tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(query.value("WEIGHT").toDouble(), 'f', 2)));
        tableWidget->setItem(row, 7, new QTableWidgetItem(query.value("MOBILEPHONE").toString()));
        row++;
    }
}

void PatientView::onSearchClicked()
{
    refreshTable(searchEdit->text().trimmed());
}

void PatientView::onAddClicked()
{
    if (!m_db) return;
    editDialog->clearData();
    if (editDialog->exec() == QDialog::Accepted) {
        QVariantMap data = editDialog->getPatientData();
        if (m_db->insert("Patient", data)) {
            QMessageBox::information(this, "成功", "患者添加成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "患者ID或身份证号已存在");
        }
    }
}

void PatientView::onEditClicked()
{
    if (!m_db) return;
    QTableWidgetItem *item = tableWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请选择要修改的患者");
        return;
    }
    int row = item->row();
    QVariantMap data;
    data["ID"] = tableWidget->item(row, 0)->text();
    data["ID_CARD"] = tableWidget->item(row, 1)->text();
    data["NAME"] = tableWidget->item(row, 2)->text();
    data["SEX"] = tableWidget->item(row, 3)->text();
    data["DOB"] = tableWidget->item(row, 4)->text();
    data["HEIGHT"] = tableWidget->item(row, 5)->text().toDouble();
    data["WEIGHT"] = tableWidget->item(row, 6)->text().toDouble();
    data["MOBILEPHONE"] = tableWidget->item(row, 7)->text();

    editDialog->setPatientData(data);
    if (editDialog->exec() == QDialog::Accepted) {
        QVariantMap newData = editDialog->getPatientData();
        if (m_db->update("Patient", newData, "ID = ?", {data["ID"]})) {
            QMessageBox::information(this, "成功", "患者修改成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "身份证号已存在");
        }
    }
}

void PatientView::onDeleteClicked()
{
    if (!m_db) return;
    QTableWidgetItem *item = tableWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请选择要删除的患者");
        return;
    }
    int id = tableWidget->item(item->row(), 0)->text().toInt();
    if (QMessageBox::question(this, "确认", "确定删除该患者？") == QMessageBox::Yes) {
        if (m_db->remove("Patient", "ID = ?", {id})) {
            QMessageBox::information(this, "成功", "患者删除成功");
            refreshTable();
        } else {
            QMessageBox::warning(this, "失败", "删除失败");
        }
    }
}
