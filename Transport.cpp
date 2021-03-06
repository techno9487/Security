#include "Transport.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct DataTransfer
{
    char* data;
    int left;
};

size_t JSONReadFunction(char* buffer,size_t size,size_t nitems,void* istream)
{
    struct DataTransfer* transfer = (struct DataTransfer*)istream;
    size_t bufferSize = size*nitems;

    size_t toCopy = transfer->left;

    if(transfer->left > bufferSize)
    {
        toCopy = bufferSize;
    }

    memcpy(buffer,transfer->data,toCopy);
    transfer->data += toCopy;
    transfer->left -= toCopy;

    return bufferSize;
}

Transport::Transport()
{
    this->handle = curl_easy_init();
    if(this->handle == nullptr)
    {
        std::cout << "failed to create curl instance" << std::endl;
    }
}

void Transport::SendTextRequest(std::string text)
{
    curl_easy_reset(handle);
    curl_easy_setopt(handle,CURLOPT_VERBOSE,1);
    curl_easy_setopt(handle,CURLOPT_URL,"http://localhost:5000/api/text");
    curl_easy_setopt(handle,CURLOPT_POST,1L);

    json j;
    j["text"] = text;

    std::string data = j.dump();

    struct DataTransfer transfer;
    transfer.data = (char*)data.c_str();
    transfer.left = data.length();

    curl_easy_setopt(handle,CURLOPT_READFUNCTION,JSONReadFunction);
    curl_easy_setopt(handle,CURLOPT_READDATA,&transfer);
    curl_easy_setopt(handle,CURLOPT_UPLOAD,1l);

    struct curl_slist* headers = NULL;
    curl_slist_append(headers,"Content-Type: application/json");
    curl_easy_setopt(handle,CURLOPT_HTTPHEADER,headers);
    curl_slist_free_all(headers);

    CURLcode res;
    res = curl_easy_perform(handle);
    
    if(res != CURLE_OK)
    {
        const char* error = curl_easy_strerror(res);
        std::cout << "failed to send text request: " << error << std::endl;
    }
}

void Transport::SendImageRequest(std::string path)
{
    curl_easy_reset(handle);
    curl_easy_setopt(handle,CURLOPT_VERBOSE,1);
    curl_easy_setopt(handle,CURLOPT_URL,"http://localhost:5000/api/image");
    curl_easy_setopt(handle,CURLOPT_POST,1L);

    json j;
    j["path"] = path;

    std::string data = j.dump();

    struct DataTransfer transfer;
    transfer.data = (char*)data.c_str();
    transfer.left = data.length();

    curl_easy_setopt(handle,CURLOPT_READFUNCTION,JSONReadFunction);
    curl_easy_setopt(handle,CURLOPT_READDATA,&transfer);
    curl_easy_setopt(handle,CURLOPT_UPLOAD,1l);

    struct curl_slist* headers = NULL;
    curl_slist_append(headers,"Content-Type: application/json");
    curl_easy_setopt(handle,CURLOPT_HTTPHEADER,headers);
    curl_slist_free_all(headers);

    CURLcode res;
    res = curl_easy_perform(handle);
    
    if(res != CURLE_OK)
    {
        const char* error = curl_easy_strerror(res);
        std::cout << "failed to send text request: " << error << std::endl;
    }
}
