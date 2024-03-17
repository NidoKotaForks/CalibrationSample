#include <iostream>
#include <opencv2/opencv.hpp>

void mamainmainmainmainmainmainmainmainmainmainmainin() {
    // カメラのフレームをキャプチャ
    cv::Mat frame = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/out.png");

    // ミラー
    //cv::flip(frame, frame, 1);

    // 入力画像をリサイズ
    cv::resize(frame, frame, cv::Size(640, 480));

    // グレースケール化
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // 認識の精度を上げるために画像を平滑化
    //cv::GaussianBlur(gray, gray, cv::Size(33, 33), 1);
    cv::imshow("gray", gray);

    // 表示用イメージ
    cv::Mat colimg = frame.clone();

    // 円検出
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 1, 1, 85, 1, 80);

    if (!circles.empty()) {
        for (size_t i = 0; i < circles.size(); ++i) {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            // 囲み線を描く
            cv::circle(colimg, center, radius, cv::Scalar(255, 255, 0), 2);
            // 中心点を描く
            cv::circle(colimg, center, 2, cv::Scalar(0, 0, 255), 3);
        }
    }

    // プレビュー
    cv::imshow("preview", colimg);
    cv::waitKey(0);
}
