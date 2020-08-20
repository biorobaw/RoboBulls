#include "gui_field.h"
#include "model/field.h"
#include "configuration/constants.h"


GuiField::GuiField()
{

}

int GuiField::width(){
    return Field::FIELD_WIDTH;
}
int GuiField::halfWidth(){
    return Field::HALF_FIELD_WIDTH;
}
int GuiField::length(){
    return Field::FIELD_LENGTH;
}
int GuiField::halfLength(){
    return Field::HALF_FIELD_LENGTH;
}

void GuiField::updateField(){

}

int GuiField::getTeamOnSide(int side){
    if (side==FIELD_SIDE_NEGATIVE) return Field::NEGATIVE_SIDE_TEAM;
    else return 1-Field::NEGATIVE_SIDE_TEAM;
}
