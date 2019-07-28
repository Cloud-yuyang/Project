//
//  imgSort.cpp
//  Third year project - Image mosaic
//
//  Created by Cloud on 2019/2/15.
//  Copyright © 2019 Cloud. All rights reserved.
//

#include "struct.h"
using namespace cv;
using namespace std;
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
