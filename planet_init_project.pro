QT += quick quick3d
QT += sql

SOURCES += \
        functions.cpp \
        main.cpp \
        net_bases.cpp \
        net_new_elements.cpp \
        net_paths.cpp \
        objects.cpp \
        orbit_calculation.cpp \
        planets.cpp \
        solvers.cpp \
        sql_database.cpp

resources.files = main.qml
resources.prefix = /$${TARGET}
RESOURCES += resources \
    planet_init_project.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    interplanetary_transport.db

HEADERS += \
    functions.h \
    net_bases.h \
    net_new_elements.h \
    net_paths.h \
    objects.h \
    orbit_calculation.h \
    planets.h \
    solvers.h \
    sql_database.h
