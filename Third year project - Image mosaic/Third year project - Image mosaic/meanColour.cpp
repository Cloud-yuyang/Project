//
//  meanColour.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/15.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
Vec3b meanColour(const Mat& image){
  //this function is designed for calculating the mean value of the 3 channels colour
  //once the mean colour is calculated another interval need to be set
  //so the comparision between database images and each tile can be done hence achieve replacing
  Vec3b colorVec;
  unsigned long b = 0;
  unsigned long g = 0;
  unsigned long r = 0;
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      b += image.at<Vec3b>(i,j)[0];
      g += image.at<Vec3b>(i,j)[1];
      r += image.at<Vec3b>(i,j)[2];
    }
  }
  unsigned totalPixels = image.rows * image.cols;
  colorVec[0] = b/totalPixels;
  colorVec[1] = g/totalPixels;
  colorVec[2] = r/totalPixels;
  return colorVec;
}
