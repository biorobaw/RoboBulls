/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *mainScrollArea;
    QGridLayout *gridLayout_2;
    QFrame *frame_gameStatus;
    QLabel *label_primeBot;
    QGraphicsView *gView_ball;
    QPushButton *btn_connectGui;
    QLCDNumber *lcd_coordY_ball;
    QLCDNumber *lcd_coordX_ball;
    QLabel *label_3;
    QLCDNumber *lcd_coordX_cursor;
    QLCDNumber *lcd_coordY_cursor;
    QLCDNumber *lcd_clock;
    QLabel *label_9;
    QPushButton *btn_multithread;
    QLabel *label_multithreaded;
    QFrame *frame_gameStatus_2;
    QLabel *label_10;
    QTextBrowser *text_guiPrint;
    QFrame *frame_field;
    QGridLayout *gridLayout_3;
    QTabWidget *tabs_field;
    QWidget *field;
    QFrame *frame_8;
    QCheckBox *check_fieldGrid;
    QComboBox *combo_gridScale;
    QFrame *frame_9;
    QComboBox *combo_fieldColor;
    QCheckBox *check_coloredGoals;
    QComboBox *combo_fieldProportions;
    QWidget *tab_robots;
    QFrame *frame_10;
    QPushButton *btn_override_none;
    QPushButton *btn_override_all;
    QLabel *label_6;
    QFrame *frame_11;
    QPushButton *btn_botKick;
    QToolButton *btn_botReverse;
    QPushButton *btn_botDrible;
    QToolButton *btn_botForward;
    QToolButton *btn_botTurnLeft;
    QToolButton *btn_botTurnRight;
    QFrame *frame_12;
    QCheckBox *check_showIDs;
    QFrame *frame_13;
    QComboBox *combo_botScale;
    QWidget *tab;
    QFrame *frame_19;
    QWidget *layoutWidget;
    QGridLayout *layout_ball_specs;
    QComboBox *combo_ballColor;
    QComboBox *combo_ballScale;
    QWidget *tab_3;
    QFrame *frame_20;
    QWidget *layoutWidget1;
    QGridLayout *layout_camera_controls;
    QPushButton *btn_rotateField_left;
    QPushButton *btn_rotateField_right;
    QSlider *zoom_slider;
    QPushButton *zoom_default;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QGraphicsView *gView_field;
    QFrame *frame_robotsPanel;
    QGridLayout *gridLayout_7;
    QFrame *frame_primeBot;
    QGroupBox *box_primeBot;
    QGraphicsView *gView_robot_prime;
    QCheckBox *check_botOverride;
    QTextBrowser *text_primeBot;
    QLabel *label_coord_6;
    QLCDNumber *lcd_coordX_prime;
    QLCDNumber *lcd_coordY_prime;
    QFrame *frame_18;
    QLCDNumber *lcd_orient_prime;
    QLCDNumber *lcd_botVel_;
    QLabel *label_4;
    QDial *dial_botVel_;
    QLabel *label_11;
    QDial *dial_botSpeed_;
    QLCDNumber *lcd_botSpeed_;
    QDial *dial_botOrient_prime;
    QLabel *label_5;
    QLabel *label_7;
    QScrollArea *scroll_robots;
    QWidget *scrollAreaWidgetContents;
    QWidget *layoutWidget2;
    QVBoxLayout *layout_robots;
    QFrame *frame_robot_0;
    QLabel *title_robPanel_0;
    QGraphicsView *gView_robot_0;
    QDial *dial_botVel_0;
    QDial *dial_botOrient_0;
    QFrame *frame_2;
    QGridLayout *gridLayout_5;
    QLCDNumber *lcd_coordY_0;
    QLCDNumber *lcd_coordX_0;
    QLabel *label_14;
    QLabel *label_13;
    QLabel *currBehavior_0;
    QFrame *frame_robot_1;
    QLabel *title_robPanel_1;
    QGraphicsView *gView_robot_1;
    QDial *dial_botVel_1;
    QDial *dial_botOrient_1;
    QFrame *frame_3;
    QGridLayout *gridLayout_6;
    QLCDNumber *lcd_coordY_1;
    QLCDNumber *lcd_coordX_1;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *currBehavior_1;
    QFrame *frame_robot_2;
    QLabel *title_robPanel_2;
    QGraphicsView *gView_robot_2;
    QDial *dial_botVel_2;
    QDial *dial_botOrient_2;
    QFrame *frame_4;
    QGridLayout *gridLayout_8;
    QLCDNumber *lcd_coordY_2;
    QLCDNumber *lcd_coordX_2;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *currBehavior_2;
    QFrame *frame_robot_3;
    QLabel *title_robPanel_3;
    QGraphicsView *gView_robot_3;
    QDial *dial_botVel_3;
    QDial *dial_botOrient_3;
    QFrame *frame_5;
    QGridLayout *gridLayout_9;
    QLCDNumber *lcd_coordY_3;
    QLCDNumber *lcd_coordX_3;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *currBehavior_3;
    QFrame *frame_robot_4;
    QLabel *title_robPanel_4;
    QGraphicsView *gView_robot_4;
    QDial *dial_botVel_4;
    QDial *dial_botOrient_4;
    QFrame *frame_6;
    QGridLayout *gridLayout_10;
    QLCDNumber *lcd_coordY_4;
    QLCDNumber *lcd_coordX_4;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *currBehavior_4;
    QFrame *frame_robot_5;
    QLabel *title_robPanel_5;
    QGraphicsView *gView_robot_5;
    QDial *dial_botVel_5;
    QDial *dial_botOrient_5;
    QFrame *frame_7;
    QGridLayout *gridLayout_11;
    QLCDNumber *lcd_coordY_5;
    QLCDNumber *lcd_coordX_5;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *currBehavior_5;
    QFrame *frame_robot_6;
    QLabel *title_robPanel_6;
    QLabel *gState;
    QFrame *frame_robot_7;
    QLabel *title_robPanel_7;
    QLabel *ballVel;
    QLabel *ballAccel;
    QFrame *frame_robot_8;
    QLabel *title_robPanel_8;
    QLabel *currStrategy;
    QFrame *frame_robot_9;
    QLabel *title_robPanel_9;
    QLabel *blueGoal;
    QLabel *yellGoal;
    QLabel *timeRem;
    QPushButton *btn_toggleTeamColor;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1200, 951);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/gearBW.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setTabShape(QTabWidget::Rounded);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setMinimumSize(QSize(640, 480));
        scrollArea->setBaseSize(QSize(1100, 800));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setLineWidth(0);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        mainScrollArea = new QWidget();
        mainScrollArea->setObjectName(QStringLiteral("mainScrollArea"));
        mainScrollArea->setGeometry(QRect(0, 0, 1200, 951));
        gridLayout_2 = new QGridLayout(mainScrollArea);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_gameStatus = new QFrame(mainScrollArea);
        frame_gameStatus->setObjectName(QStringLiteral("frame_gameStatus"));
        frame_gameStatus->setMinimumSize(QSize(431, 128));
        frame_gameStatus->setMaximumSize(QSize(431, 128));
        QPalette palette;
        QBrush brush(QColor(225, 255, 225, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        frame_gameStatus->setPalette(palette);
        frame_gameStatus->setStyleSheet(QStringLiteral("background-color: rgb(225, 255, 225);"));
        frame_gameStatus->setFrameShape(QFrame::Panel);
        frame_gameStatus->setFrameShadow(QFrame::Raised);
        frame_gameStatus->setLineWidth(3);
        label_primeBot = new QLabel(frame_gameStatus);
        label_primeBot->setObjectName(QStringLiteral("label_primeBot"));
        label_primeBot->setGeometry(QRect(10, 10, 121, 17));
        QFont font;
        font.setFamily(QStringLiteral("Courier 10 Pitch"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label_primeBot->setFont(font);
        gView_ball = new QGraphicsView(frame_gameStatus);
        gView_ball->setObjectName(QStringLiteral("gView_ball"));
        gView_ball->setGeometry(QRect(70, 43, 45, 45));
        gView_ball->setFocusPolicy(Qt::NoFocus);
        gView_ball->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        gView_ball->setFrameShape(QFrame::Panel);
        gView_ball->setFrameShadow(QFrame::Plain);
        gView_ball->setLineWidth(0);
        btn_connectGui = new QPushButton(frame_gameStatus);
        btn_connectGui->setObjectName(QStringLiteral("btn_connectGui"));
        btn_connectGui->setGeometry(QRect(335, 91, 80, 27));
        btn_connectGui->setFocusPolicy(Qt::NoFocus);
        btn_connectGui->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        lcd_coordY_ball = new QLCDNumber(frame_gameStatus);
        lcd_coordY_ball->setObjectName(QStringLiteral("lcd_coordY_ball"));
        lcd_coordY_ball->setGeometry(QRect(186, 55, 57, 27));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lcd_coordY_ball->sizePolicy().hasHeightForWidth());
        lcd_coordY_ball->setSizePolicy(sizePolicy);
        lcd_coordY_ball->setMaximumSize(QSize(16777215, 300));
        lcd_coordY_ball->setAutoFillBackground(false);
        lcd_coordY_ball->setStyleSheet(QStringLiteral("background-color: rgb(0, 100, 0);"));
        lcd_coordY_ball->setFrameShape(QFrame::Panel);
        lcd_coordY_ball->setFrameShadow(QFrame::Sunken);
        lcd_coordY_ball->setLineWidth(2);
        lcd_coordY_ball->setMidLineWidth(0);
        lcd_coordY_ball->setSmallDecimalPoint(false);
        lcd_coordY_ball->setDigitCount(5);
        lcd_coordY_ball->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_ball->setProperty("value", QVariant(0));
        lcd_coordX_ball = new QLCDNumber(frame_gameStatus);
        lcd_coordX_ball->setObjectName(QStringLiteral("lcd_coordX_ball"));
        lcd_coordX_ball->setGeometry(QRect(124, 55, 56, 27));
        sizePolicy.setHeightForWidth(lcd_coordX_ball->sizePolicy().hasHeightForWidth());
        lcd_coordX_ball->setSizePolicy(sizePolicy);
        lcd_coordX_ball->setMaximumSize(QSize(16777215, 300));
        lcd_coordX_ball->setAutoFillBackground(false);
        lcd_coordX_ball->setStyleSheet(QStringLiteral("background-color: rgb(0, 100, 0);"));
        lcd_coordX_ball->setFrameShape(QFrame::Panel);
        lcd_coordX_ball->setFrameShadow(QFrame::Sunken);
        lcd_coordX_ball->setLineWidth(2);
        lcd_coordX_ball->setSmallDecimalPoint(false);
        lcd_coordX_ball->setDigitCount(5);
        lcd_coordX_ball->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_ball->setProperty("value", QVariant(0));
        label_3 = new QLabel(frame_gameStatus);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(269, 55, 21, 31));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/images/mouse_cursor_0.png")));
        label_3->setScaledContents(true);
        lcd_coordX_cursor = new QLCDNumber(frame_gameStatus);
        lcd_coordX_cursor->setObjectName(QStringLiteral("lcd_coordX_cursor"));
        lcd_coordX_cursor->setGeometry(QRect(299, 55, 56, 27));
        sizePolicy.setHeightForWidth(lcd_coordX_cursor->sizePolicy().hasHeightForWidth());
        lcd_coordX_cursor->setSizePolicy(sizePolicy);
        lcd_coordX_cursor->setMaximumSize(QSize(16777215, 300));
        lcd_coordX_cursor->setAutoFillBackground(false);
        lcd_coordX_cursor->setStyleSheet(QLatin1String("background-color: rgb(0, 100, 0);\n"
""));
        lcd_coordX_cursor->setFrameShape(QFrame::Panel);
        lcd_coordX_cursor->setFrameShadow(QFrame::Sunken);
        lcd_coordX_cursor->setLineWidth(2);
        lcd_coordX_cursor->setSmallDecimalPoint(false);
        lcd_coordX_cursor->setDigitCount(5);
        lcd_coordX_cursor->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_cursor->setProperty("value", QVariant(0));
        lcd_coordY_cursor = new QLCDNumber(frame_gameStatus);
        lcd_coordY_cursor->setObjectName(QStringLiteral("lcd_coordY_cursor"));
        lcd_coordY_cursor->setGeometry(QRect(359, 55, 57, 27));
        sizePolicy.setHeightForWidth(lcd_coordY_cursor->sizePolicy().hasHeightForWidth());
        lcd_coordY_cursor->setSizePolicy(sizePolicy);
        lcd_coordY_cursor->setMaximumSize(QSize(16777215, 300));
        lcd_coordY_cursor->setAutoFillBackground(false);
        lcd_coordY_cursor->setStyleSheet(QLatin1String("background-color: rgb(0, 100, 0);\n"
""));
        lcd_coordY_cursor->setFrameShape(QFrame::Panel);
        lcd_coordY_cursor->setFrameShadow(QFrame::Sunken);
        lcd_coordY_cursor->setLineWidth(2);
        lcd_coordY_cursor->setMidLineWidth(0);
        lcd_coordY_cursor->setSmallDecimalPoint(false);
        lcd_coordY_cursor->setDigitCount(5);
        lcd_coordY_cursor->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_cursor->setProperty("value", QVariant(0));
        lcd_clock = new QLCDNumber(frame_gameStatus);
        lcd_clock->setObjectName(QStringLiteral("lcd_clock"));
        lcd_clock->setGeometry(QRect(325, 16, 91, 27));
        lcd_clock->setStyleSheet(QLatin1String("background-color: rgb(0, 100, 0);\n"
""));
        lcd_clock->setSmallDecimalPoint(false);
        lcd_clock->setDigitCount(8);
        lcd_clock->setMode(QLCDNumber::Dec);
        lcd_clock->setSegmentStyle(QLCDNumber::Outline);
        label_9 = new QLabel(frame_gameStatus);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(260, 6, 45, 45));
        label_9->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/images/stopwatch_1.png")));
        label_9->setScaledContents(true);
        btn_multithread = new QPushButton(frame_gameStatus);
        btn_multithread->setObjectName(QStringLiteral("btn_multithread"));
        btn_multithread->setEnabled(false);
        btn_multithread->setGeometry(QRect(190, 25, 51, 20));
        QFont font1;
        font1.setPointSize(8);
        btn_multithread->setFont(font1);
        btn_multithread->setFocusPolicy(Qt::NoFocus);
        btn_multithread->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        btn_multithread->setCheckable(false);
        label_multithreaded = new QLabel(frame_gameStatus);
        label_multithreaded->setObjectName(QStringLiteral("label_multithreaded"));
        label_multithreaded->setGeometry(QRect(110, 25, 81, 20));
        label_multithreaded->setFont(font1);
        label_multithreaded->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));

        gridLayout_2->addWidget(frame_gameStatus, 0, 1, 1, 1);

        frame_gameStatus_2 = new QFrame(mainScrollArea);
        frame_gameStatus_2->setObjectName(QStringLiteral("frame_gameStatus_2"));
        frame_gameStatus_2->setMinimumSize(QSize(200, 128));
        frame_gameStatus_2->setMaximumSize(QSize(16777215, 128));
        frame_gameStatus_2->setStyleSheet(QStringLiteral("background-color: rgb(200, 200, 200);"));
        frame_gameStatus_2->setFrameShape(QFrame::Panel);
        frame_gameStatus_2->setFrameShadow(QFrame::Raised);
        frame_gameStatus_2->setLineWidth(3);
        label_10 = new QLabel(frame_gameStatus_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(12, 12, 141, 15));
        label_10->setMaximumSize(QSize(16777215, 15));
        label_10->setFont(font);
        label_10->setIndent(1);
        text_guiPrint = new QTextBrowser(frame_gameStatus_2);
        text_guiPrint->setObjectName(QStringLiteral("text_guiPrint"));
        text_guiPrint->setGeometry(QRect(12, 33, 454, 84));
        text_guiPrint->setMinimumSize(QSize(0, 84));
        text_guiPrint->setMaximumSize(QSize(800, 16777215));
        QFont font2;
        font2.setFamily(QStringLiteral("DejaVu Sans"));
        font2.setPointSize(10);
        text_guiPrint->setFont(font2);
        text_guiPrint->setFocusPolicy(Qt::NoFocus);
        text_guiPrint->setStyleSheet(QStringLiteral("background-color: rgb(59, 59, 59);"));
        text_guiPrint->setFrameShape(QFrame::Panel);
        text_guiPrint->setFrameShadow(QFrame::Sunken);
        text_guiPrint->setLineWidth(3);
        text_guiPrint->setMidLineWidth(0);

        gridLayout_2->addWidget(frame_gameStatus_2, 0, 2, 1, 1);

        frame_field = new QFrame(mainScrollArea);
        frame_field->setObjectName(QStringLiteral("frame_field"));
        frame_field->setMinimumSize(QSize(897, 714));
        frame_field->setFrameShape(QFrame::Panel);
        frame_field->setFrameShadow(QFrame::Raised);
        frame_field->setLineWidth(4);
        gridLayout_3 = new QGridLayout(frame_field);
        gridLayout_3->setSpacing(2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        tabs_field = new QTabWidget(frame_field);
        tabs_field->setObjectName(QStringLiteral("tabs_field"));
        tabs_field->setMinimumSize(QSize(0, 70));
        tabs_field->setMaximumSize(QSize(16777215, 70));
        tabs_field->setFocusPolicy(Qt::NoFocus);
        tabs_field->setTabPosition(QTabWidget::North);
        tabs_field->setTabShape(QTabWidget::Rounded);
        tabs_field->setElideMode(Qt::ElideNone);
        tabs_field->setUsesScrollButtons(false);
        field = new QWidget();
        field->setObjectName(QStringLiteral("field"));
        frame_8 = new QFrame(field);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setGeometry(QRect(0, 0, 137, 38));
        frame_8->setFrameShape(QFrame::Panel);
        frame_8->setFrameShadow(QFrame::Sunken);
        frame_8->setLineWidth(3);
        check_fieldGrid = new QCheckBox(frame_8);
        check_fieldGrid->setObjectName(QStringLiteral("check_fieldGrid"));
        check_fieldGrid->setGeometry(QRect(4, 8, 59, 22));
        check_fieldGrid->setFocusPolicy(Qt::NoFocus);
        combo_gridScale = new QComboBox(frame_8);
        combo_gridScale->setObjectName(QStringLiteral("combo_gridScale"));
        combo_gridScale->setGeometry(QRect(67, 6, 65, 27));
        frame_9 = new QFrame(field);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setGeometry(QRect(140, 0, 245, 38));
        frame_9->setFrameShape(QFrame::Panel);
        frame_9->setFrameShadow(QFrame::Sunken);
        frame_9->setLineWidth(3);
        combo_fieldColor = new QComboBox(frame_9);
        combo_fieldColor->setObjectName(QStringLiteral("combo_fieldColor"));
        combo_fieldColor->setGeometry(QRect(126, 7, 112, 27));
        combo_fieldColor->setMinimumSize(QSize(112, 0));
        check_coloredGoals = new QCheckBox(frame_9);
        check_coloredGoals->setObjectName(QStringLiteral("check_coloredGoals"));
        check_coloredGoals->setGeometry(QRect(6, 10, 114, 22));
        check_coloredGoals->setFocusPolicy(Qt::NoFocus);
        check_coloredGoals->setChecked(true);
        combo_fieldProportions = new QComboBox(field);
        combo_fieldProportions->setObjectName(QStringLiteral("combo_fieldProportions"));
        combo_fieldProportions->setGeometry(QRect(400, 5, 96, 27));
        tabs_field->addTab(field, QString());
        tab_robots = new QWidget();
        tab_robots->setObjectName(QStringLiteral("tab_robots"));
        frame_10 = new QFrame(tab_robots);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setGeometry(QRect(400, 0, 271, 38));
        frame_10->setStyleSheet(QStringLiteral("background-color: rgba(255, 0, 0, 21);"));
        frame_10->setFrameShape(QFrame::Panel);
        frame_10->setFrameShadow(QFrame::Sunken);
        frame_10->setLineWidth(3);
        btn_override_none = new QPushButton(frame_10);
        btn_override_none->setObjectName(QStringLiteral("btn_override_none"));
        btn_override_none->setGeometry(QRect(190, 6, 75, 27));
        btn_override_none->setMaximumSize(QSize(75, 16777215));
        QPalette palette1;
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        btn_override_none->setPalette(palette1);
        btn_override_none->setFocusPolicy(Qt::NoFocus);
        btn_override_none->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        btn_override_all = new QPushButton(frame_10);
        btn_override_all->setObjectName(QStringLiteral("btn_override_all"));
        btn_override_all->setGeometry(QRect(8, 6, 75, 27));
        btn_override_all->setMaximumSize(QSize(75, 16777215));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        btn_override_all->setPalette(palette2);
        btn_override_all->setFocusPolicy(Qt::NoFocus);
        btn_override_all->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        label_6 = new QLabel(frame_10);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(86, 11, 101, 17));
        label_6->setMaximumSize(QSize(200, 500));
        label_6->setStyleSheet(QLatin1String("color: rgb(175, 0, 0);\n"
"background-color: rgba(255, 255, 255, 0);"));
        label_6->setAlignment(Qt::AlignCenter);
        frame_11 = new QFrame(tab_robots);
        frame_11->setObjectName(QStringLiteral("frame_11"));
        frame_11->setGeometry(QRect(180, 0, 218, 39));
        frame_11->setFrameShape(QFrame::Panel);
        frame_11->setFrameShadow(QFrame::Sunken);
        frame_11->setLineWidth(3);
        btn_botKick = new QPushButton(frame_11);
        btn_botKick->setObjectName(QStringLiteral("btn_botKick"));
        btn_botKick->setGeometry(QRect(150, 5, 60, 30));
        btn_botKick->setFocusPolicy(Qt::NoFocus);
        btn_botReverse = new QToolButton(frame_11);
        btn_botReverse->setObjectName(QStringLiteral("btn_botReverse"));
        btn_botReverse->setGeometry(QRect(25, 20, 40, 20));
        btn_botReverse->setFocusPolicy(Qt::NoFocus);
        btn_botDrible = new QPushButton(frame_11);
        btn_botDrible->setObjectName(QStringLiteral("btn_botDrible"));
        btn_botDrible->setGeometry(QRect(90, 5, 60, 30));
        btn_botDrible->setFocusPolicy(Qt::NoFocus);
        btn_botForward = new QToolButton(frame_11);
        btn_botForward->setObjectName(QStringLiteral("btn_botForward"));
        btn_botForward->setGeometry(QRect(25, 2, 40, 20));
        QFont font3;
        font3.setBold(false);
        font3.setWeight(50);
        btn_botForward->setFont(font3);
        btn_botForward->setFocusPolicy(Qt::NoFocus);
        btn_botTurnLeft = new QToolButton(frame_11);
        btn_botTurnLeft->setObjectName(QStringLiteral("btn_botTurnLeft"));
        btn_botTurnLeft->setGeometry(QRect(7, 2, 20, 38));
        btn_botTurnLeft->setFocusPolicy(Qt::NoFocus);
        btn_botTurnRight = new QToolButton(frame_11);
        btn_botTurnRight->setObjectName(QStringLiteral("btn_botTurnRight"));
        btn_botTurnRight->setGeometry(QRect(63, 2, 20, 38));
        btn_botTurnRight->setFocusPolicy(Qt::NoFocus);
        frame_12 = new QFrame(tab_robots);
        frame_12->setObjectName(QStringLiteral("frame_12"));
        frame_12->setGeometry(QRect(0, 0, 101, 38));
        frame_12->setFrameShape(QFrame::Panel);
        frame_12->setFrameShadow(QFrame::Sunken);
        frame_12->setLineWidth(3);
        check_showIDs = new QCheckBox(frame_12);
        check_showIDs->setObjectName(QStringLiteral("check_showIDs"));
        check_showIDs->setGeometry(QRect(6, 8, 88, 22));
        check_showIDs->setMaximumSize(QSize(95, 16777215));
        check_showIDs->setFocusPolicy(Qt::NoFocus);
        check_showIDs->setChecked(false);
        frame_13 = new QFrame(tab_robots);
        frame_13->setObjectName(QStringLiteral("frame_13"));
        frame_13->setGeometry(QRect(100, 0, 75, 37));
        frame_13->setFrameShape(QFrame::Panel);
        frame_13->setFrameShadow(QFrame::Sunken);
        frame_13->setLineWidth(3);
        combo_botScale = new QComboBox(frame_13);
        combo_botScale->setObjectName(QStringLiteral("combo_botScale"));
        combo_botScale->setGeometry(QRect(6, 5, 65, 27));
        combo_botScale->setMaximumSize(QSize(65, 16777215));
        tabs_field->addTab(tab_robots, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        frame_19 = new QFrame(tab);
        frame_19->setObjectName(QStringLiteral("frame_19"));
        frame_19->setGeometry(QRect(0, 0, 200, 38));
        frame_19->setFrameShape(QFrame::Panel);
        frame_19->setFrameShadow(QFrame::Sunken);
        frame_19->setLineWidth(3);
        layoutWidget = new QWidget(frame_19);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 0, 185, 41));
        layout_ball_specs = new QGridLayout(layoutWidget);
        layout_ball_specs->setObjectName(QStringLiteral("layout_ball_specs"));
        layout_ball_specs->setContentsMargins(0, 0, 0, 0);
        combo_ballColor = new QComboBox(layoutWidget);
        combo_ballColor->setObjectName(QStringLiteral("combo_ballColor"));
        combo_ballColor->setMinimumSize(QSize(112, 0));

        layout_ball_specs->addWidget(combo_ballColor, 0, 0, 1, 1);

        combo_ballScale = new QComboBox(layoutWidget);
        combo_ballScale->setObjectName(QStringLiteral("combo_ballScale"));

        layout_ball_specs->addWidget(combo_ballScale, 0, 1, 1, 1);

        tabs_field->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        frame_20 = new QFrame(tab_3);
        frame_20->setObjectName(QStringLiteral("frame_20"));
        frame_20->setGeometry(QRect(0, 0, 501, 38));
        frame_20->setFrameShape(QFrame::Panel);
        frame_20->setFrameShadow(QFrame::Sunken);
        frame_20->setLineWidth(3);
        layoutWidget1 = new QWidget(frame_20);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 0, 481, 41));
        layout_camera_controls = new QGridLayout(layoutWidget1);
        layout_camera_controls->setObjectName(QStringLiteral("layout_camera_controls"));
        layout_camera_controls->setContentsMargins(0, 0, 0, 0);
        btn_rotateField_left = new QPushButton(layoutWidget1);
        btn_rotateField_left->setObjectName(QStringLiteral("btn_rotateField_left"));
        btn_rotateField_left->setMinimumSize(QSize(40, 0));
        btn_rotateField_left->setMaximumSize(QSize(40, 27));
        QFont font4;
        font4.setPointSize(15);
        btn_rotateField_left->setFont(font4);
        btn_rotateField_left->setFocusPolicy(Qt::NoFocus);

        layout_camera_controls->addWidget(btn_rotateField_left, 0, 0, 1, 1);

        btn_rotateField_right = new QPushButton(layoutWidget1);
        btn_rotateField_right->setObjectName(QStringLiteral("btn_rotateField_right"));
        btn_rotateField_right->setMinimumSize(QSize(40, 0));
        btn_rotateField_right->setMaximumSize(QSize(40, 27));
        btn_rotateField_right->setFont(font4);
        btn_rotateField_right->setFocusPolicy(Qt::NoFocus);

        layout_camera_controls->addWidget(btn_rotateField_right, 0, 2, 1, 1);

        zoom_slider = new QSlider(layoutWidget1);
        zoom_slider->setObjectName(QStringLiteral("zoom_slider"));
        zoom_slider->setMinimumSize(QSize(80, 0));
        zoom_slider->setMaximumSize(QSize(303, 16777215));
        QFont font5;
        font5.setStrikeOut(false);
        font5.setKerning(true);
        zoom_slider->setFont(font5);
        zoom_slider->setFocusPolicy(Qt::WheelFocus);
        zoom_slider->setMinimum(6);
        zoom_slider->setMaximum(80);
        zoom_slider->setPageStep(3);
        zoom_slider->setValue(11);
        zoom_slider->setOrientation(Qt::Horizontal);
        zoom_slider->setInvertedAppearance(false);
        zoom_slider->setInvertedControls(false);
        zoom_slider->setTickPosition(QSlider::TicksAbove);
        zoom_slider->setTickInterval(5);

        layout_camera_controls->addWidget(zoom_slider, 0, 4, 1, 1);

        zoom_default = new QPushButton(layoutWidget1);
        zoom_default->setObjectName(QStringLiteral("zoom_default"));
        zoom_default->setMaximumSize(QSize(70, 30));
        zoom_default->setFocusPolicy(Qt::NoFocus);

        layout_camera_controls->addWidget(zoom_default, 0, 5, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(50, 30));
        label_2->setAlignment(Qt::AlignCenter);

        layout_camera_controls->addWidget(label_2, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(21, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        layout_camera_controls->addItem(horizontalSpacer, 0, 3, 1, 1);

        tabs_field->addTab(tab_3, QString());

        gridLayout_3->addWidget(tabs_field, 0, 0, 1, 1);

        gView_field = new QGraphicsView(frame_field);
        gView_field->setObjectName(QStringLiteral("gView_field"));
        gView_field->setMouseTracking(true);
        gView_field->setFocusPolicy(Qt::NoFocus);
        gView_field->setAutoFillBackground(false);
        gView_field->setFrameShape(QFrame::Panel);
        gView_field->setFrameShadow(QFrame::Sunken);
        gView_field->setLineWidth(4);
        gView_field->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        gView_field->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        gView_field->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        gView_field->setBackgroundBrush(brush2);
        QBrush brush3(QColor(0, 255, 37, 255));
        brush3.setStyle(Qt::NoBrush);
        gView_field->setForegroundBrush(brush3);
        gView_field->setInteractive(true);
        gView_field->setSceneRect(QRectF(0, 0, 0, 0));
        gView_field->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        gView_field->setDragMode(QGraphicsView::NoDrag);
        gView_field->setCacheMode(QGraphicsView::CacheBackground);
        gView_field->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        gView_field->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        gridLayout_3->addWidget(gView_field, 1, 0, 1, 1);


        gridLayout_2->addWidget(frame_field, 1, 1, 1, 2);

        frame_robotsPanel = new QFrame(mainScrollArea);
        frame_robotsPanel->setObjectName(QStringLiteral("frame_robotsPanel"));
        frame_robotsPanel->setMinimumSize(QSize(290, 0));
        frame_robotsPanel->setMaximumSize(QSize(291, 16777215));
        QFont font6;
        font6.setPointSize(9);
        font6.setBold(true);
        font6.setWeight(75);
        frame_robotsPanel->setFont(font6);
        frame_robotsPanel->setAutoFillBackground(false);
        frame_robotsPanel->setStyleSheet(QStringLiteral("background-color: rgb(225, 225, 255);"));
        frame_robotsPanel->setFrameShape(QFrame::Panel);
        frame_robotsPanel->setFrameShadow(QFrame::Raised);
        frame_robotsPanel->setLineWidth(3);
        gridLayout_7 = new QGridLayout(frame_robotsPanel);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setHorizontalSpacing(9);
        gridLayout_7->setVerticalSpacing(12);
        gridLayout_7->setContentsMargins(9, 9, 9, 9);
        frame_primeBot = new QFrame(frame_robotsPanel);
        frame_primeBot->setObjectName(QStringLiteral("frame_primeBot"));
        frame_primeBot->setEnabled(true);
        frame_primeBot->setMinimumSize(QSize(267, 350));
        frame_primeBot->setMaximumSize(QSize(16777215, 338));
        frame_primeBot->setFrameShape(QFrame::Panel);
        frame_primeBot->setFrameShadow(QFrame::Sunken);
        frame_primeBot->setLineWidth(3);
        box_primeBot = new QGroupBox(frame_primeBot);
        box_primeBot->setObjectName(QStringLiteral("box_primeBot"));
        box_primeBot->setGeometry(QRect(10, 10, 251, 333));
        box_primeBot->setMaximumSize(QSize(16777215, 340));
        QFont font7;
        font7.setBold(true);
        font7.setWeight(75);
        box_primeBot->setFont(font7);
        box_primeBot->setCheckable(false);
        gView_robot_prime = new QGraphicsView(box_primeBot);
        gView_robot_prime->setObjectName(QStringLiteral("gView_robot_prime"));
        gView_robot_prime->setGeometry(QRect(-4, 25, 90, 81));
        gView_robot_prime->setMouseTracking(true);
        gView_robot_prime->setFocusPolicy(Qt::NoFocus);
        gView_robot_prime->setFrameShape(QFrame::Panel);
        gView_robot_prime->setLineWidth(0);
        gView_robot_prime->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        check_botOverride = new QCheckBox(box_primeBot);
        check_botOverride->setObjectName(QStringLiteral("check_botOverride"));
        check_botOverride->setGeometry(QRect(0, 110, 84, 20));
        QFont font8;
        font8.setPointSize(10);
        font8.setBold(true);
        font8.setItalic(false);
        font8.setWeight(75);
        check_botOverride->setFont(font8);
        check_botOverride->setFocusPolicy(Qt::NoFocus);
        check_botOverride->setLayoutDirection(Qt::RightToLeft);
        check_botOverride->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: rgba(255, 0, 0, 100);"));
        text_primeBot = new QTextBrowser(box_primeBot);
        text_primeBot->setObjectName(QStringLiteral("text_primeBot"));
        text_primeBot->setGeometry(QRect(91, 30, 155, 100));
        QFont font9;
        font9.setFamily(QStringLiteral("DejaVu Sans"));
        font9.setPointSize(8);
        font9.setBold(true);
        font9.setWeight(75);
        text_primeBot->setFont(font9);
        text_primeBot->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        text_primeBot->setFrameShape(QFrame::Panel);
        text_primeBot->setLineWidth(2);
        label_coord_6 = new QLabel(box_primeBot);
        label_coord_6->setObjectName(QStringLiteral("label_coord_6"));
        label_coord_6->setGeometry(QRect(26, 139, 61, 20));
        label_coord_6->setMinimumSize(QSize(42, 0));
        label_coord_6->setBaseSize(QSize(20, 0));
        label_coord_6->setFont(font8);
        label_coord_6->setIndent(0);
        lcd_coordX_prime = new QLCDNumber(box_primeBot);
        lcd_coordX_prime->setObjectName(QStringLiteral("lcd_coordX_prime"));
        lcd_coordX_prime->setGeometry(QRect(91, 136, 56, 25));
        sizePolicy.setHeightForWidth(lcd_coordX_prime->sizePolicy().hasHeightForWidth());
        lcd_coordX_prime->setSizePolicy(sizePolicy);
        lcd_coordX_prime->setMinimumSize(QSize(56, 23));
        lcd_coordX_prime->setMaximumSize(QSize(56, 32));
        lcd_coordX_prime->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_prime->setAutoFillBackground(false);
        lcd_coordX_prime->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_prime->setFrameShape(QFrame::Panel);
        lcd_coordX_prime->setFrameShadow(QFrame::Sunken);
        lcd_coordX_prime->setLineWidth(2);
        lcd_coordX_prime->setMidLineWidth(0);
        lcd_coordX_prime->setSmallDecimalPoint(false);
        lcd_coordX_prime->setDigitCount(5);
        lcd_coordX_prime->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_prime->setProperty("value", QVariant(0));
        lcd_coordY_prime = new QLCDNumber(box_primeBot);
        lcd_coordY_prime->setObjectName(QStringLiteral("lcd_coordY_prime"));
        lcd_coordY_prime->setGeometry(QRect(153, 136, 56, 25));
        sizePolicy.setHeightForWidth(lcd_coordY_prime->sizePolicy().hasHeightForWidth());
        lcd_coordY_prime->setSizePolicy(sizePolicy);
        lcd_coordY_prime->setMinimumSize(QSize(56, 25));
        lcd_coordY_prime->setMaximumSize(QSize(56, 25));
        lcd_coordY_prime->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_prime->setAutoFillBackground(false);
        lcd_coordY_prime->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_prime->setInputMethodHints(Qt::ImhTime);
        lcd_coordY_prime->setFrameShape(QFrame::Panel);
        lcd_coordY_prime->setFrameShadow(QFrame::Sunken);
        lcd_coordY_prime->setLineWidth(2);
        lcd_coordY_prime->setSmallDecimalPoint(false);
        lcd_coordY_prime->setDigitCount(5);
        lcd_coordY_prime->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_prime->setProperty("value", QVariant(0));
        frame_18 = new QFrame(box_primeBot);
        frame_18->setObjectName(QStringLiteral("frame_18"));
        frame_18->setGeometry(QRect(3, 168, 241, 163));
        frame_18->setStyleSheet(QStringLiteral(""));
        frame_18->setFrameShape(QFrame::Panel);
        frame_18->setFrameShadow(QFrame::Raised);
        frame_18->setLineWidth(2);
        lcd_orient_prime = new QLCDNumber(frame_18);
        lcd_orient_prime->setObjectName(QStringLiteral("lcd_orient_prime"));
        lcd_orient_prime->setGeometry(QRect(170, 100, 40, 23));
        sizePolicy.setHeightForWidth(lcd_orient_prime->sizePolicy().hasHeightForWidth());
        lcd_orient_prime->setSizePolicy(sizePolicy);
        lcd_orient_prime->setMaximumSize(QSize(73, 23));
        lcd_orient_prime->setMouseTracking(true);
        lcd_orient_prime->setLayoutDirection(Qt::RightToLeft);
        lcd_orient_prime->setAutoFillBackground(false);
        lcd_orient_prime->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 0);"));
        lcd_orient_prime->setFrameShape(QFrame::Panel);
        lcd_orient_prime->setFrameShadow(QFrame::Sunken);
        lcd_orient_prime->setLineWidth(2);
        lcd_orient_prime->setSmallDecimalPoint(false);
        lcd_orient_prime->setDigitCount(4);
        lcd_orient_prime->setSegmentStyle(QLCDNumber::Outline);
        lcd_orient_prime->setProperty("value", QVariant(0));
        lcd_botVel_ = new QLCDNumber(frame_18);
        lcd_botVel_->setObjectName(QStringLiteral("lcd_botVel_"));
        lcd_botVel_->setGeometry(QRect(30, 100, 40, 22));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lcd_botVel_->sizePolicy().hasHeightForWidth());
        lcd_botVel_->setSizePolicy(sizePolicy1);
        lcd_botVel_->setMaximumSize(QSize(16777215, 100));
        QFont font10;
        font10.setBold(false);
        font10.setItalic(false);
        font10.setUnderline(false);
        font10.setWeight(50);
        font10.setStrikeOut(false);
        font10.setKerning(true);
        lcd_botVel_->setFont(font10);
        lcd_botVel_->setMouseTracking(true);
        lcd_botVel_->setToolTipDuration(-2);
        lcd_botVel_->setLayoutDirection(Qt::RightToLeft);
        lcd_botVel_->setAutoFillBackground(false);
        lcd_botVel_->setStyleSheet(QStringLiteral("background-color: rgb(100, 100, 100);"));
        lcd_botVel_->setFrameShape(QFrame::Panel);
        lcd_botVel_->setFrameShadow(QFrame::Sunken);
        lcd_botVel_->setLineWidth(3);
        lcd_botVel_->setSmallDecimalPoint(false);
        lcd_botVel_->setDigitCount(4);
        lcd_botVel_->setSegmentStyle(QLCDNumber::Outline);
        lcd_botVel_->setProperty("value", QVariant(888888));
        label_4 = new QLabel(frame_18);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 42, 80, 21));
        label_4->setFont(font8);
        label_4->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        label_4->setAlignment(Qt::AlignCenter);
        dial_botVel_ = new QDial(frame_18);
        dial_botVel_->setObjectName(QStringLiteral("dial_botVel_"));
        dial_botVel_->setEnabled(false);
        dial_botVel_->setGeometry(QRect(5, 7, 90, 90));
        dial_botVel_->setBaseSize(QSize(0, 0));
        dial_botVel_->setCursor(QCursor(Qt::ForbiddenCursor));
        dial_botVel_->setFocusPolicy(Qt::NoFocus);
        dial_botVel_->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botVel_->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_->setMinimum(-100);
        dial_botVel_->setMaximum(100);
        dial_botVel_->setSingleStep(1);
        dial_botVel_->setPageStep(20);
        dial_botVel_->setTracking(false);
        dial_botVel_->setInvertedAppearance(false);
        dial_botVel_->setInvertedControls(false);
        dial_botVel_->setWrapping(false);
        dial_botVel_->setNotchTarget(2);
        dial_botVel_->setNotchesVisible(true);
        label_11 = new QLabel(frame_18);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(80, 105, 80, 21));
        label_11->setFont(font8);
        label_11->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        label_11->setAlignment(Qt::AlignCenter);
        dial_botSpeed_ = new QDial(frame_18);
        dial_botSpeed_->setObjectName(QStringLiteral("dial_botSpeed_"));
        dial_botSpeed_->setEnabled(false);
        dial_botSpeed_->setGeometry(QRect(75, 72, 90, 90));
        dial_botSpeed_->setBaseSize(QSize(0, 0));
        dial_botSpeed_->setCursor(QCursor(Qt::ForbiddenCursor));
        dial_botSpeed_->setFocusPolicy(Qt::NoFocus);
        dial_botSpeed_->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botSpeed_->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botSpeed_->setMinimum(0);
        dial_botSpeed_->setMaximum(1000);
        dial_botSpeed_->setSingleStep(1);
        dial_botSpeed_->setPageStep(100);
        dial_botSpeed_->setTracking(false);
        dial_botSpeed_->setInvertedAppearance(false);
        dial_botSpeed_->setInvertedControls(false);
        dial_botSpeed_->setWrapping(false);
        dial_botSpeed_->setNotchTarget(10);
        dial_botSpeed_->setNotchesVisible(true);
        lcd_botSpeed_ = new QLCDNumber(frame_18);
        lcd_botSpeed_->setObjectName(QStringLiteral("lcd_botSpeed_"));
        lcd_botSpeed_->setGeometry(QRect(100, 45, 40, 22));
        sizePolicy1.setHeightForWidth(lcd_botSpeed_->sizePolicy().hasHeightForWidth());
        lcd_botSpeed_->setSizePolicy(sizePolicy1);
        lcd_botSpeed_->setMaximumSize(QSize(16777215, 100));
        lcd_botSpeed_->setFont(font10);
        lcd_botSpeed_->setMouseTracking(true);
        lcd_botSpeed_->setToolTipDuration(-2);
        lcd_botSpeed_->setLayoutDirection(Qt::RightToLeft);
        lcd_botSpeed_->setAutoFillBackground(false);
        lcd_botSpeed_->setStyleSheet(QStringLiteral("background-color: rgb(100, 100, 100);"));
        lcd_botSpeed_->setFrameShape(QFrame::Panel);
        lcd_botSpeed_->setFrameShadow(QFrame::Sunken);
        lcd_botSpeed_->setLineWidth(3);
        lcd_botSpeed_->setSmallDecimalPoint(false);
        lcd_botSpeed_->setDigitCount(4);
        lcd_botSpeed_->setSegmentStyle(QLCDNumber::Outline);
        lcd_botSpeed_->setProperty("value", QVariant(0));
        dial_botOrient_prime = new QDial(frame_18);
        dial_botOrient_prime->setObjectName(QStringLiteral("dial_botOrient_prime"));
        dial_botOrient_prime->setEnabled(false);
        dial_botOrient_prime->setGeometry(QRect(145, 7, 90, 90));
        dial_botOrient_prime->setBaseSize(QSize(0, 0));
        dial_botOrient_prime->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_prime->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_prime->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_prime->setMinimum(-180);
        dial_botOrient_prime->setMaximum(180);
        dial_botOrient_prime->setSingleStep(1);
        dial_botOrient_prime->setPageStep(45);
        dial_botOrient_prime->setValue(0);
        dial_botOrient_prime->setTracking(false);
        dial_botOrient_prime->setOrientation(Qt::Horizontal);
        dial_botOrient_prime->setInvertedAppearance(true);
        dial_botOrient_prime->setInvertedControls(false);
        dial_botOrient_prime->setWrapping(true);
        dial_botOrient_prime->setNotchTarget(8.7);
        dial_botOrient_prime->setNotchesVisible(true);
        label_5 = new QLabel(frame_18);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(150, 43, 80, 21));
        label_5->setFont(font8);
        label_5->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        label_5->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(frame_18);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(184, 20, 12, 22));
        label_7->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/images/forward_arrow_0.png")));
        label_7->setScaledContents(true);

        gridLayout_7->addWidget(frame_primeBot, 1, 0, 1, 1);

        scroll_robots = new QScrollArea(frame_robotsPanel);
        scroll_robots->setObjectName(QStringLiteral("scroll_robots"));
        scroll_robots->setMinimumSize(QSize(276, 0));
        scroll_robots->setFrameShape(QFrame::Panel);
        scroll_robots->setFrameShadow(QFrame::Plain);
        scroll_robots->setLineWidth(0);
        scroll_robots->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll_robots->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_robots->setWidgetResizable(false);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 269, 869));
        layoutWidget2 = new QWidget(scrollAreaWidgetContents);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(6, 6, 261, 831));
        layout_robots = new QVBoxLayout(layoutWidget2);
        layout_robots->setSpacing(0);
        layout_robots->setObjectName(QStringLiteral("layout_robots"));
        layout_robots->setContentsMargins(0, 0, 10, 0);
        frame_robot_0 = new QFrame(layoutWidget2);
        frame_robot_0->setObjectName(QStringLiteral("frame_robot_0"));
        frame_robot_0->setEnabled(true);
        frame_robot_0->setMaximumSize(QSize(16777215, 80));
        QFont font11;
        font11.setPointSize(8);
        font11.setBold(true);
        font11.setWeight(75);
        frame_robot_0->setFont(font11);
        frame_robot_0->setMouseTracking(false);
        frame_robot_0->setFocusPolicy(Qt::NoFocus);
        frame_robot_0->setAutoFillBackground(false);
        frame_robot_0->setFrameShape(QFrame::Panel);
        frame_robot_0->setFrameShadow(QFrame::Sunken);
        frame_robot_0->setLineWidth(3);
        frame_robot_0->setMidLineWidth(0);
        title_robPanel_0 = new QLabel(frame_robot_0);
        title_robPanel_0->setObjectName(QStringLiteral("title_robPanel_0"));
        title_robPanel_0->setGeometry(QRect(8, 5, 55, 21));
        QFont font12;
        font12.setFamily(QStringLiteral("Courier 10 Pitch"));
        font12.setPointSize(10);
        font12.setBold(true);
        font12.setWeight(75);
        title_robPanel_0->setFont(font12);
        title_robPanel_0->setAutoFillBackground(false);
        title_robPanel_0->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_0->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gView_robot_0 = new QGraphicsView(frame_robot_0);
        gView_robot_0->setObjectName(QStringLiteral("gView_robot_0"));
        gView_robot_0->setGeometry(QRect(10, 20, 45, 45));
        gView_robot_0->setMouseTracking(true);
        gView_robot_0->setFocusPolicy(Qt::NoFocus);
        gView_robot_0->setFrameShape(QFrame::Panel);
        gView_robot_0->setLineWidth(0);
        QBrush brush4(QColor(0, 0, 0, 0));
        brush4.setStyle(Qt::NoBrush);
        gView_robot_0->setBackgroundBrush(brush4);
        gView_robot_0->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        dial_botVel_0 = new QDial(frame_robot_0);
        dial_botVel_0->setObjectName(QStringLiteral("dial_botVel_0"));
        dial_botVel_0->setEnabled(false);
        dial_botVel_0->setGeometry(QRect(60, 5, 45, 45));
        dial_botVel_0->setFocusPolicy(Qt::NoFocus);
        dial_botVel_0->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_0->setMinimum(-100);
        dial_botVel_0->setMaximum(100);
        dial_botVel_0->setSingleStep(1);
        dial_botVel_0->setPageStep(10);
        dial_botVel_0->setWrapping(false);
        dial_botVel_0->setNotchTarget(4);
        dial_botVel_0->setNotchesVisible(false);
        dial_botOrient_0 = new QDial(frame_robot_0);
        dial_botOrient_0->setObjectName(QStringLiteral("dial_botOrient_0"));
        dial_botOrient_0->setEnabled(false);
        dial_botOrient_0->setGeometry(QRect(124, 5, 45, 45));
        dial_botOrient_0->setBaseSize(QSize(0, 0));
        dial_botOrient_0->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_0->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_0->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_0->setMinimum(-180);
        dial_botOrient_0->setMaximum(180);
        dial_botOrient_0->setSingleStep(1);
        dial_botOrient_0->setPageStep(5);
        dial_botOrient_0->setTracking(false);
        dial_botOrient_0->setInvertedAppearance(true);
        dial_botOrient_0->setWrapping(true);
        dial_botOrient_0->setNotchTarget(8.7);
        dial_botOrient_0->setNotchesVisible(false);
        frame_2 = new QFrame(frame_robot_0);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(170, 5, 81, 71));
        frame_2->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Plain);
        frame_2->setLineWidth(0);
        gridLayout_5 = new QGridLayout(frame_2);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setVerticalSpacing(0);
        gridLayout_5->setContentsMargins(9, 9, 9, 9);
        lcd_coordY_0 = new QLCDNumber(frame_2);
        lcd_coordY_0->setObjectName(QStringLiteral("lcd_coordY_0"));
        lcd_coordY_0->setMaximumSize(QSize(16777215, 18));
        lcd_coordY_0->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_0->setAutoFillBackground(false);
        lcd_coordY_0->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_0->setFrameShape(QFrame::Panel);
        lcd_coordY_0->setFrameShadow(QFrame::Sunken);
        lcd_coordY_0->setLineWidth(2);
        lcd_coordY_0->setSmallDecimalPoint(false);
        lcd_coordY_0->setDigitCount(5);
        lcd_coordY_0->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_0->setProperty("value", QVariant(0));

        gridLayout_5->addWidget(lcd_coordY_0, 1, 1, 1, 1);

        lcd_coordX_0 = new QLCDNumber(frame_2);
        lcd_coordX_0->setObjectName(QStringLiteral("lcd_coordX_0"));
        lcd_coordX_0->setMaximumSize(QSize(16777215, 18));
        lcd_coordX_0->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_0->setAutoFillBackground(false);
        lcd_coordX_0->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_0->setFrameShape(QFrame::Panel);
        lcd_coordX_0->setFrameShadow(QFrame::Sunken);
        lcd_coordX_0->setLineWidth(2);
        lcd_coordX_0->setMidLineWidth(0);
        lcd_coordX_0->setSmallDecimalPoint(false);
        lcd_coordX_0->setDigitCount(5);
        lcd_coordX_0->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_0->setProperty("value", QVariant(0));

        gridLayout_5->addWidget(lcd_coordX_0, 0, 1, 1, 1);

        label_14 = new QLabel(frame_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_5->addWidget(label_14, 0, 0, 1, 1);

        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_5->addWidget(label_13, 1, 0, 1, 1);

        currBehavior_0 = new QLabel(frame_robot_0);
        currBehavior_0->setObjectName(QStringLiteral("currBehavior_0"));
        currBehavior_0->setGeometry(QRect(60, 50, 101, 20));
        currBehavior_0->setFont(font1);

        layout_robots->addWidget(frame_robot_0);

        frame_robot_1 = new QFrame(layoutWidget2);
        frame_robot_1->setObjectName(QStringLiteral("frame_robot_1"));
        frame_robot_1->setEnabled(true);
        frame_robot_1->setMaximumSize(QSize(16777215, 80));
        frame_robot_1->setFont(font11);
        frame_robot_1->setMouseTracking(false);
        frame_robot_1->setFocusPolicy(Qt::NoFocus);
        frame_robot_1->setAutoFillBackground(false);
        frame_robot_1->setFrameShape(QFrame::Panel);
        frame_robot_1->setFrameShadow(QFrame::Sunken);
        frame_robot_1->setLineWidth(3);
        frame_robot_1->setMidLineWidth(0);
        title_robPanel_1 = new QLabel(frame_robot_1);
        title_robPanel_1->setObjectName(QStringLiteral("title_robPanel_1"));
        title_robPanel_1->setGeometry(QRect(8, 5, 55, 21));
        title_robPanel_1->setFont(font12);
        title_robPanel_1->setAutoFillBackground(false);
        title_robPanel_1->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gView_robot_1 = new QGraphicsView(frame_robot_1);
        gView_robot_1->setObjectName(QStringLiteral("gView_robot_1"));
        gView_robot_1->setGeometry(QRect(10, 20, 45, 45));
        gView_robot_1->setMouseTracking(true);
        gView_robot_1->setFocusPolicy(Qt::NoFocus);
        gView_robot_1->setFrameShape(QFrame::Panel);
        gView_robot_1->setLineWidth(0);
        QBrush brush5(QColor(0, 0, 0, 0));
        brush5.setStyle(Qt::NoBrush);
        gView_robot_1->setBackgroundBrush(brush5);
        gView_robot_1->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        dial_botVel_1 = new QDial(frame_robot_1);
        dial_botVel_1->setObjectName(QStringLiteral("dial_botVel_1"));
        dial_botVel_1->setEnabled(false);
        dial_botVel_1->setGeometry(QRect(60, 5, 45, 45));
        dial_botVel_1->setFocusPolicy(Qt::NoFocus);
        dial_botVel_1->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_1->setMinimum(-100);
        dial_botVel_1->setMaximum(100);
        dial_botVel_1->setSingleStep(1);
        dial_botVel_1->setPageStep(10);
        dial_botVel_1->setWrapping(false);
        dial_botVel_1->setNotchTarget(4);
        dial_botVel_1->setNotchesVisible(false);
        dial_botOrient_1 = new QDial(frame_robot_1);
        dial_botOrient_1->setObjectName(QStringLiteral("dial_botOrient_1"));
        dial_botOrient_1->setEnabled(false);
        dial_botOrient_1->setGeometry(QRect(124, 5, 45, 45));
        dial_botOrient_1->setBaseSize(QSize(0, 0));
        dial_botOrient_1->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_1->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_1->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_1->setMinimum(-180);
        dial_botOrient_1->setMaximum(180);
        dial_botOrient_1->setSingleStep(1);
        dial_botOrient_1->setPageStep(5);
        dial_botOrient_1->setTracking(false);
        dial_botOrient_1->setInvertedAppearance(true);
        dial_botOrient_1->setWrapping(true);
        dial_botOrient_1->setNotchTarget(8.7);
        dial_botOrient_1->setNotchesVisible(false);
        frame_3 = new QFrame(frame_robot_1);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(170, 5, 81, 71));
        frame_3->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Plain);
        frame_3->setLineWidth(0);
        gridLayout_6 = new QGridLayout(frame_3);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setVerticalSpacing(0);
        gridLayout_6->setContentsMargins(9, 9, 9, 9);
        lcd_coordY_1 = new QLCDNumber(frame_3);
        lcd_coordY_1->setObjectName(QStringLiteral("lcd_coordY_1"));
        lcd_coordY_1->setMaximumSize(QSize(16777215, 18));
        lcd_coordY_1->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_1->setAutoFillBackground(false);
        lcd_coordY_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_1->setFrameShape(QFrame::Panel);
        lcd_coordY_1->setFrameShadow(QFrame::Sunken);
        lcd_coordY_1->setLineWidth(2);
        lcd_coordY_1->setSmallDecimalPoint(false);
        lcd_coordY_1->setDigitCount(5);
        lcd_coordY_1->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_1->setProperty("value", QVariant(0));

        gridLayout_6->addWidget(lcd_coordY_1, 1, 1, 1, 1);

        lcd_coordX_1 = new QLCDNumber(frame_3);
        lcd_coordX_1->setObjectName(QStringLiteral("lcd_coordX_1"));
        lcd_coordX_1->setMaximumSize(QSize(16777215, 18));
        lcd_coordX_1->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_1->setAutoFillBackground(false);
        lcd_coordX_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_1->setFrameShape(QFrame::Panel);
        lcd_coordX_1->setFrameShadow(QFrame::Sunken);
        lcd_coordX_1->setLineWidth(2);
        lcd_coordX_1->setMidLineWidth(0);
        lcd_coordX_1->setSmallDecimalPoint(false);
        lcd_coordX_1->setDigitCount(5);
        lcd_coordX_1->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_1->setProperty("value", QVariant(0));

        gridLayout_6->addWidget(lcd_coordX_1, 0, 1, 1, 1);

        label_15 = new QLabel(frame_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_6->addWidget(label_15, 0, 0, 1, 1);

        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_6->addWidget(label_16, 1, 0, 1, 1);

        currBehavior_1 = new QLabel(frame_robot_1);
        currBehavior_1->setObjectName(QStringLiteral("currBehavior_1"));
        currBehavior_1->setGeometry(QRect(60, 50, 101, 20));
        currBehavior_1->setFont(font1);

        layout_robots->addWidget(frame_robot_1);

        frame_robot_2 = new QFrame(layoutWidget2);
        frame_robot_2->setObjectName(QStringLiteral("frame_robot_2"));
        frame_robot_2->setEnabled(true);
        frame_robot_2->setMaximumSize(QSize(16777215, 80));
        frame_robot_2->setFont(font11);
        frame_robot_2->setMouseTracking(false);
        frame_robot_2->setFocusPolicy(Qt::NoFocus);
        frame_robot_2->setAutoFillBackground(false);
        frame_robot_2->setFrameShape(QFrame::Panel);
        frame_robot_2->setFrameShadow(QFrame::Sunken);
        frame_robot_2->setLineWidth(3);
        frame_robot_2->setMidLineWidth(0);
        title_robPanel_2 = new QLabel(frame_robot_2);
        title_robPanel_2->setObjectName(QStringLiteral("title_robPanel_2"));
        title_robPanel_2->setGeometry(QRect(8, 5, 55, 21));
        title_robPanel_2->setFont(font12);
        title_robPanel_2->setAutoFillBackground(false);
        title_robPanel_2->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gView_robot_2 = new QGraphicsView(frame_robot_2);
        gView_robot_2->setObjectName(QStringLiteral("gView_robot_2"));
        gView_robot_2->setGeometry(QRect(10, 20, 45, 45));
        gView_robot_2->setMouseTracking(true);
        gView_robot_2->setFocusPolicy(Qt::NoFocus);
        gView_robot_2->setFrameShape(QFrame::Panel);
        gView_robot_2->setLineWidth(0);
        QBrush brush6(QColor(0, 0, 0, 0));
        brush6.setStyle(Qt::NoBrush);
        gView_robot_2->setBackgroundBrush(brush6);
        gView_robot_2->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        dial_botVel_2 = new QDial(frame_robot_2);
        dial_botVel_2->setObjectName(QStringLiteral("dial_botVel_2"));
        dial_botVel_2->setEnabled(false);
        dial_botVel_2->setGeometry(QRect(60, 5, 45, 45));
        dial_botVel_2->setFocusPolicy(Qt::NoFocus);
        dial_botVel_2->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_2->setMinimum(-100);
        dial_botVel_2->setMaximum(100);
        dial_botVel_2->setSingleStep(1);
        dial_botVel_2->setPageStep(10);
        dial_botVel_2->setWrapping(false);
        dial_botVel_2->setNotchTarget(4);
        dial_botVel_2->setNotchesVisible(false);
        dial_botOrient_2 = new QDial(frame_robot_2);
        dial_botOrient_2->setObjectName(QStringLiteral("dial_botOrient_2"));
        dial_botOrient_2->setEnabled(false);
        dial_botOrient_2->setGeometry(QRect(124, 5, 45, 45));
        dial_botOrient_2->setBaseSize(QSize(0, 0));
        dial_botOrient_2->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_2->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_2->setMinimum(-180);
        dial_botOrient_2->setMaximum(180);
        dial_botOrient_2->setSingleStep(1);
        dial_botOrient_2->setPageStep(5);
        dial_botOrient_2->setTracking(false);
        dial_botOrient_2->setInvertedAppearance(true);
        dial_botOrient_2->setWrapping(true);
        dial_botOrient_2->setNotchTarget(8.7);
        dial_botOrient_2->setNotchesVisible(false);
        frame_4 = new QFrame(frame_robot_2);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(170, 5, 81, 71));
        frame_4->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Plain);
        frame_4->setLineWidth(0);
        gridLayout_8 = new QGridLayout(frame_4);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setVerticalSpacing(0);
        gridLayout_8->setContentsMargins(9, 9, 9, 9);
        lcd_coordY_2 = new QLCDNumber(frame_4);
        lcd_coordY_2->setObjectName(QStringLiteral("lcd_coordY_2"));
        lcd_coordY_2->setMaximumSize(QSize(16777215, 18));
        lcd_coordY_2->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_2->setAutoFillBackground(false);
        lcd_coordY_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_2->setFrameShape(QFrame::Panel);
        lcd_coordY_2->setFrameShadow(QFrame::Sunken);
        lcd_coordY_2->setLineWidth(2);
        lcd_coordY_2->setSmallDecimalPoint(false);
        lcd_coordY_2->setDigitCount(5);
        lcd_coordY_2->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_2->setProperty("value", QVariant(0));

        gridLayout_8->addWidget(lcd_coordY_2, 1, 1, 1, 1);

        lcd_coordX_2 = new QLCDNumber(frame_4);
        lcd_coordX_2->setObjectName(QStringLiteral("lcd_coordX_2"));
        lcd_coordX_2->setMaximumSize(QSize(16777215, 18));
        lcd_coordX_2->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_2->setAutoFillBackground(false);
        lcd_coordX_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_2->setFrameShape(QFrame::Panel);
        lcd_coordX_2->setFrameShadow(QFrame::Sunken);
        lcd_coordX_2->setLineWidth(2);
        lcd_coordX_2->setMidLineWidth(0);
        lcd_coordX_2->setSmallDecimalPoint(false);
        lcd_coordX_2->setDigitCount(5);
        lcd_coordX_2->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_2->setProperty("value", QVariant(0));

        gridLayout_8->addWidget(lcd_coordX_2, 0, 1, 1, 1);

        label_17 = new QLabel(frame_4);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_8->addWidget(label_17, 0, 0, 1, 1);

        label_18 = new QLabel(frame_4);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_8->addWidget(label_18, 1, 0, 1, 1);

        currBehavior_2 = new QLabel(frame_robot_2);
        currBehavior_2->setObjectName(QStringLiteral("currBehavior_2"));
        currBehavior_2->setGeometry(QRect(60, 50, 101, 20));
        currBehavior_2->setFont(font1);

        layout_robots->addWidget(frame_robot_2);

        frame_robot_3 = new QFrame(layoutWidget2);
        frame_robot_3->setObjectName(QStringLiteral("frame_robot_3"));
        frame_robot_3->setEnabled(true);
        frame_robot_3->setMaximumSize(QSize(16777215, 80));
        frame_robot_3->setFont(font11);
        frame_robot_3->setMouseTracking(false);
        frame_robot_3->setFocusPolicy(Qt::NoFocus);
        frame_robot_3->setAutoFillBackground(false);
        frame_robot_3->setFrameShape(QFrame::Panel);
        frame_robot_3->setFrameShadow(QFrame::Sunken);
        frame_robot_3->setLineWidth(3);
        frame_robot_3->setMidLineWidth(0);
        title_robPanel_3 = new QLabel(frame_robot_3);
        title_robPanel_3->setObjectName(QStringLiteral("title_robPanel_3"));
        title_robPanel_3->setGeometry(QRect(8, 5, 55, 21));
        title_robPanel_3->setFont(font12);
        title_robPanel_3->setAutoFillBackground(false);
        title_robPanel_3->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gView_robot_3 = new QGraphicsView(frame_robot_3);
        gView_robot_3->setObjectName(QStringLiteral("gView_robot_3"));
        gView_robot_3->setGeometry(QRect(10, 20, 45, 45));
        gView_robot_3->setMouseTracking(true);
        gView_robot_3->setFocusPolicy(Qt::NoFocus);
        gView_robot_3->setFrameShape(QFrame::Panel);
        gView_robot_3->setLineWidth(0);
        QBrush brush7(QColor(0, 0, 0, 0));
        brush7.setStyle(Qt::NoBrush);
        gView_robot_3->setBackgroundBrush(brush7);
        gView_robot_3->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        dial_botVel_3 = new QDial(frame_robot_3);
        dial_botVel_3->setObjectName(QStringLiteral("dial_botVel_3"));
        dial_botVel_3->setEnabled(false);
        dial_botVel_3->setGeometry(QRect(60, 5, 45, 45));
        dial_botVel_3->setFocusPolicy(Qt::NoFocus);
        dial_botVel_3->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_3->setMinimum(-100);
        dial_botVel_3->setMaximum(100);
        dial_botVel_3->setSingleStep(1);
        dial_botVel_3->setPageStep(10);
        dial_botVel_3->setWrapping(false);
        dial_botVel_3->setNotchTarget(4);
        dial_botVel_3->setNotchesVisible(false);
        dial_botOrient_3 = new QDial(frame_robot_3);
        dial_botOrient_3->setObjectName(QStringLiteral("dial_botOrient_3"));
        dial_botOrient_3->setEnabled(false);
        dial_botOrient_3->setGeometry(QRect(124, 5, 45, 45));
        dial_botOrient_3->setBaseSize(QSize(0, 0));
        dial_botOrient_3->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_3->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_3->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_3->setMinimum(-180);
        dial_botOrient_3->setMaximum(180);
        dial_botOrient_3->setSingleStep(1);
        dial_botOrient_3->setPageStep(5);
        dial_botOrient_3->setTracking(false);
        dial_botOrient_3->setInvertedAppearance(true);
        dial_botOrient_3->setWrapping(true);
        dial_botOrient_3->setNotchTarget(8.7);
        dial_botOrient_3->setNotchesVisible(false);
        frame_5 = new QFrame(frame_robot_3);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setGeometry(QRect(170, 5, 81, 71));
        frame_5->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        frame_5->setFrameShape(QFrame::NoFrame);
        frame_5->setFrameShadow(QFrame::Plain);
        frame_5->setLineWidth(0);
        gridLayout_9 = new QGridLayout(frame_5);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setVerticalSpacing(0);
        gridLayout_9->setContentsMargins(9, 9, 9, 9);
        lcd_coordY_3 = new QLCDNumber(frame_5);
        lcd_coordY_3->setObjectName(QStringLiteral("lcd_coordY_3"));
        lcd_coordY_3->setMaximumSize(QSize(16777215, 18));
        lcd_coordY_3->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_3->setAutoFillBackground(false);
        lcd_coordY_3->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_3->setFrameShape(QFrame::Panel);
        lcd_coordY_3->setFrameShadow(QFrame::Sunken);
        lcd_coordY_3->setLineWidth(2);
        lcd_coordY_3->setSmallDecimalPoint(false);
        lcd_coordY_3->setDigitCount(5);
        lcd_coordY_3->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_3->setProperty("value", QVariant(0));

        gridLayout_9->addWidget(lcd_coordY_3, 1, 1, 1, 1);

        lcd_coordX_3 = new QLCDNumber(frame_5);
        lcd_coordX_3->setObjectName(QStringLiteral("lcd_coordX_3"));
        lcd_coordX_3->setMaximumSize(QSize(16777215, 18));
        lcd_coordX_3->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_3->setAutoFillBackground(false);
        lcd_coordX_3->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_3->setFrameShape(QFrame::Panel);
        lcd_coordX_3->setFrameShadow(QFrame::Sunken);
        lcd_coordX_3->setLineWidth(2);
        lcd_coordX_3->setMidLineWidth(0);
        lcd_coordX_3->setSmallDecimalPoint(false);
        lcd_coordX_3->setDigitCount(5);
        lcd_coordX_3->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_3->setProperty("value", QVariant(0));

        gridLayout_9->addWidget(lcd_coordX_3, 0, 1, 1, 1);

        label_19 = new QLabel(frame_5);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_9->addWidget(label_19, 0, 0, 1, 1);

        label_20 = new QLabel(frame_5);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_9->addWidget(label_20, 1, 0, 1, 1);

        currBehavior_3 = new QLabel(frame_robot_3);
        currBehavior_3->setObjectName(QStringLiteral("currBehavior_3"));
        currBehavior_3->setGeometry(QRect(60, 50, 101, 20));
        currBehavior_3->setFont(font1);

        layout_robots->addWidget(frame_robot_3);

        frame_robot_4 = new QFrame(layoutWidget2);
        frame_robot_4->setObjectName(QStringLiteral("frame_robot_4"));
        frame_robot_4->setEnabled(true);
        frame_robot_4->setMaximumSize(QSize(16777215, 80));
        frame_robot_4->setFont(font11);
        frame_robot_4->setMouseTracking(false);
        frame_robot_4->setFocusPolicy(Qt::NoFocus);
        frame_robot_4->setAutoFillBackground(false);
        frame_robot_4->setFrameShape(QFrame::Panel);
        frame_robot_4->setFrameShadow(QFrame::Sunken);
        frame_robot_4->setLineWidth(3);
        frame_robot_4->setMidLineWidth(0);
        title_robPanel_4 = new QLabel(frame_robot_4);
        title_robPanel_4->setObjectName(QStringLiteral("title_robPanel_4"));
        title_robPanel_4->setGeometry(QRect(8, 5, 55, 21));
        title_robPanel_4->setFont(font12);
        title_robPanel_4->setAutoFillBackground(false);
        title_robPanel_4->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gView_robot_4 = new QGraphicsView(frame_robot_4);
        gView_robot_4->setObjectName(QStringLiteral("gView_robot_4"));
        gView_robot_4->setGeometry(QRect(10, 20, 45, 45));
        gView_robot_4->setMouseTracking(true);
        gView_robot_4->setFocusPolicy(Qt::NoFocus);
        gView_robot_4->setFrameShape(QFrame::Panel);
        gView_robot_4->setLineWidth(0);
        QBrush brush8(QColor(0, 0, 0, 0));
        brush8.setStyle(Qt::NoBrush);
        gView_robot_4->setBackgroundBrush(brush8);
        gView_robot_4->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        dial_botVel_4 = new QDial(frame_robot_4);
        dial_botVel_4->setObjectName(QStringLiteral("dial_botVel_4"));
        dial_botVel_4->setEnabled(false);
        dial_botVel_4->setGeometry(QRect(60, 5, 45, 45));
        dial_botVel_4->setFocusPolicy(Qt::NoFocus);
        dial_botVel_4->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_4->setMinimum(-100);
        dial_botVel_4->setMaximum(100);
        dial_botVel_4->setSingleStep(1);
        dial_botVel_4->setPageStep(10);
        dial_botVel_4->setWrapping(false);
        dial_botVel_4->setNotchTarget(4);
        dial_botVel_4->setNotchesVisible(false);
        dial_botOrient_4 = new QDial(frame_robot_4);
        dial_botOrient_4->setObjectName(QStringLiteral("dial_botOrient_4"));
        dial_botOrient_4->setEnabled(false);
        dial_botOrient_4->setGeometry(QRect(124, 5, 45, 45));
        dial_botOrient_4->setBaseSize(QSize(0, 0));
        dial_botOrient_4->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_4->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_4->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_4->setMinimum(-180);
        dial_botOrient_4->setMaximum(180);
        dial_botOrient_4->setSingleStep(1);
        dial_botOrient_4->setPageStep(5);
        dial_botOrient_4->setTracking(false);
        dial_botOrient_4->setInvertedAppearance(true);
        dial_botOrient_4->setWrapping(true);
        dial_botOrient_4->setNotchTarget(8.7);
        dial_botOrient_4->setNotchesVisible(false);
        frame_6 = new QFrame(frame_robot_4);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setGeometry(QRect(170, 5, 81, 71));
        frame_6->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        frame_6->setFrameShape(QFrame::NoFrame);
        frame_6->setFrameShadow(QFrame::Plain);
        frame_6->setLineWidth(0);
        gridLayout_10 = new QGridLayout(frame_6);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setVerticalSpacing(0);
        gridLayout_10->setContentsMargins(9, 9, 9, 9);
        lcd_coordY_4 = new QLCDNumber(frame_6);
        lcd_coordY_4->setObjectName(QStringLiteral("lcd_coordY_4"));
        lcd_coordY_4->setMaximumSize(QSize(16777215, 18));
        lcd_coordY_4->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_4->setAutoFillBackground(false);
        lcd_coordY_4->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_4->setFrameShape(QFrame::Panel);
        lcd_coordY_4->setFrameShadow(QFrame::Sunken);
        lcd_coordY_4->setLineWidth(2);
        lcd_coordY_4->setSmallDecimalPoint(false);
        lcd_coordY_4->setDigitCount(5);
        lcd_coordY_4->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_4->setProperty("value", QVariant(0));

        gridLayout_10->addWidget(lcd_coordY_4, 1, 1, 1, 1);

        lcd_coordX_4 = new QLCDNumber(frame_6);
        lcd_coordX_4->setObjectName(QStringLiteral("lcd_coordX_4"));
        lcd_coordX_4->setMaximumSize(QSize(16777215, 18));
        lcd_coordX_4->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_4->setAutoFillBackground(false);
        lcd_coordX_4->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_4->setFrameShape(QFrame::Panel);
        lcd_coordX_4->setFrameShadow(QFrame::Sunken);
        lcd_coordX_4->setLineWidth(2);
        lcd_coordX_4->setMidLineWidth(0);
        lcd_coordX_4->setSmallDecimalPoint(false);
        lcd_coordX_4->setDigitCount(5);
        lcd_coordX_4->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_4->setProperty("value", QVariant(0));

        gridLayout_10->addWidget(lcd_coordX_4, 0, 1, 1, 1);

        label_21 = new QLabel(frame_6);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_10->addWidget(label_21, 0, 0, 1, 1);

        label_22 = new QLabel(frame_6);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_10->addWidget(label_22, 1, 0, 1, 1);

        currBehavior_4 = new QLabel(frame_robot_4);
        currBehavior_4->setObjectName(QStringLiteral("currBehavior_4"));
        currBehavior_4->setGeometry(QRect(60, 50, 101, 20));
        currBehavior_4->setFont(font1);

        layout_robots->addWidget(frame_robot_4);

        frame_robot_5 = new QFrame(layoutWidget2);
        frame_robot_5->setObjectName(QStringLiteral("frame_robot_5"));
        frame_robot_5->setEnabled(true);
        frame_robot_5->setMaximumSize(QSize(16777215, 80));
        frame_robot_5->setFont(font11);
        frame_robot_5->setMouseTracking(false);
        frame_robot_5->setFocusPolicy(Qt::NoFocus);
        frame_robot_5->setAutoFillBackground(false);
        frame_robot_5->setFrameShape(QFrame::Panel);
        frame_robot_5->setFrameShadow(QFrame::Sunken);
        frame_robot_5->setLineWidth(3);
        frame_robot_5->setMidLineWidth(0);
        title_robPanel_5 = new QLabel(frame_robot_5);
        title_robPanel_5->setObjectName(QStringLiteral("title_robPanel_5"));
        title_robPanel_5->setGeometry(QRect(8, 5, 55, 21));
        title_robPanel_5->setFont(font12);
        title_robPanel_5->setAutoFillBackground(false);
        title_robPanel_5->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        gView_robot_5 = new QGraphicsView(frame_robot_5);
        gView_robot_5->setObjectName(QStringLiteral("gView_robot_5"));
        gView_robot_5->setGeometry(QRect(10, 20, 45, 45));
        gView_robot_5->setMouseTracking(true);
        gView_robot_5->setFocusPolicy(Qt::NoFocus);
        gView_robot_5->setFrameShape(QFrame::Panel);
        gView_robot_5->setLineWidth(0);
        QBrush brush9(QColor(0, 0, 0, 0));
        brush9.setStyle(Qt::NoBrush);
        gView_robot_5->setBackgroundBrush(brush9);
        gView_robot_5->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        dial_botVel_5 = new QDial(frame_robot_5);
        dial_botVel_5->setObjectName(QStringLiteral("dial_botVel_5"));
        dial_botVel_5->setEnabled(false);
        dial_botVel_5->setGeometry(QRect(60, 5, 45, 45));
        dial_botVel_5->setFocusPolicy(Qt::NoFocus);
        dial_botVel_5->setStyleSheet(QStringLiteral("background-color: rgb(150, 150, 150);"));
        dial_botVel_5->setMinimum(-100);
        dial_botVel_5->setMaximum(100);
        dial_botVel_5->setSingleStep(1);
        dial_botVel_5->setPageStep(10);
        dial_botVel_5->setWrapping(false);
        dial_botVel_5->setNotchTarget(4);
        dial_botVel_5->setNotchesVisible(false);
        dial_botOrient_5 = new QDial(frame_robot_5);
        dial_botOrient_5->setObjectName(QStringLiteral("dial_botOrient_5"));
        dial_botOrient_5->setEnabled(false);
        dial_botOrient_5->setGeometry(QRect(124, 5, 45, 45));
        dial_botOrient_5->setBaseSize(QSize(0, 0));
        dial_botOrient_5->setFocusPolicy(Qt::NoFocus);
        dial_botOrient_5->setContextMenuPolicy(Qt::NoContextMenu);
        dial_botOrient_5->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        dial_botOrient_5->setMinimum(-180);
        dial_botOrient_5->setMaximum(180);
        dial_botOrient_5->setSingleStep(1);
        dial_botOrient_5->setPageStep(5);
        dial_botOrient_5->setTracking(false);
        dial_botOrient_5->setInvertedAppearance(true);
        dial_botOrient_5->setWrapping(true);
        dial_botOrient_5->setNotchTarget(8.7);
        dial_botOrient_5->setNotchesVisible(false);
        frame_7 = new QFrame(frame_robot_5);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setGeometry(QRect(170, 5, 81, 71));
        frame_7->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        frame_7->setFrameShape(QFrame::NoFrame);
        frame_7->setFrameShadow(QFrame::Plain);
        frame_7->setLineWidth(0);
        gridLayout_11 = new QGridLayout(frame_7);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        gridLayout_11->setVerticalSpacing(0);
        gridLayout_11->setContentsMargins(9, 9, 9, 9);
        lcd_coordY_5 = new QLCDNumber(frame_7);
        lcd_coordY_5->setObjectName(QStringLiteral("lcd_coordY_5"));
        lcd_coordY_5->setMaximumSize(QSize(16777215, 18));
        lcd_coordY_5->setLayoutDirection(Qt::RightToLeft);
        lcd_coordY_5->setAutoFillBackground(false);
        lcd_coordY_5->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordY_5->setFrameShape(QFrame::Panel);
        lcd_coordY_5->setFrameShadow(QFrame::Sunken);
        lcd_coordY_5->setLineWidth(2);
        lcd_coordY_5->setSmallDecimalPoint(false);
        lcd_coordY_5->setDigitCount(5);
        lcd_coordY_5->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordY_5->setProperty("value", QVariant(0));

        gridLayout_11->addWidget(lcd_coordY_5, 1, 1, 1, 1);

        lcd_coordX_5 = new QLCDNumber(frame_7);
        lcd_coordX_5->setObjectName(QStringLiteral("lcd_coordX_5"));
        lcd_coordX_5->setMaximumSize(QSize(16777215, 18));
        lcd_coordX_5->setLayoutDirection(Qt::RightToLeft);
        lcd_coordX_5->setAutoFillBackground(false);
        lcd_coordX_5->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 100);"));
        lcd_coordX_5->setFrameShape(QFrame::Panel);
        lcd_coordX_5->setFrameShadow(QFrame::Sunken);
        lcd_coordX_5->setLineWidth(2);
        lcd_coordX_5->setMidLineWidth(0);
        lcd_coordX_5->setSmallDecimalPoint(false);
        lcd_coordX_5->setDigitCount(5);
        lcd_coordX_5->setSegmentStyle(QLCDNumber::Outline);
        lcd_coordX_5->setProperty("value", QVariant(0));

        gridLayout_11->addWidget(lcd_coordX_5, 0, 1, 1, 1);

        label_23 = new QLabel(frame_7);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_11->addWidget(label_23, 0, 0, 1, 1);

        label_24 = new QLabel(frame_7);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_11->addWidget(label_24, 1, 0, 1, 1);

        currBehavior_5 = new QLabel(frame_robot_5);
        currBehavior_5->setObjectName(QStringLiteral("currBehavior_5"));
        currBehavior_5->setGeometry(QRect(60, 50, 101, 20));
        currBehavior_5->setFont(font1);

        layout_robots->addWidget(frame_robot_5);

        frame_robot_6 = new QFrame(layoutWidget2);
        frame_robot_6->setObjectName(QStringLiteral("frame_robot_6"));
        frame_robot_6->setEnabled(true);
        frame_robot_6->setMaximumSize(QSize(16777215, 80));
        frame_robot_6->setFont(font11);
        frame_robot_6->setMouseTracking(false);
        frame_robot_6->setFocusPolicy(Qt::NoFocus);
        frame_robot_6->setAutoFillBackground(false);
        frame_robot_6->setFrameShape(QFrame::Panel);
        frame_robot_6->setFrameShadow(QFrame::Sunken);
        frame_robot_6->setLineWidth(3);
        frame_robot_6->setMidLineWidth(0);
        title_robPanel_6 = new QLabel(frame_robot_6);
        title_robPanel_6->setObjectName(QStringLiteral("title_robPanel_6"));
        title_robPanel_6->setGeometry(QRect(5, 5, 241, 21));
        QFont font13;
        font13.setFamily(QStringLiteral("Courier 10 Pitch"));
        font13.setPointSize(10);
        font13.setBold(true);
        font13.setUnderline(true);
        font13.setWeight(75);
        title_robPanel_6->setFont(font13);
        title_robPanel_6->setAutoFillBackground(false);
        title_robPanel_6->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_6->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        gState = new QLabel(frame_robot_6);
        gState->setObjectName(QStringLiteral("gState"));
        gState->setGeometry(QRect(26, 29, 161, 31));
        gState->setAlignment(Qt::AlignCenter);

        layout_robots->addWidget(frame_robot_6);

        frame_robot_7 = new QFrame(layoutWidget2);
        frame_robot_7->setObjectName(QStringLiteral("frame_robot_7"));
        frame_robot_7->setEnabled(true);
        frame_robot_7->setMaximumSize(QSize(16777215, 80));
        frame_robot_7->setFont(font11);
        frame_robot_7->setMouseTracking(false);
        frame_robot_7->setFocusPolicy(Qt::NoFocus);
        frame_robot_7->setAutoFillBackground(false);
        frame_robot_7->setFrameShape(QFrame::Panel);
        frame_robot_7->setFrameShadow(QFrame::Sunken);
        frame_robot_7->setLineWidth(3);
        frame_robot_7->setMidLineWidth(0);
        title_robPanel_7 = new QLabel(frame_robot_7);
        title_robPanel_7->setObjectName(QStringLiteral("title_robPanel_7"));
        title_robPanel_7->setGeometry(QRect(5, 5, 241, 21));
        title_robPanel_7->setFont(font13);
        title_robPanel_7->setAutoFillBackground(false);
        title_robPanel_7->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_7->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        ballVel = new QLabel(frame_robot_7);
        ballVel->setObjectName(QStringLiteral("ballVel"));
        ballVel->setGeometry(QRect(10, 40, 231, 17));
        ballAccel = new QLabel(frame_robot_7);
        ballAccel->setObjectName(QStringLiteral("ballAccel"));
        ballAccel->setGeometry(QRect(10, 20, 231, 17));

        layout_robots->addWidget(frame_robot_7);

        frame_robot_8 = new QFrame(layoutWidget2);
        frame_robot_8->setObjectName(QStringLiteral("frame_robot_8"));
        frame_robot_8->setEnabled(true);
        frame_robot_8->setMaximumSize(QSize(16777215, 80));
        frame_robot_8->setFont(font11);
        frame_robot_8->setMouseTracking(false);
        frame_robot_8->setFocusPolicy(Qt::NoFocus);
        frame_robot_8->setAutoFillBackground(false);
        frame_robot_8->setFrameShape(QFrame::Panel);
        frame_robot_8->setFrameShadow(QFrame::Sunken);
        frame_robot_8->setLineWidth(3);
        frame_robot_8->setMidLineWidth(0);
        title_robPanel_8 = new QLabel(frame_robot_8);
        title_robPanel_8->setObjectName(QStringLiteral("title_robPanel_8"));
        title_robPanel_8->setGeometry(QRect(5, 5, 241, 21));
        title_robPanel_8->setFont(font13);
        title_robPanel_8->setAutoFillBackground(false);
        title_robPanel_8->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_8->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        currStrategy = new QLabel(frame_robot_8);
        currStrategy->setObjectName(QStringLiteral("currStrategy"));
        currStrategy->setGeometry(QRect(6, 40, 241, 20));
        currStrategy->setAlignment(Qt::AlignCenter);

        layout_robots->addWidget(frame_robot_8);

        frame_robot_9 = new QFrame(layoutWidget2);
        frame_robot_9->setObjectName(QStringLiteral("frame_robot_9"));
        frame_robot_9->setEnabled(true);
        frame_robot_9->setMaximumSize(QSize(16777215, 80));
        frame_robot_9->setFont(font11);
        frame_robot_9->setMouseTracking(false);
        frame_robot_9->setFocusPolicy(Qt::NoFocus);
        frame_robot_9->setAutoFillBackground(false);
        frame_robot_9->setFrameShape(QFrame::Panel);
        frame_robot_9->setFrameShadow(QFrame::Sunken);
        frame_robot_9->setLineWidth(3);
        frame_robot_9->setMidLineWidth(0);
        title_robPanel_9 = new QLabel(frame_robot_9);
        title_robPanel_9->setObjectName(QStringLiteral("title_robPanel_9"));
        title_robPanel_9->setGeometry(QRect(5, 5, 241, 21));
        title_robPanel_9->setFont(font13);
        title_robPanel_9->setAutoFillBackground(false);
        title_robPanel_9->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        title_robPanel_9->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        blueGoal = new QLabel(frame_robot_9);
        blueGoal->setObjectName(QStringLiteral("blueGoal"));
        blueGoal->setGeometry(QRect(10, 20, 231, 17));
        yellGoal = new QLabel(frame_robot_9);
        yellGoal->setObjectName(QStringLiteral("yellGoal"));
        yellGoal->setGeometry(QRect(10, 40, 231, 17));
        timeRem = new QLabel(frame_robot_9);
        timeRem->setObjectName(QStringLiteral("timeRem"));
        timeRem->setGeometry(QRect(10, 60, 231, 17));

        layout_robots->addWidget(frame_robot_9);

        scroll_robots->setWidget(scrollAreaWidgetContents);

        gridLayout_7->addWidget(scroll_robots, 2, 0, 1, 1);

        btn_toggleTeamColor = new QPushButton(frame_robotsPanel);
        btn_toggleTeamColor->setObjectName(QStringLiteral("btn_toggleTeamColor"));
        btn_toggleTeamColor->setMinimumSize(QSize(100, 27));
        btn_toggleTeamColor->setMaximumSize(QSize(100, 27));
        btn_toggleTeamColor->setFont(font7);
        btn_toggleTeamColor->setFocusPolicy(Qt::NoFocus);
        btn_toggleTeamColor->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 255);\n"
