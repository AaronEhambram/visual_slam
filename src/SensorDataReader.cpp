#include "SensorDataReader.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cmath>
#include <iostream>
#include "opencv2/imgcodecs.hpp"

namespace fs = std::filesystem;

SensorDataReader::SensorDataReader(const std::string &source_folder)
    : source_folder_(source_folder), iterators_initialized_(false)
{
    parseAccelerationCSV(source_folder_ + "/Accelerometer.csv");
    parseGyroscopeCSV(source_folder_ + "/Gyroscope.csv");
    parseGNSSCSV(source_folder_ + "/Location.csv");
    loadImages(source_folder_ + "/Camera");
}

void SensorDataReader::parseAccelerationCSV(const std::string &csv_path)
{
    std::ifstream file(csv_path);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open Accelerometer.csv at " << csv_path << std::endl;
        return;
    }

    std::string line;
    
    // Skip header row
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() < 5) {
            std::cerr << "Warning: Invalid Accelerometer.csv row (expected 5 columns): " << line << std::endl;
            continue;
        }
        
        try {
            double time_ns = std::stod(tokens[0]);
            double x_acc = std::stod(tokens[3]);
            double y_acc = std::stod(tokens[2]);
            double z_acc = std::stod(tokens[1]);
            
            Acceleration acc;
            acc.time = time_ns / 1e9;
            acc.x_acc = x_acc;
            acc.y_acc = y_acc;
            acc.z_acc = z_acc;
            
            accelerations_.insert(acc);
        } catch (const std::exception &e) {
            std::cerr << "Warning: Failed to parse Accelerometer.csv row: " << line << " (" << e.what() << ")" << std::endl;
        }
    }
}

void SensorDataReader::parseGyroscopeCSV(const std::string &csv_path)
{
    std::ifstream file(csv_path);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open Gyroscope.csv at " << csv_path << std::endl;
        return;
    }

    std::string line;
    
    // Skip header row
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() < 5) {
            std::cerr << "Warning: Invalid Gyroscope.csv row (expected 5 columns): " << line << std::endl;
            continue;
        }
        
        try {
            double time_ns = std::stod(tokens[0]);
            double x_gyro = std::stod(tokens[3]);
            double y_gyro = std::stod(tokens[2]);
            double z_gyro = std::stod(tokens[1]);
            
            Gyroscope gyro;
            gyro.time = time_ns / 1e9;
            gyro.x_gyro = x_gyro;
            gyro.y_gyro = y_gyro;
            gyro.z_gyro = z_gyro;
            
            gyroscopes_.insert(gyro);
        } catch (const std::exception &e) {
            std::cerr << "Warning: Failed to parse Gyroscope.csv row: " << line << " (" << e.what() << ")" << std::endl;
        }
    }
}

void SensorDataReader::parseGNSSCSV(const std::string &csv_path)
{
    std::ifstream file(csv_path);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open Location.csv at " << csv_path << std::endl;
        return;
    }

    std::string line;
    
    // Skip header row
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        // Expected columns: time, seconds_elapsed, bearingAccuracy, speedAccuracy, verticalAccuracy, 
        // horizontalAccuracy, speed, bearing, altitude, longitude, latitude
        if (tokens.size() < 11) {
            std::cerr << "Warning: Invalid Location.csv row (expected 11 columns): " << line << std::endl;
            continue;
        }
        
        try {
            double time_ns = std::stod(tokens[0]);
            double latitude = std::stod(tokens[10]);
            double longitude = std::stod(tokens[9]);
            double altitude = std::stod(tokens[8]);
            double vertical_accuracy = std::stod(tokens[4]);
            
            GNSS gnss;
            gnss.time = time_ns / 1e9;
            gnss.latitude = latitude;
            gnss.longitude = longitude;
            gnss.altitude = altitude;
            gnss.quality = vertical_accuracy;
            
            gnss_.insert(gnss);
        } catch (const std::exception &e) {
            std::cerr << "Warning: Failed to parse Location.csv row: " << line << " (" << e.what() << ")" << std::endl;
        }
    }
}

void SensorDataReader::loadImages(const std::string &image_folder)
{
    if (!fs::exists(image_folder)) {
        std::cerr << "Warning: Camera folder does not exist at " << image_folder << std::endl;
        return;
    }

    if (!fs::is_directory(image_folder)) {
        std::cerr << "Warning: Camera path is not a directory: " << image_folder << std::endl;
        return;
    }

    try {
        for (const auto &entry : fs::directory_iterator(image_folder)) {
            if (!entry.is_regular_file()) continue;
            
            std::string filename = entry.path().filename().string();
            std::string extension = entry.path().extension().string();
            
            // Check if file has .jpg or .jpeg extension
            if (extension != ".jpg" && extension != ".jpeg" && extension != ".JPG" && extension != ".JPEG") {
                continue;
            }
            
            try {
                // Extract timestamp from filename (remove extension)
                std::string timestamp_str = filename.substr(0, filename.find(extension));
                double time_ms = std::stod(timestamp_str);
                
                // Load image using OpenCV
                cv::Mat img = cv::imread(entry.path().string());
                
                if (img.empty()) {
                    std::cerr << "Warning: Failed to load image: " << entry.path().string() << std::endl;
                    continue;
                }
                
                Image image;
                image.time = time_ms / 1e3; // Convert ms to seconds
                image.image = img;
                
                images_.insert(image);
            } catch (const std::exception &e) {
                std::cerr << "Warning: Failed to process image " << filename << " (" << e.what() << ")" << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Warning: Error scanning camera folder: " << e.what() << std::endl;
    }
}

std::optional<std::variant<Acceleration, Gyroscope, GNSS, Image>> SensorDataReader::provideNextData()
{
    // Initialize iterators on first call
    if (!iterators_initialized_) {
        acc_it_ = accelerations_.begin();
        gyro_it_ = gyroscopes_.begin();
        gnss_it_ = gnss_.begin();
        img_it_ = images_.begin();
        iterators_initialized_ = true;
    }
    
    // Check if all iterators have reached the end
    if (acc_it_ == accelerations_.end() && 
        gyro_it_ == gyroscopes_.end() && 
        gnss_it_ == gnss_.end() && 
        img_it_ == images_.end()) {
        return std::nullopt;
    }
    
    // Find the next data with the smallest timestamp
    double min_time = std::numeric_limits<double>::max();
    int next_type = -1; // 0=accel, 1=gyro, 2=gnss, 3=image
    
    if (acc_it_ != accelerations_.end() && acc_it_->time < min_time) {
        min_time = acc_it_->time;
        next_type = 0;
    }
    if (gyro_it_ != gyroscopes_.end() && gyro_it_->time < min_time) {
        min_time = gyro_it_->time;
        next_type = 1;
    }
    if (gnss_it_ != gnss_.end() && gnss_it_->time < min_time) {
        min_time = gnss_it_->time;
        next_type = 2;
    }
    if (img_it_ != images_.end() && img_it_->time < min_time) {
        min_time = img_it_->time;
        next_type = 3;
    }
    
    if (next_type == -1) {
        return std::nullopt;
    }
    
    // Get the data and advance the iterator
    std::variant<Acceleration, Gyroscope, GNSS, Image> result;
    
    switch (next_type) {
        case 0:
            result = *acc_it_;
            ++acc_it_;
            break;
        case 1:
            result = *gyro_it_;
            ++gyro_it_;
            break;
        case 2:
            result = *gnss_it_;
            ++gnss_it_;
            break;
        case 3:
            result = *img_it_;
            ++img_it_;
            break;
    }
    
    return result;
}