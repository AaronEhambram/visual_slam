#include "VisualSlam.hpp"
#include <gtsam/slam/BetweenFactor.h>

VisualSlam::VisualSlam()
{
    // Constructor implementation
}

void VisualSlam::processAcceleration(const Acceleration &acc)
{
    std::cout << "Acceleration: time=" << std::setprecision(16) << acc.time << " x=" << acc.x_acc << " y=" << acc.y_acc << " z=" << acc.z_acc << std::endl;
    // Implementation for processing acceleration data
}

void VisualSlam::processGyroscope(const Gyroscope &gyro)
{
    std::cout << "Gyroscope: time=" << std::setprecision(16) << gyro.time << " x=" << gyro.x_gyro << " y=" << gyro.y_gyro << " z=" << gyro.z_gyro << std::endl;
    // Implementation for processing gyroscope data
}

void VisualSlam::processGNSS(const GNSS &gnss)
{
    std::cout << "GNSS: time=" << std::setprecision(16) << gnss.time << " lat=" << gnss.latitude << " lon=" << gnss.longitude << " alt=" << gnss.altitude << " quality=" << gnss.quality << std::endl;
    // Implementation for processing GNSS data
}

void VisualSlam::processImage(const Image &img)
{
    std::cout << "Image: time=" << std::setprecision(16) << img.time << " image_size=" << img.image.cols << "x" << img.image.rows << std::endl;
    // Implementation for processing image data
}
