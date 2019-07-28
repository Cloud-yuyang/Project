//
//  struct.h
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/15.
//  Copyright © 2019 Cloud. All rights reserved.
//

#ifndef struct_h
#define struct_h

#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <stdio.h>
#include <ctime>

using namespace cv;
using namespace std;
//objects,structs
typedef struct
{
  string name;
  int index;
  double meanOffset;
  int x;
  int y;
  Vec3b bgr;
} bestImg;

typedef struct
{
  int index;
  double offset;
}imgIndex;

typedef struct{
  double x;
  double y;
} point;

void imgColourReduce(const Mat& image,Mat& outImage,int div);
vector<Mat> splitImage(Mat sourceImage, double pixelsize, vector<Mat> fragmentsBase);
vector<Vec3b> fragmentData(Mat sourceImage, double pixelsize, vector<Vec3b> targetImgFragment);
vector<bestImg> pixelization(double pixelsize, vector<bestImg> pixelsBase);
vector<Vec3b> pixelizationData(double pixelsize, vector<Vec3b> pixelImgMean);
Vec3b meanColour(const Mat& image);
vector<bestImg> imgSort(vector<bestImg> bestChoice);
void initialize();
bool checkIfExist(string path);
string* ucharToString(Vec3b nameVec);
vector<bestImg> imgComparision(vector<Vec3b> targetFragment, vector<Vec3b> pixelMean, vector<bestImg> pixelsBase, int mode, bool toggle);
void imgReplace(Mat inputImg, vector<bestImg> bestChoice, vector<bestImg> rBestChoice, vector<Mat> targetFragment, double overlayAlpha, double overlayOffset, double pixelsize, int mode);
Mat imgExtend(Mat outputImage, double pixelsize);
//Mat vec3bConvertToMat(Vec3b fragment);
//Mat otsuThreshold(Mat frame);
//vector<bestImg> uEP(double pixelsize, vector<bestImg> pixelsBase);
//vector<vector<point>> coordinateSet(Mat image);
vector<bestImg> rearrange(vector<bestImg> bestChoice, Mat sourceImage, double pixelsize, int bigPixelSize);

#endif /* struct_h */
