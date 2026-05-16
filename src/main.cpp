#include <iostream>
#include "VisualSlam.hpp"
#include "SensorDataReader.hpp"

int main()
{
  VisualSlam visual_slam;
  SensorDataReader sensor_data_reader("/media/ehambram/Data1/Datasets/S25_SensorLogger/Hildesheimer_Stra_e-2026-05-16_18-45-45");
  bool next_data_available = true;
  while (next_data_available)
  {
    const std::optional<std::variant<Acceleration, Gyroscope, GNSS, Image>> data = sensor_data_reader.provideNextData();
    if (!data)
    {
      next_data_available = false;
      break;
    }

    std::visit([&visual_slam](const auto &value)
               {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, Acceleration>) {
        visual_slam.processAcceleration(value);
      } else if constexpr (std::is_same_v<T, Gyroscope>) {
        visual_slam.processGyroscope(value);
      } else if constexpr (std::is_same_v<T, GNSS>) {
        visual_slam.processGNSS(value);
      } else if constexpr (std::is_same_v<T, Image>) {
        visual_slam.processImage(value);
      } }, *data);
  }
  return 0;
}