"color: rgb(255, 255, 255);"));

        gridLayout_7->addWidget(btn_toggleTeamColor, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame_robotsPanel, 0, 0, 2, 1);

        scrollArea->setWidget(mainScrollArea);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        QWidget::setTabOrder(scrollArea, combo_ballScale);
        QWidget::setTabOrder(combo_ballScale, combo_gridScale);
        QWidget::setTabOrder(combo_gridScale, zoom_slider);
        QWidget::setTabOrder(zoom_slider, combo_fieldColor);
        QWidget::setTabOrder(combo_fieldColor, combo_ballColor);
        QWidget::setTabOrder(combo_ballColor, combo_botScale);
        QWidget::setTabOrder(combo_botScale, combo_fieldProportions);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        tabs_field->setCurrentIndex(1);
        combo_gridScale->setCurrentIndex(1);
        combo_fieldColor->setCurrentIndex(0);
        combo_fieldProportions->setCurrentIndex(0);
        combo_botScale->setCurrentIndex(1);
        combo_ballColor->setCurrentIndex(1);
        combo_ballScale->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Dashboard", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        label_primeBot->setText(QApplication::translate("MainWindow", "Game Info", 0));
#ifndef QT_NO_TOOLTIP
        btn_connectGui->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Backspace) Toggles the GUI's connection with the running game", 0));
