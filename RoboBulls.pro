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
    behavior/sendballtoregion.cpp \
    strategy/twovone.cpp \
    skill/kicktopoint.cpp \
    utilities/region.cpp \
    movement/basic_movement.cpp \
    movement/gotopositionwithorientation.cpp \
    movement/pathfinding/fppa_pathfinding.cpp \
    movement/move.cpp \
    movement/differential/closedloopcontrol.cpp \
    include/config/robot_types.cpp \
    behavior/genericmovementbehavior.cpp \
    skill/stop.cpp \
    movement/four_omni_motion/omni4_velcalculator.cpp \
    movement/three_omni_motion/omni3_velcalculator.cpp \
    movement/differential/differential_velcalculator.cpp \
    strategy/freekickstrategy.cpp \
    strategy/haltstrategy.cpp \
    strategy/indiectkickstrategy.cpp \
    strategy/kickoffstrategy.cpp \
    behavior/simplebehaviors.cpp \
    strategy/attackstrategy.cpp \
    behavior/attackbehaviors.cpp \
    gui/guiball.cpp \
    gui/guibotlabel.cpp \
    gui/guicomm.cpp \
    gui/guifield.cpp \
    gui/guirobot.cpp \
    gui/guisidelines.cpp \
    gui/mainwindow.cpp \
    gui/guidrawline.cpp \
    gui/guiscene.cpp \
    gui/robotpanel.cpp

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
    include/config/model_averages.h \
    strategy/freekickstrategy.h \
    strategy/haltstrategy.h \
    strategy/indiectkickstrategy.h \
    strategy/kickoffstrategy.h \
    behavior/simplebehaviors.h \
    strategy/attackstrategy.h \
    behavior/attackbehaviors.h \
    gui/guiball.h \
    gui/guibotlabel.h \
    gui/guicomm.h \
    gui/guifield.h \
    gui/guirobot.h \
    gui/guisidelines.h \
    gui/mainwindow.h \
    gui/guidrawline.h \
    gui/guiscene.h \
    gui/robotpanel.h

QMAKE_CXXFLAGS += -std=c++0x

unix|win32: LIBS += -lprotobuf

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
