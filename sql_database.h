#ifndef SQL_DATABASE_H
#define SQL_DATABASE_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QVector3D>

class sql_database:public QObject
{
    Q_OBJECT
public:
    explicit sql_database(QObject *parent = nullptr);
    sql_database(QString &path);

   ~sql_database();

   Q_INVOKABLE bool connectToDatabase(const QString &path);
   Q_INVOKABLE bool isConnected() const;
   Q_INVOKABLE double get_planet_mass(QString planet_name);
   Q_INVOKABLE QString check_sql();
   Q_INVOKABLE QVector3D get_init_position(QString planet_name);
   QSqlQuery runQuery(QString &queryStr);
   void printCelestialBodies();
private:
   QSqlDatabase db;
};

#endif // SQL_DATABASE_H
