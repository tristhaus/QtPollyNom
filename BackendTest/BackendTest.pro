include(../Backend/backend.pri)

GOOGLETEST_DIR = D:\\VSProject\\googletest

include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11 thread
CONFIG -= app_bundle

HEADERS += \
        tst_basex.h \
        tst_constant.h

SOURCES += \
        main.cpp
