#include "sql_database.h"

sql_database::sql_database(QObject *parent)
{
    connectToDatabase("D:/mathmod/IT_modelling/planet_init_project/interplanetary_transport.db");
}

sql_database::sql_database(QString &path)
{
    connectToDatabase(path);
}

sql_database::~sql_database()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool sql_database::connectToDatabase(const QString &path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (!db.open()) {
           qWarning() << "Failed to open database:";
           return false;
       }

       qDebug() << "Connected to database.";
       qDebug() << "Available drivers:" << QSqlDatabase::drivers();
       qDebug()<<db.databaseName();
//       if (!db.tables().contains("Celestial_Bodies", Qt::CaseInsensitive)) {
//           qDebug() << "ERROR: Table doesn't exist! Available tables:" << db.tables();
//           return false;
//       }
       // Check which database you're actually connected to
       qDebug() << "Database connection info:";
       qDebug() << "- Driver:" << db.driverName();
       qDebug() << "- Database name:" << db.databaseName();
       qDebug() << "- Host:" << db.hostName();
       qDebug() << "- User:" << db.userName();

       // For SQLite - verify the physical file
       if (db.driverName() == "QSQLITE") {
           QFileInfo dbFile(db.databaseName());
           qDebug() << "SQLite file details:";
           qDebug() << "- Path:" << dbFile.absoluteFilePath();
           qDebug() << "- Exists:" << dbFile.exists();
           qDebug() << "- Size:" << dbFile.size() << "bytes";
           qDebug() << "- Last modified:" << dbFile.lastModified();

           // Try opening the same file with another tool
           qDebug() << "Try opening this file with SQLite browser to verify contents:";
           qDebug() << dbFile.absoluteFilePath();
       }

       QStringList tables = db.tables();

       for (const QString &tableName : tables) {
           qDebug() << "Table:" << tableName;
       }
       QSqlQuery query(db);
       if (query.exec("PRAGMA table_info(Locations)")) {
           while (query.next()) {
               QString columnName = query.value(1).toString();
               qDebug() << "Column:" << columnName;
           }
       } else {
           qDebug() << "Failed to get columns:" << query.lastError().text();
       }
       if (query.exec("SELECT COUNT(*) FROM Locations")) {
           if (query.next()) {
               int rowCount = query.value(0).toInt();
               qDebug() << "Row count in Locations:" << rowCount;
           }
       }
       if (query.exec("SELECT * FROM Locations")) {
               while (query.next()) {
                   QString rowOutput;
                   QSqlRecord record = query.record();
                   for (int i = 0; i < record.count(); ++i) {
                       QString columnName = record.fieldName(i);
                       QVariant value = query.value(i);
                       rowOutput += columnName + ": " + value.toString() + " | ";
                   }
                   qDebug() << rowOutput;
               }
           } else {
               qDebug() << "Query failed:" << query.lastError().text();
           }
       return true;

}

bool sql_database::isConnected() const
{
    return db.isOpen();
}

double sql_database::get_planet_mass(QString planet_name)
{

    QString query_str ="SELECT mass FROM Celestial_Bodies WHERE name =\""+planet_name+'\"';
    QSqlQuery query(query_str,db);
   if (!query.next()) {
          qDebug() << "No results found for:" << planet_name;
          qDebug() << "Available names in database:";

          QSqlQuery namesQuery("SELECT name FROM Celestial_Bodies", db);
          while (namesQuery.next()) {
              qDebug() << "-" << namesQuery.value(0).toString();
          }

          return 0.0;
      }
    return query.value("mass").toDouble();
}

QString sql_database::check_sql()
{
    QSqlQuery query("SELECT name FROM Celestial_Bodies");

    if (query.exec()) {
        while (query.next()) {
            qDebug() << "Table name:" << query.value(0).toString();
        }
    } else {
        qDebug() << "Error executing query:" ;
    }
    return " ";

}

QVector3D sql_database::get_init_position(QString planet_name)
{
    QVector3D pos = {0,0,0};
    QString query_str ="SELECT initial_position_x,initial_position_y,initial_position_z FROM Celestial_Bodies WHERE name =\""+planet_name+'\"';
    QSqlQuery query(query_str,db);
    if (!query.next()) {
           qDebug() << "No results found for:" << planet_name;
           qDebug() << "Available names in database:";

           QSqlQuery namesQuery("SELECT name FROM Celestial_Bodies", db);
           while (namesQuery.next()) {
               qDebug() << "-" << namesQuery.value(0).toString();
           }

           return pos;
       }
    pos[0]= query.value("initial_position_x").toDouble();
    pos[1] = query.value("initial_position_y").toDouble();
    pos[2] = query.value("initial_position_z").toDouble();
    return pos;
}

QSqlQuery sql_database::runQuery(QString &queryStr)
{
   QSqlQuery query(queryStr,db);

   return query;
}

