include(prediction/Prediction.pri)

HEADERS += \
    $$PWD/networkcontext.h \
    $$PWD/mobilitycontext.h \
    $$PWD/contextmanager.h \
    $$PWD/context.h \
    $$PWD/cellmap.h \
    $$PWD/prediction/o2_rf.h \
    $$PWD/prediction/tmobile_rf.h \
    $$PWD/prediction/vodafone_rf.h \
    $$PWD/connectivitymap.h \
    $$PWD/mobilityprediction.h

SOURCES += \
    $$PWD/networkcontext.cpp \
    $$PWD/mobilitycontext.cpp \
    $$PWD/contextmanager.cpp \
    $$PWD/context.cpp \
    $$PWD/cellmap.cpp \
    $$PWD/connectivitymap.cpp \
    $$PWD/mobilityprediction.cpp
