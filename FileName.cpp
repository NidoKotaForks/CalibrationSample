#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>

#pragma once

#include <vector>
#include <optional>
#include <opencv2/opencv.hpp>
#include <opencv2/ccalib/omnidir.hpp>
#include <curl/curl.h>

class Calibrator
{
private:
    class IsNotValidException : public std::exception
    {
    public:
        IsNotValidException(const std::string& message) : _message(message)
        {
        }

        virtual const char* what() const noexcept
        {
            return _message.c_str();
        }

    private:
        std::string _message;
    };

    cv::Size _patternSize;
    cv::Size _imgSize;
    std::vector<std::vector<cv::Point2f>> _imgPoints;
    std::vector<std::vector<cv::Point3f>> _objPoints;
    float _squareLengthMm;
    int _flags;
    bool _valid;

    cv::Mat _map1;
    cv::Mat _map2;

public:
    Calibrator(const cv::Size& patternSize, const float& squareLengthMm, const int& flags)
    {
        _patternSize = patternSize;
        _squareLengthMm = squareLengthMm;
        _flags = flags;
        _valid = false;
    }

    bool IsValid()
    {
        return _valid;
    }

    bool AddChessboardImg(cv::Mat& img)
    {
        std::vector<cv::Point2f> corners;

        bool found = cv::findChessboardCorners(img, _patternSize, corners, _flags);
        if (!found) return false;

        if (_imgSize.empty()) _imgSize = img.size();
        else if (_imgSize != img.size()) throw IsNotValidException("DifferentImageSize");

        _imgPoints.push_back(corners);

        std::vector<cv::Point3f> obj;
        for (int c = 0; c < _patternSize.height; c++)
        {
            for (int r = 0; r < _patternSize.width; r++)
            {
                float x = r * _squareLengthMm;
                float y = c * _squareLengthMm;
                float z = 0;
                obj.push_back(cv::Point3f(x, y, z));
            }
        }
        _objPoints.push_back(obj);

        return true;
    }

    void CalculateParameters()
    {
        if (_objPoints.size() == 0 || _imgPoints.size() == 0) throw IsNotValidException("ChessboardDoesNotFound");

        cv::Mat cameraMatrix;
        cv::Mat distCoeffs;
        cv::Mat R;
        cv::Mat T;

        try
        {
            double persRMS = cv::calibrateCamera(_objPoints, _imgPoints, _imgSize, cameraMatrix, distCoeffs, R, T);
            cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, _imgSize, CV_32F, _map1, _map2);
        }
        catch (std::exception& e)
        {
            std::cout << e.what();
        }

        //try 
        //{
        //    double fishRMS = cv::fisheye::calibrate(_objPoints, _imgPoints, _imgSize, cameraMatrix, distCoeffs, R, T);
        //    cv::fisheye::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, _imgSize, CV_32F, _map1, _map2);
        //}
        //catch (std::exception& e) 
        //{
        //    std::cout << e.what();
        //}

        _valid = true;
    }

    void Calibrate(cv::Mat& img)
    {
        if (_map1.empty() || _map2.empty()) throw IsNotValidException("ParamatorsDoesNotFound");
        cv::remap(img, img, _map1, _map2, cv::InterpolationFlags::INTER_AREA);
    }
};

int mainmainmainmain(void)
{
    //cv::Mat src = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/img/img2_1.jpg");
    cv::Mat src = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/Images/3/capture_7.jpeg");
    //cv::Mat src = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/img/photomini.jpg");

    //cv::cvtColor(mat, mat, cv::COLOR_BGR2HSV);
    //cv::inRange(mat, cv::Scalar(0, 0, 0), cv::Scalar(179, 255, 255), mat);
    //cv::inRange(mat, cv::Scalar(0, 0, 143), cv::Scalar(179, 61, 252), mat);

    //cv::erode(msk_img, msk_img, cv::Mat(), cv::Point(-1, -1), 2); // Žûk
    //cv::dilate(msk_img, msk_img, cv::Mat(), cv::Point(-1, -1), 3); // –c’£

    //cv::Mat matGraay;
    //cv::threshold(mat, matGraay, 126, 255, cv::THRESH_BINARY); //“ñ’l‰»

    //cv::imshow("Image", matGraay);

    std::vector<cv::Point2f> corners; 
    
    //cv::Size cornerSize = cv::Size(10, 7);
    cv::Size cornerSize = cv::Size(9, 6);
    //cv::Size cornerSize = cv::Size(3, 3);

    bool found = cv::findChessboardCorners(src, cornerSize, corners, cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_FAST_CHECK);
    std::cout << found << std::endl;

    cv::Mat prevMat = src.clone();
    cv::drawChessboardCorners(prevMat, cornerSize, corners, found);
    cv::imshow("Image", prevMat);

    Calibrator c = Calibrator(cornerSize, 24, cv::CALIB_CB_FILTER_QUADS + cv::CALIB_CB_FAST_CHECK);
    c.AddChessboardImg(src);
    c.CalculateParameters();
    c.Calibrate(src);
    cv::imshow("result", src);

    cv::waitKey(0);

    return EXIT_SUCCESS;
}