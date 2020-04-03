#-------------------------------------------------

#
# Project created by QtCreator 2014-03-05T09:39:34
#
#-------------------------------------------------


QT_SELECT=5
QT += core gui network widgets concurrent serialport

TARGET = RoboBulls
CONFIG += console
CONFIG += -j
CONFIG -= app_bundle
CONFIG += c++17
CONFIG += static

TEMPLATE = app
#QMAKE_CXX = g++
#QMAKE_CXXFLAGS += -std=c++17
DESTDIR = $$PWD/bin
INCLUDEPATH += src



PROTOS += src/robot/robots/grsim/proto/grSim_Commands.proto \
         src/robot/robots/grsim/proto/grSim_Packet.proto \
         src/robot/robots/grsim/proto/grSim_Replacement.proto \
         src/ssl-vision/proto/messages_robocup_ssl_detection.proto \ #sss
         src/ssl-vision/proto/messages_robocup_ssl_geometry.proto \
         src/ssl-vision/proto/messages_robocup_ssl_refbox_log.proto \
         src/ssl-vision/proto/messages_robocup_ssl_wrapper.proto \
         src/ssl-game-controller/proto/ssl_referee.proto \ #sss
         src/ssl-game-controller/proto/ssl_game_controller_auto_ref.proto \
         src/ssl-game-controller/proto/ssl_game_controller_team.proto \
         src/ssl-game-controller/proto/ssl_game_event_2019.proto \
         src/ssl-game-controller/proto/ssl_game_event.proto \ #no deps
         src/ssl-game-controller/proto/ssl_game_controller_common.proto # no deps
include(protobuf.pri)



SOURCES += src/main.cpp \
        src/gui/data/gui_robot.cpp \
        src/gui/data/guiball.cpp \
        src/gui/data/guibotlabel.cpp \
        src/gui/data/guifield.cpp \
        src/gui/guicomm.cpp \
        src/gui/guiinterface.cpp \
        src/gui/guiscene.cpp \
        src/gui/guisidelines.cpp \
        src/gui/mainwindow.cpp \
        src/gui/panels/fieldpanel.cpp \
        src/gui/panels/gamepanel.cpp \
        src/gui/panels/robotpanel.cpp \
        src/gui/panels/selrobotpanel.cpp \
        src/gui/utils/gui_robot_drawer.cpp \
        src/gui/utils/guidrawline.cpp \
        src/gui/utils/guidrawpoint.cpp \
        src/gui/utils/guidrawregion.cpp \
        src/model/ball.cpp \
        src/model/field.cpp \
        src/model/game_state.cpp \
        src/robot/navigation/commands/CmdGoToPose.cpp \
        src/robot/navigation/drives/differential.cpp \
        src/robot/navigation/drives/omni_drive.cpp \
        src/robot/navigation/path_planning/fppa_pathfinding.cpp \
        src/robot/navigation/path_planning/move_collisions.cpp \
        src/robot/navigation/pilots/pilot_differential.cpp \
        src/robot/navigation/pilots/pilot_dummy.cpp \
        src/robot/navigation/pilots/pilot_omni.cpp \
        src/robot/navigation/robot_pilot.cpp \
        src/robot/robcomm.cpp \
        src/robot/robot.cpp \
        src/robot/robots/grsim/robcomm_grsim.cpp \
        src/robot/robots/grsim/robot_grsim.cpp \
        src/robot/robots/none/robotnone.cpp \
        src/robot/robots/yisibot/robcomm_yisibot.cpp \
        src/robot/robots/yisibot/robot_yisibot.cpp \
        src/robot/robots/yisibot/crc.cpp \
        src/ssl-game-controller/sss_refbox_listener.cpp \
        src/ssl-vision/my_kalman_filter.cpp \
        src/ssl-vision/ssl_vision_listener.cpp \
        src/strategy/behavior.cpp \
        src/strategy/controllers/joystick/joystick.cpp \
        src/strategy/controllers/joystick/scontroller_joystick.cpp \
        src/strategy/controllers/scontroller_normal_game.cpp \
        src/strategy/controllers/strategy_tester/scontroller_strategy_tester.cpp \
        src/strategy/controllers/strategy_tester/strategies/test_strategy.cpp \
        src/strategy/strategy.cpp \
        src/strategy/strategycontroller.cpp \
        src/strategy/behaviors/attackmain.cpp \
        src/strategy/behaviors/attacksupport.cpp \
        src/strategy/behaviors/genericmovementbehavior.cpp \
        src/strategy/behaviors/challengeballbot.cpp \
        src/strategy/behaviors/defendbehavior.cpp \
        src/strategy/behaviors/goalie.cpp \
        src/strategy/behaviors/markbot.cpp \
        src/strategy/behaviors/penaltygoalie.cpp \
        src/strategy/behaviors/refstop.cpp \
        src/strategy/behaviors/wall.cpp \
        src/strategy/skills/dribbletopoint.cpp \
        src/strategy/skills/dribbleback.cpp \
        src/strategy/skills/kick.cpp \
        src/strategy/skills/kicktopointomni.cpp \
        src/strategy/skills/stop.cpp \
        src/strategy/strategies/freekickstrategy.cpp \
        src/strategy/strategies/haltstrategy.cpp \
        src/strategy/strategies/indirectkickstrategy.cpp \
        src/strategy/strategies/kickoffstrategy.cpp \
        src/strategy/strategies/normalgamestrategy.cpp \
        src/strategy/strategies/penaltystrategy.cpp \
        src/strategy/strategies/stopstrategy.cpp \
        src/utilities/comparisons.cpp \
        src/utilities/debug.cpp \
        src/utilities/edges.cpp \
        src/utilities/point.cpp \
        src/utilities/velocitycalculator.cpp \
        src/utilities/region/sector.cpp \
        src/utilities/region/rectangle.cpp \
        src/utilities/measurements.cpp \
        src/utilities/region/defencearea.cpp \
        src/model/team.cpp \
        src/parameters/motion_parameters.cpp

