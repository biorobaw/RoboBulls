###########################################################################################
# Makefile for building: RoboBulls
# Project:  RoboBulls.pro
# Authors: Abdul Munim Zahid(@isMunim), Syed Tahmid Hasan(@tahmid53), Shaughn Seepaul(@sdseepaul)
# Command: cd to root robobulls directory
#	   make
###########################################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_NETWORK_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_SERIALPORT_LIB -DQT_CORE_LIB
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -m64 -pipe -std=c++0x -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I. -isystem /usr/include/x86_64-linux-gnu/qt5 -isystem /usr/include/x86_64-linux-gnu/qt5/QtNetwork -isystem /usr/include/x86_64-linux-gnu/qt5/QtWidgets -isystem /usr/include/x86_64-linux-gnu/qt5/QtGui -isystem /usr/include/x86_64-linux-gnu/qt5/QtSerialPort -isystem /usr/include/x86_64-linux-gnu/qt5/QtCore -I. -I. -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64
QMAKE         = /usr/lib/x86_64-linux-gnu/qt5/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = RoboBulls1.0.0
DISTDIR = /home/usr2-spring2018/robobulls2/.tmp/RoboBulls1.0.0
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS) -L/usr/X11R6/lib64 -lprotobuf -lSDL2 -lQt5Network -lQt5Widgets -lQt5Gui -lQt5SerialPort -lQt5Core -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
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
		movement/differential/differential_pilot.cpp qrc_images.cpp \
		moc_fieldpanel.cpp \
		moc_gamepanel.cpp \
		moc_guicomm.cpp \
		moc_guidrawline.cpp \
		moc_mainwindow.cpp \
		moc_robotpanel.cpp \
		moc_guidrawpoint.cpp \
		moc_guidrawregion.cpp
OBJECTS       = main.o \
		attackmain.o \
		attacksupport.o \
		behavior.o \
		genericmovementbehavior.o \
		nxtrobcomm.o \
		refcomm.o \
		robcomm.o \
		simrobcomm.o \
		visioncomm.o \
		fieldpanel.o \
		gamepanel.o \
		guiball.o \
		guibotlabel.o \
		guicomm.o \
		guidrawline.o \
		guifield.o \
		guiinterface.o \
		guirobot.o \
		guiscene.o \
		guisidelines.o \
		joystick.o \
		mainwindow.o \
		objectposition.o \
		robotpanel.o \
		selrobotpanel.o \
		robot_types.o \
		grSim_Commands.pb.o \
		grSim_Packet.pb.o \
		grSim_Replacement.pb.o \
		messages_robocup_ssl_detection.pb.o \
		messages_robocup_ssl_geometry.pb.o \
		messages_robocup_ssl_refbox_log.pb.o \
		messages_robocup_ssl_wrapper.pb.o \
		netraw.o \
		robocup_ssl_client.o \
		serialib.o \
		gamemodel.o \
		robot.o \
		four_wheel_omni_pilot.o \
		move_collisions.o \
		fppa_pathfinding.o \
		three_wheel_omni_pilot.o \
		kick.o \
		kicktopointomni.o \
		stop.o \
		freekickstrategy.o \
		haltstrategy.o \
		kickoffstrategy.o \
		normalgamestrategy.o \
		penaltystrategy.o \
		stopstrategy.o \
		strategycontroller.o \
		strategy.o \
		teststrategy.o \
		videostrategies.o \
		comparisons.o \
		debug.o \
		edges.o \
		point.o \
		velocitycalculator.o \
		defendbehavior.o \
		indirectkickstrategy.o \
		kfball.o \
		getclassname.o \
		sector.o \
		rectangle.o \
		measurements.o \
		defencearea.o \
		guidrawpoint.o \
		guidrawregion.o \
		dribbletopoint.o \
		challengeballbot.o \
		dribbleback.o \
		refstop.o \
		goalie.o \
		markbot.o \
		wall.o \
		penaltygoalie.o \
		yisirobcomm.o \
		crc.o \
		go_to_pose.o \
		differential_pilot.o \
		qrc_images.o \
		moc_fieldpanel.o \
		moc_gamepanel.o \
		moc_guicomm.o \
		moc_guidrawline.o \
		moc_mainwindow.o \
		moc_robotpanel.o \
		moc_guidrawpoint.o \
		moc_guidrawregion.o
DIST          = /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/linux.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/sanitize.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/qconfig.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_clucene_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designer.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designer_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designercomponents_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_help.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_help_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_script.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_script_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_scripttools.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_scripttools_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_serialport.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_svg.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uiplugin.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uitools.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uitools_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_webkit.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_webkitwidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_x11extras.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xmlpatterns.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_functions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_post.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resolve_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_post.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/warn_on.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resources.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/moc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/opengl.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/uic.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/thread.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/testcase_targets.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exceptions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/yacc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/lex.prf \
		RoboBulls.pro behavior/attackmain.h \
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
		movement/four_wheel_omni/four_wheel_omni_pilot.h main.cpp \
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
		movement/differential/differential_pilot.cpp
