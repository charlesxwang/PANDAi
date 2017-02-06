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



namespace utilsCommon{
    
    
/*
 * glob file names in a folder
 */

vector<string> globVector(const string& pattern);



/*
 * get the size of image from corresponding xml file
 */

Size getImgSize(string & xmlFilePath);



/*
 * read feature points from a given xml file
 */

vector<Point> readAllFeaturePts(string & xmlFilePath);


/*
 * get the fingertip of index finger
 */

Point getIndexFingerTip(string & xmlFilePath);
    
}