HEADERS += \
        src/gui/data/gui_robot.h \
        src/gui/data/guiball.h \
        src/gui/data/guibotlabel.h \
        src/gui/data/guifield.h \
        src/gui/guicomm.h \
        src/gui/guiinterface.h \
        src/gui/guiscene.h \
        src/gui/guisidelines.h \
        src/gui/mainwindow.h \
        src/gui/panels/fieldpanel.h \
        src/gui/panels/gamepanel.h \
        src/gui/panels/robotpanel.h \
        src/gui/panels/selrobotpanel.h \
        src/gui/utils/gui_robot_drawer.h \
        src/gui/utils/guidrawline.h \
        src/gui/utils/guidrawpoint.h \
        src/gui/utils/guidrawregion.h \
        src/model/ball.h \
        src/model/constants.h \
        src/model/field.h \
        src/model/game_state.h \
        src/model/team.h \
        src/robot/navigation/commands/CmdGoToPose.h \
        src/robot/navigation/drives/differential.h \
        src/robot/navigation/drives/omni_drive.h \
        src/robot/navigation/path_planning/fppa_pathfinding.h \
        src/robot/navigation/path_planning/move_collisions.h \
        src/robot/navigation/pilots/pilot_differential.h \
        src/robot/navigation/pilots/pilot_dummy.h \
        src/robot/navigation/pilots/pilot_omni.h \
        src/robot/navigation/robot_pilot.h \
        src/robot/robcomm.h \
        src/robot/robot.h \
        src/robot/robots/grsim/robcomm_grsim.h \
        src/robot/robots/grsim/robot_grsim.h \
        src/robot/robots/none/robotnone.h \
        src/robot/robots/yisibot/robcomm_yisibot.h \
        src/robot/robots/yisibot/robot_yisibot.h \
        src/robot/robots/yisibot/crc.h \
        src/ssl-game-controller/ssl_referee_includes.h \
        src/strategy/behavior.h \
        src/strategy/controllers/joystick/joystick.h \
        src/strategy/controllers/joystick/scontroller_joystick.h \
        src/strategy/controllers/scontroller_normal_game.h \
        src/strategy/controllers/strategy_tester/scontroller_strategy_tester.h \
        src/strategy/controllers/strategy_tester/strategies/test_strategy.h \
        src/strategy/skill.h \
        src/strategy/strategy.h \
        src/strategy/strategycontroller.h \
        src/strategy/behaviors/attackmain.h \
        src/strategy/behaviors/attacksupport.h \
        src/strategy/behaviors/challengeballbot.h \
        src/strategy/behaviors/defendbehavior.h \
        src/strategy/behaviors/genericmovementbehavior.h \
        src/strategy/behaviors/genericskillbehavior.h \
        src/strategy/behaviors/goalie.h \
        src/strategy/behaviors/markbot.h \
        src/strategy/behaviors/penaltygoalie.h \
        src/strategy/behaviors/refstop.h \
        src/strategy/behaviors/wall.h \
        src/strategy/skills/dribbleback.h \
        src/strategy/skills/dribbletopoint.h \
        src/strategy/skills/kick.h \
        src/strategy/skills/kicktopointomni.h \
        src/strategy/skills/stop.h \
        src/strategy/strategies/freekickstrategy.h \
        src/strategy/strategies/haltstrategy.h \
        src/strategy/strategies/indirectkickstrategy.h \
        src/strategy/strategies/kickoffstrategy.h \
        src/strategy/strategies/normalgamestrategy.h \
        src/strategy/strategies/penaltystrategy.h \
        src/strategy/strategies/stopstrategy.h \
        src/ssl-game-controller/sss_refbox_listener.h \
        src/ssl-vision/my_kalman_filter.h \
        src/ssl-vision/ssl_vision_listener.h \
        src/utilities/circular_buffer.h \
        src/utilities/comparisons.h \
        src/utilities/debug.h \
        src/utilities/edges.h \
        src/utilities/point.h \
        src/utilities/velocitycalculator.h \
        src/utilities/region/sector.h \
        src/utilities/region/rectangle.h \
        src/utilities/region/region.h \
        src/utilities/measurements.h \
        src/utilities/region/defencearea.h \
        src/parameters/motion_parameters.h





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



