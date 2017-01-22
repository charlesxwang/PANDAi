#define _TINY_XML_TEST_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tinyxml2/tinyxml2.h"
#include <iostream>

using namespace cv;
using namespace tinyxml2;
using namespace std;

/* 
 * draw a circle
 */

void drawFilledCircle( Mat img, Point center )
{
    int thickness = -1;
    int lineType = 8;

    circle( img,
            center,
            img.size().width/100.0,
            Scalar( 0, 0, 255 ),
            thickness,
            lineType );
}


/*
 * read feature points from xml file
 */

vector<Point> readFeaturePts(char* xmlFileName)
{
    XMLDocument doc;
    doc.LoadFile( xmlFileName );

    int x = 1, y = 1;
    vector<Point> points;

    for(XMLElement* element = doc.FirstChildElement()->FirstChildElement()->FirstChildElement( "FeaturePoint" );
            element != 0; element = element->NextSiblingElement())
    {
        element->QueryIntAttribute( "x", &x );
        element->QueryIntAttribute( "y", &y );
        cout << "x: " << x  << endl;
        cout << "y: " << y  << endl;
        points.push_back(Point(x,y));
    }

    return points;
}

int main( int argc, char** argv )
{
    if( argc != 2)
    {
        cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }


    vector<Point> points = readFeaturePts((char*)"../database/HGR2A/feature_points/1_A_hgr2A1_id02_1.xml");
    for(auto const& value: points) {
        drawFilledCircle( image, value ); // draw feature points on image
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show  image inside it.


    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
