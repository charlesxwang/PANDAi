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
    HandDetector();
    ~HandDetector();

    int prevx, prevy;
    void mouseTo(int ,int);
    void run() ;

    char* do_ocr(cv::Mat);

};


#endif // HANDDETECTOR_H
