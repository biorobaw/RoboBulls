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


SkillSequence::skillQueue& SkillSequence::getSkillQueue(Robot* robot)
{
	int  robID = robot->getID();
	auto robEntry = robQueueMap.find(robID);
	
    if(robEntry == robQueueMap.end()) //Copy full skill queue
    {
    #if SKILL_SEQUENCE_DEBUG
        std::cout << "Making New for " << robot->getID() << std::endl;
    #endif
        robQueueMap[robID] = skillQueue(masterQueue);
	}
	
	return robQueueMap[robID];
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
    #if SKILL_SEQUENCE_DEBUG
    else
        std::cout << "Skill was null" << std::endl;
    #endif

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
	if( !masterQueue.empty() ) 
	{
		bool finished;
		auto& robSkillQueue = getSkillQueue(robot);
		
		finished = executeSkills(robSkillQueue, robot);
		
		return finished;
	}
	
	return true;
}


void SkillSequence::restart(Robot* robot)
{
    auto entry = this->robQueueMap.find(robot->getID());

    if(entry != robQueueMap.end())
        this->robQueueMap.erase(entry);

    //robQueueMap[robot->getID()] = skillQueue(masterQueue);
}

/**************************************************/
