//
//  imgColourReduce.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
void imgColourReduce(const Mat& image,Mat& outImage,int div){
  //In opencv cvShowImage() shows RGB value in default
  //so we better use RBG and need to reduce the number of bit of color in the target image
  //to decrease the complexty of dealing with the huge amount of data
  //there would be a little distortion in the image but the efficiency would be much faster
  int newRows = image.rows;
  int newColumns = image.cols;
  outImage.create(image.size(), image.type());
  if(image.isContinuous() && outImage.isContinuous())
  {
    newRows = 1;
    newColumns = newColumns * image.rows * image.channels();
  }
  for(int i = 0; i < newRows; i++)
  {
    const uchar* inData = image.ptr<uchar>(i);
    uchar* outData = outImage.ptr<uchar>(i);
    for(int j = 0; j < newColumns; j++)
    {
      *outData++ = *inData++ / div * div + div/2;
    }
  }
}
