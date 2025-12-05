#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
namespace Hospital {

PatientEditView::PatientEditView(QWidget *parent)
    : QDialog(parent), ui(new Ui::PatientEditView)
{
    ui->setupUi(this);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &PatientEditView::onSave);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::reject);
}

PatientEditView::~PatientEditView() { delete ui; }

void PatientEditView::load(const QString &id)
{
    m_id = id;
    QSqlQuery q("SELECT * FROM patients WHERE id='" + id + "'");
    if (q.next()) {
        ui->lineEdit_id->setText(q.value("id").toString());
        ui->lineEdit_idCard->setText(q.value("idCard").toString());
        ui->lineEdit_name->setText(q.value("name").toString());
        ui->comboBox_sex->setCurrentText(q.value("sex").toString());
        ui->dateEdit_dob->setDate(QDate::fromString(q.value("dob").toString(), Qt::ISODate));
        ui->doubleSpinBox_height->setValue(q.value("height").toDouble());
        ui->doubleSpinBox_weight->setValue(q.value("weight").toDouble());
        ui->lineEdit_phone->setText(q.value("phone").toString());
    }
}

QVariantMap PatientEditView::data() const
{
    QVariantMap m;
    m["id"]      = ui->lineEdit_id->text();
    m["idCard"]  = ui->lineEdit_idCard->text();
    m["name"]    = ui->lineEdit_name->text();
    m["sex"]     = ui->comboBox_sex->currentText();
    m["dob"]     = ui->dateEdit_dob->date().toString(Qt::ISODate);
    m["height"]  = ui->doubleSpinBox_height->value();
    m["weight"]  = ui->doubleSpinBox_weight->value();
    m["phone"]   = ui->lineEdit_phone->text();
    return m;
}

void PatientEditView::onSave()
{
    if (ui->lineEdit_id->text().isEmpty() || ui->lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "ID 和姓名不能为空");
        return;
    }
    accept();
}

} // namespace Hospital
