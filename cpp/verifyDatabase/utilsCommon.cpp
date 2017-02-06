#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../tinyxml2/tinyxml2.h"
#include <iostream>
#include <typeinfo>
#include <string>
#include <algorithm>
#include <glob.h>

#include "utilsCommon.h"

using namespace cv;
using namespace tinyxml2;
using namespace std;



namespace utilsCommon{
    
    
/*
 * glob file names in a folder
 */

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
 * get the size of image from corresponding xml file
 */

Size getImgSize(string & xmlFilePath)
{

    XMLDocument doc;
    doc.LoadFile( xmlFilePath.c_str() );

    XMLElement* element = doc.FirstChildElement();

    int width = 1;
    int height = 1;

    element->QueryIntAttribute( "WIDTH", &width );
    element->QueryIntAttribute( "HEIGHT", &height );

    return Size(width,height);
}






/*
 * read feature points from a given xml file
 */

vector<Point> readAllFeaturePts(string & xmlFilePath)
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

/*
 * get the fingertip of index finger
 */

Point getIndexFingerTip(string & xmlFilePath)
{
    XMLDocument doc;
    //cout << typeid(xmlFilePath).name() << endl; // print type
    doc.LoadFile( xmlFilePath.c_str() );

    int x = 0;
    int y = 0;
    string category;
    string featureType;

    for(XMLElement* element = doc.FirstChildElement()->FirstChildElement()->FirstChildElement( "FeaturePoint" );
            element != 0; element = element->NextSiblingElement())
    {
        
        category = element->Attribute("category");
        featureType = element->Attribute("type");
        
        if (category == "IndexFinger" && featureType == "Fingertip") 
        {
            element->QueryIntAttribute( "x", &x );
            element->QueryIntAttribute( "y", &y );
            break;
        }

    }

    return Point(x,y);
}
    
}


