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
         src/ssl/proto/messages_robocup_ssl_detection.proto \ #sss
         src/ssl/proto/messages_robocup_ssl_geometry.proto \
         src/ssl/proto/messages_robocup_ssl_refbox_log.proto \
         src/ssl/proto/messages_robocup_ssl_wrapper.proto \
         src/ssl/proto/ssl_referee.proto \ #sss
         src/ssl/proto/ssl_game_controller_auto_ref.proto \
         src/ssl/proto/ssl_game_controller_team.proto \
         src/ssl/proto/ssl_game_event_2019.proto \
         src/ssl/proto/ssl_game_event.proto \ #no deps
         src/ssl/proto/ssl_game_controller_common.proto # no deps
include(protobuf.pri)



SOURCES += src/main.cpp \
        src/gui/data/gui_ball.cpp \
        src/gui/data/gui_field.cpp \
        src/gui/data/gui_game_state.cpp \
        src/gui/data/gui_robot.cpp \
        src/gui/data/gui_teams.cpp \
        src/gui/gui_interface.cpp \
        src/gui/main_window.cpp \
        src/gui/panels/panel_field.cpp \
        src/gui/panels/panel_game_info.cpp \
        src/gui/panels/panel_output.cpp \
        src/gui/panels/panel_selected_robot.cpp \
        src/gui/panels/panel_teams.cpp \
        src/gui/panels/panel_teams/frame_robot.cpp \
        src/gui/panels/panel_teams/tab_team.cpp \
        src/gui/style_sheets/color_palettes.cpp \
        src/gui/graphics/graphics_ball.cpp \
        src/gui/graphics/graphics_field.cpp \
        src/gui/graphics/graphics_line.cpp \
        src/gui/graphics/graphics_outter_field.cpp \
        src/gui/graphics/graphics_points.cpp \
        src/gui/graphics/graphics_polygon.cpp \
        src/gui/graphics/graphics_robot.cpp \
        src/gui/graphics/graphics_robot_label.cpp \
        src/model/ball.cpp \
        src/model/field.cpp \
        src/model/game_state.cpp \
        src/model/moving_object.cpp \
        src/robot/navigation/commands/CmdGoToPose.cpp \
        src/robot/navigation/drives/differential.cpp \
        src/robot/navigation/drives/omni_drive.cpp \
        src/robot/navigation/path_planning/fppa_pathfinding.cpp \
        src/robot/navigation/path_planning/move_collisions.cpp \
        src/robot/navigation/pilots/pilot_differential.cpp \
        src/robot/navigation/pilots/pilot_dummy.cpp \
        src/robot/navigation/pilots/pilot_omni.cpp \
        src/robot/navigation/robot_pilot.cpp \
        src/robot/robot.cpp \
        src/robot/robot_low_level_control.cpp \
        src/robot/robot_proxy.cpp \
        src/robot/robots/grsim/proxy_grsim.cpp \
        src/robot/robots/none/proxy_none.cpp \
        src/robot/robots/yisibot/proxy_yisibot.cpp \
        src/robot/robots/yisibot/crc.cpp \
        src/ssl/ssl_game_controller_listener.cpp \
        src/ssl/ssl_vision_listener.cpp \
        src/strategy/behavior.cpp \
        src/strategy/controllers/joystick/joystick.cpp \
        src/strategy/controllers/joystick/scontroller_joystick.cpp \
        src/strategy/controllers/none/scontroller_none.cpp \
        src/strategy/controllers/normal_game/scontroller_normal_game.cpp \
        src/strategy/controllers/normal_game/strategies/freekickstrategy.cpp \
        src/strategy/controllers/normal_game/strategies/haltstrategy.cpp \
        src/strategy/controllers/normal_game/strategies/indirectkickstrategy.cpp \
        src/strategy/controllers/normal_game/strategies/kickoffstrategy.cpp \
        src/strategy/controllers/normal_game/strategies/normalgamestrategy.cpp \
        src/strategy/controllers/normal_game/strategies/penaltystrategy.cpp \
        src/strategy/controllers/normal_game/strategies/stopstrategy.cpp \
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
        src/utilities/comparisons.cpp \
        src/utilities/debug.cpp \
        src/utilities/edges.cpp \
        src/utilities/my_kalman_filter.cpp \
        src/utilities/my_yaml.cpp \
        src/utilities/point.cpp \
        src/utilities/region/sector.cpp \
        src/utilities/region/rectangle.cpp \
        src/utilities/measurements.cpp \
        src/utilities/region/defencearea.cpp \
        src/model/team.cpp \
        src/parameters/motion_parameters.cpp

