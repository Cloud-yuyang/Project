//
//  imgExtend.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;

Mat imgExtend(Mat outputImage, double pixelsize){
  int rightFill = 0;
  if (outputImage.cols % (int)pixelsize != 0) {
    rightFill = pixelsize - (outputImage.cols % (int)pixelsize);
  }
  int bottomFill = 0;
  if (outputImage.rows % (int)pixelsize != 0){
    bottomFill = pixelsize - (outputImage.rows % (int)pixelsize);
  }
  copyMakeBorder(outputImage, outputImage, bottomFill/2/*top*/, bottomFill - bottomFill/2/*bottom*/, rightFill/2/*left*/, rightFill - rightFill/2/*right*/, BORDER_REPLICATE);
  return outputImage;
}
