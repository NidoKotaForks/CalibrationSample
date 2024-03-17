#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>
#pragma once

#include <vector>
#include <string>
#include <optional>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>

class Curl
{
private:
    static size_t WriteData(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        std::vector<uchar>* stream = (std::vector<uchar>*)userdata;
        size_t count = size * nmemb;
        stream->insert(stream->end(), ptr, ptr + count);
        return count;
    }

public:
    static cv::Mat* Img(const char* url, int timeout)
    {
        std::vector<uchar> stream;

        CURL* curl = curl_easy_init();

        curl_easy_setopt(curl, CURLoption::CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLoption::CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLoption::CURLOPT_WRITEDATA, &stream);
        curl_easy_setopt(curl, CURLoption::CURLOPT_TIMEOUT, timeout);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLcode::CURLE_OK)
        {
            char* ct = nullptr;
            if (curl_easy_getinfo(curl, CURLINFO::CURLINFO_CONTENT_TYPE, &ct) == CURLcode::CURLE_OK && ct)
            {
                std::string ctStr = std::string(ct);

                if (ctStr.find("image") == std::string::npos)
                {

                    curl_easy_cleanup(curl);
                    return nullptr;
                }
            }
        }
        else
        {

            curl_easy_cleanup(curl);
            return nullptr;
        }

        curl_easy_cleanup(curl);

        cv::Mat* result = new cv::Mat();
        imdecode(stream, cv::ImreadModes::IMREAD_UNCHANGED, result);
        if (result->empty())
        {

            delete result;
            return nullptr;
        }

        return result;
    }
};

int mainmainmain(void)
{
    cv::Mat* mat = static_cast<cv::Mat*>(Curl::Img("https://i.pinimg.com/564x/c8/d2/c7/c8d2c76d9785431a7026d3be400c469a.jpg", 10));

    if (mat) 
    {
        cv::imshow("Image", *mat);
        cv::waitKey(0);
    }

    return EXIT_SUCCESS;
}