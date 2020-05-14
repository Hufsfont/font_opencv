#include <jni.h>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include<string>

using namespace cv;
using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_font_1opencv_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    void printWindow(string nameTag, Mat inputImg) {

        namedWindow(nameTag, WINDOW_AUTOSIZE);
        resize(inputImg, inputImg, Size(500, 200));
        imshow(nameTag, inputImg);

    }

    void ROI_save(Mat &Input, Rect rect, int cnt) {
        Mat roi = Input(rect);
        string char_num = to_string(cnt);
        string name = "num" + char_num + ".jpg";
        imwrite(name, roi);
    }

    Mat drawingContours(Mat &inputImg) {
        Mat drawing = Mat::zeros(inputImg.size(), CV_8UC3);

        std::vector<vector<Point>> contours;
        std::vector<Vec4i> hierarchy;

        findContours(inputImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        /// Draw contours
        for (int i = 0; i < contours.size(); i++) {
            Scalar color = Scalar(0, 255, 0);
            drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        }
        return drawing;
    }

    Mat drawingRectangle(Mat &inputImg, Mat &output) {
        Mat Rectangle = output.clone();
        int cnt = 0;

        std::vector<vector<Point>> contours;
        std::vector<Vec4i> hierarchy;

        findContours(inputImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

        if (contours.size() > 0) {
            for (int idx = 0; idx < contours.size(); idx++) {
                Rect rect = boundingRect(contours[idx]);

                if (rect.height * rect.width > 3000) { //입력값에 따라 조건이 달라져야 할듯(정교하게 다듬는 건 나중에)
                    rectangle(Rectangle, Point(rect.x, rect.y),
                              Point(rect.x + rect.width, rect.y + rect.height), Scalar(0, 255, 0),
                              5);
                    ROI_save(output, rect, cnt);
                    cnt++;
                }
            }
        }
        return Rectangle;

    }

    int main() {
        //이미지 파일을 불러와 그레이 이미지로 변환한다.
        Mat input_origin_image = imread("input4.jpg", IMREAD_COLOR);
        Mat input_gray_image;
        Mat result_binary_image;
        Mat kernel(5, 5, CV_8U, cv::Scalar(1));


        cvtColor(input_origin_image, input_gray_image, COLOR_RGBA2GRAY);


        /*
        threshold값을 127로 해서 이진화 한다.
        입력 이미지의 특정 필셀값이 threshold값보다 크면 결과
        이미지상의 같은 위치의 픽셀값을 255로 한다.
        thshold값보다 작을 경우에는 0이 된다.

        threshold(input_gray_image, result_binary_image, 127, 255, THRESH_BINARY);
        */

        //이진화
        Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(1, 1));
        GaussianBlur(input_gray_image, input_gray_image, cv::Point(5, 5), 0);
        adaptiveThreshold(input_gray_image, result_binary_image, 255, ADAPTIVE_THRESH_MEAN_C,
                          THRESH_BINARY, 17, 3);
        morphologyEx(result_binary_image, result_binary_image, cv::MORPH_CLOSE, kernel); //close
        bitwise_not(result_binary_image, result_binary_image); //색 반전

        //문자 갯수만큼 바운딩 박스가 나오는지 확인하고 침식 정도를 결정(if, for)_팬그램 확정 후 수정
        dilate(result_binary_image, result_binary_image, mask, cv::Point(-1, -1), 3);
        dilate(result_binary_image, result_binary_image, mask, cv::Point(-1, -1), 3);



        //counters 찾기
        Mat Counters = drawingContours(result_binary_image);

        //ractangle
        Mat Rectangle = drawingRectangle(result_binary_image, input_origin_image);


        //출력
        /*
        printWindow("입력 이미지", input_origin_image);
        printWindow("이진화 이미지", result_binary_image);
        printWindow("counters", Counters);
        printWindow("rectangle", Rectangle);
        */


        //아무키나 누를 때 까지 대기한다.
        while (cv::waitKey(0) < 0);

    }
}