#include "idatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

namespace Hospital {

static IDatabase* _ins = nullptr;
IDatabase* IDatabase::instance()
{
    if (!_ins) _ins = new IDatabase;
    return _ins;
}

IDatabase::IDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("Qt_Lab3a.db");
    if (!m_db.open()) qFatal("DB open fail");
    initDb();
}
bool IDatabase::initDb()
{
    QSqlQuery q;

    // 用户、患者、医生、日志（保持不变）
    q.exec("CREATE TABLE IF NOT EXISTS users(username TEXT PRIMARY KEY, password TEXT)");
    q.exec("INSERT OR IGNORE INTO users VALUES('admin','123456')");

    q.exec("CREATE TABLE IF NOT EXISTS patients("
           "id TEXT PRIMARY KEY, idCard TEXT, name TEXT, sex TEXT, "
           "dob TEXT, height REAL, weight REAL, phone TEXT)");

    q.exec("CREATE TABLE IF NOT EXISTS doctors("
           "id TEXT PRIMARY KEY, workNo TEXT, name TEXT, dept TEXT)");

    q.exec("CREATE TABLE IF NOT EXISTS user_logs("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "username TEXT, action TEXT, ts TEXT)");

    // ===== 修复：智能升级 departments 表 =====
    // 检查 departments 表是否存在，以及是否有 id 列
    bool hasIdColumn = false;
    q.exec("PRAGMA table_info(departments);");
    while (q.next()) {
        if (q.value("name").toString() == "id") {
            hasIdColumn = true;
            break;
        }
    }

    if (!hasIdColumn) {
        // 表存在但缺少 id 列，需要升级
        qDebug() << "Upgrading departments table to include 'id' column...";

        // 创建新表
        q.exec("CREATE TABLE IF NOT EXISTS departments_new("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT UNIQUE NOT NULL)");

        // 迁移旧数据（不管 id 是否为 NULL，全部迁移 name）
        q.exec("INSERT INTO departments_new(name) "
               "SELECT DISTINCT name FROM departments WHERE name IS NOT NULL AND name != '';");

        // 删除旧表并重命名
        q.exec("DROP TABLE IF EXISTS departments;");
        q.exec("ALTER TABLE departments_new RENAME TO departments;");
    } else {
        // 表已经是新结构，什么都不做
        qDebug() << "departments table already has 'id' column. Skip upgrade.";
    }

    return true;
}
bool IDatabase::validateUser(const QString &u, const QString &p)
{
    QSqlQuery q;
    q.prepare("SELECT 1 FROM users WHERE username=? AND password=?");
    q.addBindValue(u); q.addBindValue(p);
    return q.exec() && q.next();
}
bool IDatabase::addUserLog(const QString &u, const QString &a)
{
    QSqlQuery q;
    q.prepare("INSERT INTO user_logs(username,action,ts) VALUES(?,?,?)");
    q.addBindValue(u); q.addBindValue(a);
    q.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    return q.exec();
}

QSqlQueryModel* IDatabase::patientModel(const QString &f)
{
    auto *m = new QSqlQueryModel;
    QString s = "SELECT id, idCard, name, sex, dob, height, weight, phone FROM patients";
    if (!f.isEmpty()) s += " WHERE name LIKE '%" + f + "%'";
    m->setQuery(s); return m;
}
bool IDatabase::addPatient(const QVariantMap &d)
{
    QSqlQuery q;
    q.prepare("INSERT INTO patients(id,idCard,name,sex,dob,height,weight,phone) "
              "VALUES(?,?,?,?,?,?,?,?)");
    q.addBindValue(d["id"]); q.addBindValue(d["idCard"]);
    q.addBindValue(d["name"]); q.addBindValue(d["sex"]);
    q.addBindValue(d["dob"]); q.addBindValue(d["height"]);
    q.addBindValue(d["weight"]); q.addBindValue(d["phone"]);
    return q.exec();
}
bool IDatabase::updatePatient(const QString &id, const QVariantMap &d)
{
    QSqlQuery q;
    q.prepare("UPDATE patients SET idCard=?,name=?,sex=?,dob=?,height=?,weight=?,phone=? "
              "WHERE id=?");
    q.addBindValue(d["idCard"]); q.addBindValue(d["name"]);
    q.addBindValue(d["sex"]); q.addBindValue(d["dob"]);
    q.addBindValue(d["height"]); q.addBindValue(d["weight"]);
    q.addBindValue(d["phone"]); q.addBindValue(id);
    return q.exec();
}
bool IDatabase::deletePatient(const QString &id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM patients WHERE id=?");
    q.addBindValue(id); return q.exec();
}

QSqlQueryModel* IDatabase::doctorModel(const QString &f)
{
    auto *m = new QSqlQueryModel;
    QString s = "SELECT id,workNo,name,dept FROM doctors";
    if (!f.isEmpty()) s += " WHERE name LIKE '%" + f + "%'";
    m->setQuery(s); return m;
}
bool IDatabase::addDoctor(const QVariantMap &d)
{
    QSqlQuery q;
    q.prepare("INSERT INTO doctors(id,workNo,name,dept) VALUES(?,?,?,?)");
    q.addBindValue(d["id"]); q.addBindValue(d["workNo"]);
    q.addBindValue(d["name"]); q.addBindValue(d["dept"]);
    return q.exec();
}
bool IDatabase::updateDoctor(const QString &id, const QVariantMap &d)
{
    QSqlQuery q;
    q.prepare("UPDATE doctors SET workNo=?,name=?,dept=? WHERE id=?");
    q.addBindValue(d["workNo"]); q.addBindValue(d["name"]);
    q.addBindValue(d["dept"]); q.addBindValue(id);
    return q.exec();
}
bool IDatabase::deleteDoctor(const QString &id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM doctors WHERE id=?");
    q.addBindValue(id); return q.exec();
}

// ===== 科室：带 ID =====
QSqlQueryModel* IDatabase::departmentModel(const QString &f)
{
    auto *m = new QSqlQueryModel;
    QString s = "SELECT id, name FROM departments";
    if (!f.isEmpty()) s += " WHERE name LIKE '%" + f + "%'";
    m->setQuery(s); return m;
}
bool IDatabase::addDepartment(const QString &name)
{
    QSqlQuery q;
    q.prepare("INSERT INTO departments(name) VALUES(?)");
    q.addBindValue(name); return q.exec();
}
bool IDatabase::updateDepartment(int id, const QString &newName)
{
    QSqlQuery q;
    q.prepare("UPDATE departments SET name=? WHERE id=?");
    q.addBindValue(newName); q.addBindValue(id);
    return q.exec();
}
bool IDatabase::deleteDepartment(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM departments WHERE id=?");
    q.addBindValue(id); return q.exec();
}

} // namespace Hospital
