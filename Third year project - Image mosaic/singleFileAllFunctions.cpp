//
//  main.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2018/10/8.
//  Copyright © 2018 Cloud. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>
#include <dirent.h>
//#include <string.h>
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
} bestImg;

typedef struct
{
  int index;
  double offset;
}imgIndex;

////////////////////////////////////////////////////////////////////////////////////////

//variables
vector<Vec3b> targetImgFragment;
vector<Vec3b> pixelImgMean;
vector<bestImg> pixelsBase;
double PixelSize = 64;
//============//
vector<Mat> fragmentsBase;
//============//

////////////////////////////////////////////////////////////////////////////////////////

//functions
void imgColourReduce(const Mat& image,Mat& outImage,int div);
void splitImage(Mat sourceImage);
void pixelization();
Vec3b meanColour(const Mat& image);
vector<bestImg> imgSort(vector<bestImg> bestChoice);
void initialize();
bool checkIfExist(string path);
string* ucharToString(Vec3b nameVec);
vector<bestImg> imgComparision(vector<Vec3b> targetFragment, vector<Vec3b> pixelMean);
void imgReplace(Mat inputImg, vector<bestImg> bestChoice, vector<Mat> targetFragment, double overlayAlpha, double overlayOffset);
Mat imgExtend(Mat outputImage);
Mat vec3bConvertToMat(Vec3b fragment);
//////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[]) {
  //main function to convert a normal target picture into a mosaic montage photo 'artwork'
  srand(time_t(0));
  initialize();
  char imageName[1024];
  bool ifExist = true;
  double overlayAlpha = 0.0;
  double overlayOffset = 0.0;
//  int pixelsize;
  cout<<"Input your file: ";
  cin >> imageName;
  ifExist = checkIfExist("/Users/apple/Desktop/" + string(imageName));
  while (!ifExist) {
    cout<<"File not found, please input again: ";
    cin >> imageName;
    ifExist = checkIfExist("/Users/apple/Desktop/" + string(imageName));
  }
  cout<<"What is your expected pixel size?(With smaller pixel size we have better rendering result) : ";
  cin >> PixelSize;
  while (PixelSize != floor(PixelSize)) {
    cout<<"Please input integer: ";
    cin >> PixelSize;
  }
  cout<<"Input the Alpha value(0.0 - 1.0) : ";
  cin >> overlayAlpha;
  while (overlayAlpha < 0.0 || overlayAlpha > 1.0) {
    cout<<"Please input between 0 and 1: ";
    cin >> overlayAlpha;
  }
  Mat inputImage = imread("/Users/apple/Desktop/" + string(imageName));
  Mat outputImage = inputImage.clone();
  outputImage = imgExtend(outputImage);
  //  Mat outputImage;
//  cout<<"Reducing colour..."<<endl;
//  imgColourReduce(inputImage, outputImage, 64);
  cout<<"Splitting target image..."<<endl;
  splitImage(outputImage);
  cout<<"Pixelizing image database..."<<endl;
  pixelization();
  cout<<"Calculating the best choice..."<<endl;
  vector<bestImg> bestChoice = imgComparision(targetImgFragment, pixelImgMean);
  cout<<"Replacing..."<<endl;
  imgReplace(outputImage, bestChoice, fragmentsBase, overlayAlpha, overlayOffset);
//  addWeighted(inputImage, overlayAlpha, outputImage, overlayBeta, 0.0, outputImage);
//  imwrite("/Users/apple/Desktop/outputImage-test.jpeg", outputImage);
//  namedWindow("Output", WINDOW_NORMAL);
//  imshow("Output", outputImage);
//  waitKey(0);
  cout<<"Done"<<endl;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