DISTFILES += \
    src/robot/robots/grsim/proto/grSim_Commands.proto \
    src/robot/robots/grsim/proto/grSim_Packet.proto \
    src/robot/robots/grsim/proto/grSim_Replacement.proto \
    src/ssl-game-controller/proto/ssl_game_controller_auto_ref.proto \
    src/ssl-game-controller/proto/ssl_game_controller_common.proto \
    src/ssl-game-controller/proto/ssl_game_controller_team.proto \
    src/ssl-game-controller/proto/ssl_game_event.proto \
    src/ssl-game-controller/proto/ssl_game_event_2019.proto \
    src/ssl-game-controller/proto/ssl_referee.proto \
    src/ssl-vision/proto/messages_robocup_ssl_detection.proto \
    src/ssl-vision/proto/messages_robocup_ssl_geometry.proto \
    src/ssl-vision/proto/messages_robocup_ssl_refbox_log.proto \
    src/ssl-vision/proto/messages_robocup_ssl_wrapper.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_detection_tracked.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_geometry_legacy.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_wrapper_legacy.proto \
    src/ssl-vision/proto/not used/messages_robocup_ssl_wrapper_tracked.proto





#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += protobuf





#INCLUDEPATH += $$PWD/libs/kalman
#DEPENDPATH += $$PWD/libs/kalman
#INCLUDEPATH += $$PWD/libs
#DEPENDPATH += $$PWD/libs