#endif // QT_NO_TOOLTIP
        btn_connectGui->setText(QApplication::translate("MainWindow", "Connect", 0));
#ifndef QT_NO_TOOLTIP
        lcd_coordY_ball->setToolTip(QApplication::translate("MainWindow", "Ball Y coordinate in the Field Panel", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_ball->setToolTip(QApplication::translate("MainWindow", "Ball X coordinate in the Field Panel", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QString());
#ifndef QT_NO_TOOLTIP
        lcd_coordX_cursor->setToolTip(QApplication::translate("MainWindow", "Mouse cursor X coordinate in the Field Panel", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_cursor->setToolTip(QApplication::translate("MainWindow", "Mouse cursor Y coordinate in the Field Panel", 0));
#endif // QT_NO_TOOLTIP
        label_9->setText(QString());
#ifndef QT_NO_TOOLTIP
        btn_multithread->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Backspace) Toggles the GUI's connection with the running game", 0));
#endif // QT_NO_TOOLTIP
        btn_multithread->setText(QApplication::translate("MainWindow", "Enabled", 0));
        label_multithreaded->setText(QApplication::translate("MainWindow", "Multithreaded: ", 0));
        label_10->setText(QApplication::translate("MainWindow", "Program Output", 0));
#ifndef QT_NO_TOOLTIP
        text_guiPrint->setToolTip(QApplication::translate("MainWindow", "GuiInterface::getGuiInterface()->guiPrintTerminal()", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        check_fieldGrid->setToolTip(QApplication::translate("MainWindow", "(Hotkey: G) Toggle display of grid", 0));
#endif // QT_NO_TOOLTIP
        check_fieldGrid->setText(QApplication::translate("MainWindow", "Grid", 0));
        combo_gridScale->clear();
        combo_gridScale->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "200\302\262", 0)
         << QApplication::translate("MainWindow", "500\302\262", 0)
         << QApplication::translate("MainWindow", "1000\302\262", 0)
        );
#ifndef QT_NO_TOOLTIP
        combo_gridScale->setToolTip(QApplication::translate("MainWindow", "Interval size of grid lines", 0));
#endif // QT_NO_TOOLTIP
        combo_gridScale->setCurrentText(QApplication::translate("MainWindow", "500\302\262", 0));
        combo_fieldColor->clear();
        combo_fieldColor->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Default", 0)
         << QApplication::translate("MainWindow", "Ice Rink", 0)
         << QApplication::translate("MainWindow", "Basketball", 0)
         << QApplication::translate("MainWindow", "Air Hockey", 0)
         << QApplication::translate("MainWindow", "Arctic", 0)
         << QApplication::translate("MainWindow", "Game Boy", 0)
        );
#ifndef QT_NO_TOOLTIP
        combo_fieldColor->setToolTip(QApplication::translate("MainWindow", "Field color scheme", 0));
#endif // QT_NO_TOOLTIP
        combo_fieldColor->setCurrentText(QApplication::translate("MainWindow", "Default", 0));
#ifndef QT_NO_TOOLTIP
        check_coloredGoals->setToolTip(QApplication::translate("MainWindow", "Toggle team-colored goals", 0));
#endif // QT_NO_TOOLTIP
        check_coloredGoals->setText(QApplication::translate("MainWindow", "Team Colors", 0));
        combo_fieldProportions->clear();
        combo_fieldProportions->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Simulator", 0)
         << QApplication::translate("MainWindow", "Lab Field", 0)
        );
        tabs_field->setTabText(tabs_field->indexOf(field), QApplication::translate("MainWindow", "Field", 0));
#ifndef QT_NO_TOOLTIP
        btn_override_none->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Alt+O)  Disables manual override on entire team", 0));
#endif // QT_NO_TOOLTIP
        btn_override_none->setText(QApplication::translate("MainWindow", "Disable", 0));
#ifndef QT_NO_TOOLTIP
        btn_override_all->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Ctrl+O)  Enables manual override on entire team", 0));
