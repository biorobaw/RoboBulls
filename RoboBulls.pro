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
	behavior/behavior.cpp \
	behavior/defendoneonone.cpp \
	behavior/driveballandkick.cpp \
	behavior/kicktogoal.cpp \
	behavior/penaltybehavior.cpp \
	behavior/stopbehavior.cpp \
	communication/nxtrobcomm.cpp \
	communication/refcomm.cpp \
	communication/robcomm.cpp \
	communication/simrobcomm.cpp \
	communication/visioncomm.cpp \
	include/grSim_Commands.pb.cc \
	include/grSim_Packet.pb.cc \
	include/grSim_Replacement.pb.cc \
	include/messages_robocup_ssl_detection.pb.cc \
	include/messages_robocup_ssl_geometry.pb.cc \
	include/messages_robocup_ssl_refbox_log.pb.cc \
	include/messages_robocup_ssl_wrapper.pb.cc \
	include/netraw.cpp \
	include/robocup_ssl_client.cpp \
	include/serialib.cpp \
	model/gamemodel.cpp \
	model/robot.cpp \
	skill/basic_movement.cpp \
	skill/differential_control/closedloopcontrol.cpp \
	skill/differential_control/matrixcalculator.cpp \
	skill/differential_control/robottoinertialvelocities.cpp \
	skill/driveball.cpp \
	skill/gotopositionwithorientation.cpp \
	skill/kick.cpp \
	skill/obstacleavoidmove.cpp \
	skill/pathfinding/fppa_pathfinding.cpp \
	skill/waitforball.cpp \
	strategy/penaltystrategy.cpp \
	strategy/stopstrategy.cpp \
	strategy/strategy.cpp \
	strategy/strategycontroller.cpp \
	strategy/teststrategy.cpp \
	utilities/measurments.cpp \
	utilities/paramlist.cpp \
	utilities/point.cpp \
    utilities/skillsequence.cpp

HEADERS += \
	behavior/behavior.h \
	behavior/behaviorassignment.h \
	behavior/defendoneonone.h \
	behavior/driveballandkick.h \
	behavior/kicktogoal.h \
	behavior/obstacleavoidbehavior.h \
	behavior/penaltybehavior.h \
	behavior/stopbehavior.h \
	communication/nxtrobcomm.h \
	communication/refcomm.h \
	communication/robcomm.h \
	communication/simrobcomm.h \
	communication/visioncomm.h \
	include/globals.h \
	include/grSim_Commands.pb.h \
	include/grSim_Packet.pb.h \
	include/grSim_Replacement.pb.h \
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
	model/robot.h \
	skill/basic_movement.h \
	skill/differential_control/closedloopcontrol.h \
	skill/differential_control/matrixcalculator.h \
	skill/differential_control/robottoinertialvelocities.h \
	skill/driveball.h \
	skill/gotopositionwithorientation.h \
	skill/kick.h \
	skill/obstacleavoidmove.h \
	utilities/skillsequence.h \
	skill/pathfinding/fppa_pathfinding.h \
	skill/skill.h \
	skill/waitforball.h \
	strategy/penaltystrategy.h \
	strategy/stopstrategy.h \
	strategy/strategy.h \
	strategy/strategycontroller.h \
	strategy/teststrategy.h \
	utilities/measurments.h \
	utilities/paramlist.h \
	utilities/point.h \

QMAKE_CXXFLAGS += -std=c++0x

unix|win32: LIBS += -lprotobuf
