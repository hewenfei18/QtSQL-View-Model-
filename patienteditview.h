#pragma once
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class PatientEditView; }
QT_END_NAMESPACE

namespace Hospital {

class PatientEditView : public QDialog
{
    Q_OBJECT
public:
    explicit PatientEditView(QWidget *parent = nullptr);
    ~PatientEditView();
    void load(const QString &id);
    QVariantMap data() const;
private slots:
    void onSave();
private:
    Ui::PatientEditView *ui;
    QString m_id;
};

} // namespace Hospital
