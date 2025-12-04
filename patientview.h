#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>  // 修复 QHeaderView 不完整类型
#include "idatabase.h"
#include "patienteditview.h"

class PatientView : public QWidget
{
    Q_OBJECT

public:
    explicit PatientView(QWidget *parent = nullptr);
    void setDatabase(idatabase* db);

private slots:
    void onSearchClicked();
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void refreshTable(const QString& filter = "");

    // 修复成员初始化顺序：m_db 移到最前面
    idatabase* m_db;
    PatientEditView *editDialog;
    QLineEdit *searchEdit;
    QTableWidget *tableWidget;
    QPushButton *searchBtn, *addBtn, *editBtn, *deleteBtn;
};

#endif // PATIENTVIEW_H
