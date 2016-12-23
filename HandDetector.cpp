#include "HandDetector.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <ctime>

#include <opencv2/core.hpp>
#include <QString>
using namespace std;
using namespace cv;

Mat3b dbg;


cv::Scalar RED(0, 0, 255);
cv::Scalar GREEN(255, 0, 0);
cv::Scalar BLUE(0, 255, 255);
cv::Scalar YELLOW(0, 225, 225);

HandDetector::HandDetector()
{
      cout << "OpenCV version : " << CV_VERSION << endl;
      cout << "Major version : " << CV_MAJOR_VERSION << endl;
      cout << "Minor version : " << CV_MINOR_VERSION << endl;
      cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;

}

HandDetector::~HandDetector()
{
}

char* HandDetector::do_ocr(cv::Mat im)
{
// OCR function

    if (im.empty())
    {
        std::cout << "Cannot open source image!" << std::endl;
        //return -1;
    }

    cv::Mat gray;
    cv::cvtColor(im, gray, CV_BGR2GRAY);
    // ...other image pre-processing here...

    // Pass it to Tesseract API
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // Get the text
    char* out = tess.GetUTF8Text();
    std::cout << out << std::endl;

    return out;
}


void HandDetector::mouseTo(int x,int y){
    x = (x*1366)/600;
    y = (y*768)/400;
    if(sqrt( pow(x-prevx,2) + pow(y-prevy,2)) < 7){
        return;
    }
    prevx = x;
    prevy = y;
    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    XFlush(display);
    XCloseDisplay(display);
}

bool HandDetector::getAngle(Point s, Point f, Point e){
    float l1 = sqrt(fabs( pow(s.x-f.x,2) + pow(s.y-f.y,2) )) ;
    float l2 = sqrt(fabs( pow(f.x-e.x,2) + pow(f.y-e.y,2) )) ;
    float dot=(s.x-f.x)*(e.x-f.x) + (s.y-f.y)*(e.y-f.y);
    float angle = acos(dot/(l1*l2));
    angle=angle*180/M_PI;
    return true;
}

void HandDetector::run() {
    VideoCapture cap(1);
　　　 // TODO

}


