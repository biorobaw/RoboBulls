//This is an example showing how to use GenericMovementBehavior in a behavior
//We inhert from GenericMovementBehavior

class ExampleBehavior : public GenericMovementBehavior
{
	void perform(Robot* robot) override {
		//GMB Method: We set a movement poisiton target to prepare for movement.
		//In this case, we're setting the robot to move to point (0,0)
		setMovementTargets( Point(0,0) );

		//Actually perform movement
		GenericMovementBehavior::perform(robot);
	}
};

//This behavior can be assigned to a robot with:
robot->assignBeh<ExampleBehavior>();

