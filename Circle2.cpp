#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat src = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/out.png");
    if (src.empty()) {
        std::cerr << "Error: Couldn't load image." << std::endl;
        return -1;
    }

    cv::resize(src, src, cv::Size(), 0.5, 0.5);

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    // 抽出したい色の範囲を設定（ここでは緑色を示します）
    cv::Scalar lower_green = cv::Scalar(20 / 2.0, 30 * 2.56, 0 * 2.56);
    cv::Scalar upper_green = cv::Scalar(40 / 2.0, 100 * 2.56, 100 * 2.56);

    // 指定した色の範囲を抽出
    cv::Mat mask;
    cv::inRange(hsv, lower_green, upper_green, mask);
    cv::bitwise_not(mask, mask);

    // 抽出した領域を白色に置き換え
    cv::Mat result;
    src.copyTo(result, mask); // 抽出された領域のみをコピー

    cv::Mat gray = mask;
    //cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 15);
    cv::imshow("gray.png", gray);
    //cv::GaussianBlur(gray, gray, cv::Size(33, 33), 1);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 30, 1, 10, 1, 40);

    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        if (radius < 20) continue;

        // draw the outer circle
        cv::circle(src, center, radius, cv::Scalar(0, 255, 0), 2);
        std::cout << radius << std::endl;
        // draw the center of the circle
        cv::circle(src, center, 2, cv::Scalar(0, 0, 255), 3);
    }


    cv::imshow("output.png", src);
    cv::waitKey(0);

    return 0;
}
