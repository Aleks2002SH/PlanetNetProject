#include "net_bases.h"


sql_database *net_bases::objects_database() const
{
    return m_objects_database;
}

void net_bases::setObjects_database(sql_database *newObjects_database)
{
    if (m_objects_database == newObjects_database)
        return;
    m_objects_database = newObjects_database;
    emit objects_databaseChanged();
}

const QString &net_bases::object_name() const
{
    return m_object_name;
}

void net_bases::setObjet_name(const QString &newObject_name)
{
    if (m_object_name == newObject_name)
        return;
    m_object_name = newObject_name;
    emit object_nameChanged();
}
QVector<double> net_bases::parse_coordinates(QString coords)
{
    QStringList parts = coords.split(' ', Qt::SkipEmptyParts);
    bool ok1, ok2;
    double latitude = parts[0].toDouble(&ok1);
    double longitude = parts[1].toDouble(&ok2);
    return QVector<double>{latitude,longitude};

}

QString net_bases::get_location_info(int ind)
{
    QString base_coords ="Coordinates: Latitude:" +  QString::number(m_lat_lon[ind][0]) + "; Longitide:"+QString::number(m_lat_lon[ind][1])+"; \n";
    QString base_info = m_objects_info[ind];

    return base_coords+base_info;
}

void net_bases::calculate_positions(QVector3D planetPosition, QQuaternion sphereRotation,QVector3D radius)
{
    qDebug()<<m_lat_lon;
    for (QVector<double>& coords : m_lat_lon) {
            if (coords.size() != 2)
                continue;
            qDebug()<<coords;
            double latDeg = coords[0];
            double lonDeg = coords[1];
            if (lonDeg < 0){
                        lonDeg += 360;}

            double latRad = M_PI*(latDeg)/180;
            double lonRad = M_PI*(lonDeg)/180;

            qDebug()<<planetPosition;
            qDebug()<<sphereRotation;
            qDebug()<<radius[0];
            float x = radius[0] * qCos(latRad) * qCos(lonRad);
            float y = radius[0] * qSin(latRad);
            float z = radius[0] * qCos(latRad) * qSin(lonRad);

            QVector3D localPos(x, y, z);
            qDebug()<<"local_pos"<<localPos;

            QVector3D rotatedPos = sphereRotation.rotatedVector(localPos);
            qDebug()<<"rotated_pos"<<rotatedPos;

            QVector3D finalPos = rotatedPos + planetPosition;

            qDebug()<<"final_pos"<<finalPos;
            qDebug()<<m_objects_info;

            m_obj_coords.append(finalPos);
        }
    qDebug()<<m_obj_coords;
}

QString net_bases::get_minerals_from_location(int ind)
{
    QString location_id = m_location_ids[ind];
    QString query_str = "SELECT M.name, M.formula, LM.percentage FROM Location_Minerals LM JOIN Minerals M ON LM.mineral_id = M.id WHERE LM.location_id = "+location_id;
    QSqlQuery query = m_objects_database->runQuery(query_str);
    QString result = "";
    qDebug()<<"query"<<query_str;
    qDebug()<<"location id"<<location_id;
    while (query.next()) {
           QString mineral_name = query.value("name").toString();
           //QString mineral_formula = query.value("formula").toString();
           QString percentage_per_location = query.value("percentage").toString();
//           result+=mineral_name + " " + mineral_formula + " " + percentage_per_location +"\n";
//           qDebug()<<"mineral info"<<mineral_name<<mineral_formula<<percentage_per_location;
           result+="\n";
           result+=mineral_name + " " + percentage_per_location +"\n";

           qDebug()<<"mineral info"<<mineral_name<<percentage_per_location;
    }
    qDebug()<<"result of minerals"<<result;
    return result;
}

QString net_bases::get_elements_from_location(int ind)
{
    QString location_id = m_location_ids[ind];
    QString query_str = R"(
        SELECT
            e.id AS element_id,
            e.name AS element_name,
            e.symbol AS element_symbol,
            mc.percentage * lm.percentage / 100.0 AS effective_percentage
        FROM
            Location_Minerals lm
        JOIN
            Minerals m ON lm.mineral_id = m.id
        JOIN
            Mineral_Composition mc ON mc.mineral_id = m.id
        JOIN
            Elements e ON mc.element_id = e.id
        WHERE
            lm.location_id =)";
    query_str+=location_id;
    QSqlQuery query = m_objects_database->runQuery(query_str);
    QString result = "";
    while (query.next()) {
           QString element_name = query.value("element_name").toString();
           QString element_symbol = query.value("element_symbol").toString();
           QString percentage_element = query.value("effective_percentage").toString();
           result+="\n";
           result+=element_name + " " + element_symbol + " " + percentage_element +"\n";

           qDebug()<<"elements res"<<element_name<<element_symbol<<percentage_element;
    }

    return result;
}

