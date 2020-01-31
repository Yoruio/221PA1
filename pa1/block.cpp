#include "block.h"
#include <vector>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"

using namespace std;
using namespace cs221util;


// 2D container for pixel data
// Be aware that a newly declared vector has a size of 0
vector<vector<HSLAPixel>> data; //column(row())


// set values in data attribute, based on a dimension x dimension square region
//   with upper-left corner at (left, upper) in the input PNG image
// The orientation of the pixels in the data vector must match the orientation
//   of the pixels in the PNG.
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Build(PNG& im, int upper, int left, int dimension){    //upper left (left, upper)
  cout << "starting block.build" << endl;
  for(int x = 0; x < dimension; x++){
    data.push_back(vector<HSLAPixel>());
    for(int y = 0; y < dimension; y++){
      cout << "inner for loop" << endl;
      HSLAPixel *pixel = im.getPixel(left + x,upper - y);
      cout << "HSLAPIXEL ASSIGNED" << endl;
      cout << pixel->h << endl;
      cout << pixel->s << endl;
      cout << pixel->l << endl;
      cout << pixel->a << endl;
      data[x].push_back(*pixel);
      cout << "data assigned" << endl;
    }
  }
  cout << "finished block.build" << endl;
}

// write the pixel colour data fom data attribute into im,
//   with upper-left corner at (left, upper)
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Render(PNG& im, int upper, int left) const{
  cout << "starting block.render" << endl;
  for(int x = 0; (unsigned) x < data.size(); x++){
    for(int y = 0; (unsigned) y < data.size(); y++){
      *im.getPixel(left + x, upper - y) = data[x][y];
    }
  }
  cout << "finished block.render" << endl;
}

// "Reverse" the Hue and Luminance channels for each pixel in the data attribute
//   to simulate a photo-negative effect.
// Refer to the HSLAPixel documentation to determine an appropriate transformation
//   for "reversing" hue and luminance.
void Block::Negative(){
  cout << "starting block.negative" << endl;
  int dimension = Dimension(); 
  for(int x = 0; x < dimension; x++){
    for(int y = 0; y < dimension; y++){
      HSLAPixel* pixel = &data[x][y]; 
      double sat = pixel->s; 
      pixel->s = pixel->h; 
      pixel->h = sat; 
    }
  }
  cout << "finished block.negative" << endl;
}

// Return the horizontal (or vertical) size of the data block's image region
int Block::Dimension() const{
  cout << "dimensions" << endl;
  return data.size(); 
}