QMAKE_TARGET  = RoboBulls
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = RoboBulls


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET): ui_mainwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: RoboBulls.pro /usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf /usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/linux.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/sanitize.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-base.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-unix.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/qconfig.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_clucene_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designer.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designer_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designercomponents_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_help.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_help_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_script.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_script_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_scripttools.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_scripttools_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_serialport.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_svg.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uiplugin.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uitools.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uitools_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_webkit.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_webkitwidgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_x11extras.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xmlpatterns.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_functions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_post.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_pre.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resolve_config.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_post.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/warn_on.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resources.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/moc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/opengl.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/uic.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/thread.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/testcase_targets.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exceptions.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/yacc.prf \
		/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/lex.prf \
		RoboBulls.pro \
		gui/images.qrc \
		/usr/lib/x86_64-linux-gnu/libQt5Network.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Widgets.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Gui.prl \
		/usr/lib/x86_64-linux-gnu/libQt5SerialPort.prl \
		/usr/lib/x86_64-linux-gnu/libQt5Core.prl
	$(QMAKE) -o Makefile RoboBulls.pro
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_pre.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/unix.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/linux.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/sanitize.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/gcc-base-unix.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-base.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/common/g++-unix.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/qconfig.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_clucene_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_concurrent_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_core_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_dbus_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designer.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designer_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_designercomponents_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_gui_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_help.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_help_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimedia.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_multimediawidgets.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_network_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_opengl_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_printsupport_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qml_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmldevtools_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_qmltest_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quick_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickparticles_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_quickwidgets_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_script.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_script_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_scripttools.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_scripttools_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_serialport.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_sql_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_svg.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_testlib_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uiplugin.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uitools.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_uitools_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_webkit.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_webkitwidgets.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_widgets_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_x11extras.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xml_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xmlpatterns.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/modules/qt_lib_xmlpatterns_private.pri:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_functions.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt_config.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64/qmake.conf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/spec_post.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exclusive_builds.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_pre.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resolve_config.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/default_post.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/warn_on.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/qt.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/resources.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/moc.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/opengl.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/uic.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/unix/thread.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/testcase_targets.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/exceptions.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/yacc.prf:
/usr/lib/x86_64-linux-gnu/qt5/mkspecs/features/lex.prf:
RoboBulls.pro:
gui/images.qrc:
/usr/lib/x86_64-linux-gnu/libQt5Network.prl:
/usr/lib/x86_64-linux-gnu/libQt5Widgets.prl:
/usr/lib/x86_64-linux-gnu/libQt5Gui.prl:
/usr/lib/x86_64-linux-gnu/libQt5SerialPort.prl:
/usr/lib/x86_64-linux-gnu/libQt5Core.prl:
qmake: FORCE
	@$(QMAKE) -o Makefile RoboBulls.pro

qmake_all: FORCE


