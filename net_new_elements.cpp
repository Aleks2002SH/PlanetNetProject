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
    qDebug()<<"object_name"<<object_name;
    QString query_str = R"(SELECT A.id AS id,
           A.min_latitude AS min_latitude,
           A.max_latitude AS max_latitude,
           A.min_longitude AS min_longitude,
           A.max_longitude AS max_longitude
    FROM areas A
    JOIN celestial_bodies CB ON A.celestial_body_id = CB.id
    WHERE CB.name = )";
    query_str += "\""+object_name + "\"";
    qDebug()<<query_str;
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

 QVariantList net_new_elements::get_lat_lon(QVector3D position, QVector3D globe_position, QQuaternion rotation)
{
    QVector3D localRotated = position - globe_position;
    QVector3D local = rotation.inverted().rotatedVector(localRotated);
    QVector3D normalized = local.normalized();

    double latRad = qAsin(normalized.y());
    double lonRad = qAtan2(normalized.z(), normalized.x());

    double latitude = latRad * 180.0 / M_PI;
    double longitude = lonRad * 180.0 / M_PI;
    QVariantList list;
    list.append(latitude);
    list.append(longitude);


    return list;
 }

 QString net_new_elements::add_location_to_database(QString cel_body_name, QVariantMap text_data, double lat, double lon)
 {
     QString query_str = R"(
         SELECT id
         FROM Celestial_Bodies
         WHERE name = )";
      query_str+= "\""+cel_body_name+"\"";
      QSqlQuery query = m_objects_database->runQuery(query_str);
      while  (query.next()){
        QString id = query.value("id").toString();
        QString coordinates =  QString::number(lat)+" "+QString::number(lon);
        QString add_query_str = QString(R"(
            INSERT INTO Locations (name, type, coordinates, celestial_body_id, description)
            VALUES ('%1', '%2', '%3', '%4', '%5')
        )").arg(text_data["name"].toString())
          .arg(text_data["type"].toString())
          .arg(coordinates)
          .arg(id)
          .arg(text_data["description"].toString());
        QSqlQuery add_query = m_objects_database->runQuery(add_query_str);
        if (add_query.lastError().isValid()) {
                return add_query.lastError().text();
            }
        QString mineral_info = text_data["mineral_text"].toString();
        qDebug()<<mineral_info;
        QStringList minerals = parse_mineral_text(mineral_info);
        qDebug()<<"Minerals which supposed to be added to database"<<minerals;
        int location_id = add_query.lastInsertId().toInt();
        for(int i =0;i<minerals.size();i++){
            QString minerals_query_str = R"(
                SELECT id
                FROM Minerals
                WHERE name = )";
             minerals_query_str+= "\""+minerals[i]+"\"";
             qDebug()<<minerals_query_str;
             QSqlQuery minerals_query = m_objects_database->runQuery(minerals_query_str);
             while (minerals_query.next()){
             QString mineral_id = minerals_query.value("id").toString();
             qDebug()<<"added mineral"<<mineral_id;
             qDebug()<<location_id;
             QString add_minerals_query_str = QString(R"(
                 INSERT INTO Location_Minerals (mineral_id, location_id, percentage)
                 VALUES ('%1', '%2', '%3')
             )").arg(mineral_id)
               .arg(location_id)
               .arg(QRandomGenerator::global()->bounded(101));
             QSqlQuery add_minerals_query = m_objects_database->runQuery(add_minerals_query_str);
             if(add_minerals_query.lastError().isValid()){
                 qDebug()<<"Mineral not inserted"<<add_minerals_query.lastError().text();
             }
             }

        }

        return "Added location to database";
      }
      return "something went wrong";
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

QStringList net_new_elements::parse_mineral_text(QString mineral_text)
{

        QStringList mineralList = mineral_text.split('\n', Qt::SkipEmptyParts);
        return mineralList;
}



QString net_new_elements::check_area(QVector3D position_object, QVector3D globe_position, QQuaternion rotation)
{
    QVector3D local_position = rotation.rotatedVector(position_object - globe_position);
    double x = local_position.x();
    double y = local_position.y();
    double z = local_position.z();

    double r = qSqrt(x * x + y * y + z * z);
    if (r == 0.0) return "";

    double latitude = qAsin(z / r) * 180.0 / M_PI;
    double longitude = qAtan2(y, x) * 180.0 / M_PI;
    qDebug()<<"Coords"<<latitude<<longitude;
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
