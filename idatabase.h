#pragma once
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QVariantMap>

namespace Hospital {

class IDatabase
{
public:
    static IDatabase* instance();
    QSqlDatabase db() const { return m_db; }

    bool validateUser(const QString &u, const QString &p);
    bool addUserLog(const QString &u, const QString &a);

    QSqlQueryModel* patientModel(const QString &f = {});
    bool addPatient(const QVariantMap &d);
    bool updatePatient(const QString &id, const QVariantMap &d);
    bool deletePatient(const QString &id);

    QSqlQueryModel* doctorModel(const QString &f = {});
    bool addDoctor(const QVariantMap &d);
    bool updateDoctor(const QString &id, const QVariantMap &d);
    bool deleteDoctor(const QString &id);

    // 科室：带ID
    QSqlQueryModel* departmentModel(const QString &f = {});
    bool addDepartment(const QString &name);
    bool updateDepartment(int id, const QString &newName);
    bool deleteDepartment(int id);

private:
    IDatabase();
    bool initDb();          // 仅加列，不删表
    QSqlDatabase m_db;
};

} // namespace Hospital
