#ifndef DIFFERENTIAL_PILOT_H
#define DIFFERENTIAL_PILOT_H

#include <math.h>
#include <deque>

#include "utilities/measurements.h"
#include "../drives/differential.h"
#include "../robot_pilot.h"

    /*! @brief Constants for the differential-drive closed-loop control algorithm
     * @details Frequently used constant sets for closed loop control motion can be defined here.
     * Refer to the motion control folder on Robobulls Google Drive for papers on what
     * exactly the constants do. The diagram below shows how modifying the constants can
     * change the robots’ paths. The constants actually used focus on reducing the
     * distance/rho error more than the orientation error because straighter paths make
     * obstacle avoidance calculations easier. */
    namespace DifferentialConstants
    {
        const double defaultConstants[3] = {3, 12, -1.3};
        const double noSlowdownConstants[3] = {0, 8, -1.5};
        const double sharpTurnConstants[3] = {3, 25, 0};
        const double ROTATE_VEL = 30;
    }

    /*! @brief Wheel velocity calculator for two-wheel (Differential) robots
     * @author Muhaimen Shamsi
     * @details This class exists to provide wheel velocities for the LEGO robots
     * used for code development. It uses the functions defined in closedloopcontrol.h
     * @see ThreeWheelCalculator
     * @see DifferentialCalculator */
    class PilotDifferential : public Pilot
    {
    public:
        PilotDifferential(Robot* robot);
        void driveTo (Point goalPoint, float theta_goal, Point nextPoint) override;

    private:
        Differential differential = Differential(0.115 * 1000, 0.027 * 1000);

        static const unsigned int sizeRhoQ   = 100;
        static const unsigned int sizeAlphaQ = 300;
        static const unsigned int sizeBetaQ  = 50;
        double newRho  , krho;
        double newAlpha, kalpha;
        double newBeta , kbeta;
        double kRhoI   = krho/(5.0 * sizeRhoQ);
        double kAlphaI = kalpha/(2.0 * sizeAlphaQ);
        double kBetaI  = kbeta/(1.2 * sizeBetaQ);
        double rotateOnly = false;

        std::pair<std::deque<double>, double> errorContainers[3];
        void updateIntegralError(Point newTarget);

        Point lastTargetPoint;
    };

#endif
