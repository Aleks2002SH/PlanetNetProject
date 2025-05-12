#include "net_new_elements.h"

net_new_elements::net_new_elements()
{

}

sql_database *net_new_elements::objects_database() const
{
    return m_objects_database;
}

void net_new_elements::setObjects_database(sql_database *newObjects_database)
{
    if (m_objects_database == newObjects_database)
        return;
    m_objects_database = newObjects_database;
    emit objects_databaseChanged();
}

const QVector<QVector<double> > &net_new_elements::latitudes() const
{
    return m_latitudes;
}

void net_new_elements::setLatitudes(const QVector<QVector<double> > &newLatitudes)
{
    if (m_latitudes == newLatitudes)
        return;
    m_latitudes = newLatitudes;
    emit latitudesChanged();
}

void net_new_elements::get_area_data(QString object_name)
{
    m_object_name = object_name;
    qDegug()<<"object_name"<<object_name;
    QString query_str = "SELECT A.id AS id,A.min_latitude AS min_latitude, A.max_latitude AS max_latitude, A.min_longitude AS min_longitude, A.max_longitude AS max_longitude "
                        "FROM areas A "
                        "JOIN celestial_bodies CB ON A.celestial_body_id = CB.id "
                        "WHERE CB.name = \"" + object_name + "\"";
    QSqlQuery query = m_objects_database->runQuery(query_str);

    while (query.next()) {
           QString area_id = query.value("id").toString();
           double min_longitude = query.value("min_longitude").toDouble();
           double max_longitude = query.value("max_longitude").toDouble();
           double min_latitude = query.value("min_latitude").toDouble();
           double max_latitude = query.value("max_latitude").toDouble();

           m_latitudes.push_back(QVector<double>{min_latitude,max_latitude});
           m_longitudes.push_back(QVector<double>{min_longitude,max_longitude});
           m_area_ids.push_back(area_id);
           qDebug()<<area_id<<min_latitude<<min_longitude<<max_latitude<<max_longitude;
    }

}

QString net_new_elements::get_area_description(QString area_id)
{
    QString query_str = "SELECT name, description FROM areas WHERE id =";
    query_str+=area_id;
    QSqlQuery query = m_objects_database->runQuery(query_str);
    QString res = "Area description: ";
    while (query.next()){
        QString description = query.value("description").toString();
        res += description;
    }
    return res;
}

QString net_new_elements::get_area_elements(QString area_id)
{
    QString query_str = R"(
        SELECT E.name AS element_name
        FROM area_elements AE
        JOIN elements E ON AE.element_id = E.id
        WHERE AE.area_id = )" + area_id;

    QSqlQuery query = m_objects_database->runQuery(query_str);
    QStringList elements;
    QString res= "";

    while (query.next()) {
        QString elementName = query.value("element_name").toString();
        res+=elementName + "\n";
     }
    return res;
}

QString net_new_elements::get_area_minerals(QString area_id)
{
    QString query_str = R"(
        SELECT M.name AS mineral_name
        FROM area_minerals AM
        JOIN minerals M ON AM.mineral_id = M.id
        WHERE AM.area_id = )" + area_id;
    QString res = "";
    QSqlQuery query = m_objects_database->runQuery(query_str);
    while (query.next()) {
        QString mineralName = query.value("mineral_name").toString();
        res+=mineralName+ "\n";
    }
    return res;
}

const QVector<QVector<double> > &net_new_elements::longitudes() const
{
    return m_longitudes;
}

void net_new_elements::setLongitudes(const QVector<QVector<double> > &newLongitudes)
{
    if (m_longitudes == newLongitudes)
        return;
    m_longitudes = newLongitudes;
    emit longitudesChanged();
}

const QString &net_new_elements::object_name() const
{
    return m_object_name;
}

void net_new_elements::setObjet_name(const QString &newObject_name)
{
    if (m_object_name == newObject_name)
        return;
    m_object_name = newObject_name;
    emit object_nameChanged();
}

const QVector<QString> &net_new_elements::area_ids() const
{
    return m_area_ids;
}

void net_new_elements::setArea_ids(const QVector<QString> &newArea_ids)
{
    if (m_area_ids == newArea_ids)
        return;
    m_area_ids = newArea_ids;
    emit area_idsChanged();
}



QString net_new_elements::check_area(QVector3D position_object, QVector3D globe_position, QQuaternion rotation)
{
    QVector3D local_position = rotation.inverted().rotatedVector(position_object - globe_position);
    double x = local_position.x();
    double y = local_position.y();
    double z = local_position.z();

    double r = qSqrt(x * x + y * y + z * z);
    if (r == 0.0) return "";

    double latitude = qAsin(z / r) * 180.0 / M_PI;
    double longitude = qAtan2(y, x) * 180.0 / M_PI;
    if (longitude > 180.0) longitude -= 360.0;
    if (longitude < -180.0) longitude += 360.0;
    for (int i = 0; i < m_latitudes.size(); ++i)
        {
            double minLat = m_latitudes[i][0];
            double maxLat = m_latitudes[i][1];
            double minLon = m_longitudes[i][0];
            double maxLon = m_longitudes[i][1];

            if (latitude >= minLat && latitude <= maxLat &&
                longitude >= minLon && longitude <= maxLon)
            {
                return m_area_ids[i];
            }
        }
    return "";
}
