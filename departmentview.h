#ifndef DEPARTMENTVIEW_H
#define DEPARTMENTVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>  // 修复 QHeaderView 不完整类型
#include "idatabase.h"

class DepartmentView : public QWidget
{
    Q_OBJECT

public:
    explicit DepartmentView(QWidget *parent = nullptr);
    void setDatabase(idatabase* db);

private slots:
    void onSearchClicked();
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void refreshTable(const QString& filter = "");

    QLineEdit *searchEdit;
    QTableWidget *tableWidget;
    QPushButton *searchBtn, *addBtn, *editBtn, *deleteBtn;
    idatabase* m_db;
};

#endif // DEPARTMENTVIEW_H
