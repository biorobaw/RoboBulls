#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T09:39:34
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = RoboBulls
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
QMAKE_CXX = g++-4.8

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
    behavior/obstacleavoidbehavior.cpp \
    skill/pathfinding/fppa_pathfinding.cpp \
    skill/basic_movement.cpp \
    utilities/paramlist.cpp \
    strategy/strategy.cpp \
    strategy/stopstrategy.cpp \
    strategy/penaltystrategy.cpp \
    skill/gotopositionwithorientation.cpp \
    communication/simrobcomm.cpp \
    communication/nxtrobcomm.cpp \
    include/grSim_Replacement.pb.cc \
    include/grSim_Packet.pb.cc \
    include/grSim_Commands.pb.cc \
    skill/differential_control/robottoinertialvelocities.cpp \
    skill/differential_control/matrixcalculator.cpp \
    skill/differential_control/closedloopcontrol.cpp \
    behavior/kicktogoal.cpp \
    strategy/teststrategy.cpp \
    behavior/defendoneonone.cpp \
    skill/obstacleavoidmove.cpp

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
    behavior/obstacleavoidbehavior.h \
	utilities/paramlist.h \
    behavior/behaviorassignment.h \
    strategy/stopstrategy.h \
    strategy/penaltystrategy.h \
    skill/gotopositionwithorientation.h \
    communication/simrobcomm.h \
    communication/nxtrobcomm.h \
    include/grSim_Replacement.pb.h \
    include/grSim_Packet.pb.h \
    include/grSim_Commands.pb.h \
    include/globals.h \
    skill/differential_control/robottoinertialvelocities.h \
    skill/differential_control/matrixcalculator.h \
    skill/differential_control/closedloopcontrol.h \
    behavior/kicktogoal.h \
    strategy/teststrategy.h \
    behavior/defendoneonone.h \
    skill/obstacleavoidmove.h \
    skill/pathfinding/fppa_pathfinding.h

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wunused-local-typedefs

unix|win32: LIBS += -lprotobuf
