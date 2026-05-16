#ifndef SENSORDATAREADER_HPP
#define SENSORDATAREADER_HPP

#include <iostream>
#include <set>
#include <variant>
#include <optional>
#include <limits>
#include "SensorDataTypes.hpp"

template <typename T>
struct comp
{
  bool operator()(const T &a, const T &b) const
  {
    return a.time < b.time;
  }
};

class SensorDataReader
{
public:
  SensorDataReader(const std::string &source_folder);
  std::optional<std::variant<Acceleration, Gyroscope, GNSS, Image>> provideNextData();

private:
  std::string source_folder_;
  std::set<Acceleration, comp<Acceleration>> accelerations_;
  std::set<Gyroscope, comp<Gyroscope>> gyroscopes_;
  std::set<GNSS, comp<GNSS>> gnss_;
  std::set<Image, comp<Image>> images_;

  // Iterators for provide_next_data()
  std::set<Acceleration, comp<Acceleration>>::iterator acc_it_;
  std::set<Gyroscope, comp<Gyroscope>>::iterator gyro_it_;
  std::set<GNSS, comp<GNSS>>::iterator gnss_it_;
  std::set<Image, comp<Image>>::iterator img_it_;
  bool iterators_initialized_;

  void parseAccelerationCSV(const std::string &csv_path);
  void parseGyroscopeCSV(const std::string &csv_path);
  void parseGNSSCSV(const std::string &csv_path);
  void loadImages(const std::string &image_folder);
};

#endif // SENSORDATAREADER_HPP
