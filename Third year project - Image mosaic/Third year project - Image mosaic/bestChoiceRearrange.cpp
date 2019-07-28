//
//  bestChoiceRearrange.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/3/5.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"

using namespace cv;
using namespace std;

vector<bestImg> rearrange(vector<bestImg> bestChoice, Mat sourceImage, double pixelsize, int bigPixelSize){
  vector<bestImg> rBestChoice;
  bestImg newPixel;
  int noOfCols =  sourceImage.cols / pixelsize;
  int noOfRows = sourceImage.rows / pixelsize;
  int checked[noOfRows][noOfCols];
  bestImg tempArray[noOfRows][noOfCols];
  int x = 0;
  int y = 0;
  int count = 0;
  for (x = 0; x < noOfRows; x++) {
    for (y = 0; y < noOfCols; y++) {
      tempArray[x][y] = bestChoice[y + x*noOfCols];
      checked[x][y] = 0;
    }
  }
  for (x = 0; x < noOfRows; x++) {
    for (y = 0; y < noOfCols; y++) {
//////////////////////////////////////
/* the following is for dynamic pixelizing*/
      for (int i = bigPixelSize; i > 1; i--) {
        if (x < noOfRows - (i-1) && y < noOfCols - (i-1)){
          for (int m = 0; m < i; m++) {//x increment
            for (int n = 0; n < i; n++) {//y increment
              if (checked[x+m][y+n] == 0 && tempArray[x][y].index == tempArray[x+m][y+n].index) {
                count++;
              }
            }
          }
          if (count == i*i) {
            for (int m = 0; m < i; m++) {//x increment
              for (int n = 0; n < i; n++) {//y increment
                checked[x+m][y+n] = i*i;
              }
            }
            newPixel.name = tempArray[x][y].name;
            newPixel.index = tempArray[x][y].index;
            newPixel.meanOffset = i;
            newPixel.x = x;
            newPixel.y = y;
            rBestChoice.push_back(newPixel);
            count = 0;
          }
          else{
            count = 0;
          }
        }
      }
///////////////////////////////////////////////////////
    }
  }
  return rBestChoice;
}
