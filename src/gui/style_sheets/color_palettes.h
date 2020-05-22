#ifndef COLOR_PALETTES_H
#define COLOR_PALETTES_H


#include "model/constants.h"

#include <QMap>
#include <QBrush>
#include <QString>
#include <QPen>


// =============== FIELD COLORS ===========================
// ========================================================

#define LINE_THICKNESS 40
extern const QPen blueLinePen;
extern const QPen yellowLinePen;
extern const QPen blackLinePen;
extern const QBrush goal_area_brush;

struct FieldPallete {
    QBrush brush;
    QPen   defaultPen;
    QPen   borderPen;
    QPen   goalPen;
};

extern const QMap<QString, FieldPallete> field_pallete_maps;


// ================ BALL COLORS ===========================
// ========================================================

struct BallPallete {
    QBrush brush;
    QPen   pen;
};
extern const QMap<QString, BallPallete> ball_pallete_maps;

// ====== PANEL SELECTED ROBOT COLORS & ROBOT FRAMES ======
// ========================================================

#define VALUE_NEGATIVE 0
#define VALUE_ZERO     1
#define VALUE_POSITIVE 2

struct ColorsDial{
    QString dial_background;
    QString lcd_background;
};

#define SIGN(X) (X<0 ? 0 : X==0 ? 1 : 2)
extern const ColorsDial colors_dial_sign[3];

// Colors of dial according to percentage
// of max-min value (split into 0.2 intervals, and starting less than minimum)
int PERCENTAGE_INDEX(float val, float min, float max);
extern const ColorsDial colors_dial_value[6];


// ==================== TEAM COLORS =======================
// ========================================================

struct TeamColors {
    QString frame_background;
    QString dial_orientation_background;
    QString text_background;
};

// OBS: BLUE = 0 , YELLOW = 1
extern const TeamColors team_colors[2];


// ==================== GRAPHICS LINE =====================
// ========================================================

struct LineAgeColors {
    QPen pen;
    QBrush brush;
};

extern const LineAgeColors line_age_colors[];
extern const QPen graphics_line_ellipse_pen;

#endif // COLOR_PALETTES_H
