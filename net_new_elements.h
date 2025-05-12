#ifndef NET_NEW_ELEMENTS_H
#define NET_NEW_ELEMENTS_H
#include <QObject>
#include "sql_database.h"
#include <QVector>
#include <QVector3D>
#include <QQuaternion>



class net_new_elements:public QObject
{
    Q_OBJECT
    Q_PROPERTY(sql_database* objects_database READ objects_database WRITE setObjects_database NOTIFY objects_databaseChanged)
    Q_PROPERTY(QVector<QVector<double>> latitudes READ latitudes WRITE setLatitudes NOTIFY latitudesChanged)
    Q_PROPERTY(QVector<QVector<double>> longitudes READ longitudes WRITE setLongitudes NOTIFY longitudesChanged)
    Q_PROPERTY(QString object_name READ object_name WRITE setObjet_name NOTIFY object_nameChanged)
    Q_PROPERTY(QVector<QString> area_ids READ area_ids WRITE setArea_ids NOTIFY area_idsChanged)


public:
    net_new_elements();
    sql_database *objects_database() const;
    void setObjects_database(sql_database *newObjects_database);

    const QVector<QVector<double> > &latitudes() const;
    void setLatitudes(const QVector<QVector<double> > &newLatitudes);


    const QVector<QVector<double> > &longitudes() const;
    void setLongitudes(const QVector<QVector<double> > &newLongitudes);

    const QString &object_name() const;
    void setObjet_name(const QString &newObject_name);

    const QVector<QString> &area_ids() const;
    void setArea_ids(const QVector<QString> &newArea_ids);


    Q_INVOKABLE QString check_area(QVector3D position_object,QVector3D globe_position,QQuaternion rotation);

    Q_INVOKABLE void get_area_data(QString object_name);

    Q_INVOKABLE QString get_area_description(QString area_id);

    Q_INVOKABLE QString get_area_elements(QString area_id);

    Q_INVOKABLE QString get_area_minerals(QString area_id);



signals:
    void objects_databaseChanged();
    void latitudesChanged();

    void longitudesChanged();

    void object_nameChanged();

    void area_idsChanged();

private:
    sql_database *m_objects_database = nullptr;
    QVector<QVector<double> > m_latitudes;
    QVector<QVector<double> > m_longitudes;
    QString m_object_name;
    QVector<QString> m_area_ids;
};

#endif // NET_NEW_ELEMENTS_H
