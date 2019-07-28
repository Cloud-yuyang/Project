//
//  ucharToString.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
string* ucharToString(Vec3b nameVec){
  //change the unsigned char into string so that it can be used to
  //print out the mean RGB value of each pixel
  string* colorVec = new string[3];
  colorVec[0] = to_string(nameVec[0]);
  colorVec[1] = to_string(nameVec[1]);
  colorVec[2] = to_string(nameVec[2]);
  return colorVec;
}