#endif // QT_NO_TOOLTIP
        btn_override_all->setText(QApplication::translate("MainWindow", "Enable", 0));
        label_6->setText(QApplication::translate("MainWindow", "Team Override", 0));
#ifndef QT_NO_TOOLTIP
        btn_botKick->setToolTip(QApplication::translate("MainWindow", "(Hotkey: SPACEBAR) Kick with selected robot", 0));
#endif // QT_NO_TOOLTIP
        btn_botKick->setText(QApplication::translate("MainWindow", "Kick", 0));
#ifndef QT_NO_TOOLTIP
        btn_botReverse->setToolTip(QApplication::translate("MainWindow", "(Hotkey: S) Move selected robot backward", 0));
#endif // QT_NO_TOOLTIP
        btn_botReverse->setText(QApplication::translate("MainWindow", "\342\226\274", 0));
#ifndef QT_NO_TOOLTIP
        btn_botDrible->setToolTip(QApplication::translate("MainWindow", "(Hotkey: J) Drible with selected robot", 0));
#endif // QT_NO_TOOLTIP
        btn_botDrible->setText(QApplication::translate("MainWindow", "Drible", 0));
#ifndef QT_NO_TOOLTIP
        btn_botForward->setToolTip(QApplication::translate("MainWindow", "(Hotkey: W) Move selected robot forward", 0));
