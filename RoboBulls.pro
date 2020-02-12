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
QMAKE_CXXFLAGS += -std=c++0x
DESTDIR = $$PWD



PROTOS = src/ssl-grsim/proto/grSim_Commands.proto \
         src/ssl-grsim/proto/grSim_Packet.proto \
         src/ssl-grsim/proto/grSim_Replacement.proto \
         src/ssl-vision/proto/messages_robocup_ssl_detection.proto \
         src/ssl-vision/proto/messages_robocup_ssl_geometry.proto \
         src/ssl-vision/proto/messages_robocup_ssl_refbox_log.proto \
         src/ssl-vision/proto/messages_robocup_ssl_wrapper.proto
include(protobuf.pri)



SOURCES += src/main.cpp \
        src/behavior/attackmain.cpp \
        src/behavior/attacksupport.cpp \
        src/behavior/behavior.cpp \
        src/behavior/genericmovementbehavior.cpp \
        src/communication/nxtrobcomm.cpp \
        src/communication/robcomm.cpp \
        src/communication/serialib.cpp \
        src/communication/simrobcomm.cpp \
        src/gui/fieldpanel.cpp \
        src/gui/gamepanel.cpp \
        src/gui/guiball.cpp \
        src/gui/guibotlabel.cpp \
        src/gui/guicomm.cpp \
        src/gui/guidrawline.cpp \
        src/gui/guifield.cpp \
        src/gui/guiinterface.cpp \
        src/gui/guirobot.cpp \
        src/gui/guiscene.cpp \
        src/gui/guisidelines.cpp \
        src/gui/joystick.cpp \
        src/gui/mainwindow.cpp \
        src/gui/objectposition.cpp \
        src/gui/robotpanel.cpp \
        src/gui/selrobotpanel.cpp \
        src/model/ball.cpp \
        src/model/field.cpp \
        src/model/game_state.cpp \
        src/model/robot.cpp \
        src/movement/four_wheel_omni/four_wheel_omni_pilot.cpp \
        src/movement/move_collisions.cpp \
        src/movement/pathfinding/fppa_pathfinding.cpp \
        src/movement/three_wheel_omni/three_wheel_omni_pilot.cpp \
        src/skill/kick.cpp \
        src/skill/kicktopointomni.cpp \
        src/skill/stop.cpp \
        src/ssl-game-controller/sss_refbox_listener.cpp \
        src/ssl-vision/ssl_vision_listener.cpp \
        src/strategy/freekickstrategy.cpp \
        src/strategy/haltstrategy.cpp \
        src/strategy/kickoffstrategy.cpp \
        src/strategy/normalgamestrategy.cpp \
        src/strategy/penaltystrategy.cpp \
        src/strategy/stopstrategy.cpp \
        src/strategy/strategycontroller.cpp \
        src/strategy/strategy.cpp \
        src/strategy/teststrategy.cpp \
        src/strategy/videostrategies.cpp \
        src/utilities/comparisons.cpp \
        src/utilities/debug.cpp \
        src/utilities/edges.cpp \
        src/utilities/point.cpp \
        src/utilities/velocitycalculator.cpp \
        src/behavior/defendbehavior.cpp \
        src/strategy/indirectkickstrategy.cpp \
        src/communication/kfball.cpp \
        src/utilities/getclassname.cpp \
        src/utilities/region/sector.cpp \
        src/utilities/region/rectangle.cpp \
        src/utilities/measurements.cpp \
        src/utilities/region/defencearea.cpp \
        src/gui/guidrawpoint.cpp \
        src/gui/guidrawregion.cpp \
        src/skill/dribbletopoint.cpp \
        src/behavior/challengeballbot.cpp \
        src/skill/dribbleback.cpp \
        src/behavior/refstop.cpp \
        src/behavior/goalie.cpp \
        src/behavior/markbot.cpp \
        src/behavior/wall.cpp \
        src/behavior/penaltygoalie.cpp \
        src/communication/yisirobcomm.cpp \
        src/communication/crc.cpp \
        src/movement/go_to_pose.cpp \
        src/movement/differential/differential_pilot.cpp \
        src/model/team.cpp \
        src/parameters/motion_parameters.cpp

