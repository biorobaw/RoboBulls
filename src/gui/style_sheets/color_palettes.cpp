#include "color_palettes.h"


#include "configuration/constants.h"


// ================ BALL COLORS ===========================
// ========================================================

const QMap<QString, BallPallete> ball_pallete_maps = {
    {
        "Red-Orange", {
            QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern),
            QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
         "Red", {
            QBrush(Qt::red, Qt::SolidPattern),
            QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
        "Green", {
            QBrush(Qt::green, Qt::SolidPattern),
             QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
        "White", {
            QBrush(Qt::white, Qt::Dense1Pattern),
             QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
        "Orange", {
            QBrush(QColor::fromRgb(255,165,0,255), Qt::SolidPattern),
             QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
        "Blue", {
            QBrush(Qt::blue, Qt::SolidPattern),
             QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
        "Black", {
            QBrush(Qt::black, Qt::SolidPattern),
             QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {

        "Yellow", {
            QBrush(Qt::yellow, Qt::SolidPattern),
            QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }
    },
    {
        "Purple", {
            QBrush(QColor::fromRgb(160,32,240,255), Qt::SolidPattern),
             QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
    {
        "Hello Kitty!", {
             QBrush(QColor::fromRgb(255,20,147,255), Qt::SolidPattern),
             QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)
        }

    },
};




// =============== FIELD COLORS ===========================
// ========================================================

#define LINE_THICKNESS 40
const QPen blueLinePen(QColor::fromRgb(0,0,255,255), LINE_THICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
const QPen yellowLinePen(Qt::yellow, LINE_THICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
const QPen blackLinePen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
const QBrush goal_area_brush = QBrush(Qt::white, Qt::DiagCrossPattern);

const QMap<QString, FieldPallete> field_pallete_maps = {
    {"Default",   {QBrush(Qt::darkGreen, Qt::SolidPattern),
                   QPen(QColor::fromRgb(245,245,245,255), LINE_THICKNESS, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::white, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::lightGray, 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)}},
    {"Ice Rink",  {QBrush(QColor::fromRgb(200,255,255,255), Qt::SolidPattern),
                   QPen(Qt::red, 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::red, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::darkRed, 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)}},
    {"Air Hockey",{QBrush(QColor::fromRgb(50,50,200,255), Qt::SolidPattern),
                   QPen(Qt::white, 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::white, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(QColor::fromRgb(56,56,56,255), 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)}},
    {"Basketball",{QBrush(QColor::fromRgb(205,133,63,255), Qt::SolidPattern),
                   QPen(Qt::white, 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::white, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(QColor::fromRgb(152,0,0,255), 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)}},
    {"Arctic",    {QBrush(Qt::white, Qt::SolidPattern),
                   QPen(Qt::darkGray, 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(Qt::darkGray, 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(QColor::fromRgb(32,32,32,255), 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)}},
    {"Game Boy",  {QBrush(QColor::fromRgb(152,251,152,255), Qt::SolidPattern),
                   QPen(QColor::fromRgb(85,107,47,255), 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(QColor::fromRgb(85,107,47,255), 50, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin),
                   QPen(QColor::fromRgb(37,57,0,255), 40, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin)}},
};


// ====== PANEL SELECTED ROBOT COLORS & ROBOT FRAMES ======
// ========================================================

#define SIGN(X) (X<0 ? 0 : X==0 ? 1 : 2)
const ColorsDial colors_dial_sign[3] = {
    {
        "background-color: rgb(200, 0, 0);",
        "background-color: rgb(100, 0, 0);"
    },
    {
        "background-color: rgb(150, 150, 150);",
        "background-color: rgb(100, 100, 100);"
    },
    {
        "background-color: rgb(0, 200, 0);",
        "background-color: rgb(0, 100, 0);"
    }
};

// Colors of dial according to percentage
// of max-min value (split into 0.2 intervals, and starting less than minimum)
int PERCENTAGE_INDEX(float val, float min, float max){
    float percentage = (val-min)/(max-min);
    int i =  percentage / 0.2;
    return i < 0 ? 0 : i > 5 ? 5 : i;
}
const ColorsDial colors_dial_value[6] = {
    {
        "background-color: rgb(150, 150, 150);",
        "background-color: rgb(100, 100, 100);"
    },
    {
        "background-color: rgb(0, 191, 255);",
        "background-color: rgb(0, 140, 200);"
    },
    {
        "background-color: rgb(0, 0, 255);",
        "background-color: rgb(0, 0, 200);"
    },
    {
        "background-color: rgb(160, 32, 240);",
        "background-color: rgb(100, 0, 190);"
    },
    {
        "background-color: rgb(208, 32, 144);",
        "background-color: rgb(150, 0, 90);"
    },
    {
        "background-color: rgb(255, 20, 147);",
        "background-color: rgb(200, 0, 90);"
    }
};


// ==================== TEAM COLORS =======================
// ========================================================

// OBS: BLUE = 0 , YELLOW = 1
const TeamColors team_colors[2] = {
    {
        "background-color: rgb(150, 150, 255);", // frame
        "background-color: rgb(255, 255, 0);",   // dial
        "background-color: rgb(100, 100, 100);"    // text
    },
    {
        "background-color: rgb(255, 255, 150);",
        "background-color: rgb(225, 225, 0);",
        "background-color: rgb(100, 100, 100);"
    }
};



// ==================== GRAPHICS LINE =====================
// ========================================================


const LineAgeColors line_age_colors[6] = {
    {
        QPen(QColor::fromRgb(255,0,0,5), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin),
        QBrush(QColor::fromRgb(255,0,0,5), Qt::SolidPattern)
    },
    {
        QPen(QColor::fromRgb(255,69,0,55), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin),
        QBrush(QColor::fromRgb(255,69,0,55), Qt::SolidPattern)
    },
    {
        QPen(QColor::fromRgb(255,165,0,100), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin),
        QBrush(QColor::fromRgb(255,165,0,100), Qt::SolidPattern)
    },
    {
        QPen(QColor::fromRgb(255,255,0,155), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin),
        QBrush(QColor::fromRgb(255,255,0,155), Qt::SolidPattern)
    },
    {
        QPen(QColor::fromRgb(173,255,47,200), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin),
        QBrush(QColor::fromRgb(173,255,47,200), Qt::SolidPattern)
    },
    {
        QPen(QColor::fromRgb(0,255,0,255), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin),
        QBrush(QColor::fromRgb(0,255,0,255), Qt::SolidPattern)
    }
};


const QPen graphics_line_ellipse_pen = QPen(QColor::fromRgb(255,0,0,0), 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);