#endif // QT_NO_TOOLTIP
        btn_botForward->setText(QApplication::translate("MainWindow", "\342\226\262", 0));
#ifndef QT_NO_TOOLTIP
        btn_botTurnLeft->setToolTip(QApplication::translate("MainWindow", "(Hotkey: A) Turn selected robot left", 0));
#endif // QT_NO_TOOLTIP
        btn_botTurnLeft->setText(QApplication::translate("MainWindow", "\342\227\200", 0));
#ifndef QT_NO_TOOLTIP
        btn_botTurnRight->setToolTip(QApplication::translate("MainWindow", "(Hotkey: D) Turn selected robot right", 0));
#endif // QT_NO_TOOLTIP
        btn_botTurnRight->setText(QApplication::translate("MainWindow", "\342\226\266", 0));
#ifndef QT_NO_TOOLTIP
        check_showIDs->setToolTip(QApplication::translate("MainWindow", "(Hotkey: I) Show Robot Identification Numbers", 0));
#endif // QT_NO_TOOLTIP
        check_showIDs->setText(QApplication::translate("MainWindow", "Show IDs", 0));
        combo_botScale->clear();
        combo_botScale->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "150%", 0)
         << QApplication::translate("MainWindow", "120%", 0)
         << QApplication::translate("MainWindow", "100%", 0)
        );
