#include "BTree.h"                                      // file-specific header
#include "DataEntry.h"                                  // for DataEntry
#include "LeafNode.h"                                   // for LeafNode
#include "TreeNode.h"                                   // for TreeNode
#include "Utilities.h"                                  // for Key alias
#include <cassert>                                      // for assert
#include <iostream>                                     // for ostream
#include <vector>                                       // for vector

using std::vector;
using std::ostream;


// constructor; root begins as empty leaf node
BTree::BTree()
    : root{ new LeafNode{} }, height{ 0 }, size{ 0 } {}

// destructor
BTree::~BTree() {
    delete root;
}

// return height
size_t BTree::getHeight() const {
    return height;
}

// return number of entries
size_t BTree::getSize() const {
    return size;
}

void BTree::insertEntry(const DataEntry& newEntry) {
    // TO DO: implement this function
	if (root->contains(newEntry)) return;
	TreeNode* temp = root;
	root = root->insertIntoRoot(newEntry);
	if (temp != root) ++height;
	++size;
}

void BTree::deleteEntry(const DataEntry& entryToRemove) {
    // TO DO: implement this function
	--size;
	return root->deleteEntry(entryToRemove);
}

vector<DataEntry> BTree::rangeFind(const Key& begin, const Key& end) const {
    // TO DO: implement this function
	return root->rangeFind(begin, end);
}

// print tree
void BTree::print(ostream& os) const {
    os << kPrintPrefix << "Height = " << height << "  |  Size = " << size << "\n";
    os << kPrintPrefix << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    root->print(os);
}




void updateNeighbors()
{
	
}