all: Makefile $(TARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || mkdir -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/
	$(COPY_FILE) --parents gui/images.qrc $(DISTDIR)/
	$(COPY_FILE) --parents behavior/attackmain.h behavior/attacksupport.h behavior/behavior.h behavior/defendbehavior.h behavior/genericmovementbehavior.h behavior/genericskillbehavior.h communication/nxtrobcomm.h communication/refcomm.h communication/robcomm.h communication/simrobcomm.h communication/visioncomm.h gui/fieldpanel.h gui/gamepanel.h gui/guiball.h gui/guibotlabel.h gui/guicomm.h gui/guidrawline.h gui/guifield.h gui/guiinterface.h gui/guirobot.h gui/guiscene.h gui/guisidelines.h gui/joystick.h gui/mainwindow.h gui/objectposition.h gui/robotpanel.h gui/selrobotpanel.h include/config/globals.h include/config/robot_types.h include/config/simulated.h include/config/team.h include/config/tolerances.h include/config/communication.h include/grSim_Commands.pb.h include/grSim_Packet.pb.h include/grSim_Replacement.pb.h include/messages_robocup_ssl_detection.pb.h include/messages_robocup_ssl_geometry.pb.h include/messages_robocup_ssl_refbox_log.pb.h include/messages_robocup_ssl_wrapper.pb.h include/netraw.h include/robocup_ssl_client.h include/serialib.h model/gamemodel.h model/robot.h movement/go_to_pose.h movement/move_collisions.h movement/movetype.h movement/pathfinding/fppa_pathfinding.h skill/kick.h skill/kicktopointomni.h skill/skill.h skill/stop.h strategy/freekickstrategy.h strategy/haltstrategy.h strategy/kickoffstrategy.h strategy/normalgamestrategy.h strategy/penaltystrategy.h strategy/stopstrategy.h strategy/strategycontroller.h strategy/strategy.h strategy/teststrategy.h strategy/videostrategies.h utilities/comparisons.h utilities/debug.h utilities/edges.h utilities/point.h utilities/velocitycalculator.h strategy/indirectkickstrategy.h communication/kfball.h utilities/getclassname.h utilities/region/sector.h utilities/region/rectangle.h utilities/region/region.h utilities/measurements.h utilities/region/defencearea.h gui/guidrawpoint.h gui/guidrawregion.h skill/dribbletopoint.h behavior/challengeballbot.h skill/dribbleback.h behavior/refstop.h behavior/goalie.h behavior/markbot.h behavior/wall.h behavior/penaltygoalie.h communication/yisirobcomm.h communication/crc.h movement/differential/differential_pilot.h movement/three_wheel_omni/three_wheel_omni_pilot.h movement/four_wheel_omni/four_wheel_omni_pilot.h $(DISTDIR)/
	$(COPY_FILE) --parents main.cpp behavior/attackmain.cpp behavior/attacksupport.cpp behavior/behavior.cpp behavior/genericmovementbehavior.cpp communication/nxtrobcomm.cpp communication/refcomm.cpp communication/robcomm.cpp communication/simrobcomm.cpp communication/visioncomm.cpp gui/fieldpanel.cpp gui/gamepanel.cpp gui/guiball.cpp gui/guibotlabel.cpp gui/guicomm.cpp gui/guidrawline.cpp gui/guifield.cpp gui/guiinterface.cpp gui/guirobot.cpp gui/guiscene.cpp gui/guisidelines.cpp gui/joystick.cpp gui/mainwindow.cpp gui/objectposition.cpp gui/robotpanel.cpp gui/selrobotpanel.cpp include/config/robot_types.cpp include/grSim_Commands.pb.cc include/grSim_Packet.pb.cc include/grSim_Replacement.pb.cc include/messages_robocup_ssl_detection.pb.cc include/messages_robocup_ssl_geometry.pb.cc include/messages_robocup_ssl_refbox_log.pb.cc include/messages_robocup_ssl_wrapper.pb.cc include/netraw.cpp include/robocup_ssl_client.cpp include/serialib.cpp model/gamemodel.cpp model/robot.cpp movement/four_wheel_omni/four_wheel_omni_pilot.cpp movement/move_collisions.cpp movement/pathfinding/fppa_pathfinding.cpp movement/three_wheel_omni/three_wheel_omni_pilot.cpp skill/kick.cpp skill/kicktopointomni.cpp skill/stop.cpp strategy/freekickstrategy.cpp strategy/haltstrategy.cpp strategy/kickoffstrategy.cpp strategy/normalgamestrategy.cpp strategy/penaltystrategy.cpp strategy/stopstrategy.cpp strategy/strategycontroller.cpp strategy/strategy.cpp strategy/teststrategy.cpp strategy/videostrategies.cpp utilities/comparisons.cpp utilities/debug.cpp utilities/edges.cpp utilities/point.cpp utilities/velocitycalculator.cpp behavior/defendbehavior.cpp strategy/indirectkickstrategy.cpp communication/kfball.cpp utilities/getclassname.cpp utilities/region/sector.cpp utilities/region/rectangle.cpp utilities/measurements.cpp utilities/region/defencearea.cpp gui/guidrawpoint.cpp gui/guidrawregion.cpp skill/dribbletopoint.cpp behavior/challengeballbot.cpp skill/dribbleback.cpp behavior/refstop.cpp behavior/goalie.cpp behavior/markbot.cpp behavior/wall.cpp behavior/penaltygoalie.cpp communication/yisirobcomm.cpp communication/crc.cpp movement/go_to_pose.cpp movement/differential/differential_pilot.cpp $(DISTDIR)/
	$(COPY_FILE) --parents gui/mainwindow.ui $(DISTDIR)/


clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


####### Sub-libraries

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compiler_rcc_make_all: qrc_images.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_images.cpp
qrc_images.cpp: gui/images.qrc \
		gui/images/forward_arrow_0.png \
		gui/images/go-next.png \
		gui/images/gearBW.png \
		gui/images/stopwatch_0.png \
		gui/images/stopwatch_1.png \
		gui/images/mouse_cursor_0.png \
		gui/images/bot0.png \
		gui/images/bot1.png \
		gui/images/bot2.png \
		gui/images/bot3.png \
		gui/images/0.png \
		gui/images/1.png \
		gui/images/bot4.png \
		gui/images/2.png \
		gui/images/bot5.png \
		gui/images/3.png \
		gui/images/4.png \
		gui/images/5.png \
		gui/images/gear.png \
		gui/images/gear2.png \
		gui/images/process-stop.png
	/usr/lib/x86_64-linux-gnu/qt5/bin/rcc -name images gui/images.qrc -o qrc_images.cpp

compiler_moc_header_make_all: moc_fieldpanel.cpp moc_gamepanel.cpp moc_guicomm.cpp moc_guidrawline.cpp moc_mainwindow.cpp moc_robotpanel.cpp moc_guidrawpoint.cpp moc_guidrawregion.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_fieldpanel.cpp moc_gamepanel.cpp moc_guicomm.cpp moc_guidrawline.cpp moc_mainwindow.cpp moc_robotpanel.cpp moc_guidrawpoint.cpp moc_guidrawregion.cpp
moc_fieldpanel.cpp: gui/guisidelines.h \
		utilities/point.h \
		gui/guidrawpoint.h \
		gui/guidrawregion.h \
		gui/fieldpanel.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/fieldpanel.h -o moc_fieldpanel.cpp

moc_gamepanel.cpp: gui/gamepanel.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/gamepanel.h -o moc_gamepanel.cpp

moc_guicomm.cpp: gui/guicomm.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/guicomm.h -o moc_guicomm.cpp

moc_guidrawline.cpp: utilities/point.h \
		gui/guidrawline.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/guidrawline.h -o moc_guidrawline.cpp

moc_mainwindow.cpp: gui/mainwindow.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/mainwindow.h -o moc_mainwindow.cpp

moc_robotpanel.cpp: gui/robotpanel.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/robotpanel.h -o moc_robotpanel.cpp

moc_guidrawpoint.cpp: utilities/point.h \
		gui/guidrawpoint.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/guidrawpoint.h -o moc_guidrawpoint.cpp

moc_guidrawregion.cpp: utilities/point.h \
		gui/guidrawregion.h
	/usr/lib/x86_64-linux-gnu/qt5/bin/moc $(DEFINES) -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I/home/usr2-spring2018/robobulls2 -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtNetwork -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtSerialPort -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/c++/5 -I/usr/include/x86_64-linux-gnu/c++/5 -I/usr/include/c++/5/backward -I/usr/lib/gcc/x86_64-linux-gnu/5/include -I/usr/local/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed -I/usr/include/x86_64-linux-gnu -I/usr/include gui/guidrawregion.h -o moc_guidrawregion.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h
ui_mainwindow.h: gui/mainwindow.ui
	/usr/lib/x86_64-linux-gnu/qt5/bin/uic gui/mainwindow.ui -o ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_rcc_clean compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp include/config/simulated.h \
		include/config/team.h \
		include/config/communication.h \
		communication/visioncomm.h \
		include/netraw.h \
		include/messages_robocup_ssl_detection.pb.h \
		include/messages_robocup_ssl_geometry.pb.h \
		include/messages_robocup_ssl_wrapper.pb.h \
		include/robocup_ssl_client.h \
		include/messages_robocup_ssl_refbox_log.pb.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		communication/kfball.h \
		gui/guiinterface.h \
		communication/robcomm.h \
		communication/refcomm.h \
		utilities/debug.h \
		strategy/strategycontroller.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

attackmain.o: behavior/attackmain.cpp behavior/attackmain.h \
		behavior/behavior.h \
		include/config/simulated.h \
		skill/kicktopointomni.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h \
		skill/dribbletopoint.h \
		model/gamemodel.h \
		utilities/region/sector.h \
		utilities/region/region.h \
		behavior/genericmovementbehavior.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		include/config/team.h \
		utilities/comparisons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o attackmain.o behavior/attackmain.cpp

attacksupport.o: behavior/attacksupport.cpp behavior/attacksupport.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		model/gamemodel.h \
		utilities/region/sector.h \
		include/config/team.h \
		utilities/comparisons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o attacksupport.o behavior/attacksupport.cpp

behavior.o: behavior/behavior.cpp behavior/behavior.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o behavior.o behavior/behavior.cpp

genericmovementbehavior.o: behavior/genericmovementbehavior.cpp behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o genericmovementbehavior.o behavior/genericmovementbehavior.cpp

nxtrobcomm.o: communication/nxtrobcomm.cpp communication/nxtrobcomm.h \
		include/serialib.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		communication/robcomm.h \
		utilities/debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o nxtrobcomm.o communication/nxtrobcomm.cpp

refcomm.o: communication/refcomm.cpp communication/refcomm.h \
		include/netraw.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		include/config/communication.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o refcomm.o communication/refcomm.cpp

robcomm.o: communication/robcomm.cpp include/config/simulated.h \
		communication/simrobcomm.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		communication/robcomm.h \
		communication/nxtrobcomm.h \
		include/serialib.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/globals.h \
		communication/yisirobcomm.h \
		communication/crc.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o robcomm.o communication/robcomm.cpp

simrobcomm.o: communication/simrobcomm.cpp include/grSim_Packet.pb.h \
		include/grSim_Commands.pb.h \
		include/grSim_Replacement.pb.h \
		include/config/team.h \
		include/config/communication.h \
		include/config/simulated.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		communication/simrobcomm.h \
		communication/robcomm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o simrobcomm.o communication/simrobcomm.cpp

visioncomm.o: communication/visioncomm.cpp include/config/simulated.h \
		include/config/team.h \
		include/config/communication.h \
		communication/visioncomm.h \
		include/netraw.h \
		include/messages_robocup_ssl_detection.pb.h \
		include/messages_robocup_ssl_geometry.pb.h \
		include/messages_robocup_ssl_wrapper.pb.h \
		include/robocup_ssl_client.h \
		include/messages_robocup_ssl_refbox_log.pb.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		communication/kfball.h \
		gui/guiinterface.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o visioncomm.o communication/visioncomm.cpp

fieldpanel.o: gui/fieldpanel.cpp gui/fieldpanel.h \
		gui/guisidelines.h \
		utilities/point.h \
		gui/guidrawpoint.h \
		gui/guidrawregion.h \
		gui/objectposition.h \
		gui/robotpanel.h \
		gui/selrobotpanel.h \
		gui/mainwindow.h \
		include/config/team.h \
		ui_mainwindow.h \
		gui/guiinterface.h \
		gui/guirobot.h \
		gui/guiscene.h \
		gui/guiball.h \
		gui/guibotlabel.h \
		gui/guidrawline.h \
		gui/guifield.h \
		include/config/globals.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fieldpanel.o gui/fieldpanel.cpp

gamepanel.o: gui/gamepanel.cpp gui/mainwindow.h \
		ui_mainwindow.h \
		gui/gamepanel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gamepanel.o gui/gamepanel.cpp

guiball.o: gui/guiball.cpp gui/guiball.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guiball.o gui/guiball.cpp

guibotlabel.o: gui/guibotlabel.cpp include/config/team.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		gui/guibotlabel.h \
		movement/move_collisions.h \
		utilities/comparisons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guibotlabel.o gui/guibotlabel.cpp

guicomm.o: gui/guicomm.cpp gui/guicomm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guicomm.o gui/guicomm.cpp

guidrawline.o: gui/guidrawline.cpp gui/guidrawline.h \
		utilities/point.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guidrawline.o gui/guidrawline.cpp

guifield.o: gui/guifield.cpp gui/guifield.h \
		include/config/globals.h \
		gui/mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guifield.o gui/guifield.cpp

guiinterface.o: gui/guiinterface.cpp gui/guiinterface.h \
		gui/mainwindow.h \
		gui/selrobotpanel.h \
		gui/fieldpanel.h \
		gui/guisidelines.h \
		utilities/point.h \
		gui/guidrawpoint.h \
		gui/guidrawregion.h \
		gui/guidrawline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guiinterface.o gui/guiinterface.cpp

guirobot.o: gui/guirobot.cpp gui/guirobot.h \
		gui/mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guirobot.o gui/guirobot.cpp

guiscene.o: gui/guiscene.cpp gui/guiscene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guiscene.o gui/guiscene.cpp

guisidelines.o: gui/guisidelines.cpp gui/guisidelines.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guisidelines.o gui/guisidelines.cpp

joystick.o: gui/joystick.cpp gui/joystick.h \
		include/config/team.h \
		utilities/debug.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o joystick.o gui/joystick.cpp

mainwindow.o: gui/mainwindow.cpp gui/robotpanel.h \
		gui/selrobotpanel.h \
		gui/objectposition.h \
		utilities/point.h \
		gui/mainwindow.h \
		ui_mainwindow.h \
		gui/gamepanel.h \
		gui/fieldpanel.h \
		gui/guisidelines.h \
		gui/guidrawpoint.h \
		gui/guidrawregion.h \
		gui/guifield.h \
		include/config/globals.h \
		gui/guiball.h \
		gui/guibotlabel.h \
		gui/guidrawline.h \
		gui/guicomm.h \
		gui/guiscene.h \
		gui/guiinterface.h \
		gui/guirobot.h \
		gui/joystick.h \
		include/config/team.h \
		utilities/debug.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o gui/mainwindow.cpp

objectposition.o: gui/objectposition.cpp gui/guiscene.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o objectposition.o gui/objectposition.cpp

robotpanel.o: gui/robotpanel.cpp gui/robotpanel.h \
		gui/mainwindow.h \
		gui/objectposition.h \
		utilities/point.h \
		gui/gamepanel.h \
		gui/fieldpanel.h \
		gui/guisidelines.h \
		gui/guidrawpoint.h \
		gui/guidrawregion.h \
		gui/selrobotpanel.h \
		ui_mainwindow.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/getclassname.h \
		gui/guirobot.h \
		gui/guiball.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o robotpanel.o gui/robotpanel.cpp

selrobotpanel.o: gui/selrobotpanel.cpp gui/selrobotpanel.h \
		gui/mainwindow.h \
		gui/robotpanel.h \
		gui/objectposition.h \
		utilities/point.h \
		gui/gamepanel.h \
		gui/fieldpanel.h \
		gui/guisidelines.h \
		gui/guidrawpoint.h \
		gui/guidrawregion.h \
		gui/guiinterface.h \
		ui_mainwindow.h \
		gui/guirobot.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o selrobotpanel.o gui/selrobotpanel.cpp

robot_types.o: include/config/robot_types.cpp include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o robot_types.o include/config/robot_types.cpp

grSim_Commands.pb.o: include/grSim_Commands.pb.cc include/grSim_Commands.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o grSim_Commands.pb.o include/grSim_Commands.pb.cc

grSim_Packet.pb.o: include/grSim_Packet.pb.cc include/grSim_Packet.pb.h \
		include/grSim_Commands.pb.h \
		include/grSim_Replacement.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o grSim_Packet.pb.o include/grSim_Packet.pb.cc

grSim_Replacement.pb.o: include/grSim_Replacement.pb.cc include/grSim_Replacement.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o grSim_Replacement.pb.o include/grSim_Replacement.pb.cc

messages_robocup_ssl_detection.pb.o: include/messages_robocup_ssl_detection.pb.cc include/messages_robocup_ssl_detection.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o messages_robocup_ssl_detection.pb.o include/messages_robocup_ssl_detection.pb.cc

messages_robocup_ssl_geometry.pb.o: include/messages_robocup_ssl_geometry.pb.cc include/messages_robocup_ssl_geometry.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o messages_robocup_ssl_geometry.pb.o include/messages_robocup_ssl_geometry.pb.cc

messages_robocup_ssl_refbox_log.pb.o: include/messages_robocup_ssl_refbox_log.pb.cc include/messages_robocup_ssl_refbox_log.pb.h \
		include/messages_robocup_ssl_detection.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o messages_robocup_ssl_refbox_log.pb.o include/messages_robocup_ssl_refbox_log.pb.cc

messages_robocup_ssl_wrapper.pb.o: include/messages_robocup_ssl_wrapper.pb.cc include/messages_robocup_ssl_wrapper.pb.h \
		include/messages_robocup_ssl_detection.pb.h \
		include/messages_robocup_ssl_geometry.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o messages_robocup_ssl_wrapper.pb.o include/messages_robocup_ssl_wrapper.pb.cc

netraw.o: include/netraw.cpp include/netraw.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o netraw.o include/netraw.cpp

robocup_ssl_client.o: include/robocup_ssl_client.cpp include/robocup_ssl_client.h \
		include/netraw.h \
		include/messages_robocup_ssl_detection.pb.h \
		include/messages_robocup_ssl_geometry.pb.h \
		include/messages_robocup_ssl_wrapper.pb.h \
		include/messages_robocup_ssl_refbox_log.pb.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o robocup_ssl_client.o include/robocup_ssl_client.cpp

serialib.o: include/serialib.cpp include/serialib.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o serialib.o include/serialib.cpp

gamemodel.o: model/gamemodel.cpp include/config/team.h \
		include/config/simulated.h \
		strategy/strategycontroller.h \
		utilities/comparisons.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/edges.h \
		utilities/debug.h \
		gui/guiinterface.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gamemodel.o model/gamemodel.cpp

robot.o: model/robot.cpp include/config/simulated.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o robot.o model/robot.cpp

four_wheel_omni_pilot.o: movement/four_wheel_omni/four_wheel_omni_pilot.cpp movement/four_wheel_omni/four_wheel_omni_pilot.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/movetype.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		utilities/debug.h \
		gui/guiinterface.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o four_wheel_omni_pilot.o movement/four_wheel_omni/four_wheel_omni_pilot.cpp

move_collisions.o: movement/move_collisions.cpp utilities/measurements.h \
		utilities/point.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		utilities/comparisons.h \
		model/gamemodel.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/move_collisions.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o move_collisions.o movement/move_collisions.cpp

fppa_pathfinding.o: movement/pathfinding/fppa_pathfinding.cpp include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		utilities/measurements.h \
		utilities/point.h \
		utilities/comparisons.h \
		model/gamemodel.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/region/sector.h \
		include/config/team.h \
		movement/pathfinding/fppa_pathfinding.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fppa_pathfinding.o movement/pathfinding/fppa_pathfinding.cpp

three_wheel_omni_pilot.o: movement/three_wheel_omni/three_wheel_omni_pilot.cpp include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/movetype.h \
		utilities/measurements.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o three_wheel_omni_pilot.o movement/three_wheel_omni/three_wheel_omni_pilot.cpp

kick.o: skill/kick.cpp model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		include/config/globals.h \
		skill/kick.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o kick.o skill/kick.cpp

kicktopointomni.o: skill/kicktopointomni.cpp model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/comparisons.h \
		utilities/debug.h \
		skill/kick.h \
		skill/kicktopointomni.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o kicktopointomni.o skill/kicktopointomni.cpp

stop.o: skill/stop.cpp skill/stop.h \
		skill/skill.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stop.o skill/stop.cpp

freekickstrategy.o: strategy/freekickstrategy.cpp strategy/freekickstrategy.h \
		strategy/strategy.h \
		utilities/point.h \
		behavior/attackmain.h \
		behavior/behavior.h \
		include/config/simulated.h \
		skill/kicktopointomni.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h \
		skill/dribbletopoint.h \
		model/gamemodel.h \
		utilities/region/sector.h \
		utilities/region/region.h \
		behavior/genericmovementbehavior.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		include/config/team.h \
		utilities/comparisons.h \
		behavior/attacksupport.h \
		behavior/goalie.h \
		skill/dribbleback.h \
		strategy/normalgamestrategy.h \
		behavior/refstop.h \
		behavior/wall.h \
		behavior/markbot.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o freekickstrategy.o strategy/freekickstrategy.cpp

haltstrategy.o: strategy/haltstrategy.cpp strategy/haltstrategy.h \
		strategy/strategy.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		behavior/refstop.h \
		behavior/genericmovementbehavior.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		utilities/comparisons.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/region/sector.h \
		include/config/team.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o haltstrategy.o strategy/haltstrategy.cpp

kickoffstrategy.o: strategy/kickoffstrategy.cpp behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		behavior/goalie.h \
		skill/kicktopointomni.h \
		gui/guiinterface.h \
		skill/dribbleback.h \
		model/gamemodel.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/sector.h \
		include/config/team.h \
		utilities/comparisons.h \
		strategy/kickoffstrategy.h \
		strategy/strategy.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o kickoffstrategy.o strategy/kickoffstrategy.cpp

normalgamestrategy.o: strategy/normalgamestrategy.cpp include/config/team.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		behavior/goalie.h \
		skill/kicktopointomni.h \
		gui/guiinterface.h \
		skill/dribbleback.h \
		model/gamemodel.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/sector.h \
		utilities/comparisons.h \
		behavior/attackmain.h \
		skill/dribbletopoint.h \
		behavior/attacksupport.h \
		behavior/defendbehavior.h \
		behavior/refstop.h \
		behavior/wall.h \
		behavior/markbot.h \
		behavior/challengeballbot.h \
		behavior/penaltygoalie.h \
		utilities/edges.h \
		strategy/normalgamestrategy.h \
		strategy/strategy.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o normalgamestrategy.o strategy/normalgamestrategy.cpp

penaltystrategy.o: strategy/penaltystrategy.cpp strategy/penaltystrategy.h \
		strategy/strategy.h \
		strategy/normalgamestrategy.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/region/defencearea.h \
		include/config/globals.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		utilities/region/sector.h \
		include/config/team.h \
		behavior/genericmovementbehavior.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		behavior/wall.h \
		utilities/comparisons.h \
		skill/kicktopointomni.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o penaltystrategy.o strategy/penaltystrategy.cpp

stopstrategy.o: strategy/stopstrategy.cpp include/config/team.h \
		utilities/measurements.h \
		utilities/point.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		behavior/goalie.h \
		skill/kicktopointomni.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h \
		skill/dribbleback.h \
		model/gamemodel.h \
		behavior/genericmovementbehavior.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/sector.h \
		utilities/comparisons.h \
		behavior/refstop.h \
		strategy/stopstrategy.h \
		strategy/strategy.h \
		utilities/edges.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stopstrategy.o strategy/stopstrategy.cpp

strategycontroller.o: strategy/strategycontroller.cpp strategy/strategy.h \
		movement/move.h \
		include/config/globals.h \
		movement/pathfinding/fppa_pathfinding.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/movetype.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		model/gamemodel.h \
		utilities/measurements.h \
		strategy/stopstrategy.h \
		utilities/edges.h \
		strategy/teststrategy.h \
		strategy/kickoffstrategy.h \
		strategy/freekickstrategy.h \
		strategy/normalgamestrategy.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/region/sector.h \
		include/config/team.h \
		behavior/genericmovementbehavior.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		strategy/penaltystrategy.h \
		behavior/wall.h \
		utilities/comparisons.h \
		skill/kicktopointomni.h \
		strategy/indirectkickstrategy.h \
		strategy/haltstrategy.h \
		communication/robcomm.h \
		include/config/communication.h \
		strategy/strategycontroller.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o strategycontroller.o strategy/strategycontroller.cpp

strategy.o: strategy/strategy.cpp strategy/strategy.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o strategy.o strategy/strategy.cpp

teststrategy.o: strategy/teststrategy.cpp strategy/teststrategy.h \
		strategy/strategy.h \
		skill/kicktopointomni.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h \
		skill/dribbletopoint.h \
		model/gamemodel.h \
		behavior/genericmovementbehavior.h \
		utilities/comparisons.h \
		behavior/defendbehavior.h \
		behavior/goalie.h \
		skill/dribbleback.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/sector.h \
		include/config/team.h \
		behavior/attackmain.h \
		behavior/attacksupport.h \
		behavior/refstop.h \
		behavior/penaltygoalie.h \
		behavior/wall.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o teststrategy.o strategy/teststrategy.cpp

videostrategies.o: strategy/videostrategies.cpp behavior/genericskillbehavior.h \
		behavior/behavior.h \
		skill/stop.h \
		skill/skill.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		include/config/robot_types.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		strategy/videostrategies.h \
		strategy/strategy.h \
		behavior/genericmovementbehavior.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		skill/kicktopointomni.h \
		gui/guiinterface.h \
		utilities/comparisons.h \
		utilities/edges.h \
		behavior/goalie.h \
		skill/dribbleback.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/sector.h \
		include/config/team.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o videostrategies.o strategy/videostrategies.cpp

comparisons.o: utilities/comparisons.cpp include/config/globals.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/comparisons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o comparisons.o utilities/comparisons.cpp

debug.o: utilities/debug.cpp model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		include/config/team.h \
		utilities/debug.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o debug.o utilities/debug.cpp

edges.o: utilities/edges.cpp utilities/edges.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o edges.o utilities/edges.cpp

point.o: utilities/point.cpp utilities/point.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o point.o utilities/point.cpp

velocitycalculator.o: utilities/velocitycalculator.cpp include/config/globals.h \
		utilities/velocitycalculator.h \
		utilities/point.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o velocitycalculator.o utilities/velocitycalculator.cpp

defendbehavior.o: behavior/defendbehavior.cpp include/config/simulated.h \
		utilities/comparisons.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		behavior/defendbehavior.h \
		behavior/genericmovementbehavior.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		skill/kicktopointomni.h \
		gui/guiinterface.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/defencearea.h \
		utilities/region/sector.h \
		include/config/team.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o defendbehavior.o behavior/defendbehavior.cpp

indirectkickstrategy.o: strategy/indirectkickstrategy.cpp strategy/indirectkickstrategy.h \
		strategy/strategy.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/comparisons.h \
		include/config/team.h \
		behavior/goalie.h \
		skill/kicktopointomni.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		skill/dribbleback.h \
		behavior/genericmovementbehavior.h \
		utilities/region/defencearea.h \
		utilities/region/sector.h \
		behavior/refstop.h \
		behavior/attackmain.h \
		skill/dribbletopoint.h \
		behavior/attacksupport.h \
		behavior/wall.h \
		behavior/markbot.h \
		strategy/normalgamestrategy.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o indirectkickstrategy.o strategy/indirectkickstrategy.cpp

kfball.o: communication/kfball.cpp communication/kfball.h \
		model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o kfball.o communication/kfball.cpp

getclassname.o: utilities/getclassname.cpp utilities/getclassname.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o getclassname.o utilities/getclassname.cpp

sector.o: utilities/region/sector.cpp utilities/region/sector.h \
		utilities/region/region.h \
		utilities/point.h \
		gui/guiinterface.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sector.o utilities/region/sector.cpp

rectangle.o: utilities/region/rectangle.cpp utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/point.h \
		gui/guiinterface.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rectangle.o utilities/region/rectangle.cpp

measurements.o: utilities/measurements.cpp model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o measurements.o utilities/measurements.cpp

defencearea.o: utilities/region/defencearea.cpp utilities/region/defencearea.h \
		include/config/globals.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/point.h \
		gui/guiinterface.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		model/gamemodel.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		utilities/region/sector.h \
		include/config/team.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o defencearea.o utilities/region/defencearea.cpp

guidrawpoint.o: gui/guidrawpoint.cpp gui/guidrawpoint.h \
		utilities/point.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guidrawpoint.o gui/guidrawpoint.cpp

guidrawregion.o: gui/guidrawregion.cpp gui/guidrawregion.h \
		utilities/point.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o guidrawregion.o gui/guidrawregion.cpp

dribbletopoint.o: skill/dribbletopoint.cpp skill/dribbletopoint.h \
		skill/skill.h \
		utilities/point.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		model/gamemodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dribbletopoint.o skill/dribbletopoint.cpp

challengeballbot.o: behavior/challengeballbot.cpp behavior/challengeballbot.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		model/gamemodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o challengeballbot.o behavior/challengeballbot.cpp

dribbleback.o: skill/dribbleback.cpp skill/dribbleback.h \
		skill/skill.h \
		utilities/point.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		model/gamemodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dribbleback.o skill/dribbleback.cpp

refstop.o: behavior/refstop.cpp behavior/refstop.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		utilities/comparisons.h \
		model/gamemodel.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/region/sector.h \
		include/config/team.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o refstop.o behavior/refstop.cpp

goalie.o: behavior/goalie.cpp behavior/goalie.h \
		skill/kicktopointomni.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h \
		skill/dribbleback.h \
		model/gamemodel.h \
		behavior/genericmovementbehavior.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		utilities/region/sector.h \
		include/config/team.h \
		utilities/comparisons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o goalie.o behavior/goalie.cpp

markbot.o: behavior/markbot.cpp behavior/markbot.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		model/gamemodel.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/region/sector.h \
		include/config/team.h \
		utilities/comparisons.h \
		skill/kicktopointomni.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o markbot.o behavior/markbot.cpp

wall.o: behavior/wall.cpp behavior/wall.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		model/gamemodel.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		utilities/region/sector.h \
		include/config/team.h \
		utilities/comparisons.h \
		skill/kicktopointomni.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o wall.o behavior/wall.cpp

penaltygoalie.o: behavior/penaltygoalie.cpp behavior/penaltygoalie.h \
		behavior/genericmovementbehavior.h \
		behavior/behavior.h \
		movement/go_to_pose.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		utilities/region/defencearea.h \
		utilities/region/rectangle.h \
		utilities/region/region.h \
		gui/guiinterface.h \
		model/gamemodel.h \
		utilities/region/sector.h \
		include/config/team.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o penaltygoalie.o behavior/penaltygoalie.cpp

yisirobcomm.o: communication/yisirobcomm.cpp communication/yisirobcomm.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		communication/robcomm.h \
		communication/crc.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o yisirobcomm.o communication/yisirobcomm.cpp

crc.o: communication/crc.cpp communication/crc.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o crc.o communication/crc.cpp

go_to_pose.o: movement/go_to_pose.cpp model/gamemodel.h \
		utilities/point.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h \
		include/config/globals.h \
		model/robot.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		movement/movetype.h \
		movement/go_to_pose.h \
		movement/differential/differential_pilot.h \
		movement/four_wheel_omni/four_wheel_omni_pilot.h \
		utilities/debug.h \
		movement/three_wheel_omni/three_wheel_omni_pilot.h \
		movement/pathfinding/fppa_pathfinding.h \
		gui/guiinterface.h \
		movement/move.h \
		movement/move_collisions.h \
		utilities/comparisons.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o go_to_pose.o movement/go_to_pose.cpp

differential_pilot.o: movement/differential/differential_pilot.cpp movement/differential/differential_pilot.h \
		movement/movetype.h \
		include/config/globals.h \
		model/robot.h \
		utilities/point.h \
		utilities/velocitycalculator.h \
		behavior/behavior.h \
		skill/skill.h \
		behavior/genericskillbehavior.h \
		include/config/robot_types.h \
		utilities/measurements.h \
		include/config/tolerances.h \
		include/config/simulated.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o differential_pilot.o movement/differential/differential_pilot.cpp

qrc_images.o: qrc_images.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_images.o qrc_images.cpp

moc_fieldpanel.o: moc_fieldpanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_fieldpanel.o moc_fieldpanel.cpp

moc_gamepanel.o: moc_gamepanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_gamepanel.o moc_gamepanel.cpp

moc_guicomm.o: moc_guicomm.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_guicomm.o moc_guicomm.cpp

moc_guidrawline.o: moc_guidrawline.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_guidrawline.o moc_guidrawline.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_robotpanel.o: moc_robotpanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_robotpanel.o moc_robotpanel.cpp

moc_guidrawpoint.o: moc_guidrawpoint.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_guidrawpoint.o moc_guidrawpoint.cpp

moc_guidrawregion.o: moc_guidrawregion.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_guidrawregion.o moc_guidrawregion.cpp

####### Install

install:  FORCE

uninstall:  FORCE

FORCE:

