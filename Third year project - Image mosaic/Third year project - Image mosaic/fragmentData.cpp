//
//  fragmentData.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
vector<Vec3b> fragmentData(Mat sourceImage, double pixelsize, vector<Vec3b> targetImgFragment) {
  //split image into several parts, store tile datas
  //so that each 'tile' can be compared with database images
  int commonFactor = pixelsize;//testing purpose, will be changed into configurable mode when most of the funtions are achieved
  int noOfRow = sourceImage.rows/commonFactor;
  int noOfColumn = sourceImage.cols/commonFactor;
  int index = 0;
  vector<int> nameIndex;
  vector<Mat> subImages;
  for (index = 0; index < noOfRow * noOfColumn; index++) {
    nameIndex.push_back(index);
  }
  Mat image_cut, roi_image;
  for (int i = 0; i < noOfRow; i++) {
    for (int j = 0; j < noOfColumn; j++) {
      Rect rect(j * commonFactor, i * commonFactor, commonFactor, commonFactor);
      image_cut = Mat(sourceImage, rect);
      roi_image = image_cut.clone();
      subImages.push_back(roi_image);
    }
  }
  for (index = 0; index < noOfColumn * noOfRow; index++) {
//    string* nameVec = new string[3];
//    nameVec = ucharToString(meanColour(subImages[index]));
    targetImgFragment.push_back(meanColour(subImages[index]));
  }
  return targetImgFragment;
}
