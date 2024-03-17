#include <iostream>
#include <opencv2/opencv.hpp>

void mamainmainmainmainmainmainmainmainmainmainmainin() {
    // �J�����̃t���[�����L���v�`��
    cv::Mat frame = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/out.png");

    // �~���[
    //cv::flip(frame, frame, 1);

    // ���͉摜�����T�C�Y
    cv::resize(frame, frame, cv::Size(640, 480));

    // �O���[�X�P�[����
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // �F���̐��x���グ�邽�߂ɉ摜�𕽊���
    //cv::GaussianBlur(gray, gray, cv::Size(33, 33), 1);
    cv::imshow("gray", gray);

    // �\���p�C���[�W
    cv::Mat colimg = frame.clone();

    // �~���o
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 1, 1, 85, 1, 80);

    if (!circles.empty()) {
        for (size_t i = 0; i < circles.size(); ++i) {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            // �͂ݐ���`��
            cv::circle(colimg, center, radius, cv::Scalar(255, 255, 0), 2);
            // ���S�_��`��
            cv::circle(colimg, center, 2, cv::Scalar(0, 0, 255), 3);
        }
    }

    // �v���r���[
    cv::imshow("preview", colimg);
    cv::waitKey(0);
}
