#include <opencv2/opencv.hpp>

int mainmainmainmainmainmainmainmainmainmainmainmainmainmainmainmainmain() {
    // �摜��ǂݍ���
    cv::Mat src = cv::imread("C:/Users/nidok/Desktop/Unity/firework2/Assets/out.png");
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // �G�b�W�����o����
    cv::Mat edge;
    cv::Canny(gray, edge, 30, 150); // 50��150�͂������l�̉����Ə��
    //cv::medianBlur(edge, edge, 5);
    // 
      // �c�����s���J�[�l�����`����
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    // �G�b�W��c��������
    cv::dilate(edge, edge, kernel);

    cv::add(edge, gray, gray);

    // ���ʂ�\������
    cv::imshow("Edges", gray);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 70, 1, 55, 5, 50);

    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        // draw the outer circle
        cv::circle(src, center, radius, cv::Scalar(0, 255, 0), 2);
        // draw the center of the circle
        cv::circle(src, center, 2, cv::Scalar(0, 0, 255), 3);
    }


    cv::imshow("output.png", src);
    cv::waitKey(0);

    return 0;
}