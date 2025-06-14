#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "SensorData.hpp"
#include <random>

class Client {
public:
    Client(const std::string& server_ip, int server_port);
    ~Client();
    SensorData readSensorData();
    bool connectToServer(int max_retries = 10, int retry_delay_ms = 1000);
    bool sendData(const SensorData& data);
    void disconnect();

private:
    std::string server_ip_;
    int server_port_;
    int sock_;
    bool connected_;
    std::mt19937 rng_;
    std::uniform_real_distribution<double> temp_dist_;
    std::uniform_real_distribution<double> hum_dist_;
    std::uniform_real_distribution<double> light_dist_;
    void initializeSocketLib();
    void cleanupSocketLib();
    std::string receiveResponse();
    void run();
};

#endif // CLIENT_HPP

