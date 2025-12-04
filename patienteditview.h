#ifndef PATIENTEDITVIEW_H
#define PATIENTEDITVIEW_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

class PatientEditView : public QDialog
{
    Q_OBJECT

public:
    explicit PatientEditView(QWidget *parent = nullptr);
    void clearData();
    QVariantMap getPatientData() const;
    void setPatientData(const QVariantMap& data);

public:
    QLineEdit *idEdit, *idCardEdit, *nameEdit, *phoneEdit;
    QComboBox *sexCombo;
    QDateEdit *dobEdit;
    QDoubleSpinBox *heightSpin, *weightSpin;

private slots:
    void onSaveClicked();
    void onCancelClicked();
};

#endif // PATIENTEDITVIEW_H
