//
//  pixelizationData.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
vector<Vec3b> pixelizationData(double pixelsize, vector<Vec3b> pixelImgMean){
  //this function is for regulating the size of images from data base; store data
  //seaerch all images in the given dir and all of them should be regulated into square or rectangular shape
  //so the the montage can be in form of uniform pictures.
  Mat databaseSrcImg;
  String path = "/Users/apple/Desktop/imageDatabase/";
  DIR* directory;
  directory = opendir(path.c_str());
  String imageName;
  struct dirent* file;
  if (directory != NULL) {
    while ((file = readdir(directory)) != NULL) {
      if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 && strcmp(file->d_name, ".DS_Store")) {
        String imgPath(path + file->d_name);
        databaseSrcImg = imread(imgPath);
        pixelImgMean.push_back(meanColour(databaseSrcImg));
      }
    }
    closedir(directory);
  }
  else
    cout<<"Directory doesn't extst, please check you path configuration"<<endl;
  return pixelImgMean;
}
