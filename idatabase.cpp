#include "idatabase.h"
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>

idatabase::idatabase() {
    m_connectionName = "SQLITE_CONN_" + QString::number((quintptr)this);
    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
}

idatabase::~idatabase() {
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool idatabase::connect(const QString& dbName) {
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + dbName;
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        QString errMsg = QString("数据库连接失败：%1\n路径：%2")
                             .arg(m_db.lastError().text())
                             .arg(dbPath);
        qCritical() << errMsg;
        QMessageBox::critical(nullptr, "数据库错误", errMsg);
        return false;
    }

    createTables();
    qInfo() << "数据库连接成功：" << dbPath;
    return true;
}

QVariantMap idatabase::fetchOne(const QString& sql, const QVariantList& params) {
    QVariantMap result;
    if (!m_db.isOpen()) {
        qWarning() << "数据库未连接，无法查询";
        return result;
    }

    QSqlQuery query = exec(sql, params);
    if (query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            result[record.fieldName(i)] = query.value(i);
        }
    }
    return result;
}

QSqlQuery idatabase::exec(const QString& sql, const QVariantList& params) {
    QSqlQuery query(m_db);
    if (!m_db.isOpen()) {
        qWarning() << "数据库未连接，无法执行SQL";
        return query;
    }

    if (!query.prepare(sql)) {
        QString errMsg = QString("SQL预处理失败：%1\nSQL: %2")
                             .arg(query.lastError().text())
                             .arg(sql);
        qCritical() << errMsg;
        QMessageBox::warning(nullptr, "SQL错误", errMsg);
        return query;
    }

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        // 修复 QVariantList::toString() 错误：手动拼接参数字符串
        QString paramsStr;
        for (const auto& p : params) {
            paramsStr += p.toString() + ", ";
        }
        if (!paramsStr.isEmpty()) paramsStr.chop(2); // 移除最后一个逗号和空格

        QString errMsg = QString("SQL执行失败：%1\nSQL: %2\n参数: %3")
                             .arg(query.lastError().text())
                             .arg(sql)
                             .arg(paramsStr);
        qCritical() << errMsg;
        QMessageBox::warning(nullptr, "SQL错误", errMsg);
    }

    return query;
}

bool idatabase::insert(const QString& table, const QVariantMap& data) {
    if (data.isEmpty() || !m_db.isOpen()) {
        qWarning() << "插入失败：数据为空或数据库未连接";
        return false;
    }

    QStringList fields;
    QStringList placeholders;
    QVariantList values;

    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        fields << it.key();
        placeholders << ":" + it.key();
        values << it.value();
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                      .arg(table)
                      .arg(fields.join(", "))
                      .arg(placeholders.join(", "));

    QSqlQuery query(m_db);
    if (!query.prepare(sql)) {
        qCritical() << "插入预处理失败：" << query.lastError().text();
        return false;
    }

    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    return query.exec();
}

bool idatabase::update(const QString& table, const QVariantMap& data, const QString& whereClause, const QVariantList& params) {
    if (data.isEmpty() || whereClause.isEmpty() || !m_db.isOpen()) {
        qWarning() << "更新失败：数据/条件为空或数据库未连接";
        return false;
    }

    QStringList setClauses;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        setClauses << QString("%1 = :%1").arg(it.key());
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE %3")
                      .arg(table)
                      .arg(setClauses.join(", "))
                      .arg(whereClause);

    QSqlQuery query(m_db);
    if (!query.prepare(sql)) {
        qCritical() << "更新预处理失败：" << query.lastError().text();
        return false;
    }

    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i + data.size(), params[i]);
    }

    return query.exec();
}

bool idatabase::remove(const QString& table, const QString& whereClause, const QVariantList& params) {
    if (whereClause.isEmpty() || !m_db.isOpen()) {
        qWarning() << "删除失败：条件为空或数据库未连接";
        return false;
    }

    QString sql = QString("DELETE FROM %1 WHERE %2").arg(table).arg(whereClause);

    QSqlQuery query(m_db);
    if (!query.prepare(sql)) {
        qCritical() << "删除预处理失败：" << query.lastError().text();
        return false;
    }

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    return query.exec();
}

void idatabase::createTables() {
    exec(R"(
        CREATE TABLE IF NOT EXISTS User (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            USERNAME TEXT UNIQUE NOT NULL,
            PASSWORD TEXT NOT NULL,
            FULLNAME TEXT NOT NULL,
            CREATE_TIME DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )");

    exec(R"(
        CREATE TABLE IF NOT EXISTS Patient (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            ID_CARD TEXT UNIQUE,
            NAME TEXT NOT NULL,
            SEX TEXT,
            DOB DATE,
            HEIGHT REAL,
            WEIGHT REAL,
            MOBILEPHONE TEXT,
            CREATE_TIME DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )");

    exec(R"(
        CREATE TABLE IF NOT EXISTS Department (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            NAME TEXT UNIQUE NOT NULL
        )
    )");

    exec(R"(
        CREATE TABLE IF NOT EXISTS Doctor (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            WORK_NO TEXT UNIQUE NOT NULL,
            NAME TEXT NOT NULL,
            DEPARTMENT_ID INTEGER,
            FOREIGN KEY(DEPARTMENT_ID) REFERENCES Department(ID)
        )
    )");

    if (fetchOne("SELECT * FROM User WHERE USERNAME = 'admin'").isEmpty()) {
        insert("User", {
                           {"USERNAME", "admin"},
                           {"PASSWORD", "123456"},
                           {"FULLNAME", "系统管理员"}
                       });
    }
}
