#include "orbit_calculation.h"


void orbit_calculation::rebuildGeometry()
{
        QByteArray vertices;
        QByteArray indices;

        planets_data pl_dat;

//        qDebug()<<"Planet name"<<m_planet_mass;
        qDebug()<<"Scale"<<m_scale;

//        planets pl = pl_dat._planets_data.at(m_planet_mass);


        objects obj(m_init_state,0,8400, m_planet_mass,&m_rksolv);
        obj.set_dt(20);
        coordinates dat = obj.get_data();

        const float radius = 1.0f;

        qDebug()<<dat.X.size();

        vertices.resize(dat.X.size() * 3 * sizeof(float));
        float* vertexData = reinterpret_cast<float*>(vertices.data());


        for (int i = 0; i < dat.X.size(); ++i) {
            vertexData[i * 3 + 0] = m_scale*radius*dat.X[i];
            vertexData[i * 3 + 1] = m_scale*radius*dat.Y[i];
            vertexData[i * 3 + 2] = m_scale*radius*dat.Z[i];
        }

        indices.resize(dat.X.size() * sizeof(quint32));
        quint32* indexData = reinterpret_cast<quint32*>(indices.data());
        for (int i = 0; i < dat.X.size(); ++i) {
            indexData[i] = i;
        }

        clear();
        QQuick3DGeometry::setPrimitiveType(QQuick3DGeometry::PrimitiveType::LineStrip);
        setVertexData(vertices);
        setIndexData(indices);
        setStride(3 * sizeof(float));
        setBounds(QVector3D(-radius, -1, -radius), QVector3D(radius, 1, radius));

        addAttribute( QQuick3DGeometry::Attribute::PositionSemantic, 0,QQuick3DGeometry::Attribute::F32Type);
}

bool orbit_calculation::created() const
{
    return m_created;
}

void orbit_calculation::setCreated(bool newCreated)
{
    if (m_created == newCreated)
        return;
    m_created = newCreated;
    emit createdChanged();
}

void orbit_calculation::updateData()
{
    rebuildGeometry();
}

const QVector<double> &orbit_calculation::init_state() const
{
    return m_init_state;
}

const rksolver3d &orbit_calculation::rksolv() const
{
    return m_rksolv;
}

const double &orbit_calculation::planet_mass() const
{
    return m_planet_mass;
}

double orbit_calculation::scale() const
{
    return m_scale;
}

void orbit_calculation::scale(double newScale)
{
    if (qFuzzyCompare(m_scale, newScale))
        return;
    m_scale = newScale;
    emit scaleChanged();
}

void orbit_calculation::init_state(const QVector<double> &newInit_state)
{
   if (m_init_state == newInit_state)
        return;
    m_init_state = newInit_state;
    emit init_stateChanged();
}


void orbit_calculation::planet_mass(const double &newplanet_mass)
{
    if (m_planet_mass == newplanet_mass)
        return;
    m_planet_mass = newplanet_mass;
    emit planet_massChanged();
}

void orbit_calculation::componentComplete()
{
    QQuick3DGeometry::componentComplete();
    rebuildGeometry();
}
