//
//  Otsu.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/3/4.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"

using namespace cv;
using namespace std;

Mat otsuThreshold(Mat frame)
{
  const int GrayScale = 256;
  int width = frame.cols;
  int height = frame.rows;
  int pixelCount[GrayScale];
  float pixelPro[GrayScale];
  int i, j, pixelSum = width * height, threshold = 0;
  uchar* data = frame.data;
  for (i = 0; i < GrayScale; i++)
  {
    pixelCount[i] = 0;
    pixelPro[i] = 0;
  }
  //sum up the total number of pixels in different gray scale level
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width; j++)
    {
      pixelCount[(int)data[i * width + j]]++;
    }
  }
  //calculate the ratio of each pixel to the whole image
  float maxPro = 0.0;
  int kk = 0;
  for (i = 0; i < GrayScale; i++)
  {
    pixelPro[i] = (float)pixelCount[i] / pixelSum;
    if (pixelPro[i] > maxPro)
    {
      maxPro = pixelPro[i];
      kk = i;
    }
  }
  
  //traverse different grayscale level from 0-255, i is the threshold
  float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
  for (i = 0; i < GrayScale; i++)
  {
    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {
      if (j <= i)   //background
      {
        w0 += pixelPro[j];
        u0tmp += j * pixelPro[j];
      }
      else        //foreground
      {
        w1 += pixelPro[j];
        u1tmp += j * pixelPro[j];
      }
    }
    u0 = u0tmp / w0;
    u1 = u1tmp / w1;
    u = u0tmp + u1tmp;
    deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
    if (deltaTmp > deltaMax)
    {
      deltaMax = deltaTmp;
      threshold = i;
    }
  }
  
  for (int i = 0; i < frame.rows; i++) {
    for (int j = 0; j < frame.cols; j++) {
      if (frame.at<uchar>(i, j) < threshold) {
        frame.at<uchar>(i, j) = 0;//1.black bg
//        frame.at<uchar>(i, j) = 255;//2.white bg
      }else{
        frame.at<uchar>(i, j) = 255;//1.white fg
//        frame.at<uchar>(i, j) = 0;//2.black fg
      }
    }
  }
  return frame;
}
//---------------------
//author：Naruto_Qing
//source：CSDN
//reference：https://blog.csdn.net/piaoxuezhong/article/details/78302893
