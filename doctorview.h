#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DoctorView; }
QT_END_NAMESPACE

namespace Hospital {

class DoctorView : public QWidget
{
    Q_OBJECT
public:
    explicit DoctorView(QWidget *parent = nullptr);
    ~DoctorView();
signals:
    void back();   // 返回欢迎页
private slots:
    void onSearch();
    void onAdd();
    void onEdit();
    void onDelete();
    void onBack();
private:
    void refresh();
    Ui::DoctorView *ui;
};

} // namespace Hospital
