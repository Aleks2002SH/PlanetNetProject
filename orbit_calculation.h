#ifndef ORBIT_CALCULATION_H
#define ORBIT_CALCULATION_H
#include <QtQuick3D/QQuick3DGeometry>
#include <QVector3D>
#include <QString>
#include "objects.h"
#include <QDebug>

class orbit_calculation: public QQuick3DGeometry
{
    Q_OBJECT
    Q_PROPERTY(bool created READ created WRITE setCreated NOTIFY createdChanged)
    QML_NAMED_ELEMENT(OrbitCalculator)
    Q_PROPERTY(QVector<double> init_state READ init_state WRITE init_state NOTIFY init_stateChanged)
    Q_PROPERTY(rksolver3d rksolv READ rksolv)
    Q_PROPERTY(double planet_mass READ planet_mass WRITE planet_mass NOTIFY planet_massChanged)
    Q_PROPERTY(double scale READ scale WRITE scale NOTIFY scaleChanged)

public:
    orbit_calculation(QObject *parent=nullptr){m_created = true;};

    void setOrbit();
    bool created() const;


    void setCreated(bool newCreated);

    const QVector<double> &init_state() const;

    const rksolver3d &rksolv() const;

    const double &planet_mass() const;

    double scale() const;

    void scale(double newScale);


    void init_state(const QVector<double> &newInit_state);


    void planet_mass(const double &newplanet_mass);

    void componentComplete() override;

signals:
    void createdChanged();

    void init_stateChanged();

    void planet_massChanged();

    void scaleChanged();

private:
    void updateData();
    void rebuildGeometry();
    bool m_created;
    QVector<double> m_init_state;
    rksolver3d m_rksolv;
    double m_planet_mass;
    double m_scale;
};

#endif // ORBIT_CALCULATION_H
