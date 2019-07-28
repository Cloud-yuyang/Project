//
//  imgComparision.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/18.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;

vector<bestImg> imgComparision(vector<Vec3b> targetFragment, vector<Vec3b> pixelMean, vector<bestImg> pixelsBase, int mode, bool toggle){
  //calculate the difference of each fragment and pixels
  //find out the one with smallest difference and return a set of choice
  vector<bestImg> choiceSet;
  vector<bool> repeated(targetFragment.size());
//  Vec3b colourDifference[targetFragment.size()];
//  int repeatRange = 3;
//  bool done = false;
  for (int i = 0; i < targetFragment.size(); i++) {
    vector<bestImg> bestChoice;
    for (int j = 0; j < pixelMean.size(); j++) {
      bestImg sample;
      sample.meanOffset = norm(targetFragment[i], pixelMean[j]);
      sample.name = pixelsBase[j].name;
      sample.index = pixelsBase[j].index;
      sample.bgr = pixelMean[j];
      bestChoice.push_back(sample);
    }
    bestChoice = imgSort(bestChoice);
    ////////////////////////////////////////////
    if (mode == 1 && toggle == true) {
    //interval of 1
      if (repeated[bestChoice[0].index] == false) {
        choiceSet.push_back(bestChoice[0]);
        repeated[bestChoice[0].index] = true;
      }else{
        choiceSet.push_back(bestChoice[1]);
        repeated[bestChoice[0].index] = false;
      }
//      choiceSet[0] = repetitionAvoid(bestChoice);
    }
    ////////////////////////////////////////////
    else{
      //no repetition avoid
      choiceSet.push_back(bestChoice[0]);
      
    }
  }

//  int hist_w = 512; int hist_h = 400;int histSize = 256;
//  int bin_w = cvRound( (double) hist_w/histSize );
//  Mat b_hist;
//  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 255,255,255) );
//  line(histImage, Point(0,0), Point(0,hist_h), Scalar(0,0,0),2,8,0);
//  line(histImage, Point(0,hist_h/2), Point(hist_w,hist_h/2), Scalar(0,0,0),2,8,0);
//  for (int i = 1; i < targetFragment.size(); i++) {
////    colourDifference[i] =  choiceSet[i].meanOffset;
//    cout<<"No." << i << ": " << choiceSet[i-1].meanOffset <<endl;
//    line( histImage, Point( bin_w*(i-1), (hist_h - abs(targetFragment[i-1][0] - choiceSet[i-1].bgr[0]))/2 ) ,
//         Point( bin_w*(i), (hist_h - abs(targetFragment[i][0] - choiceSet[i].bgr[0]))/2 ),
//         Scalar( 255, 0, 0), 1, 8, 0  );
//    line( histImage, Point( bin_w*(i-1), (hist_h - abs(targetFragment[i-1][1] - choiceSet[i-1].bgr[1]))/2 ) ,
//         Point( bin_w*(i), (hist_h - abs(targetFragment[i][1] - choiceSet[i].bgr[1]))/2 ),
//         Scalar( 0, 255, 0), 1, 8, 0  );
//    line( histImage, Point( bin_w*(i-1), (hist_h - abs(targetFragment[i-1][2] - choiceSet[i-1].bgr[2]))/2 ) ,
//         Point( bin_w*(i), (hist_h - abs(targetFragment[i][2] - choiceSet[i].bgr[2]))/2 ),
//         Scalar( 0, 0, 255), 1, 8, 0  );
//  }
//  imwrite("/Users/apple/Desktop/histogram32.jpg", histImage);
  return choiceSet;
}
//(15.02.2019)at the mid-end stage of developing, repetition avoid method is added
//and after testing the result with different intervals, it seems like 0 interval and 1 interval would be better
//because more intervals may lead to lower precision
//////////////////////////////////////////////////
//here is the dynamic loop operation for different numbers of intervals, not working and not needed anymore
//due to the above reasons
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
//////////////////////////////////////////////
//interval of 2
//    if (repeated[bestChoice[0].index] == false) {
//      choiceSet.push_back(bestChoice[0]);
//      repeated[bestChoice[0].index] = true;
//    }else if(repeated[bestChoice[1].index] == false){
//      choiceSet.push_back(bestChoice[1]);
//      repeated[bestChoice[1].index] = true;
//    }else{
//      choiceSet.push_back(bestChoice[2]);
//      repeated[bestChoice[0].index] = false;
//    }
////////////////////////////////////////////
//interval of 3
//    if (repeated[bestChoice[0].index] == false) {
//      choiceSet.push_back(bestChoice[0]);
//      repeated[bestChoice[0].index] = true;
//    }else if(repeated[bestChoice[1].index] == false){
//      choiceSet.push_back(bestChoice[1]);
//      repeated[bestChoice[1].index] = true;
//    }else if(repeated[bestChoice[2].index] == false){
//      choiceSet.push_back(bestChoice[2]);
//      repeated[bestChoice[2].index] = true;
//    }else{
//      choiceSet.push_back(bestChoice[3]);
//      repeated[bestChoice[0].index] = false;
//    }
//////////////////////////////////////////////