#ifndef QT_NO_TOOLTIP
        combo_botScale->setToolTip(QApplication::translate("MainWindow", "Robot Scale", 0));
#endif // QT_NO_TOOLTIP
        combo_botScale->setCurrentText(QApplication::translate("MainWindow", "120%", 0));
        tabs_field->setTabText(tabs_field->indexOf(tab_robots), QApplication::translate("MainWindow", "Robots", 0));
        combo_ballColor->clear();
        combo_ballColor->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Red", 0)
         << QApplication::translate("MainWindow", "Red-Orange", 0)
         << QApplication::translate("MainWindow", "Orange", 0)
         << QApplication::translate("MainWindow", "Yellow", 0)
         << QApplication::translate("MainWindow", "Green", 0)
         << QApplication::translate("MainWindow", "Blue", 0)
         << QApplication::translate("MainWindow", "Purple", 0)
         << QApplication::translate("MainWindow", "White", 0)
         << QApplication::translate("MainWindow", "Black", 0)
         << QApplication::translate("MainWindow", "Hello Kitty!", 0)
        );
#ifndef QT_NO_TOOLTIP
        combo_ballColor->setToolTip(QApplication::translate("MainWindow", "Ball Color", 0));
#endif // QT_NO_TOOLTIP
        combo_ballScale->clear();
        combo_ballScale->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "150%", 0)
         << QApplication::translate("MainWindow", "120%", 0)
         << QApplication::translate("MainWindow", "100%", 0)
        );
