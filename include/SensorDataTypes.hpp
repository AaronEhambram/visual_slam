#ifndef SENSORDATATYPES_HPP
#define SENSORDATATYPES_HPP
#include "opencv2/core.hpp"

struct Acceleration
{
  double time;
  double x_acc;
  double y_acc;
  double z_acc;
};

struct Gyroscope
{
  double time;
  double x_gyro;
  double y_gyro;
  double z_gyro;
};

struct GNSS
{
  double time;
  double latitude;
  double longitude;
  double altitude;
  double quality;
};

struct Image
{
  double time;
  cv::Mat image;
};

#endif // SENSORDATATYPES_HPP
