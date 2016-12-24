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

char* HandDetector::do_ocr2(cv::Mat img)
{
    //Mat1b img = imread("../6.png", IMREAD_GRAYSCALE);
    cvtColor(img, img, CV_RGB2GRAY);
    cvtColor(img, dbg, COLOR_GRAY2BGR);
    //cvtColor(img, dbg);//cvtColor(img, dbg, COLOR_GRAY2BGR);

    //cout<< img.type() << CV_8UC3 <<endl;
    Mat3b result;
    // img.copyTo(result);
    cvtColor(img, result, COLOR_GRAY2BGR);
    //cvtColor(img, result);//cvtColor(img, dbg, COLOR_GRAY2BGR);

    Mat1b img2;
    adaptiveThreshold(img, img2, 255,CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11, 2);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,3));//5,3
    morphologyEx(img2, img2, MORPH_OPEN, kernel);
    imshow("Mor",img2);

    // Apply a small border
    copyMakeBorder(img2, img2, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(0));
    //copyMakeBorder(img2, img2, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0));

    // Take the bounding boxes of all connected components
    vector<vector<Point> > contours;
    findContours(img2.clone(), contours, CV_RETR_LIST, CHAIN_APPROX_NONE);

    int minArea = 30;
    int maxArea = 200;
    vector<Rect> rects;
    for (int i = 0; i < contours.size(); ++i)
    {
        Rect r = boundingRect(contours[i]);
        if (r.area() >= minArea && r.area() >= maxArea)
        {
            // Account for border
            r -= Point(5,5);
            /*
            if ((r.x+r.width/2+5/2)<=img2.cols && (r.y+r.height/2+5/2)<=img2.rows && (r.x-r.width/2-5/2)>=0 && (r.y-r.height/2-5/2)>=0){
                r += Size(5,5);
            }
            */

            //r += Point(1,1);
            rects.push_back(r);
        }
    }

    int middle = img.cols / 2;

    // Keep bottom rect, containig middle point
    if (rects.empty()) return " ";

    Rect word;
    for (int i = 1; i < rects.size(); ++i)
    {
        Point pt(middle, rects[i].y + rects[i].height/2);
        if (rects[i].contains(pt))
        {
            if (rects[i].y > word.y)
            {
                word = rects[i];
            }
        }
    }

    // Show results
    Mat3b res;
    //img.copyTo(res);
    cvtColor(img, res, COLOR_GRAY2BGR);
    //cvtColor(img, res);//cvtColor(img, dbg, COLOR_GRAY2BGR);
    for (int i = 0; i < rects.size(); ++i)
    {
        rectangle(res, rects[i], Scalar(0, 255, 0));
    }


    rectangle(result, word, Scalar(13, 230, 5), 2);
    imshow("Result", result);

    //imshow("Rects", res);

    //waitKey();
    //imwrite( "./result.jpg", result );
    //imwrite( "./result2.jpg", result );

    if (word.width>0){


    cv::Mat gray_origion;
    cv::Mat gray;
    result(word).copyTo(gray_origion);
    imshow("tess", gray_origion);


    //gray_origion.copyTo(gray);
    cvtColor(gray_origion, gray, CV_BGR2GRAY);
    //cv::cvtColor(gray_origion, gray, CV_BGR2GRAY);

    //cout<< gray.type() <<endl;


    // Pass it to Tesseract API
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // Get the text
    char* out = tess.GetUTF8Text();
    //std::cout << out << std::endl;
    return out;
    }else{
        //std::cout << "No letters." << std::endl;
        return " ";
    }

}

