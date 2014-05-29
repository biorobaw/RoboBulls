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
	model/gamemodel.cpp \
    strategy/strategycontroller.cpp \
    communication/robcomm.cpp \
    behavior/behavior.cpp \
    behavior/stopbehavior.cpp \
    communication/visioncomm.cpp \
    skill/kick.cpp \
    skill/waitforball.cpp \
	model/robot.cpp \
    skill/gotoposition.cpp \
	utilities/point.cpp \
	utilities/measurments.cpp \
    skill/rotate.cpp \
    behavior/penaltybehavior.cpp \
    skill/gotoposition2.cpp \
    behavior/gotoposition2behavior.cpp \
    behavior/obstacleavoidbehavior.cpp \
    skill/pathfinding/fppa_pathfinding.cpp \
    skill/basic_movement.cpp \
    utilities/paramlist.cpp \
    strategy/strategy.cpp \
    strategy/stopstrategy.cpp \
    strategy/penaltystrategy.cpp

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
	model/gamemodel.h \
    Robot/robot.h \
    strategy/strategy.h \
    strategy/strategycontroller.h \
    communication/robcomm.h \
    behavior/behavior.h \
    communication/visioncomm.h \
    skill/kick.h \
    skill/waitforball.h \
	model/robot.h \
    skill/gotoposition.h \
    skill/skill.h \
    behavior/stopbehavior.h \
	utilities/measurments.h \
	utilities/point.h \
    skill/rotate.h \
	skill/basic_movement.h \
    behavior/penaltybehavior.h \
    skill/gotoposition2.h \
    behavior/gotoposition2behavior.h \
    behavior/obstacleavoidbehavior.h \
    skill/pathfinding/fppa_pathfinding.hpp \
	utilities/paramlist.h \
    behavior/behaviorassignment.h \
    strategy/stopstrategy.h \
    strategy/penaltystrategy.h

QMAKE_CXXFLAGS += -std=c++0x

unix|win32: LIBS += -lprotobuf
