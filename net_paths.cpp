#include "net_paths.h"

net_paths::net_paths()
{

}

net_bases *net_paths::net_base() const
{
    return m_net_base;
}

void net_paths::setNet_base(net_bases *newNet_base)
{
    if (m_net_base == newNet_base)
        return;
    m_net_base = newNet_base;
    emit net_baseChanged();
}

void net_paths::componentComplete()
{
    QQuick3DGeometry::componentComplete();
    rebuildGeometry();
}

QVector3D net_paths::geo_to_cartesian(double latDeg, double lonDeg){

    if (lonDeg < 0){
                lonDeg += 360;}

    double latRad = M_PI*(latDeg)/180;
    double lonRad = M_PI*(lonDeg)/180;

    float x = m_planet_radius * qCos(latRad) * qCos(lonRad);
    float y = m_planet_radius* qSin(latRad);
    float z = m_planet_radius * qCos(latRad) * qSin(lonRad);
    return QVector3D(x, y, z);
}

QVector3D net_paths::rotate_by_quaternion_and_add_position(QVector3D vec){
    return m_planet_rotation.rotatedVector(vec) + m_planet_position;
}

void net_paths::tryRebuildGeometry()
{
    if (!m_net_base)
        return;
    if (m_planet_radius <= 0.0)
        return;
    if (!m_planet_position.isNull())
        return;
    if (!m_planet_rotation.isIdentity())
        return;

    rebuildGeometry();
}

QVector<QVector3D> net_paths::plot_great_circle(QVector3D p1, QVector3D p2, int segments){
    QVector<QVector3D> path;
    double dot = QVector3D::dotProduct(p1.normalized(), p2.normalized());
    double omega = acos(qBound(-1.0, dot, 1.0));

    for (int i = 0; i <= segments; ++i) {
        double t = static_cast<double>(i) / segments;
        if (omega < 1e-6) {
            path.append(p1);
            continue;
        }
        double sin_omega = sin(omega);
        double scale1 = sin((1 - t) * omega) / sin_omega;
        double scale2 = sin(t * omega) / sin_omega;
        QVector3D interpolated = (p1 * scale1 + p2 * scale2).normalized() * p1.length();
        path.append(interpolated);
    }

    return path;
}

void net_paths::rebuildGeometry()
{
    if (!m_net_base)
        return;

    QByteArray vertexBuffer;
    QByteArray indexBuffer;

    QVector<QVector<double>> lat_lon = m_net_base->get_lat_lon();
    int vertexCount = 0;
    int segments = 50;

    for (int i = 0; i < lat_lon.size(); ++i) {
        for (int j = i + 1; j < lat_lon.size(); ++j) {
            auto base1 = lat_lon[i];
            auto base2 = lat_lon[j];

            QVector3D p1 = geo_to_cartesian(base1[0], base1[1]);
            QVector3D p2 = geo_to_cartesian(base2[0], base2[1]);

            p1 = rotate_by_quaternion_and_add_position(p1);
            p2 = rotate_by_quaternion_and_add_position(p2);

            QVector<QVector3D> arc = plot_great_circle(p1, p2, segments);

            for (const QVector3D &v : arc) {
                float x = v.x(), y = v.y(), z = v.z();
                vertexBuffer.append(reinterpret_cast<const char*>(&x), sizeof(float));
                vertexBuffer.append(reinterpret_cast<const char*>(&y), sizeof(float));
                vertexBuffer.append(reinterpret_cast<const char*>(&z), sizeof(float));
                quint32 index = vertexCount++;
                indexBuffer.append(reinterpret_cast<const char*>(&index), sizeof(quint32));
            }
        }
    }

    clear();
    setVertexData(vertexBuffer);
    setIndexData(indexBuffer);
    setStride(3 * sizeof(float));
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0, QQuick3DGeometry::Attribute::F32Type);

    setPrimitiveType(QQuick3DGeometry::PrimitiveType::LineStrip);
    update();
}

double net_paths::planet_radius() const
{
    return m_planet_radius;
}

void net_paths::setPlanet_radius(double newPlanet_radius)
{
    if (qFuzzyCompare(m_planet_radius, newPlanet_radius))
        return;
    m_planet_radius = newPlanet_radius;
    emit planet_radiusChanged();
}

const QVector3D &net_paths::planet_position() const
{
    return m_planet_position;
}

void net_paths::setPlanet_position(const QVector3D &newPlanet_position)
{
    if (m_planet_position == newPlanet_position)
        return;
    m_planet_position = newPlanet_position;
    emit planet_positionChanged();
}

const QQuaternion &net_paths::planet_rotation() const
{
    return m_planet_rotation;
}

void net_paths::setPlanet_rotation(const QQuaternion &newPlanet_rotation)
{
    if (m_planet_rotation == newPlanet_rotation)
        return;
    m_planet_rotation = newPlanet_rotation;
    emit planet_rotationChanged();
}
