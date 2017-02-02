#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../tinyxml2/tinyxml2.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <algorithm>
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
 * get handRect []
 */

vector<int> getHandRect(string & xmlFilePath)
{
    XMLDocument doc;
    //cout << typeid(xmlFilePath).name() << endl; // print type
    doc.LoadFile( xmlFilePath.c_str() );

    int x = 1;
    int y = 1;
    vector<int> xVector;
    vector<int> yVector;

    for(XMLElement* element = doc.FirstChildElement()->FirstChildElement()->FirstChildElement( "FeaturePoint" );
            element != 0; element = element->NextSiblingElement())
    {
        element->QueryIntAttribute( "x", &x );
        element->QueryIntAttribute( "y", &y );
        xVector.push_back(x);
        yVector.push_back(y);
    }

    auto minx = min_element(xVector.begin(), xVector.end());
    auto maxx = max_element(xVector.begin(), xVector.end());
    auto miny = min_element(yVector.begin(), yVector.end());
    auto maxy = max_element(yVector.begin(), yVector.end());
    vector<int> handRect = {*minx, *maxx, *miny, *maxy};
    return handRect;
}

/*
 * crop at the center of given img
 */

Mat cropHand(string xmlPath, string imgPath)
{
    Mat img = imread(imgPath.c_str(), CV_LOAD_IMAGE_COLOR);   // Read the img file
    vector<int> handRect = getHandRect(xmlPath); // [minx maxx miny maxy]
    int handPadding = 5;
    handRect[0] -= handPadding;
    handRect[1] += handPadding;
    handRect[2] -= handPadding;
    handRect[3] += handPadding;
    
    
    int longedge = max(img.size().width, img.size().height);
    int shortedge = min(img.size().width, img.size().height);
    int x = 0;
    int y = 0;
    if (img.size().width < longedge){//verticle img
        x = 0;
        y = (longedge - shortedge) / 2;
        if (y > handRect[2] || (y+shortedge < handRect[3]))
        {
            //cout << imgPath << " y" << endl;
            if ( handRect[3] -  handRect[2] > shortedge)
                cout << handRect[2] << " " << handRect[3] << " y-: " << y << " y+: " << y+shortedge << endl;
        }
    }else{// horizontal img
        y = 0;
        x = (longedge - shortedge) / 2;
        if (x > handRect[0] || x+shortedge < handRect[1])
        {
            //cout << imgPath << " x" << endl;
            if ( handRect[1] -  handRect[0] > shortedge)
                cout << handRect[0] << " " << handRect[1] << " x-: " << x << " x+: " << x+shortedge << endl;
        }
            
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

    
    vector<string> feature_points_files = globVector("../database/db1/points/*.xml"); // get xml file names 
    for(auto const& feature_points_file: feature_points_files) {
        //cout << feature_points_file << endl;
        string xmlPath = feature_points_file;
        Point p = getImgSize(xmlPath);
        //cout << p.x << " , " << p.y << endl;
        
        string fileName = feature_points_file.substr( feature_points_file.find_last_of("/") + 1);
        string imgTitle = fileName.substr(0, fileName.find_last_of("."));
        string imgPath = "../database/db1/images/" + imgTitle + ".jpg";
        //cout << imgPath << endl;
        vector<int> thisHandRect = getHandRect(xmlPath);
        cropHand(xmlPath, imgPath);
    }

    string fileName( argv[1]) ;
    string imgFilePath = "../database/db2A/images/" + fileName + ".jpg";
    string xmlFilePath = "../database/db2A/points/" + fileName + ".xml";

    image = imread(imgFilePath.c_str(), CV_LOAD_IMAGE_COLOR);   // Read the img file


    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }



    vector<Point> points = readFeaturePts(xmlFilePath); // read feature points from xml file
    for(auto const& point: points) {
        drawFilledCircle( image, point ); // draw feature points on image
    }
    
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


    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;

}
