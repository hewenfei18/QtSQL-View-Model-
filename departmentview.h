#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DepartmentView; }
QT_END_NAMESPACE

namespace Hospital {

class DepartmentView : public QWidget
{
    Q_OBJECT
public:
    explicit DepartmentView(QWidget *parent = nullptr);
    ~DepartmentView();
signals:
    void back();
private slots:
    void onSearch();
    void onAdd();
    void onEdit();
    void onDelete();
    void onBack();
private:
    void refresh();
    Ui::DepartmentView *ui;
};

} // namespace Hospital
