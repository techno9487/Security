#pragma once
#include <iostream>
#include <curl/curl.h>

class Transport
{
private:
    CURL* handle;
public:
    Transport();
    void SendTextRequest(std::string text);
    void SendImageRequest(std::string path);
};
