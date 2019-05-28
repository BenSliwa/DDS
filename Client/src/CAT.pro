QT += charts qml quick location

android {
    QT += androidextras
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

SOURCES += \
    main.cpp \
    uimanager.cpp

OTHER_FILES += \
    qml/main.qml \
    android-sources/AndroidManifest.xml

RESOURCES += \
    main.qrc

HEADERS += \
    uimanager.h

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/notification
INSTALLS += target

include(app/App.pri)
include(context/Context.pri)
include(settings/Settings.pri)

DISTFILES += \
    android-sources/src/org/qtproject/example/notification/NetworkInfo.java
