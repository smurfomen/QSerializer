contains(DEFINES, QS_HAS_XML) {
    QT += xml
}

HEADERS += \
        $$PWD/src/qserializer.h

INCLUDEPATH += $$PWD/

DISTFILES += \
        $$PWD/QSerializer

CONFIG += QSERIALIZER_INCLUDED
DEFINES += QSERIALIZER_INCLUDED
