#-------------------------------------------------

#
# Project created by QtCreator 2014-03-05T09:39:34
#
#-------------------------------------------------


QT_SELECT=5
QT += core network widgets
QT -= gui
QT += serialport
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = RoboBulls
CONFIG += console
CONFIG += -j
CONFIG -= app_bundle

TEMPLATE = app
QMAKE_CXX = g++

SOURCES += main.cpp \
	behavior/attackmain.cpp \
	behavior/attacksupport.cpp \
	behavior/behavior.cpp \
	behavior/genericmovementbehavior.cpp \
	communication/nxtrobcomm.cpp \
	communication/refcomm.cpp \
	communication/robcomm.cpp \
	communication/simrobcomm.cpp \
	communication/visioncomm.cpp \
	gui/fieldpanel.cpp \
	gui/gamepanel.cpp \
	gui/guiball.cpp \
	gui/guibotlabel.cpp \
	gui/guicomm.cpp \
	gui/guidrawline.cpp \
	gui/guifield.cpp \
	gui/guiinterface.cpp \
	gui/guirobot.cpp \
	gui/guiscene.cpp \
	gui/guisidelines.cpp \
        gui/joystick.cpp \
	gui/mainwindow.cpp \
	gui/objectposition.cpp \
	gui/robotpanel.cpp \
	gui/selrobotpanel.cpp \
	include/config/robot_types.cpp \
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
        movement/four_wheel_omni/four_wheel_omni_pilot.cpp \
	movement/move_collisions.cpp \
	movement/pathfinding/fppa_pathfinding.cpp \
        movement/three_wheel_omni/three_wheel_omni_pilot.cpp \
	skill/kick.cpp \
	skill/kicktopointomni.cpp \
	skill/stop.cpp \
	strategy/freekickstrategy.cpp \
	strategy/haltstrategy.cpp \
	strategy/kickoffstrategy.cpp \
	strategy/normalgamestrategy.cpp \
	strategy/penaltystrategy.cpp \
	strategy/stopstrategy.cpp \
	strategy/strategycontroller.cpp \
	strategy/strategy.cpp \
	strategy/teststrategy.cpp \
	strategy/videostrategies.cpp \
	utilities/comparisons.cpp \
	utilities/debug.cpp \
	utilities/edges.cpp \
	utilities/point.cpp \
	utilities/velocitycalculator.cpp \
        behavior/defendbehavior.cpp \
        strategy/indirectkickstrategy.cpp \
        communication/kfball.cpp \
        utilities/getclassname.cpp \
        utilities/region/sector.cpp \
        utilities/region/rectangle.cpp \
        utilities/measurements.cpp \
        utilities/region/defencearea.cpp \
        gui/guidrawpoint.cpp \
        gui/guidrawregion.cpp \
        skill/dribbletopoint.cpp \
        behavior/challengeballbot.cpp \
        skill/dribbleback.cpp \
        behavior/refstop.cpp \
        behavior/goalie.cpp \
        behavior/markbot.cpp \
        behavior/wall.cpp \
        behavior/penaltygoalie.cpp \
        communication/yisirobcomm.cpp \
        communication/crc.cpp \
        movement/go_to_pose.cpp \
        movement/differential/differential_pilot.cpp \

HEADERS += \
	behavior/attackmain.h \
	behavior/attacksupport.h \
	behavior/behavior.h \
	behavior/defendbehavior.h \
	behavior/genericmovementbehavior.h \
	behavior/genericskillbehavior.h \
	communication/nxtrobcomm.h \
	communication/refcomm.h \
	communication/robcomm.h \
	communication/simrobcomm.h \
	communication/visioncomm.h \
	gui/fieldpanel.h \
	gui/gamepanel.h \
	gui/guiball.h \
	gui/guibotlabel.h \
	gui/guicomm.h \
	gui/guidrawline.h \
	gui/guifield.h \
	gui/guiinterface.h \
	gui/guirobot.h \
	gui/guiscene.h \
	gui/guisidelines.h \
        gui/joystick.h \
	gui/mainwindow.h \
	gui/objectposition.h \
	gui/robotpanel.h \
	gui/selrobotpanel.h \
	include/config/globals.h \
	include/config/robot_types.h \
	include/config/simulated.h \
	include/config/team.h \
	include/config/tolerances.h \
	include/config/communication.h \
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
	model/gamemodel.h \
	model/robot.h \
	movement/go_to_pose.h \
	movement/move_collisions.h \
	movement/movetype.h \
	movement/pathfinding/fppa_pathfinding.h \
	skill/kick.h \
	skill/kicktopointomni.h \
	skill/skill.h \
	skill/stop.h \
	strategy/freekickstrategy.h \
	strategy/haltstrategy.h \
	strategy/kickoffstrategy.h \
	strategy/normalgamestrategy.h \
	strategy/penaltystrategy.h \
	strategy/stopstrategy.h \
	strategy/strategycontroller.h \
	strategy/strategy.h \
	strategy/teststrategy.h \
	strategy/videostrategies.h \
	utilities/comparisons.h \
	utilities/debug.h \
	utilities/edges.h \
	utilities/point.h \
	utilities/velocitycalculator.h \
        strategy/indirectkickstrategy.h \
        communication/kfball.h \
        utilities/getclassname.h \
        utilities/region/sector.h \
        utilities/region/rectangle.h \
        utilities/region/region.h \
        utilities/measurements.h \
        utilities/region/defencearea.h \
        gui/guidrawpoint.h \
        gui/guidrawregion.h \
        skill/dribbletopoint.h \
        behavior/challengeballbot.h \
        skill/dribbleback.h \
        behavior/refstop.h \
        behavior/goalie.h \
        behavior/markbot.h \
        behavior/wall.h \
        behavior/penaltygoalie.h \
        communication/yisirobcomm.h \
        communication/crc.h \
        movement/differential/differential_pilot.h \
        movement/three_wheel_omni/three_wheel_omni_pilot.h \
        movement/four_wheel_omni/four_wheel_omni_pilot.h

QMAKE_CXXFLAGS += -std=c++0x

unix|win32: LIBS +=  -lprotobuf -lSDL2

OTHER_FILES += \
    gui/images/0.png \
    gui/images/1.png \
    gui/images/2.png \
    gui/images/3.png \
    gui/images/4.png \
    gui/images/5.png \
    gui/images/ball.png \
    gui/images/bot0.png \
    gui/images/bot1.png \
    gui/images/bot2.png \
    gui/images/bot3.png \
    gui/images/bot4.png \
    gui/images/bot5.png \
    gui/images/gear.png \
    gui/images/gear2.png \
    gui/images/go-next.png \
    gui/images/slider-handle.png \
    RoboBulls.supp

RESOURCES += \
    gui/images.qrc

FORMS += \
    gui/mainwindow.ui
