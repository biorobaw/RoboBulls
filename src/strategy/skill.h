#ifndef SKILL_H
#define SKILL_H
class Robot;

//! @brief Namespace containing all Skills
namespace Skill
{

//! @addtogroup baseclasses
//! @{

/*! @brief The Skill Base Class
 * @details Skill class is the base class for all the skills
 * defined in robobulls project. It requires one <i>perform</i>
 * function */

class Skill
{
public:
    /*! @brief Perform the Skill's code on a robot.
     * @details The main function required by a Skill.
     * Typically Skills will call either other skills or
     * Movement classes. The perform function returns <i>true</i>
     * if the skill is considered finished. This is important.
     * @return True if skill is considered finished */
    virtual bool perform(Robot* robot) = 0;

    virtual ~Skill(){}
};

//!@}

}

#endif // SKILL_H
