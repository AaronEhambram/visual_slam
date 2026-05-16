#ifndef VISUALSLAM_HPP
#define VISUALSLAM_HPP

#include "opencv2/core.hpp"
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>
#include "SensorDataTypes.hpp"

class VisualSlam
{
public:
    VisualSlam();
    void processAcceleration(const Acceleration &acc);
    void processGyroscope(const Gyroscope &gyro);
    void processGNSS(const GNSS &gnss);
    void processImage(const Image &img);

private:
    // Internal state and methods for visual SLAM
    // For example, feature detection, matching, pose estimation, etc.
};

#endif // VISUALSLAM_HPP
