#include "Cooker.h"
#include "utilsCV.h"


Cooker::Cooker()
{
    //this->imgFilePath = imgFilePath;
    //this->xmlFilePath = xmlFilePath;
};

Cooker::~Cooker()
{

};


tuple<Mat, float, float> Cooker::cook(string imgFilePath, Point indexTip, Size siz)
{
    Mat imgOriginal = imread(imgFilePath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);   // Read the img file
    Mat imgResized;
    Mat imgPadded;
    int longedge = max(imgOriginal.size().width, imgOriginal.size().height);
    int shortedge = min(imgOriginal.size().width, imgOriginal.size().height);
    //int pad = (longedge - shortedge) / 2 + (longedge - shortedge) % 2;

    float ratio = float(siz.width) / float(longedge);
    
    
    resize(imgOriginal,imgResized,Size(int(imgOriginal.size().width*ratio),int(imgOriginal.size().height*ratio)));//resize image

    int widthPad = siz.width - imgResized.size().width;
    int heightPad = siz.height - imgResized.size().height;
    copyMakeBorder( imgResized, imgPadded, heightPad/2, heightPad/2, widthPad/2, widthPad/2, BORDER_REPLICATE );
    
    resize(imgPadded,imgPadded,siz);//resize image
    
    

    float yRatio, xRatio;
    if (imgOriginal.size().width < longedge){//verticle img
        yRatio = float(indexTip.y) / float(imgOriginal.size().height);
        xRatio = (float(indexTip.x) + float(longedge - shortedge) / 2.0) / float(longedge);
    }else{// horizontal img
        xRatio = float(indexTip.x) / float(imgOriginal.size().width);
        yRatio = (float(indexTip.y) + float(longedge - shortedge) / 2.0) / float(longedge);
    }
  
    utilsCV::drawFilledCircle( imgPadded, Point(int(xRatio*siz.width), int(yRatio*siz.height)) ); // draw feature points on image

    
    //if (max(imgPadded.size().width , imgPadded.size().height)!=100 || imgPadded.size().width != imgPadded.size().height)
    //        cout << imgPadded.size().width << " , " << imgPadded.size().height << endl;

    return make_tuple(imgPadded, xRatio, yRatio);
};