char* HandDetector::do_ocr_backup(cv::Mat img)
{
    //Mat1b img = imread("../6.png", IMREAD_GRAYSCALE);
    //cvtColor(img, dbg);//cvtColor(img, dbg, COLOR_GRAY2BGR);

    cout << "IMREAD_GRAYSCALE:" << IMREAD_GRAYSCALE << endl;

    Mat3b result;
    img.copyTo(result);
    //cvtColor(img, result);//cvtColor(img, dbg, COLOR_GRAY2BGR);

    Mat1b img2;
    img2 = img ;//< 150;

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5,3));
    morphologyEx(img2, img2, MORPH_DILATE, kernel);

    // Apply a small border
    copyMakeBorder(img2, img2, 5, 5, 5, 5, BORDER_CONSTANT, Scalar(0));

    // Take the bounding boxes of all connected components
    vector<vector<Point> > contours;
    findContours(img2.clone(), contours, CV_RETR_LIST, CHAIN_APPROX_NONE);

    int minArea = 60;
    vector<Rect> rects;
    for (int i = 0; i < contours.size(); ++i)
    {
        Rect r = boundingRect(contours[i]);
        if (r.area() >= minArea)
        {
            // Account for border
            r -= Point(5,5);
            rects.push_back(r);
        }
    }

    int middle = img.cols / 2;

    // Keep bottom rect, containig middle point
    if (rects.empty()) return "No words";

    Rect word;
    for (int i = 1; i < rects.size(); ++i)
    {
        Point pt(middle, rects[i].y + rects[i].height/2);
        if (rects[i].contains(pt))
        {
            if (rects[i].y > word.y)
            {
                word = rects[i];
            }
        }
    }

    // Show results
    Mat3b res;
    img.copyTo(res);
    //cvtColor(img, res);//cvtColor(img, dbg, COLOR_GRAY2BGR);
    for (int i = 0; i < rects.size(); ++i)
    {
        rectangle(res, rects[i], Scalar(0, 255, 0));
    }

    rectangle(result, word, Scalar(0, 0, 255), 2);

    //imshow("Rects", res);
    //imshow("Result", result);
    //waitKey();

    cv::Mat gray_origion;
    cv::Mat gray;
    result(word).copyTo(gray_origion);
    gray_origion.copyTo(gray);
    //cv::cvtColor(gray_origion, gray, CV_BGR2GRAY);



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

void HandDetector::mouseClick(){
    int button=Button1;
    Display *display = XOpenDisplay(NULL);
    XEvent event;
    if(display == NULL){
        cout<<"Error connecting to display"<<endl;
        exit(EXIT_FAILURE);
    }
    memset(&event, 0x00, sizeof(event));
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    event.xbutton.subwindow = event.xbutton.window;
    while(event.xbutton.subwindow){
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) cout<<"ERROR SENDING CLICK"<<endl;
    XFlush(display);
    XCloseDisplay(display);
}

