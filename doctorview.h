#ifndef DOCTORVIEW_H
#define DOCTORVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHeaderView>  // 修复 QHeaderView 不完整类型
#include "idatabase.h"

class DoctorView : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorView(QWidget *parent = nullptr);
    void setDatabase(idatabase* db);

private slots:
    void onSearchClicked();
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void refreshTable(const QString& filter = "");
    void loadDepartments();

    QLineEdit *searchEdit;
    QTableWidget *tableWidget;
    QPushButton *searchBtn, *addBtn, *editBtn, *deleteBtn;
    QComboBox *deptCombo;
    idatabase* m_db;
};

#endif // DOCTORVIEW_H
