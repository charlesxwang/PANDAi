#include "utilsCV.h"


using namespace cv;
using namespace tinyxml2;
using namespace std;

namespace utilsCV{

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
 * crop a part of image that contains hand 
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


}