//When called, it simulates a mouse release event at the current cursor location
void HandDetector::mouseRelease(){
    int button=Button1;
    Display *display = XOpenDisplay(NULL);
    XEvent event;
    if(display == NULL){
        cout<<"Error connecting to display"<<endl;
        exit(EXIT_FAILURE);
    }
    memset(&event, 0x00, sizeof(event));
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    event.xbutton.subwindow = event.xbutton.window;
    while(event.xbutton.subwindow){
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) cout<<"ERROR RELEASING"<<endl;
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
        namedWindow("Control", CV_WINDOW_AUTOSIZE);

        int iLowH = 0;
        int iHighH = 39;

        int iLowS = 46;
        int iHighS = 230;

        int iLowV = 0;
        int iHighV = 244;

        cvCreateTrackbar("LowH", "Control", &iLowH, 179);
        cvCreateTrackbar("HighH", "Control", &iHighH, 179);

        cvCreateTrackbar("LowS", "Control", &iLowS, 255);
        cvCreateTrackbar("HighS", "Control", &iHighS, 255);

        cvCreateTrackbar("LowV", "Control", &iLowV, 255);
        cvCreateTrackbar("HighV", "Control", &iHighV, 255);

        while(true){
            Mat imgOriginal;
            Mat imgOriginal_true;

            bool bSuccess = cap.read(imgOriginal);
            imgOriginal.copyTo(imgOriginal_true);
            //flip(imgOriginal, imgOriginal, 1);

            Mat imgHSV;

            cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

            Mat imgThresholded;
            Mat threshed;
            inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

            erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

            dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) );
            erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) );

            inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshed);

            erode(threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
            dilate( threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

            dilate( threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) );
            erode(threshed, threshed, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) );



            double largest_area = 0;
            int largest_contour_index = 0;
            Rect bounding_rect;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            findContours(imgThresholded, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
            for(int i = 0; i < contours.size(); i++){
                double a = contourArea(contours[i], false);
                if(a > largest_area){
                    largest_area = a;
                    largest_contour_index = i;
                    bounding_rect = boundingRect(contours[i]);
                }
            }



            if(contours.size() > 0 && contours[largest_contour_index].size() > 3){
                approxPolyDP( Mat(contours[largest_contour_index]), contours[largest_contour_index], 20, true );

                drawContours(imgOriginal, contours,largest_contour_index, Scalar(0,255,255), 2);
                //rectangle(imgOriginal, bounding_rect,  Scalar(0,255,0), 1, 8, 0);
                Moments mu = moments(contours[largest_contour_index], false);
                Point2f mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
                //mouseTo(mc.x, mc.y);
                circle( imgOriginal, mc, 4, Scalar(0,255,0), -1, 8, 0 );

                vector<Point> hull;
                vector<int> hulli;
                vector<Vec4i> defects;
                vector<Vec4i> fingers;
                convexHull(contours[largest_contour_index], hull, false);
                vector<vector<Point> > hulls(1, hull);
                drawContours(imgOriginal, hulls, -1, Scalar(255,0,0), 2);
                convexHull(contours[largest_contour_index], hulli, false);

                if(hulli.size() > 3){
                    convexityDefects(contours[largest_contour_index], hulli, defects);
                    for(int i = 0; i < defects.size(); i++){
                        if(defects[i][3] > 10000){
                            if(getAngle(contours[largest_contour_index][defects[i][0]], contours[largest_contour_index][defects[i][2]], contours[largest_contour_index][defects[i][1]])){
                                fingers.push_back(defects[i]);
                            }
                        }
                    }


                    if(fingers.size() > 0){
                        reverse(fingers.begin(), fingers.end());
                        Point indexFinger = contours[largest_contour_index][fingers[0][0]];
                        for(int i = 0; i < fingers.size(); i++){
                            if(contours[largest_contour_index][fingers[i][0]].y < indexFinger.y){
                                indexFinger = contours[largest_contour_index][fingers[i][0]];
                            }
                            if(contours[largest_contour_index][fingers[i][1]].y < indexFinger.y){
                                indexFinger = contours[largest_contour_index][fingers[i][1]];
                            }
                            circle(imgOriginal, contours[largest_contour_index][fingers[i][0]],5,Scalar(0,255,0),-1);
                            circle(imgOriginal, contours[largest_contour_index][fingers[i][1]],5,Scalar(0,255,0),-1);
                            circle(imgOriginal, contours[largest_contour_index][fingers[i][2]],5,Scalar(0,255,0),-1);
                        }
                        circle(imgOriginal, indexFinger,5,Scalar(0,0,0),-1);
                        //std::cout << "finger tip: " << indexFinger << std::endl;
                        cv::Mat img2(imgOriginal.rows,imgOriginal.cols,CV_8UC3);

                        float roi_width = 200;
                        float roi_height = 100;
                        float roi_x = indexFinger.x - roi_width/2;
                        float roi_y = indexFinger.y - roi_height-10;

                        if(indexFinger.x > 0 && indexFinger.y > 0){
                              if(0 <= roi_x && 0 <= roi_width && roi_x + roi_width <= imgOriginal.cols && 0 <= roi_y && 0 <= roi_height && roi_y + roi_height <= imgOriginal.rows){
                                imgOriginal(cv::Rect(roi_x,roi_y,roi_width,roi_height)).copyTo(img2);
                                //flip(img2, img2, 1);
                                imshow("MainWindow2", img2);
                                //std::cout<< "do ocr: " << roi_x << "," << roi_y << "============================================================================" << std::endl;
                                char* temp_word = do_ocr2(img2);
                                /*
                                if(temp_word == " "){
                                    //std::cout << "it's space" << std::endl;
                                }else{//check

                                        int len = strlen(temp_word);
                                        int is_word = 1;
                                        for ( int i=0; i<len; ++i )
                                        {

                                            if ( !isalpha(temp_word[i]) )
                                            {
                                                is_word = 0;
                                                break;

                                            }
                                        }
                                        if (is_word){
                                            std::cout << temp_word << std::endl;
                                            m_string = temp_word;
                                        }

                                }
                                */
                                std::cout << temp_word << std::endl;
                                m_string = "word tetected, time stamp is:" + QString::number(std::rand());

                    }
                        }


                        /*cout << fingers.size() << endl;
                        circle(imgOriginal, contours[largest_contour_index][fingers[0][0]],5,Scalar(0,255,0),-1);
                        circle(imgOriginal, contours[largest_contour_index][fingers[0][1]],5,Scalar(0,255,0),-1);
                        circle(imgOriginal, contours[largest_contour_index][fingers[0][2]],5,Scalar(0,255,0),-1);*/
                        bool click = false;
                        if(fingers.size() == 1){
                            //mouseTo(indexFinger.x, indexFinger.y);
                            //mouseRelease();
                        }
                        else if(fingers.size() == 2){
                            //(indexFinger.x, indexFinger.y);
                            //mouseClick();
                        }
                    }
                }
            }

            imshow("Thresholded Image", threshed); //show the thresholded image
            imshow("Original", imgOriginal); //show the original image

            if(waitKey(30) == 27){
                cout << "esc key is pressed" << endl;
                break;
            }

        }


}


