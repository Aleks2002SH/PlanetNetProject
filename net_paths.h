#ifndef NET_PATHS_H
#define NET_PATHS_H


#include <QtQuick3D/QQuick3DGeometry>
#include <QVector3D>
#include <QString>
#include <QDebug>
#include "net_bases.h"

class net_paths : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(NetBasePath)
    Q_PROPERTY(net_bases* net_base READ net_base WRITE setNet_base NOTIFY net_baseChanged)
    Q_PROPERTY(double planet_radius READ planet_radius WRITE setPlanet_radius NOTIFY planet_radiusChanged)
    Q_PROPERTY(QVector3D planet_position READ planet_position WRITE setPlanet_position NOTIFY planet_positionChanged)
    Q_PROPERTY(QQuaternion planet_rotation READ planet_rotation WRITE setPlanet_rotation NOTIFY planet_rotationChanged)

public:
    net_paths();
    net_bases *net_base() const;
    void setNet_base(net_bases *newNet_base);
    void componentComplete() override;
    double planet_radius() const;
    void setPlanet_radius(double newPlanet_radius);

    const QVector3D &planet_position() const;
    void setPlanet_position(const QVector3D &newPlanet_position);

    const QQuaternion &planet_rotation() const;
    void setPlanet_rotation(const QQuaternion &newPlanet_rotation);
    QVector<QVector3D> plot_great_circle(QVector3D p1, QVector3D p2, int segments);
    QVector3D geo_to_cartesian(double latDeg, double lonDeg);
    QVector3D rotate_by_quaternion_and_add_position(QVector3D vec);
    Q_INVOKABLE void tryRebuildGeometry();

signals:
    void net_baseChanged();
    void planet_radiusChanged();
    void planet_positionChanged();
    void planet_rotationChanged();


private:
    net_bases *m_net_base = nullptr;
    void rebuildGeometry();
    double m_planet_radius;
    QVector3D m_planet_position;
    QQuaternion m_planet_rotation;
};

#endif // NET_PATHS_H
