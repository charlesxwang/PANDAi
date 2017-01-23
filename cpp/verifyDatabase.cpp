#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "tinyxml2/tinyxml2.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <glob.h>



using namespace cv;
using namespace tinyxml2;
using namespace std;

vector<string> globVector(const string& pattern) {
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> files;
    for(unsigned int i=0; i<glob_result.gl_pathc; ++i) {
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}

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
 * get the size of image from corresponding xml file
 */

Point getImgSize(string & xmlFilePath)
{

    XMLDocument doc;
    doc.LoadFile( xmlFilePath.c_str() );

    XMLElement* element = doc.FirstChildElement();

    int width = 1;
    int height = 1;

    element->QueryIntAttribute( "WIDTH", &width );
    element->QueryIntAttribute( "HEIGHT", &height );

    return Point(width,height);
}


/*
 * crop at the center of given img
 */

Mat cropCenter(Mat img)
{
    int longedge = max(img.size().width, img.size().height);
    int shortedge = min(img.size().width, img.size().height);
    int x = 0;
    int y = 0;
    if (img.size().width < longedge){//verticle img
        x = 0;
        y = (longedge - shortedge) / 2;
    }else{
        y = 0;
        x = (longedge - shortedge) / 2;
    }
    Rect myROI(x, y, shortedge, shortedge);
    return img(myROI);
}





/*
 * read feature points from a given xml file
 */

vector<Point> readFeaturePts(string & xmlFilePath)
{
    XMLDocument doc;
    //cout << typeid(xmlFilePath).name() << endl; // print type
    doc.LoadFile( xmlFilePath.c_str() );

    int x = 1;
    int y = 1;
    vector<Point> points;

    for(XMLElement* element = doc.FirstChildElement()->FirstChildElement()->FirstChildElement( "FeaturePoint" );
            element != 0; element = element->NextSiblingElement())
    {
        element->QueryIntAttribute( "x", &x );
        element->QueryIntAttribute( "y", &y );
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

    vector<string> feature_points_files = globVector("../database/HGR1/feature_points/*.xml");
    for(auto const& feature_points_file: feature_points_files) {
        //cout << feature_points_file << endl;
        string xmlPath = feature_points_file;
        Point p = getImgSize(xmlPath);
        //cout << p.x << " , " << p.y << endl;
        /*
        string filename = feature_points_file.substr( feature_points_file.find_last_of("/") + 1);
        string fileTitle = filename.substr(0, filename.find_last_of("."));
        cout << fileTitle << endl;
        */

    }

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


    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;

}
