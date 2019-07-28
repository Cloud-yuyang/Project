//
//  main.cpp
//  Third year project - Image mosaic
//
//  Created by Yuyang on 2018/10/8.
//  Copyright © 2018 Yuyang. All rights reserved.
//

#include "struct.h"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
  //main function to convert a normal target picture into a mosaic montage photo 'artwork'
  initialize();
  char imageName[1024];
  bool ifExist = true;
  double overlayAlpha = 0.0;
  double overlayOffset = 0.0;
  double pixelsize = 0;
  string option1;
  string option2;
  vector<Vec3b> targetImgFragment;
  vector<Vec3b> pixelImgMean;
  vector<bestImg> pixelsBase;
  vector<Mat> fragmentsBase;
  vector<bestImg> rBestChoice;
  int mode = 0;
  int bigPixelSize = 0;
  int equal = 1;
  int unequal = 2;
  bool toggle = false;
  cout<<"Input your file: ";
  cin >> imageName;
  ifExist = checkIfExist("/Users/apple/Desktop/test/" + string(imageName));
  while (!ifExist) {
    cout<<"File not found, please input again: ";
    cin >> imageName;
    ifExist = checkIfExist("/Users/apple/Desktop/test/" + string(imageName));
  }
  cout<<"Please choose rendering mode: 1. Equal size pixelization" << endl;
  cout<<"                              2. Unequal size pixelization ";
  cin >> mode;
  while (mode != equal && mode != unequal) {
    cout<< "Input 1 or 2:" ;
    cin >> mode;
  }
  Mat inputImage = imread("/Users/apple/Desktop/test/" + string(imageName));
  Mat outputImage = inputImage.clone();
  cout<<"What is your expected pixel size?(With smaller pixel size we have better rendering result) : ";
  cin >> pixelsize;                    //input invalid
  while (pixelsize != floor(pixelsize) || cin.fail() || (cin.peek() != '\r' && cin.peek() != '\n')) {
    cout<<"Please input INTEGER: ";
    cin.clear();
    cin.ignore(1000, '\n');
    cin >> pixelsize;
  }
  if (mode == 2) {
    cout<<"Input the biggest pixels size"<<endl;
    cout<<"(this should be a square number which is the multiple of unit pixel): ";
    cin>> bigPixelSize;
    while (sqrt(bigPixelSize) != floor(sqrt(bigPixelSize)) || cin.fail() || (cin.peek() != '\r' && cin.peek() != '\n')) {
      cout<<"Please input a SQUARE number: ";
      cin.clear();
      cin.ignore(1000, '\n');
      cin >> bigPixelSize;
    }
    bigPixelSize = sqrt(bigPixelSize);
  }
  cout<<"Input the Alpha value(0.0 - 1.0) : ";
  cin >> overlayAlpha;
  while (overlayAlpha < 0.0 || overlayAlpha > 1.0 || cin.fail() || (cin.peek() != '\r' && cin.peek() != '\n')) {
    cout<<"Please input DOUBLE between 0 and 1: ";
    cin.clear();
    cin.ignore(1000, '\n');
    cin >> overlayAlpha;
  }
  outputImage = imgExtend(outputImage, pixelsize);
  cout<<"Do you want to reduce colour? Y: yes, N: no"<<endl;
  cin >> option1;
  while (option1.compare("Y") != 0 && option1.compare("N") != 0) {
    cout<<"Input Y or N:"<<endl;
    cin >> option1;
  }
  if(mode == 1){
    cout<<"Do you want to avoid some repetition? Y: yes, N: no"<<endl;
    cin >> option2;
    while (option2.compare("Y") != 0 && option2.compare("N") != 0) {
      cout<<"Input Y or N:"<<endl;
      cin >> option2;
    }
    if (option2.compare("Y") == 0) {
      toggle = true;
    }
  }
  clock_t start, end;
  double cpu_time_used;
  start = clock();
  if (option1.compare("Y") == 0) {
    cout<<"Reducing colour..."<<endl;
    imgColourReduce(inputImage, outputImage, 64);
  }
  cout<<"Splitting target image..."<<endl;
  fragmentsBase = splitImage(outputImage, pixelsize, fragmentsBase);
  cout<<"Storing target image data..."<<endl;
  targetImgFragment = fragmentData(outputImage, pixelsize, targetImgFragment);
  cout<<"Pixelizing image database..."<<endl;
  if (mode == 1) {
    pixelsBase = pixelization(pixelsize, pixelsBase);
  }
  else if (mode == 2){
    pixelsBase = pixelization(sqrt(bigPixelSize)*pixelsize, pixelsBase);
  }
  cout<<"Storing pixels data..."<<endl;
  pixelImgMean = pixelizationData(pixelsize, pixelImgMean);
  cout<<"Calculating the best choice..."<<endl;
  vector<bestImg> bestChoice = imgComparision(targetImgFragment, pixelImgMean, pixelsBase, mode, toggle);
  if (mode == 2) {
    cout<<"Calculating the bigger pixels..."<<endl;
    rBestChoice = rearrange(bestChoice, outputImage, pixelsize, bigPixelSize);
  }
  cout<<"Replacing..."<<endl;
  imgReplace(outputImage, bestChoice, rBestChoice, fragmentsBase, overlayAlpha, overlayOffset, pixelsize, mode);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("time costed: %f\n", cpu_time_used);
  cout<<"Done"<<endl;
  return 0;
}
