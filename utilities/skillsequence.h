#ifndef SKILLSEQUENCE_H
#define SKILLSEQUENCE_H

#include <deque>
#include <unordered_map>
#include <functional>
#include "skill/skill.h"
#include "model/robot.h"

#define SKILL_SEQUENCE_DEBUG 1

/* Prototype of update function. Each time this function is called,
 * it is passed the current executing skill (paired with addSkill)
 * and the robot it is currently executing on
 */
typedef std::function<int(Skill::Skill&, Robot*)> skillUpdateFn;


/* Data type held by the internal queue of SkillSequence
 */
struct skillQueueEntry
{
    skillQueueEntry(Skill::Skill* s, skillUpdateFn up_fn)
        : skill(s), updateFn(up_fn)
        {}

    /* This copy constructor is meant to be used when copying the
     * masterQueue into the queue for each robot. However, this could
     * be a problem because the skill pointers then point to the same
     * skill in the masterQueue. This is not a problem currently because
     * BehaviorAssignment assigns each robot a seperate Behavior
     */
    skillQueueEntry(const skillQueueEntry& rhs)
        : skill(rhs.skill)                //<--- Potential Problem
        , updateFn(rhs.updateFn)
        {}

    Skill::Skill* skill; //Skill to perform
	skillUpdateFn updateFn;		//Option function to update skill and determine if finished 
};


/* Class SkillSequence
 * This is a utility class aimed at simplifying the creation of "state machine"-type behaviors.
 * It is a replacement for the traditional switch statement construction of a sequence of skills,
 * executing one skill until it is deemed "done," after which the next skill is started
 */
class SkillSequence
{
public:

    ~SkillSequence();

    /* void addSkill(ptr to skill, updateFn)
     * Brief: Add a skill to the queue.
     * This pushes back a new skill in the sequence. The updatefn
     * is an optional parameter that will be called each time skill sk
     * is executed on the robot. the updateFn is a function that determines
     * if the skill sk is done or not.
     *
     * The updateFn function should return:
     * false: Execute the skill again, as it is not doen
     * true : This skill is done, move to the next in the queue
     * -1   : Use the skill sk's built-in perform() to determine if finished
     */

    void addSkill(Skill::Skill* sk, skillUpdateFn updateFn = nullptr);


    /* bool executeOn(robot)
     * Brief: Execute the queue of skills in robot robot.
     * Starting from the beginning, each skill is performed on robot until
     * it is "done," as either determines by the updateFn, or the skill itself.
     *
     * Returns true if all skills are done (i.e, queue is empty for robot),
     * false otherwise (more skills need to be executed)
     */
	bool executeOn(Robot* robot);


    /* void restart()
     * Brief: restart the queue for robot* robot.
     * Resets the queue held for robot, so that the next call to executeOn
     * will be at the first added skill
     */
    void restart(Robot* robot);

	
    typedef std::deque<skillQueueEntry> skillQueue;

private:
	skillQueue& getSkillQueue(Robot* robot);
	bool executeSkills(skillQueue& queue, Robot* robot);
	
	skillQueue masterQueue;
	std::unordered_map<int, skillQueue> robQueueMap;
};	

#endif
