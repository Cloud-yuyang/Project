//
//  initialize.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;

void initialize(){
  //initialize file directory which make sure if the directory is clean
  //to avoid files redundancy
  string path = "/Users/apple/Desktop/pixels/";
  DIR* directory;
  directory = opendir(path.c_str());
  if (directory != NULL) {
    system("exec rm -r /Users/apple/Desktop/pixels/*");
    cout<<"All files are cleaned"<<endl;
  }
  path = "/Users/apple/Desktop/split/";
  directory = opendir(path.c_str());
  if (directory != NULL) {
    system("exec rm -r /Users/apple/Desktop/split/*");
    cout<<"All files are cleaned"<<endl;
  }
}