void imgColourReduce(const Mat& image,Mat& outImage,int div){
  //In opencv cvShowImage() shows RGB value in default
  //so we better use RBG and need to reduce the number of bit of color in the target image
  //to decrease the complexty of dealing with the huge amount of data
  //there would be a little distortion in the image but the efficiency would be much faster
  int newRows = image.rows;
  int newColumns = image.cols;
  outImage.create(image.size(), image.type());
  if(image.isContinuous() && outImage.isContinuous())
  {
    newRows = 1;
    newColumns = newColumns * image.rows * image.channels();
  }
  for(int i = 0; i < newRows; i++)
  {
    const uchar* inData = image.ptr<uchar>(i);
    uchar* outData = outImage.ptr<uchar>(i);
    for(int j = 0; j < newColumns; j++)
    {
      *outData++ = *inData++ / div * div + div/2;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void splitImage(Mat sourceImage) {
  //split image into several parts
  //so that each 'tile' can be compared with database images
  int commonFactor = PixelSize;//testing purpose, will be changed into configurable mode when most of the funtions are achieved
  int noOfRow = sourceImage.rows/commonFactor;
  int noOfColumn = sourceImage.cols/commonFactor;
  int index = 0;
  vector<int> nameIndex;
  vector<Mat> subImages;
  for (index = 0; index < noOfRow * noOfColumn; index++) {
    nameIndex.push_back(index);
  }
  Mat image_cut, roi_image;
  for (int i = 0; i < noOfRow; i++) {
    for (int j = 0; j < noOfColumn; j++) {
      Rect rect(j * commonFactor, i * commonFactor, commonFactor, commonFactor);
      image_cut = Mat(sourceImage, rect);
      roi_image = image_cut.clone();
      subImages.push_back(roi_image);
    }
  }
  for (index = 0; index < noOfColumn * noOfRow; index++) {
    string* nameVec = new string[3];
    nameVec = ucharToString(meanColour(subImages[index]));
    targetImgFragment.push_back(meanColour(subImages[index]));
    string fileName = "/Users/apple/Desktop/split/" + to_string(index + 1) + "-R" + nameVec[2] + "|G" + nameVec[1] + "|B" + nameVec[0] + ".jpeg";
    //imwrite(fileName, subImages[index]);
    //========================//
    fragmentsBase.push_back(subImages[index]);
    //=======================//
  }
}

/////////////////////////////////////////////////////////////////////////////////

void pixelization(){
  //this function is for regulating the size of images from data base;
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
    while ((file = readdir(directory)) != NULL) {
      if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 && strcmp(file->d_name, ".DS_Store")) {
        String imgPath(path + file->d_name);
        databaseSrcImg = imread(imgPath);
        Mat pixels = databaseSrcImg.clone();
        resize(databaseSrcImg, pixels, Size(PixelSize,PixelSize));
        string* nameVec = new string[3];
        nameVec = ucharToString(meanColour(pixels));
        pixelImgMean.push_back(meanColour(pixels));
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
  
}

/////////////////////////////////////////////////////////////////////////////////////////

Vec3b meanColour(const Mat& image){
  //this function is designed for calculating the mean value of the 3 channels colour
  //once the mean colour is calculated another interval need to be set
  //so the comparision between database images and each tile can be done hence achieve replacing
  Vec3b colorVec;
  unsigned long b = 0;
  unsigned long g = 0;
  unsigned long r = 0;
  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      b += image.at<Vec3b>(i,j)[0];
      g += image.at<Vec3b>(i,j)[1];
      r += image.at<Vec3b>(i,j)[2];
    }
  }
  unsigned totalPixels = image.rows * image.cols;
  colorVec[0] = b/totalPixels;
  colorVec[1] = g/totalPixels;
  colorVec[2] = r/totalPixels;
  return colorVec;
}

//////////////////////////////////////////////////////////////////////////////////

bool checkIfExist(string path){
  //check if the file exists
  //ask user to re-input if the file doesn't exist.
  ifstream f(path.c_str());
    return f.good();
}

//////////////////////////////////////////////////////////////////////////////////

string* ucharToString(Vec3b nameVec){
  //change the unsigned char into string so that it can be used to
  //print out the mean RGB value of each pixel
  string* colorVec = new string[3];
  colorVec[0] = to_string(nameVec[0]);
  colorVec[1] = to_string(nameVec[1]);
  colorVec[2] = to_string(nameVec[2]);
  return colorVec;
}

///////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////

vector<bestImg> imgComparision(vector<Vec3b> targetFragment, vector<Vec3b> pixelMean){
  //calculate the difference of each fragment and pixels
  //find out the one with smallest difference and return a set of choice
  vector<bestImg> choiceSet;
  vector<bool> repeated(targetFragment.size());
  int repeatRange = 3;
  bool done = false;
  printf("Fragment amount: %lu ===========\n", targetFragment.size());
  printf("Pixel amount: %lu ===========\n", pixelMean.size());
  for (int i = 0; i < targetFragment.size(); i++) {
    vector<bestImg> bestChoice;
    for (int j = 0; j < pixelMean.size(); j++) {
      bestImg sample;
      sample.meanOffset = norm(targetFragment[i], pixelMean[j]);
      sample.name = pixelsBase[j].name;
      bestChoice.push_back(sample);
    }
    bestChoice = imgSort(bestChoice);
    //////////////////////////////////////////////////
//    for (int k = 0; k < 4; k++) {
//      if (done == false) {
//        if (k != 3) {
//          if (repeated[bestChoice[k].index] == false) {
//            choiceSet.push_back(bestChoice[k]);
//            repeated[bestChoice[k].index] = true;
//            done = true;
//          }
//        }else{
//          choiceSet.push_back(bestChoice[3]);
//          for (int n = 0; n < 3; n++) {
//            repeated[bestChoice[n].index] = false;
//          }
//          done = true;
//        }
//      }else{
//        break;
//      }
//    }
//    done = false;
    ////////////////////////////////////////////
    if (repeated[bestChoice[0].index] == false) {
      choiceSet.push_back(bestChoice[0]);
      repeated[bestChoice[0].index] = true;
    }else if(repeated[bestChoice[1].index] == false){
      choiceSet.push_back(bestChoice[1]);
      repeated[bestChoice[1].index] = true;
    }else if(repeated[bestChoice[2].index] == false){
      choiceSet.push_back(bestChoice[2]);
      repeated[bestChoice[2].index] = true;
    }else{
      choiceSet.push_back(bestChoice[3]);
      repeated[bestChoice[0].index] = false;
      repeated[bestChoice[1].index] = false;
      repeated[bestChoice[2].index] = false;
    }
    //////////////////////////////////////////////
//    choiceSet.push_back(bestChoice[0]);
  }
  return choiceSet;
}

//////////////////////////////////////////////////////////////////////////////////////////

vector<bestImg> imgSort(vector<bestImg> bestChoice){
  //compare and sort offset from the choice set and return
  //in acsending order(0 - smallest, n - biggest)
  for (int i = 0; i < bestChoice.size() - 1; i++) {
    for (int j = 0; j < bestChoice.size() - i - 1; j++) {
      if (bestChoice[j].meanOffset > bestChoice[j+1].meanOffset) {
        bestImg temp = bestChoice[j];
        bestChoice[j] = bestChoice[j+1];
        bestChoice[j+1] = temp;
      }
    }
  }
  return bestChoice;
}

///////////////////////////////////////////////////////////////////////////////////////////

void imgReplace(Mat inputImg, vector<bestImg> bestChoice, vector<Mat> targetFragment, double overlayAlpha, double overlayOffset){
  //locating each coordinate of image and use best choice set
  //to replace each roi with best choice
  Mat3b output(inputImg.rows,inputImg.cols);
  int j = 0;
  int temp = 0;
  double overlayBeta = 1.0 - overlayAlpha;
  //printf("loooooool: %d\n", output.rows);
  for (int i = 0; i < bestChoice.size(); i++) {
    Mat jigsaw = imread(bestChoice[i].name);
    Mat tempImg = targetFragment[i];
    addWeighted(jigsaw, overlayAlpha, tempImg, overlayBeta, overlayOffset, jigsaw);
    if ((temp+1)*PixelSize > output.cols) {
      j += 1;
      temp = 0;
    }
    jigsaw.copyTo(output.rowRange(j*PixelSize, (j+1)*PixelSize).colRange(temp*PixelSize, (temp+1)*PixelSize));
    //this step is appending all result together
    temp += 1;
  }
  imwrite("/Users/apple/Desktop/final-test1.jpeg", output);
//  printf("/Users/apple/Desktop/final-" + to_string(rand()%10) + ".jpeg");
//  return output;
}

/////////////////////////////////////////////////////////////////////////////
Mat imgExtend(Mat outputImage){
  int rightFill = 0;
  if (outputImage.cols % (int)PixelSize != 0) {
    rightFill = PixelSize - (outputImage.cols % (int)PixelSize);
  }
  int bottomFill = 0;
  if (outputImage.rows % (int)PixelSize != 0){
    bottomFill = PixelSize - (outputImage.rows % (int)PixelSize);
  }
  copyMakeBorder(outputImage, outputImage, 0/*top*/, bottomFill/*bottom*/, 0/*left*/, rightFill/*right*/, BORDER_REPLICATE);
  return outputImage;
}
//////////////////////////////////////////////////////////////////////////////