HEADERS += \
        src/gui/data/gui_ball.h \
        src/gui/data/gui_field.h \
        src/gui/data/gui_game_state.h \
        src/gui/data/gui_robot.h \
        src/gui/data/gui_teams.h \
        src/gui/gui_interface.h \
        src/gui/main_window.h \
        src/gui/panels/panel_field.h \
        src/gui/panels/panel_game_info.h \
        src/gui/panels/panel_output.h \
        src/gui/panels/panel_selected_robot.h \
        src/gui/panels/panel_teams.h \
        src/gui/panels/panel_teams/frame_robot.h \
        src/gui/panels/panel_teams/tab_team.h \
        src/gui/style_sheets/color_palettes.h \
        src/gui/graphics/graphics_ball.h \
        src/gui/graphics/graphics_field.h \
        src/gui/graphics/graphics_line.h \
        src/gui/graphics/graphics_outter_field.h \
        src/gui/graphics/graphics_points.h \
        src/gui/graphics/graphics_polygon.h \
        src/gui/graphics/graphics_robot.h \
        src/gui/graphics/graphics_robot_label.h \
        src/model/ball.h \
        src/model/constants.h \
        src/model/field.h \
        src/model/game_state.h \
        src/model/moving_object.h \
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
        src/robot/robot.h \
        src/robot/robot_low_level_controls.h \
        src/robot/robot_proxy.h \
        src/robot/robots/grsim/proxy_grsim.h \
        src/robot/robots/none/proxy_none.h \
        src/robot/robots/yisibot/proxy_yisibot.h \
        src/robot/robots/yisibot/crc.h \
        src/ssl/ssl_game_controller_listener.h \
        src/ssl/ssl_vision_listener.h \
        src/strategy/behavior.h \
        src/strategy/controllers/joystick/joystick.h \
        src/strategy/controllers/joystick/scontroller_joystick.h \
        src/strategy/controllers/none/scontroller_none.h \
        src/strategy/controllers/normal_game/normal_game_roles.h \
        src/strategy/controllers/normal_game/scontroller_normal_game.h \
        src/strategy/controllers/normal_game/strategies/freekickstrategy.h \
        src/strategy/controllers/normal_game/strategies/haltstrategy.h \
        src/strategy/controllers/normal_game/strategies/indirectkickstrategy.h \
        src/strategy/controllers/normal_game/strategies/kickoffstrategy.h \
        src/strategy/controllers/normal_game/strategies/normalgamestrategy.h \
        src/strategy/controllers/normal_game/strategies/penaltystrategy.h \
        src/strategy/controllers/normal_game/strategies/stopstrategy.h \
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
        src/utilities/circular_buffer.h \
        src/utilities/comparisons.h \
        src/utilities/debug.h \
        src/utilities/edges.h \
        src/utilities/my_kalman_filter.h \
        src/utilities/my_yaml.h \
        src/utilities/point.h \
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
    src/gui/main_window.ui \
    src/gui/panels/panel_field.ui \
    src/gui/panels/panel_game_info.ui \
    src/gui/panels/panel_output.ui \
    src/gui/panels/panel_selected_robot.ui \
    src/gui/panels/panel_teams.ui \
    src/gui/panels/panel_teams/frame_robot.ui \
    src/gui/panels/panel_teams/tab_team.ui



DISTFILES += \
    src/robot/robots/grsim/proto/grSim_Commands.proto \
    src/robot/robots/grsim/proto/grSim_Packet.proto \
    src/robot/robots/grsim/proto/grSim_Replacement.proto \
    src/ssl/proto/not used/messages_robocup_ssl_detection_tracked.proto \
    src/ssl/proto/not used/messages_robocup_ssl_geometry_legacy.proto \
    src/ssl/proto/not used/messages_robocup_ssl_wrapper_legacy.proto \
    src/ssl/proto/not used/messages_robocup_ssl_wrapper_tracked.proto \
    src/ssl/proto/messages_robocup_ssl_detection.proto \
    src/ssl/proto/messages_robocup_ssl_geometry.proto \
    src/ssl/proto/messages_robocup_ssl_refbox_log.proto \
    src/ssl/proto/messages_robocup_ssl_wrapper.proto \
    src/ssl/proto/ssl_game_controller_auto_ref.proto \
    src/ssl/proto/ssl_game_controller_common.proto \
    src/ssl/proto/ssl_game_controller_team.proto \
    src/ssl/proto/ssl_game_event.proto \
    src/ssl/proto/ssl_game_event_2019.proto \
    src/ssl/proto/ssl_referee.proto





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





