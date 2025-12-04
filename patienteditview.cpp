#include "patienteditview.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>

PatientEditView::PatientEditView(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("编辑患者信息");
    setFixedSize(400, 400);

    idEdit = new QLineEdit;
    idCardEdit = new QLineEdit;
    nameEdit = new QLineEdit;
    sexCombo = new QComboBox;
    sexCombo->addItems({"男", "女"});
    dobEdit = new QDateEdit(QDate::currentDate());
    dobEdit->setCalendarPopup(true);
    heightSpin = new QDoubleSpinBox;
    heightSpin->setRange(0.5, 3.0);
    heightSpin->setValue(1.70);
    weightSpin = new QDoubleSpinBox;
    weightSpin->setRange(1.0, 500.0);
    weightSpin->setValue(60.0);
    phoneEdit = new QLineEdit;

    QPushButton *saveBtn = new QPushButton("保存");
    QPushButton *cancelBtn = new QPushButton("取消");

    QFormLayout *form = new QFormLayout;
    form->addRow("患者ID：", idEdit);
    form->addRow("身份证号：", idCardEdit);
    form->addRow("姓名：", nameEdit);
    form->addRow("性别：", sexCombo);
    form->addRow("出生日期：", dobEdit);
    form->addRow("身高(m)：", heightSpin);
    form->addRow("体重(kg)：", weightSpin);
    form->addRow("手机号：", phoneEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    form->addRow(btnLayout);

    setLayout(form);

    connect(saveBtn, &QPushButton::clicked, this, &PatientEditView::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &PatientEditView::onCancelClicked);
}

void PatientEditView::clearData()
{
    idEdit->clear();
    idCardEdit->clear();
    nameEdit->clear();
    sexCombo->setCurrentIndex(0);
    dobEdit->setDate(QDate::currentDate());
    heightSpin->setValue(1.70);
    weightSpin->setValue(60.0);
    phoneEdit->clear();
}

QVariantMap PatientEditView::getPatientData() const
{
    return {
        {"ID", idEdit->text().trimmed()},
        {"ID_CARD", idCardEdit->text().trimmed()},
        {"NAME", nameEdit->text().trimmed()},
        {"SEX", sexCombo->currentText()},
        {"DOB", dobEdit->date().toString("yyyy-MM-dd")},
        {"HEIGHT", heightSpin->value()},
        {"WEIGHT", weightSpin->value()},
        {"MOBILEPHONE", phoneEdit->text().trimmed()}
    };
}

void PatientEditView::setPatientData(const QVariantMap& data)
{
    idEdit->setText(data["ID"].toString());
    idCardEdit->setText(data["ID_CARD"].toString());
    nameEdit->setText(data["NAME"].toString());
    sexCombo->setCurrentText(data["SEX"].toString());
    dobEdit->setDate(QDate::fromString(data["DOB"].toString(), "yyyy-MM-dd"));
    heightSpin->setValue(data["HEIGHT"].toDouble());
    weightSpin->setValue(data["WEIGHT"].toDouble());
    phoneEdit->setText(data["MOBILEPHONE"].toString());
}

void PatientEditView::onSaveClicked()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "提示", "姓名不能为空");
        return;
    }
    accept();
}

void PatientEditView::onCancelClicked()
{
    reject();
}
