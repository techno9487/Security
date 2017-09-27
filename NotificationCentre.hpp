#pragma once

#include <mutex>
#include <queue>
#include <iostream>
#include <thread>
#include "Notification.hpp"
#include "Transport.hpp"

class NotificationCentre
{
private:
    std::mutex lock;
    std::queue<Notification> jobs;
    void NotificationThread();
    bool isActive = true;
    std::thread worker;
    Transport trans;
public:
    void SubmitJob(Notification note);
    void Run();
    void Stop();
};