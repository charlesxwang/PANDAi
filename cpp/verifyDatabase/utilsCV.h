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

namespace utilsCV
{
/*
* draw a circle
*/
void drawFilledCircle( Mat img, Point center);

/*
* crop at the center of given img
*/

Mat cropCenter(Mat img);

/*
 * get handRect []
 */

vector<int> getHandRect(string & xmlFilePath);

/*
 * crop a part of image that contains hand
 */

Mat cropHand(string xmlPath, string imgPath);



};