HEADERS += \
        src/behavior/attackmain.h \
        src/behavior/attacksupport.h \
        src/behavior/behavior.h \
        src/behavior/defendbehavior.h \
        src/behavior/genericmovementbehavior.h \
        src/behavior/genericskillbehavior.h \
        src/communication/nxtrobcomm.h \
        src/communication/robcomm.h \
        src/communication/serialib.h \
        src/communication/simrobcomm.h \
        src/gui/fieldpanel.h \
        src/gui/gamepanel.h \
        src/gui/guiball.h \
        src/gui/guibotlabel.h \
        src/gui/guicomm.h \
        src/gui/guidrawline.h \
        src/gui/guifield.h \
        src/gui/guiinterface.h \
        src/gui/guirobot.h \
        src/gui/guiscene.h \
        src/gui/guisidelines.h \
        src/gui/joystick.h \
        src/gui/mainwindow.h \
        src/gui/objectposition.h \
        src/gui/robotpanel.h \
        src/gui/selrobotpanel.h \
        src/model/ball.h \
        src/model/field.h \
        src/model/game_state.h \
        src/model/robot.h \
        src/movement/go_to_pose.h \
        src/movement/move_collisions.h \
        src/movement/movetype.h \
        src/movement/pathfinding/fppa_pathfinding.h \
        src/skill/kick.h \
        src/skill/kicktopointomni.h \
        src/skill/skill.h \
        src/skill/stop.h \
        src/ssl-game-controller/sss_refbox_listener.h \
        src/ssl-vision/ssl_vision_listener.h \
        src/strategy/freekickstrategy.h \
        src/strategy/haltstrategy.h \
        src/strategy/kickoffstrategy.h \
        src/strategy/normalgamestrategy.h \
        src/strategy/penaltystrategy.h \
        src/strategy/stopstrategy.h \
        src/strategy/strategycontroller.h \
        src/strategy/strategy.h \
        src/strategy/teststrategy.h \
        src/strategy/videostrategies.h \
        src/utilities/comparisons.h \
        src/utilities/debug.h \
        src/utilities/edges.h \
        src/utilities/point.h \
        src/utilities/velocitycalculator.h \
        src/strategy/indirectkickstrategy.h \
        src/communication/kfball.h \
        src/utilities/getclassname.h \
        src/utilities/region/sector.h \
        src/utilities/region/rectangle.h \
        src/utilities/region/region.h \
        src/utilities/measurements.h \
        src/utilities/region/defencearea.h \
        src/gui/guidrawpoint.h \
        src/gui/guidrawregion.h \
        src/skill/dribbletopoint.h \
        src/behavior/challengeballbot.h \
        src/skill/dribbleback.h \
        src/behavior/refstop.h \
        src/behavior/goalie.h \
        src/behavior/markbot.h \
        src/behavior/wall.h \
        src/behavior/penaltygoalie.h \
        src/communication/yisirobcomm.h \
        src/communication/crc.h \
        src/movement/differential/differential_pilot.h \
        src/movement/three_wheel_omni/three_wheel_omni_pilot.h \
        src/movement/four_wheel_omni/four_wheel_omni_pilot.h \
        src/model/team.h \
        src/parameters/motion_parameters.h



unix|win32: LIBS += -lSDL2

OTHER_FILES += \
    src/gui/images/0.png \
    src/gui/images/1.png \
    src/gui/images/2.png \
    src/gui/images/3.png \
    src/gui/images/4.png \
    src/gui/images/5.png \
    src/gui/images/ball.png \
    src/gui/images/bot0.png \
    src/gui/images/bot1.png \
    src/gui/images/bot2.png \
    src/gui/images/bot3.png \
    src/gui/images/bot4.png \
    src/gui/images/bot5.png \
    src/gui/images/gear.png \
    src/gui/images/gear2.png \
    src/gui/images/go-next.png \
    src/gui/images/slider-handle.png \
    src/RoboBulls.supp

RESOURCES += \
    src/gui/images.qrc

FORMS += \
    src/gui/mainwindow.ui

unix:!macx: LIBS += -L$$PWD/libs/yaml-cpp-0.6.3/build/ -lyaml-cpp

INCLUDEPATH += $$PWD/libs/yaml-cpp-0.6.3/include
DEPENDPATH += $$PWD/libs/yaml-cpp-0.6.3/include

#unix:!macx: PRE_TARGETDEPS += $$PWD/libs/yaml-cpp-0.6.3/build/libyaml-cpp.a

DISTFILES += \
    src/ssl-game-controller/proto/ssl_game_controller_auto_ref.proto \
    src/ssl-game-controller/proto/ssl_game_controller_common.proto \
    src/ssl-game-controller/proto/ssl_game_controller_team.proto \
    src/ssl-game-controller/proto/ssl_game_event.proto \
    src/ssl-game-controller/proto/ssl_game_event_2019.proto \
    src/ssl-game-controller/proto/ssl_referee.proto \
    src/ssl-grsim/proto/grSim_Commands.proto \
    src/ssl-grsim/proto/grSim_Packet.proto \
    src/ssl-grsim/proto/grSim_Replacement.proto \
    src/ssl-vision/proto/messages_robocup_ssl_detection.proto \
    src/ssl-vision/proto/messages_robocup_ssl_geometry.proto \
    src/ssl-vision/proto/messages_robocup_ssl_refbox_log.proto \
    src/ssl-vision/proto/messages_robocup_ssl_wrapper.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_detection_tracked.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_geometry_legacy.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_wrapper_legacy.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_wrapper_tracked.proto


unix:!macx: LIBS += -L$$PWD/libs/kalman/bin/ -lkalman

INCLUDEPATH += $$PWD/libs/kalman
INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs/kalman
DEPENDPATH += $$PWD/libs

unix:!macx: PRE_TARGETDEPS += $$PWD/libs/kalman/bin/libkalman.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += protobuf
