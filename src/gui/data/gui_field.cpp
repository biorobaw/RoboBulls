#include "gui_field.h"
#include "model/field.h"
#include "model/constants.h"


GuiField::GuiField()
{

}

int GuiField::fieldWidth(){
    return Field::FIELD_WIDTH;
}
int GuiField::fieldHalfWidth(){
    return Field::HALF_FIELD_WIDTH;
}
int GuiField::fieldLength(){
    return Field::FIELD_LENGTH;
}
int GuiField::fieldHalfLength(){
    return Field::HALF_FIELD_LENGTH;
}

void GuiField::updateField(){

}

int GuiField::getTeamOnSide(int side){
    if (side==FIELD_SIDE_NEGATIVE) return Field::NEGATIVE_SIDE_TEAM;
    else return 1-Field::NEGATIVE_SIDE_TEAM;
}
