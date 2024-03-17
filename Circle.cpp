#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

cv::Point2f getCircle(cv::Mat frame, cv::Scalar lower_color, cv::Scalar upper_color) {
    const int MIN_RADIUS = 25;

    // BGR�摜��HSV�ɕϊ�
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // �K�E�V�A���ڂ�����K�p���ĔF�����x�����コ����
    //cv::Mat blur;
    //cv::GaussianBlur(hsv, blur, cv::Size(9, 9), 0);

    // �w�肵���F�͈݂͂̂𒊏o����
    cv::Mat color;
    cv::inRange(hsv, lower_color, upper_color, color);

    // �I�[�v�j���O�E�N���[�W���O�ɂ��m�C�Y����
    cv::Mat element8 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(color, color, cv::MORPH_OPEN, element8);
    cv::morphologyEx(color, color, cv::MORPH_CLOSE, element8);

    // �֊s���o
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(color, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::cout << contours.size() << " contours." << std::endl;

    if (!contours.empty()) {
        // �ő�̗̈��������
        std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
            return cv::contourArea(a) > cv::contourArea(b);
            });
        std::vector<cv::Point> largestContour = contours[0];

        // �ŏ��O�ډ~��p���ĉ~�����o����
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(largestContour, center, radius);

        // ���a���\���傫�����`�F�b�N
        if (radius >= MIN_RADIUS) {
            return center;
        }
    }

    return cv::Point2f(-1, -1); // ������Ȃ������ꍇ��(-1, -1)��Ԃ�
}

int mainmainmainmainmainmain() {
    cv::Mat frame = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/out.png");

    // �ԐF�̉~�𒊏o����
    //cv::Point2f center = getCircle(frame, cv::Scalar(130, 80, 80), cv::Scalar(200, 255, 255));
    cv::Point2f center = getCircle(frame, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
    if (center != cv::Point2f(-1, -1)) {
        // ���������~�̒��S�ɐ��~��`��
        cv::circle(frame, center, 2, cv::Scalar(255, 0, 0), 2);
        std::cout << "Circle found at: " << center << std::endl;
    }

    // ���o���ʂ�\��
    cv::imshow("Circle Detect", frame);

    cv::waitKey(0);

    return 0;
}
