#ifndef SKILLSEQUENCE_H
#define SKILLSEQUENCE_H

#include <deque>
#include <unordered_map>
#include <functional>
#include "skill/skill.h"
#include "model/robot.h"

typedef std::function<int(Skill::Skill&, Robot*)> skillUpdateFn;

struct skillQueueEntry
{
	skillQueueEntry(Skill::Skill& s, skillUpdateFn up_fn)
		: skill(s), updateFn(up_fn) {}

	Skill::Skill skill;			//Skill to perform
	skillUpdateFn updateFn;		//Option function to update skill and determine if finished 
};


typedef std::deque<skillQueueEntry> skillQueue;


/* Class SkillSequence
 * This is a utility class aimed at simplifying the creation of "state machine"-type behaviors.
 * It is a replacement for the traditional switch statement construction of a sequence of skills,
 * executing one skill until it is deemed "done," after which the next skill is started
 */
class SkillSequence
{
public:
    void addSkill(Skill::Skill& sk, skillUpdateFn updateFn = nullptr);
	bool executeOn(Robot* robot);
	
private:
	skillQueue& getSkillQueue(Robot* robot);
	bool executeSkills(skillQueue& queue, Robot* robot);
	
	skillQueue masterQueue;
	std::unordered_map<int, skillQueue> robQueueMap;
};	

#endif
