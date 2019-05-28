QT += location charts qml quick network
CONFIG += c++11

android {
    QT += androidextras
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources
    DISTFILES += \
        android-sources/src/org/qtproject/example/notification/NetworkInfo.java

    target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/notification

    INSTALLS += target

    OTHER_FILES += \
        android-sources/AndroidManifest.xml
}



DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += main.cpp
RESOURCES += qml.qrc
QML_IMPORT_PATH =
QML_DESIGNER_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(prediction/Prediction.pri)
include(data/Data.pri)
include(cat/CAT.pri)
include(dds/DDS.pri)
include(ui/UI.pri)
include(device/Device.pri)



