//
//  checkIfExist.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
bool checkIfExist(string path){
  //check if the file exists
  //ask user to re-input if the file doesn't exist.
  ifstream f(path.c_str());
  return f.good();
}
