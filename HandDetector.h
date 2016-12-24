#ifndef HANDDETECTOR_H
#define HANDDETECTOR_H

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QString>

using namespace std;
using namespace cv;

class HandDetector
{
public:
    int m_number = 0;
    QString m_string = "Hello";
    HandDetector();
    ~HandDetector();

    int prevx, prevy;
    void mouseTo(int ,int );
    void run() ;

    void mouseClick();
    void mouseRelease();
    bool getAngle(Point , Point , Point );
    char* do_ocr(cv::Mat );
    char* do_ocr2(cv::Mat );
    char* do_ocr_backup(cv::Mat );


};


#endif // HANDDETECTOR_H
