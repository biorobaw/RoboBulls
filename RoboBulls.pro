#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T09:39:34
#
#-------------------------------------------------

QT       += core network

QT       -= gui

QT      += widgets

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
	behavior/attackmain.cpp \
	behavior/attacksupport.cpp \
	behavior/behavior.cpp \
	behavior/defendbehavior.cpp \
	behavior/defendfarfromball.cpp \
	behavior/driveballandkick.cpp \
	behavior/genericmovementbehavior.cpp \
	behavior/kicktogoal.cpp \
	behavior/passballreceiver.cpp \
	behavior/passballsender.cpp \
	behavior/penaltybehavior.cpp \
	behavior/sendballtoregion.cpp \
	behavior/simplebehaviors.cpp \
	communication/nxtrobcomm.cpp \
	communication/refcomm.cpp \
	communication/robcomm.cpp \
	communication/simrobcomm.cpp \
	communication/visioncomm.cpp \
	gui/fieldpanel.cpp \
	gui/gamepanel.cpp \
	gui/getbehavior.cpp \
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
	include/config/tolerances.cpp \
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
	movement/differential/closedloopcontrol.cpp \
	movement/differential/differential_velcalculator.cpp \
	movement/four_omni_motion/omni4_velcalculator.cpp \
	movement/gotopositionwithorientation.cpp \
	movement/move_collisions.cpp \
	movement/move.cpp \
	movement/move_randomization.cpp \
	movement/pathfinding/fppa_pathfinding.cpp \
	movement/three_omni_motion/omni3_velcalculator.cpp \
	skill/driveball.cpp \
	skill/kick.cpp \
	skill/kicktopoint.cpp \
	skill/kicktopointomni.cpp \
	skill/stop.cpp \
	strategy/freekickstrategy.cpp \
	strategy/haltstrategy.cpp \
	strategy/indiectkickstrategy.cpp \
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
	utilities/measurments.cpp \
	utilities/point.cpp \
	utilities/region.cpp \
	utilities/velocitycalculator.cpp

HEADERS += \
	behavior/attackmain.h \
	behavior/attacksupport.h \
	behavior/behaviorassignment.h \
	behavior/behavior.h \
	behavior/defendbehavior.h \
	behavior/defendfarfromball.h \
	behavior/driveballandkick.h \
	behavior/genericmovementbehavior.h \
	behavior/genericskillbehavior.h \
	behavior/kicktogoal.h \
	behavior/passballreceiver.h \
	behavior/passballsender.h \
	behavior/penaltybehavior.h \
	behavior/rotateonpoint.h \
	behavior/sendballtoregion.h \
	behavior/simplebehaviors.h \
	communication/nxtrobcomm.h \
	communication/refcomm.h \
	communication/robcomm.h \
	communication/simrobcomm.h \
	communication/visioncomm.h \
	gui/fieldpanel.h \
	gui/gamepanel.h \
	gui/getbehavior.h \
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
	movement/differential/closedloopcontrol.h \
	movement/differential/differential_velcalculator.h \
	movement/four_omni_motion/omni4_velcalculator.h \
	movement/gotoposition.h \
	movement/gotopositionwithorientation.h \
	movement/move_collisions.h \
	movement/move.h \
	movement/move_randomization.h \
	movement/movetype.h \
	movement/pathfinding/fppa_pathfinding.h \
	movement/three_omni_motion/matrixcalculator.h \
	movement/three_omni_motion/omni3_velcalculator.h \
	skill/driveball.h \
	skill/kick.h \
	skill/kicktopoint.h \
	skill/kicktopointomni.h \
	skill/skill.h \
	skill/stop.h \
	strategy/freekickstrategy.h \
	strategy/haltstrategy.h \
	strategy/indiectkickstrategy.h \
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
	utilities/measurments.h \
	utilities/point.h \
	utilities/region.h \
	utilities/velocitycalculator.h

QMAKE_CXXFLAGS += -std=c++0x

unix|win32: LIBS += -lprotobuf -lSDL2

OTHER_FILES += \
    gui/images/field.jpg \
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
    gui/images/bubble.png \
    gui/images/button.png \
    gui/images/button-pressed.png \
    gui/images/document-open.png \
    gui/images/document-open@2x.png \
    gui/images/document-save-as.png \
    gui/images/document-save-as@2x.png \
    gui/images/folder_new.png \
    gui/images/gear.png \
    gui/images/gear2.png \
    gui/images/go-next.png \
    gui/images/go-previous.png \
    gui/images/preferences-system.png \
    gui/images/process-stop.png \
    gui/images/progress-background.png \
    gui/images/progress-fill.png \
    gui/images/slider-handle.png \
    gui/images/tab.png \
    gui/images/tab_selected.png \
    gui/images/textfield.png \
    gui/images/toplevel_window.png \
    gui/images/view-refresh.png \
    gui/images/window-new.png \
    gui/images/window-new@2x.png

RESOURCES += \
    gui/images.qrc

FORMS += \
    gui/mainwindow.ui
