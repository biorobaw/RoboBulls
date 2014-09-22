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

#DESTDIR = ../../bin
#UI_DIR = .

#CONFIG(debug, debug|release) {
#        TARGET = myappd
#        OBJECTS_DIR = ../../Documents/RoboBulls-build-desktop-Qt_4_8_3_in_PATH__System__Debug
#        MOC_DIR = ../../Documents/RoboBulls-build-desktop-Qt_4_8_3_in_PATH__System__Debug
#}

#CONFIG(release, debug|release) {
#        TARGET = myapp
#        OBJECTS_DIR = ../../Documents/RoboBulls-build-desktop-Qt_4_8_3_in_PATH__System__Release
#        MOC_DIR = ../../Documents/RoboBulls-build-desktop-Qt_4_8_3_in_PATH__System__Release
#}

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
	skill/driveball.cpp \
	skill/kick.cpp \
	skill/waitforball.cpp \
	strategy/penaltystrategy.cpp \
	strategy/stopstrategy.cpp \
	strategy/strategy.cpp \
	strategy/strategycontroller.cpp \
	strategy/teststrategy.cpp \
	utilities/measurments.cpp \
	utilities/paramlist.cpp \
	utilities/point.cpp \
    utilities/skillsequence.cpp \
    behavior/mytestbehavior.cpp \
    behavior/defendclosetoball.cpp \
    behavior/defendfarfromball.cpp \
    behavior/attackmain.cpp \
    behavior/attacksupport.cpp \
    behavior/positionforkickoff.cpp \
    behavior/sendballtoregion.cpp \
    strategy/twovone.cpp \
    skill/kicktopoint.cpp \
    utilities/region.cpp \
    movement/basic_movement.cpp \
    movement/gotopositionwithorientation.cpp \
    movement/pathfinding/fppa_pathfinding.cpp \
    movement/three_omni_motion/matrixcalculator.cpp \
    movement/move.cpp \
    movement/differential/closedloopcontrol.cpp \
    include/config/robot_types.cpp \
    behavior/genericmovementbehavior.cpp \
    skill/stop.cpp \
    movement/four_omni_motion/omni4_velcalculator.cpp \
    movement/three_omni_motion/omni3_velcalculator.cpp \
    movement/differential/differential_velcalculator.cpp

HEADERS += \
	behavior/behavior.h \
	behavior/behaviorassignment.h \
	behavior/defendoneonone.h \
	behavior/driveballandkick.h \
	behavior/kicktogoal.h \
	behavior/penaltybehavior.h \
	behavior/stopbehavior.h \
	communication/nxtrobcomm.h \
	communication/refcomm.h \
	communication/robcomm.h \
	communication/simrobcomm.h \
	communication/visioncomm.h \
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
	skill/driveball.h \
	skill/kick.h \
	utilities/skillsequence.h \
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
    utilities/skillsequence.h \
    behavior/mytestbehavior.h \
    behavior/defendclosetoball.h \
    behavior/defendfarfromball.h \
    behavior/attackmain.h \
    behavior/attacksupport.h \
    behavior/positionforkickoff.h \
    behavior/sendballtoregion.h \
    strategy/twovone.h \
    skill/kicktopoint.h \
    utilities/region.h \
    movement/basic_movement.h \
    movement/gotopositionwithorientation.h \
    movement/pathfinding/fppa_pathfinding.h \
    movement/three_omni_motion/matrixcalculator.h \
	movement/move.h \
    movement/differential/closedloopcontrol.h \
    movement/gotoposition.h \
    movement/movetype.h \
    include/config/globals.h \
    include/config/robot_types.h \
    include/config/simulated.h \
    include/config/team.h \
    include/config/tolerances.h \
    include/config/trace.h \
    behavior/genericmovementbehavior.h \
    skill/stop.h \
    movement/three_omni_motion/omni3_velcalculator.h \
    movement/differential/differential_velcalculator.h \
    movement/four_omni_motion/omni4_velcalculator.h \
    include/config/model_averages.h

QMAKE_CXXFLAGS += -std=c++0x

unix|win32: LIBS += -lprotobuf

OTHER_FILES +=
