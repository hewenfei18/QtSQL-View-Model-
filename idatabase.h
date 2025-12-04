#ifndef IDATABASE_H
#define IDATABASE_H

#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class idatabase {
public:
    idatabase();
    ~idatabase();

    bool connect(const QString& dbName = "hospital.db");
    QVariantMap fetchOne(const QString& sql, const QVariantList& params = {});
    QSqlQuery exec(const QString& sql, const QVariantList& params = {});
    bool insert(const QString& table, const QVariantMap& data);
    bool update(const QString& table, const QVariantMap& data, const QString& whereClause, const QVariantList& params = {});
    bool remove(const QString& table, const QString& whereClause, const QVariantList& params = {});

private:
    void createTables();
    QSqlDatabase m_db;
    QString m_connectionName;
};

#endif // IDATABASE_H
