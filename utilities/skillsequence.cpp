#include "utilities/skillsequence.h"

SkillSequence::~SkillSequence()
{
    for(auto& entry : this->masterQueue)
        delete entry.skill;
    masterQueue.clear();
}


void SkillSequence::addSkill(Skill::Skill* sk, skillUpdateFn updateFn)
{
	this->masterQueue.emplace_back(sk, updateFn);
}


bool SkillSequence::executeSkills(skillQueue& queue, Robot* robot)
{
    int isFinished = -1;    //Default to let skill decide
    bool skillReportedFinished = false;

    Skill::Skill* sk = queue.front().skill;
	skillUpdateFn updateFn = queue.front().updateFn;

	if(updateFn != nullptr)
       isFinished = updateFn(*sk, robot);
	   
    if(sk != nullptr)
        skillReportedFinished = sk->perform(robot);

	if(isFinished < 0)
	   isFinished = skillReportedFinished;
	
    if(isFinished == true)
    {
    #if SKILL_SEQUENCE_DEBUG
        std::cout << typeid(*sk).name() << "Finished" << std::endl;
    #endif
		queue.pop_front();

		if(queue.empty()) 
			return true;	//All skills finished
	}
	
	return false;	//Still skills to be done
}


bool SkillSequence::executeOn(Robot* robot)
{
	bool finished = true;

	if(!masterQueue.empty()) 
		finished = executeSkills(masterQueue, robot);
	
	return finished;
}


/**************************************************/