INCLUDEPATH += $$PWD/libs/include
DEPENDPATH += $$PWD/libs/include


win32{

    #add libra
    INCLUDEPATH += $$PWD/libs/windows/include
    DEPENDPATH += $$PWD/libs/windows/include
    Release: LIBS += -L$$PWD/libs/windows/x64/release/lib/ -llibprotobuf -lyaml-cpp -lSDL2 # -llibKalman
#    Release: PRE_TARGETDEPS += $$PWD/libs/windows/x64/release/lib/libKalman.lib

    Debug: LIBS += -L$$PWD/libs/windows/x64/debug/lib/ -llibprotobufd -lyaml-cpp -lSDL2d -llibKalman
#    Debug: PRE_TARGETDEPS += $$PWD/libs/windows/x64/debug/lib/libKalman.lib
#    Debug: PRE_TARGETDEPS += $$PWD/libs/windows/x64/debug/lib/libprotobufd.lib

    # copy libraries to DESTDIR folder
    #Following lines will copy QT libraries to DESTDIT
    TARGET_CUSTOM_EXT = .exe

    ADD_QT_BIN_PATH = set PATH=$$shell_quote($$shell_path($$[QT_INSTALL_BINS]));%PATH%
    DEPLOY_TARGET = $$shell_quote($$shell_path($${DESTDIR}/$${TARGET}$${TARGET_CUSTOM_EXT}))
    DEPLOY_COMMAND = $$ADD_QT_BIN_PATH && $$shell_quote($$shell_path(windeployqt))
    warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}

    #Following lines will copy dlls to DESTDITR
    Release: install_dlls.files = $$PWD/libs/windows/x64/release/bin/yaml-cpp.dll \
                                  $$PWD/libs/windows/x64/release/bin/SDL2.dll \
                                  $$PWD/libs/windows/x64/release/bin/libprotobuf.dll
    Debug: install_dlls.files = $$PWD/libs/windows/x64/debug/bin/yaml-cpp.dll \
                                $$PWD/libs/windows/x64/debug/bin/SDL2d.dll \
                                $$PWD/libs/windows/x64/debug/bin/libprotobufd.dll
    install_dlls.path  = $$DESTDIR
    INSTALLS += install_dlls


    # clean step:
    QMAKE_DISTCLEAN += /s /f /q $$DESTDIR/* && rd /s /q $$DESTDIR/*
    QMAKE_DEL_FILE = del /q
    QMAKE_DEL_DIR  = rmdir /s /q

}


#Debug: LIBS += -L$$PWD/libs/linux/lib -llibprotobufd -lyaml-cpp -lSDL2d
#unix:!macx:
#unix:!macx: INCLUDEPATH += $$PWD/libs/yaml-cpp-0.6.3/

#INCLUDEPATH += $$PWD/libs/yaml-cpp-old/include
#DEPENDPATH += $$PWD/libs/yaml-cpp-old/include
#LIBS += -L$$PWD/libs/yaml-cpp/build/ -lyaml-cpp
#INCLUDEPATH += /home/vision/Downloads/yaml-cpp-0.6.3/include
#DEPENDPATH += $$PWD/libs/yaml-cpp-old/include





unix:!macx {

    INCLUDEPATH += $$PWD/libs/linux/include
    DEPENDPATH += $$PWD/libs/linux/include
    LIBS += -L$$PWD/libs/linux/lib/ -lyaml-cpp -lprotobuf -lSDL2
    CONFIG += link_pkgconfig
    PKGCONFIG += protobuf

}



# copy config files to bin folder:
install_config_files.files = $$PWD/config/comm.yaml \
                             $$PWD/config/field.yaml \
                             $$PWD/config/motion.yaml \
                             $$PWD/config/team.yaml
install_config_files.path =  $$DESTDIR/config
INSTALLS += install_config_files





#QMAKE_DISTCLEAN += $$PWD/bin/
#QMAKE_DISTCLEAN += -r $$DESTDIR/





