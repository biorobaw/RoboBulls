#include "utilities/skillsequence.h"


void SkillSequence::addSkill(Skill::Skill* sk, skillUpdateFn updateFn)
{
	this->masterQueue.emplace_back(sk, updateFn);
}


skillQueue& SkillSequence::getSkillQueue(Robot* robot)
{
	int  robID = robot->getID();
	auto robEntry = robQueueMap.find(robID);
	
    if(robEntry == robQueueMap.end()) { //Copy full skill queue
        robQueueMap[robID] = skillQueue(masterQueue);
	}
	
	return robQueueMap[robID];
}


bool SkillSequence::executeSkills(skillQueue& queue, Robot* robot)
{
    int isFinished = -1;    //Default to let skill decide
    bool skillReportedFinished = false;

    Skill::Skill* sk = queue.front().skill.get();
	skillUpdateFn updateFn = queue.front().updateFn;

	if(updateFn != nullptr)
       isFinished = updateFn(*sk, robot);
	   
    if(sk != nullptr)
        skillReportedFinished = sk->perform(robot);
	
	if(isFinished < 0)
	   isFinished = skillReportedFinished;
	
	if(isFinished == true) {
        std::cout << typeid(*sk).name() << "Finished" << std::endl;
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

/**************************************************/
