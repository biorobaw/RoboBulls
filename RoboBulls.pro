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
	include/serialib.cpp \
    libs/grsim/src/messages/grSim_Commands.pb.cc \
    libs/grsim/src/messages/grSim_Packet.pb.cc \
    libs/grsim/src/messages/grSim_Replacement.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_detection.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_detection_tracked.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_geometry.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_geometry_legacy.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_refbox_log.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_wrapper.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_wrapper_legacy.pb.cc \
    libs/ssl-vision/src/messages/messages_robocup_ssl_wrapper_tracked.pb.cc \
    libs/ssl-vision/src/netraw.cpp \
    libs/ssl-vision/src/robocup_ssl_client.cpp \
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
    model/team.cpp \
    include/field.cpp \
    include/motion_parameters.cpp

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
	include/config/tolerances.h \
	include/serialib.h \
    libs/grsim/include/messages/grSim_Commands.pb.h \
    libs/grsim/include/messages/grSim_Packet.pb.h \
    libs/grsim/include/messages/grSim_Replacement.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_detection.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_detection_tracked.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_geometry.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_geometry_legacy.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_refbox_log.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_wrapper.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_wrapper_legacy.pb.h \
    libs/ssl-vision/include/messages/messages_robocup_ssl_wrapper_tracked.pb.h \
    libs/ssl-vision/include/netraw.h \
    libs/ssl-vision/include/robocup_ssl_client.h \
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
        movement/four_wheel_omni/four_wheel_omni_pilot.h \
    model/team.h \
    include/game_constants.h \
    include/field.h \
    include/motion_parameters.h



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

unix:!macx: LIBS += -L$$PWD/libs/yaml-cpp-0.6.3/build/ -lyaml-cpp

INCLUDEPATH += $$PWD/libs/yaml-cpp-0.6.3/include
DEPENDPATH += $$PWD/libs/yaml-cpp-0.6.3/include

#unix:!macx: PRE_TARGETDEPS += $$PWD/libs/yaml-cpp-0.6.3/build/libyaml-cpp.a

DISTFILES += \
    libs/ssl-vision/proto/compile.sh \
    libs/ssl-vision/proto/messages_robocup_ssl_detection.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_detection_tracked.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_geometry.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_geometry_legacy.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_refbox_log.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_wrapper.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_wrapper_legacy.proto \
    libs/ssl-vision/proto/messages_robocup_ssl_wrapper_tracked.proto \
    libs/yaml-cpp-0.6.3/CMakeLists.txt \
    libs/yaml-cpp-0.6.3/CONTRIBUTING.md \
    libs/yaml-cpp-0.6.3/LICENSE \
    libs/yaml-cpp-0.6.3/README.md \
    libs/yaml-cpp-0.6.3/install.txt \
    libs/yaml-cpp-0.6.3/src/contrib/yaml-cpp.natvis \
    libs/yaml-cpp-0.6.3/src/contrib/yaml-cpp.natvis.md \
    libs/yaml-cpp-0.6.3/test/CMakeLists.txt \
    libs/yaml-cpp-0.6.3/test/create-emitter-tests.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/CMakeLists.txt \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/README.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/appveyor.yml \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/CHANGES \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/CMakeLists.txt \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/CONTRIBUTORS \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/LICENSE \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/README.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/configure.ac \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/CheatSheet.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/CookBook.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/DesignDoc.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/DevGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/ForDummies.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/FrequentlyAskedQuestions.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/KnownIssues.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_5/CheatSheet.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_5/CookBook.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_5/Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_5/ForDummies.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_5/FrequentlyAskedQuestions.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_6/CheatSheet.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_6/CookBook.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_6/Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_6/ForDummies.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_6/FrequentlyAskedQuestions.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_7/CheatSheet.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_7/CookBook.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_7/Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_7/ForDummies.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/docs/v1_7/FrequentlyAskedQuestions.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/include/gmock/gmock-generated-nice-strict.h.pump \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2005/gmock.sln \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2005/gmock.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2005/gmock_config.vsprops \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2005/gmock_main.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2005/gmock_test.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2010/gmock.sln \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2010/gmock.vcxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2010/gmock_config.props \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2010/gmock_main.vcxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2010/gmock_test.vcxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2015/gmock.sln \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2015/gmock.vcxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2015/gmock_config.props \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2015/gmock_main.vcxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/msvc/2015/gmock_test.vcxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/fuse_gmock_files.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/LICENSE \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/README \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/README.cppclean \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/__init__.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/ast.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/gmock_class.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/gmock_class_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/keywords.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/tokenize.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/cpp/utils.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/generator/gmock_gen.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/gmock-config.in \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/gmock_doctor.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/upload.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/scripts/upload_gmock.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/test/gmock_leak_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/test/gmock_output_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/test/gmock_output_test_golden.txt \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googlemock/test/gmock_test_utils.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/CHANGES \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/CMakeLists.txt \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/CONTRIBUTORS \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/LICENSE \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/README.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/cmake/internal_utils.cmake \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/codegear/gtest.cbproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/codegear/gtest.groupproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/codegear/gtest_main.cbproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/codegear/gtest_unittest.cbproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/configure.ac \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/AdvancedGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/DevGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/FAQ.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/Primer.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/PumpManual.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/Samples.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_5_AdvancedGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_5_Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_5_FAQ.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_5_Primer.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_5_PumpManual.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_5_XcodeGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_AdvancedGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_FAQ.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_Primer.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_PumpManual.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_Samples.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_6_XcodeGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_AdvancedGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_Documentation.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_FAQ.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_Primer.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_PumpManual.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_Samples.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/V1_7_XcodeGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/docs/XcodeGuide.md \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/include/gtest/internal/gtest-param-util-generated.h.pump \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/include/gtest/internal/gtest-type-util.h.pump \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/m4/acx_pthread.m4 \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/m4/gtest.m4 \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest-md.sln \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest-md.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest.sln \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest_main-md.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest_main.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest_prod_test-md.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest_prod_test.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest_unittest-md.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/msvc/gtest_unittest.vcproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/common.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/fuse_gtest_files.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/gen_gtest_pred_impl.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/gtest-config.in \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/pump.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/release_docs.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/upload.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/scripts/upload_gtest.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_break_on_failure_unittest.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_catch_exceptions_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_color_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_env_var_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_filter_unittest.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_help_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_list_tests_unittest.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_output_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_output_test_golden_lin.txt \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_shuffle_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_test_utils.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_throw_on_failure_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_uninitialized_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_xml_outfiles_test.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_xml_output_unittest.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/test/gtest_xml_test_utils.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Config/FrameworkTarget.xcconfig \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Config/General.xcconfig \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Config/StaticLibraryTarget.xcconfig \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Config/TestTarget.xcconfig \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Resources/Info.plist \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Samples/FrameworkSample/Info.plist \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Samples/FrameworkSample/WidgetFramework.xcodeproj/project.pbxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Samples/FrameworkSample/runtests.sh \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Scripts/runtests.sh \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/Scripts/versiongenerate.py \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/googletest/xcode/gtest.xcodeproj/project.pbxproj \
    libs/yaml-cpp-0.6.3/test/gtest-1.8.0/travis.sh \
    libs/yaml-cpp-0.6.3/util/CMakeLists.txt \
    libs/yaml-cpp-0.6.3/yaml-cpp-config-version.cmake.in \
    libs/yaml-cpp-0.6.3/yaml-cpp-config.cmake.in \
    libs/yaml-cpp-0.6.3/yaml-cpp.pc.cmake
