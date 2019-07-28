//
//  pixelization.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
vector<bestImg> pixelization(double pixelsize, vector<bestImg> pixelsBase){
  //this function is for regulating the size of images from data base; store pixels
  //seaerch all images in the given dir and all of them should be regulated into square or rectangular shape
  //so the the montage can be in form of uniform pictures.
  Mat databaseSrcImg;
  String path = "/Users/apple/Desktop/imageDatabase/";
  DIR* directory;
  directory = opendir(path.c_str());
  String imageName;
  struct dirent* file;
  int number = 1;
  if (directory != NULL) {
    //pixelize pixels
    while ((file = readdir(directory)) != NULL) {
      if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 && strcmp(file->d_name, ".DS_Store")) {
        String imgPath(path + file->d_name);
        databaseSrcImg = imread(imgPath);
        Mat pixels = databaseSrcImg.clone();
        resize(databaseSrcImg, pixels, Size(pixelsize,pixelsize));
        string* nameVec = new string[3];
        nameVec = ucharToString(meanColour(pixels));
        string fileName = "/Users/apple/Desktop/pixels/"+ to_string(number) + "-R" + nameVec[2] + "|G" + nameVec[1] + "|B" + nameVec[0] + ".jpeg";
        imwrite(fileName, pixels);
        bestImg pixel;
        pixel.name = fileName;
        pixel.meanOffset = 0;
        pixel.index = number;
        pixelsBase.push_back(pixel);
        number++;
      }
    }
    closedir(directory);
  }
  else
    cout<<"Directory doesn't extst, please check you path configuration"<<endl;
  return pixelsBase;
}
