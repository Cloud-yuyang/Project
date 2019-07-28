//
//  imgReplace.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
void imgReplace(Mat inputImg, vector<bestImg> bestChoice, vector<bestImg> rBestChoice, vector<Mat> targetFragment, double overlayAlpha, double overlayOffset, double pixelsize, int mode){
  //locating each coordinate of image and use best choice set
  //to replace each roi with best choice
  Mat3b output(inputImg.rows,inputImg.cols);
  string outputName;
  int y = 0;
  int x = 0;
  double overlayBeta = 1.0 - overlayAlpha;
  int noOfCols = inputImg.cols/pixelsize;
  int noOfRows = inputImg.rows/pixelsize;
//  int checked[noOfRows][noOfCols];
  vector<unsigned long> rIndex(targetFragment.size());
//  int rIndexI = 0;
//  int reuseCount = 0;
  ////////////////////////////////
  //normal version
  cout<<"Replacing small pixels..."<<endl;
  for (x = 0; x < noOfRows; x++) {
    for (y = 0; y < noOfCols; y++) {
      Mat jigsaw = imread(bestChoice[y+x*noOfCols].name);
      if (mode == 2) {
        resize(jigsaw, jigsaw, Size(pixelsize,pixelsize));
      }
//      printf("%d/%lu\n",y+x*noOfCols+1,bestChoice.size());
      jigsaw.copyTo(output.rowRange(x*pixelsize, (x+1)*pixelsize).colRange(y*pixelsize, (y+1)*pixelsize));
      //this step is appending all result together
    }
  }
  /*
   this part is designed for dynamic reducing the size of images for replacing, which is for unequal size pixels
   if we are using equal size replacing, this would make replacing much slower.
   */
  if (mode == 2){
    cout<<"Replacing bigger pixels..."<<endl;
    for (int i = 0; i < rBestChoice.size(); i++) {
      Mat jigsaw = imread(rBestChoice[i].name);
      resize(jigsaw, jigsaw, Size(rBestChoice[i].meanOffset*pixelsize,rBestChoice[i].meanOffset*pixelsize));
//      printf("%d/%lu\n",i+1,rBestChoice.size());
      jigsaw.copyTo(output.rowRange(rBestChoice[i].x*pixelsize, (rBestChoice[i].x+rBestChoice[i].meanOffset)*pixelsize).colRange(rBestChoice[i].y*pixelsize, (rBestChoice[i].y+rBestChoice[i].meanOffset)*pixelsize));
      //this step is appending all result together
    }
  }
  ///////////////////////////////////
  addWeighted(output, overlayAlpha, inputImg, overlayBeta, overlayOffset, output);
  Vec3b inputdata = meanColour(inputImg);
  Vec3b outputdata = meanColour(output);
  cout<< "B: " << abs(outputdata[0] - inputdata[0]) <<endl;
  cout<< "G: " << abs(outputdata[1] - inputdata[1]) <<endl;
  cout<< "R: " << abs(outputdata[2] - inputdata[2]) <<endl;
  cout<< "Total: " << norm(outputdata,inputdata) <<endl;
  cout<<"Output name: ";
  cin >> outputName;
  while(!isdigit(outputName.find("."))){
    if (outputName.find(".") < 10000 && outputName.find(".") >= 0) {
      break;
    }
    cout<<"Input name invalid, please input with form: filename.extension"<<endl;
    cin >> outputName;
  }
  imwrite("/Users/apple/Desktop/output/" + outputName, output);
}


//the following trys to avoid redundant resizing // these algo are designed for unequal size pixelization