#ifndef QT_NO_TOOLTIP
        combo_ballScale->setToolTip(QApplication::translate("MainWindow", "Ball Scale", 0));
#endif // QT_NO_TOOLTIP
        combo_ballScale->setCurrentText(QApplication::translate("MainWindow", "120%", 0));
        tabs_field->setTabText(tabs_field->indexOf(tab), QApplication::translate("MainWindow", "Ball", 0));
#ifndef QT_NO_TOOLTIP
        btn_rotateField_left->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Alt+R) Rotate field 45 degrees to the left", 0));
#endif // QT_NO_TOOLTIP
        btn_rotateField_left->setText(QApplication::translate("MainWindow", "\342\206\266", 0));
#ifndef QT_NO_TOOLTIP
        btn_rotateField_right->setToolTip(QApplication::translate("MainWindow", "(Hotkey: R) Rotate field 45 degrees to the right", 0));
#endif // QT_NO_TOOLTIP
        btn_rotateField_right->setText(QApplication::translate("MainWindow", "\342\206\267", 0));
#ifndef QT_NO_TOOLTIP
        zoom_slider->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Z, Alt+Z) Zoom Camera", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        zoom_default->setToolTip(QApplication::translate("MainWindow", "(Hotkey: Ctrl+Z) Return camera to default zoom and rotation", 0));
