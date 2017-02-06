#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../tinyxml2/tinyxml2.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <algorithm>
#include <glob.h>
#include <tuple>
#include "utilsCommon.h"
#include "utilsCV.h"

#include "Cooker.h"


using namespace utilsCV;
using namespace utilsCommon;

using namespace cv;
using namespace tinyxml2;
using namespace std;



int main( int argc, char** argv )
{
    if( argc != 2)
    {
        cout <<" File title not given. " << endl;
        return -1;
    }

    string fileTitle( argv[1]) ;
    string imgFilePath = "../database/db1/images/" + fileTitle + ".jpg";
    string xmlFilePath = "../database/db1/points/" + fileTitle + ".xml";

    Mat image;
    image = imread(imgFilePath.c_str(), CV_LOAD_IMAGE_COLOR);   // Read the img file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    // read all feature points from xml file, and draw them on the image
    vector<Point> points = readAllFeaturePts(xmlFilePath); 
    for(auto const& point: points) {
        drawFilledCircle( image, point ); // draw feature points on image
    }
    
    // get the edges of the rect that contains hand
    vector<int> aHandRect = getHandRect(xmlFilePath);
    for(auto const& edge: aHandRect) {
        cout << "Rec edge: " << edge << endl;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show  image inside it.
/*
    Mat newimg;
    resize(image, newimg, Size(), 0.75, 0.75);

    namedWindow( "Display window1", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window1", newimg );                   // Show  image inside it.
*/
    
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
    Mat croped = cropCenter(image);
    namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window2", croped );                   // Show  image inside it.
    
    Mat img_resized;
    resize(image, img_resized, Size(200,200),0,0);
    namedWindow( "resized image", WINDOW_AUTOSIZE );// Create a window for display.
    imshow("resized image", img_resized);

    
    vector<string> feature_points_files = globVector("../database/db1/points/*.xml"); // get xml file names 
    for(auto const& feature_points_file: feature_points_files) {
        //cout << feature_points_file << endl;
        string xmlPath = feature_points_file;
        Size siz = getImgSize(xmlPath);
        //cout << siz.width << " , " << siz.height << endl;
        
        string fileName = feature_points_file.substr( feature_points_file.find_last_of("/") + 1);
        string imgTitle = fileName.substr(0, fileName.find_last_of("."));
        string imgPath = "../database/db1/images/" + imgTitle + ".jpg";
        //cout << imgPath << endl;
        vector<int> thisHandRect = getHandRect(xmlPath);
        //cropHand(xmlPath, imgPath);
    }
    
    namedWindow( "Gray", WINDOW_AUTOSIZE );// Create a window for grayscale.
    // check index finger tip labeled
    vector<string> feature_points_files_db = globVector("../database/db/points/*.xml"); // get xml file names 
    int fingerTipCount = 0;
    Mat inputImg;
    float xRatio,yRatio ;
    for(auto const& feature_points_file: feature_points_files_db) {
        string xmlPath = feature_points_file;
        string fileName = feature_points_file.substr( feature_points_file.find_last_of("/") + 1);
        string imgTitle = fileName.substr(0, fileName.find_last_of("."));
        string imgPath = "../database/db/images/" + imgTitle + ".jpg";
        
        Point indexTip = getIndexFingerTip(xmlPath);
        if (indexTip.x > 0.0)
        {
            
            
            if (true)
            {
            Cooker cooker;
            tie( inputImg,xRatio,yRatio )= cooker.cook( imgPath,  indexTip, Size(72,72));
            cout << xRatio << " @ " << yRatio << endl;
            if (fingerTipCount == 100)
            {
                imshow("Gray",inputImg);
                imwrite( "../../Gray_Image.jpg", inputImg );
            }
            //cout << imgPath << endl;
            }
            fingerTipCount++;
        }
    }
    
    cout << "There are " << fingerTipCount << " images with finger tip data." << endl;
 
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;

}
