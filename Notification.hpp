#pragma once

#include <iostream>

enum NotificationType
{
    TEXT,
    IMAGE
};

struct Notification
{
    NotificationType type;
    std::string info;
};