#endif // QT_NO_TOOLTIP
        zoom_default->setText(QApplication::translate("MainWindow", "Default", 0));
        label_2->setText(QApplication::translate("MainWindow", "Rotate", 0));
        tabs_field->setTabText(tabs_field->indexOf(tab_3), QApplication::translate("MainWindow", "Camera", 0));
#ifndef QT_NO_TOOLTIP
        frame_robotsPanel->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        box_primeBot->setTitle(QApplication::translate("MainWindow", "no robot selected", 0));
#ifndef QT_NO_TOOLTIP
        gView_robot_prime->setToolTip(QApplication::translate("MainWindow", "Selected robot", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        check_botOverride->setToolTip(QApplication::translate("MainWindow", "(Hotkey: O) Toggles manual override on selected robot", 0));
#endif // QT_NO_TOOLTIP
        check_botOverride->setText(QApplication::translate("MainWindow", "Override", 0));
#ifndef QT_NO_TOOLTIP
        text_primeBot->setToolTip(QApplication::translate("MainWindow", "GuiInterface::getGuiInterface()->guiPrintRobot()", 0));
#endif // QT_NO_TOOLTIP
        label_coord_6->setText(QApplication::translate("MainWindow", "Position:", 0));
#ifndef QT_NO_TOOLTIP
        lcd_coordX_prime->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_prime->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_orient_prime->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", "test"));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_botVel_->setToolTip(QApplication::translate("MainWindow", "Average forward or reverse force of the robot's wheels", 0));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("MainWindow", "Velocity", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
        label_11->setText(QApplication::translate("MainWindow", "Speed", 0));
#ifndef QT_NO_TOOLTIP
        dial_botSpeed_->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_botSpeed_->setToolTip(QApplication::translate("MainWindow", "Average forward or reverse force of the robot's wheels", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_prime->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("MainWindow", "Bearing", 0));
        label_7->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_0->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_0->setText(QApplication::translate("MainWindow", "Robot  ", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_0->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_0->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_0->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_0->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
        label_14->setText(QApplication::translate("MainWindow", "x", 0));
        label_13->setText(QApplication::translate("MainWindow", "y", 0));
        currBehavior_0->setText(QApplication::translate("MainWindow", "TextLabel", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_1->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_1->setText(QApplication::translate("MainWindow", "Robot  ", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_1->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_1->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_1->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_1->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
        label_15->setText(QApplication::translate("MainWindow", "x", 0));
        label_16->setText(QApplication::translate("MainWindow", "y", 0));
        currBehavior_1->setText(QApplication::translate("MainWindow", "TextLabel", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_2->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_2->setText(QApplication::translate("MainWindow", "Robot  ", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_2->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_2->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_2->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_2->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
        label_17->setText(QApplication::translate("MainWindow", "x", 0));
        label_18->setText(QApplication::translate("MainWindow", "y", 0));
        currBehavior_2->setText(QApplication::translate("MainWindow", "TextLabel", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_3->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_3->setText(QApplication::translate("MainWindow", "Robot  ", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_3->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_3->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_3->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_3->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
        label_19->setText(QApplication::translate("MainWindow", "x", 0));
        label_20->setText(QApplication::translate("MainWindow", "y", 0));
        currBehavior_3->setText(QApplication::translate("MainWindow", "TextLabel", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_4->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_4->setText(QApplication::translate("MainWindow", "Robot  ", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_4->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_4->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_4->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_4->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
        label_21->setText(QApplication::translate("MainWindow", "x", 0));
        label_22->setText(QApplication::translate("MainWindow", "y", 0));
        currBehavior_4->setText(QApplication::translate("MainWindow", "TextLabel", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_5->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_5->setText(QApplication::translate("MainWindow", "Robot  ", 0));
#ifndef QT_NO_TOOLTIP
        dial_botVel_5->setToolTip(QApplication::translate("MainWindow", "Average wheel velocity", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        dial_botOrient_5->setToolTip(QApplication::translate("MainWindow", "Robot's orientation in relation to the opposing end of the field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordY_5->setToolTip(QApplication::translate("MainWindow", "Y coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        lcd_coordX_5->setToolTip(QApplication::translate("MainWindow", "X coordinate from center of field", 0));
#endif // QT_NO_TOOLTIP
        label_23->setText(QApplication::translate("MainWindow", "x", 0));
        label_24->setText(QApplication::translate("MainWindow", "y", 0));
        currBehavior_5->setText(QApplication::translate("MainWindow", "TextLabel", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_6->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_6->setText(QApplication::translate("MainWindow", "Game State", 0));
        gState->setText(QApplication::translate("MainWindow", "Default Game State", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_7->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_7->setText(QApplication::translate("MainWindow", "Ball Info", 0));
        ballVel->setText(QApplication::translate("MainWindow", "Velocity Reading", 0));
        ballAccel->setText(QApplication::translate("MainWindow", "Acceleration Reading", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_8->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_8->setText(QApplication::translate("MainWindow", "Strategy", 0));
        currStrategy->setText(QApplication::translate("MainWindow", "Current Strategy", 0));
#ifndef QT_NO_ACCESSIBILITY
        frame_robot_9->setAccessibleName(QApplication::translate("MainWindow", "Robot 0", 0));
#endif // QT_NO_ACCESSIBILITY
        title_robPanel_9->setText(QApplication::translate("MainWindow", "Team Info", 0));
        blueGoal->setText(QApplication::translate("MainWindow", "Blue Goals", 0));
        yellGoal->setText(QApplication::translate("MainWindow", "Yellow Goals", 0));
        timeRem->setText(QApplication::translate("MainWindow", "Time Remaining", 0));
#ifndef QT_NO_TOOLTIP
        btn_toggleTeamColor->setToolTip(QApplication::translate("MainWindow", "Toggle user team color", 0));
#endif // QT_NO_TOOLTIP
        btn_toggleTeamColor->setText(QApplication::translate("MainWindow", "My Team", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