QString net_bases::get_materials_and_products_from_location(int ind)
{
    QString location_id = m_location_ids[ind];
    QString query_str = R"(
        SELECT
            m.name AS mineral_name,
            m.description AS mineral_description,
            mai.name AS material_name,
            mai.description AS material_description,
            p.name AS product_name,
            p.description AS product_description
        FROM
            Location_Minerals lm
        JOIN
            Minerals m ON m.id = lm.mineral_id
        JOIN
            Mineral_Composition mc ON mc.mineral_id = m.id
        JOIN
            Material_Composition matc ON matc.element_id = mc.element_id
        JOIN
            Materials_And_Intermediates mai ON mai.id = matc.material_id
        LEFT JOIN
            Product_Composition pc ON pc.material_id = mai.id
        LEFT JOIN
            Products p ON p.id = pc.product_id
        WHERE
            lm.location_id = )";
    query_str+=location_id;
    qDebug()<<query_str;
    QSqlQuery query = m_objects_database->runQuery(query_str);
    QString result = "";
    while (query.next()) {
           QString mineral_name = query.value("mineral_name").toString();
           QString material_name = query.value("material_name").toString();
           QString material_description = query.value("material_description").toString();
           QString product_name = query.value("product_name").toString();
           QString product_description  = query.value("product_description").toString();
           result+="\n";
           result+="Mineral - "+mineral_name+":\n"+"Material - "+material_name+" "+material_description+"\n";
           result+="Product - "+product_name+" "+product_description+"\n";

    }
    return result;
}

void net_bases::reload_net_base()
{

    m_object_name.clear();
    m_obj_coords.clear();
    m_lat_lon.clear();
    m_objects_info.clear();
    m_location_ids.clear();

}



void net_bases::get_location_data()
{
    QString query_str ="SELECT L.* FROM Locations L JOIN Celestial_Bodies CB ON L.celestial_body_id = CB.id WHERE CB.name = \""+m_object_name+"\"";
    QSqlQuery query = m_objects_database->runQuery(query_str);
    while (query.next()) {
           QString location_id = query.value("id").toString();
           QString name = query.value("name").toString();
           QString type = query.value("type").toString();
           QString description = query.value("description").toString();
           QString coordinates = query.value("coordinates").toString();
           m_objects_info.append("name: "+ name+";\n Type: "+type+";\n Description: "+description+"\n Location id: "+location_id);
           m_lat_lon.append(parse_coordinates(coordinates));
           m_location_ids.append(location_id);
           qDebug() << "Location:" << name << type << coordinates<<location_id;
    }
}





const QVector<QVector3D> &net_bases::obj_coords() const
{
    return m_obj_coords;
}

void net_bases::setObj_coords(const QVector<QVector3D> &newObj_coords)
{
    if (m_obj_coords == newObj_coords)
        return;
    m_obj_coords = newObj_coords;
    emit obj_coordsChanged();
}

const QVector<QVector<double> > &net_bases::lat_lon() const
{
    return m_lat_lon;
}

void net_bases::setlat_lon(const QVector<QVector<double> > &newLat_lon)
{
    if (m_lat_lon == newLat_lon)
        return;
    m_lat_lon = newLat_lon;
    emit lat_lonChanged();
}

const QVector<QString> &net_bases::objects_info() const
{
    return m_objects_info;
}

void net_bases::setobjects_info(const QVector<QString> &newObjects_info)
{
    if (m_objects_info == newObjects_info)
        return;
    m_objects_info = newObjects_info;
    emit objects_infoChanged();
}



const QVector<QString> &net_bases::location_ids() const
{
    return m_location_ids;
}

void net_bases::setLocation_ids(const QVector<QString> &newLocation_ids)
{
    if (m_location_ids == newLocation_ids)
        return;
    m_location_ids = newLocation_ids;
    emit location_idsChanged();
}
