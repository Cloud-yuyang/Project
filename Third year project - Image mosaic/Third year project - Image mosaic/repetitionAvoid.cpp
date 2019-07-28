//
//  repetitionAvoid.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/3/19.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"

using namespace cv;
using namespace std;

bestImg repetitionAvoid(vector<bestImg> bestChoice, double pixelSize, Mat sourceImg){
  int noOfRows = sourceImg.rows/pixelSize;
  int noOfCols = sourceImg.cols/pixelSize;
  bestImg result;
  for (int i = 0; i < noOfCols; i++) {
    for (int j = 0;  j < noOfRows; j++) {
      
    }
  }
  return result;
}
