#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class PatientView; }
QT_END_NAMESPACE

namespace Hospital {

class PatientEditView;

class PatientView : public QWidget
{
    Q_OBJECT
public:
    explicit PatientView(QWidget *parent = nullptr);
    ~PatientView();
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
    Ui::PatientView *ui;
    PatientEditView *m_edit = nullptr;
};

} // namespace Hospital
