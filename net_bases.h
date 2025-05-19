#ifndef NET_BASES_H
#define NET_BASES_H
#include <QObject>
#include "sql_database.h"
#include <QVector>
#include <QVector3D>
#include <QQuaternion>

class net_bases:public QObject
{
    Q_OBJECT
    Q_PROPERTY(sql_database* objects_database READ objects_database WRITE setObjects_database NOTIFY objects_databaseChanged)
    Q_PROPERTY(QString object_name READ object_name WRITE setObjet_name NOTIFY object_nameChanged)
    Q_PROPERTY(QVector<QVector3D> obj_coords READ obj_coords WRITE setObj_coords NOTIFY obj_coordsChanged)
    Q_PROPERTY(QVector<QVector<double>> lat_lon READ lat_lon WRITE setlat_lon NOTIFY lat_lonChanged)
    Q_PROPERTY(QVector<QString> objects_info READ objects_info WRITE setobjects_info NOTIFY objects_infoChanged)
    Q_PROPERTY(QVector<QString> location_ids READ location_ids WRITE setLocation_ids NOTIFY location_idsChanged)
public:
    net_bases(){};
    sql_database *objects_database() const;
    void setObjects_database(sql_database *newObjects_database);
    const QString &object_name() const;
    void setObjet_name(const QString &newObject_name);
    Q_INVOKABLE void get_location_data();
    QVector<double> parse_coordinates(QString coords);

    Q_INVOKABLE QString get_location_info(int ind);

    Q_INVOKABLE void calculate_positions(QVector3D planetPosition, QQuaternion sphereRotation,QVector3D radius);

    Q_INVOKABLE QString get_minerals_from_location(int ind);

    Q_INVOKABLE QString get_elements_from_location(int ind);

    Q_INVOKABLE QString get_materials_and_products_from_location(int ind);
    Q_INVOKABLE void reload_net_base();


    const QVector<QVector3D> &obj_coords() const;
    void setObj_coords(const QVector<QVector3D> &newObj_coords);

    const QVector<QVector<double> > &lat_lon() const;
    void setlat_lon(const QVector<QVector<double> > &newLat_lon);

    const QVector<QString> &objects_info() const;
    void setobjects_info(const QVector<QString> &newObjects_info);

    const QVector<QString> &location_ids() const;
    void setLocation_ids(const QVector<QString> &newLocation_ids);

signals:
    void objects_databaseChanged();
    void object_nameChanged();

    void obj_coordsChanged();

    void lat_lonChanged();

    void objects_infoChanged();

    void location_idsChanged();

private:
    sql_database *m_objects_database = nullptr;
    QString m_object_name;
    QVector<QVector3D> m_obj_coords;
    QVector<QVector<double> > m_lat_lon;
    QVector<QString> m_objects_info;
    QVector<QString> m_location_ids;
};

#endif // NET_BASES_H
