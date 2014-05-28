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


SOURCES += main.cpp \
    communication/refcomm.cpp \
    include/messages_robocup_ssl_detection.pb.cc \
    include/messages_robocup_ssl_geometry.pb.cc \
    include/messages_robocup_ssl_refbox_log.pb.cc \
    include/messages_robocup_ssl_wrapper.pb.cc \
    include/netraw.cpp \
    include/robocup_ssl_client.cpp \
    include/serialib.cpp \
    Model/gamemodel.cpp \
    strategy/strategy.cpp \
    strategy/strategycontroller.cpp \
    strategy/stopstrategy.cpp \
    behavior/behavior.cpp \
    skill/skill.cpp \
    behavior/stopbehavior.cpp \
    communication/visioncomm.cpp \
    skill/kick.cpp \
    skill/waitforball.cpp \
    Model/robot.cpp \
    skill/gotoposition.cpp \
    Utilities/point.cpp \
    Utilities/measurments.cpp \
    skill/gobackward.cpp \
    skill/goforward.cpp \
    skill/rotate.cpp \
    skill/stop.cpp \
    behavior/penaltybehavior.cpp \
    strategy/penaltystrategy.cpp \
    skill/robottoinertialvelocities.cpp \
    skill/closedloopcontrol.cpp \
    skill/matrixcalculator.cpp \
    skill/gotopositionwithorientation.cpp \
    communication/simrobcomm.cpp \
    include/grSim_Replacement.pb.cc \
    include/grSim_Packet.pb.cc \
    include/grSim_Commands.pb.cc \
    communication/nxtrobcomm.cpp \
    communication/robcomm.cpp \
    skill/gotopositionfacing.cpp

HEADERS += \
    communication/refcomm.h \
    include/messages_robocup_ssl_detection.pb.h \
    include/messages_robocup_ssl_geometry.pb.h \
    include/messages_robocup_ssl_refbox_log.pb.h \
    include/messages_robocup_ssl_wrapper.pb.h \
    include/netraw.h \
    include/robocup_ssl_client.h \
    include/serialib.h \
    include/timer.h \
    include/util.h \
    Model/gamemodel.h \
    strategy/strategy.h \
    strategy/strategycontroller.h \
    strategy/stopstrategy.h \
    behavior/behavior.h \
    communication/visioncomm.h \
    skill/kick.h \
    skill/waitforball.h \
    Model/robot.h \
    skill/gotoposition.h \
    skill/skill.h \
    behavior/stopbehavior.h \
    Utilities/measurments.h \
    Utilities/point.h \
    skill/gobackward.h \
    skill/goforward.h \
    skill/rotate.h \
    skill/stop.h \
    behavior/penaltybehavior.h \
    strategy/penaltystrategy.h \
    skill/robottoinertialvelocities.h \
    skill/matrixcalculator.h \
    skill/closedloopcontrol.h \
    skill/gotopositionwithorientation.h \
    communication/simrobcomm.h \
    include/grSim_Replacement.pb.h \
    include/grSim_Packet.pb.h \
    include/grSim_Commands.pb.h \
    include/globals.h \
    communication/nxtrobcomm.h \
    communication/robcomm.h \
    skill/gotopositionfacing.h

unix|win32: LIBS += -lprotobuf
