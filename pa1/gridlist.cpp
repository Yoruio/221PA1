// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

/*GridNode* northwest; // pointer to first (upper-left) node in the grid
GridNode* southeast; // pointer to last (lower-right) node in the grid
int dimx; // horizontal dimension of grid (in blocks)
int dimy; // vertical dimension of grid (in blocks)*/

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const
{
  int blockDimension = northwest->data.Dimension();
  int imageHeight = dimy*blockDimension;
  int imageWidth = dimx*blockDimension;

  cout << "image Height: " << imageHeight << endl;
  cout << "dimx: " << dimx << endl;
  cout << "image Width: " << imageWidth << endl;
  cout << "blockDimension: " << blockDimension << endl;

  PNG image(imageWidth, imageHeight);
  int x = 0;
  int y = 0;
  for(GridNode* currentNode = northwest; currentNode != NULL; currentNode = currentNode->next){
    cout << "rendering: (" << x*blockDimension << "," << y*blockDimension << ")" << endl;
    currentNode->data.Render(image, y*blockDimension, x*blockDimension);
    if (x+blockDimension >= imageWidth) {
      x = 0;
      y ++;
    }else{
      x ++;
    }
  }
  return image;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata)
{
  if (northwest == NULL){
    northwest = new GridNode(bdata);
    southeast = northwest;
  } else {
    southeast->next = new GridNode(bdata);
    southeast->next->prev = southeast;
    southeast = southeast->next;

  }
}

// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner)
{
  cout << "inner dimy: " << inner.dimy << endl;
  cout << "dimw: " << dimy << endl;
  if (inner.northwest == northwest || dimx < 2 || inner.dimx < 1 || inner.dimy != dimy || inner.northwest->data.Dimension() != northwest->data.Dimension()){
    return;
  }

  cout << "sandwich h" << endl;
  // enter your code here
  GridNode* thisCurrent = northwest;
  GridNode* innerCurrent = inner.northwest;
  int middle = dimx / 2;

  //for every row
  for(int y = 1; y <= dimy; y++){
    cout << "outer for loop" << endl;
    cout << "middle: " << middle << endl;

    //original left side
    for(int x = 2; x <= middle; x++){
      cout << "inside first loop: " << x << endl;
      thisCurrent = thisCurrent->next;
    }

    //new middle
    GridNode* nextNode = thisCurrent->next;
    thisCurrent->next = innerCurrent;
    thisCurrent->next->prev = thisCurrent;

    for(int x = 1; x <= inner.dimx; x ++){
    innerCurrent = innerCurrent->next;
    thisCurrent = thisCurrent->next;
    }

    //original right
    cout << "third for loop" << endl;
    nextNode->prev = thisCurrent;
    cout << "assigned prevNode" << endl;
    thisCurrent->next = nextNode;
    cout << "assigned nextNode" << endl;

    for(int x = middle; x <= dimx; x++){
      thisCurrent = thisCurrent->next;
    }
  }

  dimx += inner.dimx;

  inner.northwest = NULL;
  inner.southeast = NULL;
  inner.dimx = 0;
  inner.dimy = 0;

  
}

// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner)
{
  if (northwest == inner.northwest || dimy < 2 || inner.dimy < 1 || inner.dimx != dimx || inner.northwest->data.Dimension() != northwest->data.Dimension()){
    return;
  }

  GridNode* thisNode = northwest;
  int middle = dimy / 2;
  cout << "middle: " << middle << endl;
  for(int i = 0; i < middle; i++){
    for(int j = 0; j < dimx; j++){
      thisNode = thisNode->next;
      cout << "++" << endl;
    }
  }
  thisNode->prev->next = inner.northwest;
  inner.northwest->prev = thisNode->prev;

  inner.southeast->next = thisNode;
  thisNode->prev = inner.southeast;

  cout << "spliced" << endl;

  dimy += inner.dimy;

  inner.northwest = NULL;
  inner.southeast = NULL;
  inner.dimx = 0;
  inner.dimy = 0;
  // enter your code here
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  if (dimx != otherlist.dimx || dimy != otherlist.dimy || northwest->data.Dimension()!=otherlist.northwest->data.Dimension()){
    return;
  }
  bool firstSwapped = false;
  GridNode* currentNode = northwest;
  GridNode* otherNode = otherlist.northwest;
  while(currentNode != NULL){
    cout << "firstSwapped: " << firstSwapped << endl;
    bool currentSwapped = firstSwapped;
    for(int i = 0; i < dimx; i++){
      if (currentSwapped){
        GridNode *currentNext = currentNode->next;
        GridNode *currentPrev = currentNode->prev;
        
        currentNode->prev->next = otherNode;
        currentNode->prev = otherNode->prev;

        otherNode->prev->next = currentNode;
        otherNode->prev = currentPrev;

        currentNode->next = otherNode->next;
        currentNode->next->prev = currentNode;

        otherNode->next = currentNext;
        otherNode->next->prev = otherNode;
        

      }
      currentSwapped = !currentSwapped;
      currentNode = currentNode->next;
      otherNode = otherNode->next;
    }
    firstSwapped = !firstSwapped;
  }
  // enter your code here
}

// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN()
{
  // enter your code here
  bool firstBlack = false;
  GridNode* currentNode = northwest;
  while (currentNode != NULL){
    cout << "firstBlack: " << firstBlack << endl;
    bool currentBlack = firstBlack;
    for(int i = 0; i < dimx; i++){
      if (currentBlack) currentNode->data.Negative();
      currentBlack = !currentBlack;
      currentNode = currentNode->next;
    }
    firstBlack = !firstBlack;
  }

}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  while(northwest != southeast){
    GridNode* nextNode = northwest->next;
    delete northwest;
    northwest = nextNode;
    cout << "while" << endl;
  }
  delete northwest;
  northwest = NULL;
  southeast = NULL;
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  cout << "COPY" << endl;
  GridNode* current = otherlist.northwest;
  while(current != NULL){
    southeast->next = new GridNode(current->data);
    southeast->next->prev = southeast;
  }
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//