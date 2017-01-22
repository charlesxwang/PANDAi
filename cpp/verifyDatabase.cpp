#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tinyxml2/tinyxml2.h"
#include <iostream>
#include <typeinfo>
#include <string>

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

vector<Point> readFeaturePts(string & xmlFilePath)
{
    XMLDocument doc;
    cout << typeid(xmlFilePath).name() << endl;
    doc.LoadFile( xmlFilePath.c_str() );

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

    string fileName( argv[1]) ;
    string imgFilePath = "../database/HGR2A/original_images/" + fileName + ".jpg";
    string xmlFilePath = "../database/HGR2A/feature_points/" + fileName + ".xml";

    image = imread(imgFilePath.c_str(), CV_LOAD_IMAGE_COLOR);   // Read the img file


    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }


    vector<Point> points = readFeaturePts(xmlFilePath); // read feature points from xml file

    for(auto const& value: points) {
        drawFilledCircle( image, value ); // draw feature points on image
    }


    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show  image inside it.


    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;

}
