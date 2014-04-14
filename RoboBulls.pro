#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T09:39:34
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = RoboBulls
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    communication/refcomm.cpp \
    include/messages_robocup_ssl_detection.pb.cc \
    include/messages_robocup_ssl_geometry.pb.cc \
    include/messages_robocup_ssl_refbox_log.pb.cc \
    include/messages_robocup_ssl_wrapper.pb.cc \
    include/netraw.cpp \
    include/robocup_ssl_client.cpp \
    include/serialib.cpp \
    Model/gamemodel.cpp \
    Robot/robot.cpp \
    strategy/strategy.cpp \
    strategy/strategycontroller.cpp \
    strategy/stopstrategy.cpp \
    communication/robcomm.cpp \
    Performance/behavior.cpp \
    Performance/skill.cpp \
    Performance/gotoposition.cpp \
    Performance/stopbehavior.cpp \
    Functionality/goforward.cpp \
    Functionality/gobackward.cpp \
    Measure/point.cpp \
    Measure/measurments.cpp \
    communication/visioncomm.cpp \
    Functionality/stop.cpp

HEADERS += \
    communication/refcomm.h \
    include/messages_robocup_ssl_detection.pb.h \
    include/messages_robocup_ssl_geometry.pb.h \
    include/messages_robocup_ssl_refbox_log.pb.h \
    include/messages_robocup_ssl_wrapper.pb.h \
    include/netraw.h \
    include/robocup_ssl_client.h \
    include/serialib.h \
    include/timer.h \
    include/util.h \
    Model/gamemodel.h \
    Robot/robot.h \
    strategy/strategy.h \
    strategy/strategycontroller.h \
    strategy/stopstrategy.h \
    communication/robcomm.h \
    Performance/behavior.h \
    Performance/skill.h \
    Performance/gotoposition.h \
    Performance/stopbehavior.h \
    Functionality/goforward.h \
    Functionality/gobackward.h \
    Measure/point.h \
    Measure/measurments.h \
    communication/visioncomm.h \
    Functionality/stop.h

unix|win32: LIBS += -lprotobuf
