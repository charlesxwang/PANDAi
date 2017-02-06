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

using namespace cv;
using namespace tinyxml2;
using namespace std;

class Cooker
{
private:
   
    
/*
* cook image 
*/
public:
    Cooker();
    ~Cooker();
    tuple<Mat, float, float> cook(string imgFilePath, Point indexTip, Size siz);





};



