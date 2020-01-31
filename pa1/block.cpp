#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "block.h"
#include <vector>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"

using namespace std;
using namespace cs221util;

class Block {

public:

  // set values in data attribute, based on a dimension x dimension square region
  //   with upper-left corner at (left, upper) in the input PNG image
  // The orientation of the pixels in the data vector must match the orientation
  //   of the pixels in the PNG.
  // PRE: upper and left (and upper + dimension, left + dimension) are valid
  //        vector indices
  void Build(PNG& im, int upper, int left, int dimension){    //upper left (left, upper)
    for(int x = 0; x < dimension; x++){
      for(int y = 0; y < dimension; y++){
        data[x][y] = *im.getPixel(left + x,upper - y);
      }
    }
  }

  // write the pixel colour data fom data attribute into im,
  //   with upper-left corner at (left, upper)
  // PRE: upper and left (and upper + dimension, left + dimension) are valid
  //        vector indices
  void Render(PNG& im, int upper, int left) const{
      for(int x = 0; (unsigned) x < data.size(); x++){
        for(int y = 0; (unsigned) y < data.size(); y++){
          *im.getPixel(left + x, upper - y) = data[x][y];
        }
      }
  }

  // "Reverse" the Hue and Luminance channels for each pixel in the data attribute
  //   to simulate a photo-negative effect.
  // Refer to the HSLAPixel documentation to determine an appropriate transformation
  //   for "reversing" hue and luminance.
  void Negative(){
    int dimension = Dimension(); 
    for(int x = 0; x < dimension; x++){
        for(int y = 0; y < dimension; y++){
          HSLAPixel* pixel = &data[x][y]; 
          double sat = pixel->s; 
          pixel->s = pixel->h; 
          pixel->h = sat; 
        }
    }
  }

  // Return the horizontal (or vertical) size of the data block's image region
  int  Dimension() const{
    return data.size(); 
  }

private:
  // 2D container for pixel data
  // Be aware that a newly declared vector has a size of 0
  vector<vector<HSLAPixel>> data; //column(row())

};
#endif