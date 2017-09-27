#include "NotificationCentre.hpp"
#include "Notification.hpp"
#include <iostream>

void NotificationCentre::Run()
{
    worker = std::thread{&NotificationCentre::NotificationThread,this};
}

void NotificationCentre::NotificationThread()
{
    std::cout << "Starting worker thread" << std ::endl;
    while(this->isActive)
    {
        if(this->lock.try_lock())
        {
            while(this->jobs.size() > 0)
            {
                Notification note = jobs.front();
                jobs.pop();

                switch(note.type)
                {
                    case TEXT:
                    trans.SendTextRequest(note.info);
                    break;
                }
            }
            lock.unlock();
        }
    }
}

void NotificationCentre::SubmitJob(Notification note)
{
    while(this->lock.try_lock() == false){}
    this->jobs.push(note);
    lock.unlock();
}

void NotificationCentre::Stop()
{
    this->isActive = false;
    this->worker.join();
}