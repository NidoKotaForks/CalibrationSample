#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

cv::Point2f getCircle(cv::Mat frame, cv::Scalar lower_color, cv::Scalar upper_color) {
    const int MIN_RADIUS = 25;

    // BGR画像をHSVに変換
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // ガウシアンぼかしを適用して認識精度を向上させる
    //cv::Mat blur;
    //cv::GaussianBlur(hsv, blur, cv::Size(9, 9), 0);

    // 指定した色範囲のみを抽出する
    cv::Mat color;
    cv::inRange(hsv, lower_color, upper_color, color);

    // オープニング・クロージングによるノイズ除去
    cv::Mat element8 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(color, color, cv::MORPH_OPEN, element8);
    cv::morphologyEx(color, color, cv::MORPH_CLOSE, element8);

    // 輪郭抽出
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(color, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::cout << contours.size() << " contours." << std::endl;

    if (!contours.empty()) {
        // 最大の領域を見つける
        std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
            return cv::contourArea(a) > cv::contourArea(b);
            });
        std::vector<cv::Point> largestContour = contours[0];

        // 最小外接円を用いて円を検出する
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(largestContour, center, radius);

        // 半径が十分大きいかチェック
        if (radius >= MIN_RADIUS) {
            return center;
        }
    }

    return cv::Point2f(-1, -1); // 見つからなかった場合は(-1, -1)を返す
}

int mainmainmainmainmainmain() {
    cv::Mat frame = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/out.png");

    // 赤色の円を抽出する
    //cv::Point2f center = getCircle(frame, cv::Scalar(130, 80, 80), cv::Scalar(200, 255, 255));
    cv::Point2f center = getCircle(frame, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    if (center != cv::Point2f(-1, -1)) {
        // 見つかった円の中心に青い円を描画
        cv::circle(frame, center, 2, cv::Scalar(255, 0, 0), 2);
        std::cout << "Circle found at: " << center << std::endl;
    }

    // 検出結果を表示
    cv::imshow("Circle Detect", frame);

    cv::waitKey(0);

    return 0;
}
