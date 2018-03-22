#include "InnerNode.h"                                  // for InnerNode
#include "TreeNode.h"                                   // file-specific header
#include "Utilities.h"                                  // for DataEntry
#include <cassert>                                      // for assert


// constructor
TreeNode::TreeNode(InnerNode* parent)
    : parent{ parent } {}

// destructor (no memory to deallocate)
TreeNode::~TreeNode() {}

// call generic insert (get derived class behavior), then check
// parent to return root
TreeNode* TreeNode::insertIntoRoot(const DataEntry& newEntry) {
    assert(!parent);
    
    insertEntry(newEntry);
    if (parent) {                       // nullptr is FALSE, means no parent (i.e. root)
        return parent;
    }
    return this;
}

// use maximum and sorted invariant
bool TreeNode::operator<(const Key& key) const {
    return (maxKey() < key);
}

// use minimum and sorted invariant
bool TreeNode::operator>=(const Key& key) const {
    return (minKey() >= key);
}

// change parent
void TreeNode::updateParent(InnerNode* newParent) {
    parent = newParent;
}

// return parent
InnerNode* TreeNode::getParent() {
    return parent;
}

// return parent
const InnerNode* TreeNode::getParent() const {
    return parent;
}

// recurse up tree until common ancestor found
InnerNode* TreeNode::getCommonAncestor(const TreeNode* relative) {
    assert(relative);
    assert(this != relative);

    if (parent == relative->parent) {
        return parent;
    }
    return parent->getCommonAncestor(relative->parent);
}

// recurse up tree until common ancestor found
const InnerNode* TreeNode::getCommonAncestor(const TreeNode* relative) const {
    assert(relative);
    assert(this != relative);

    if (parent == relative->parent) {
        return parent;
    }
    return parent->getCommonAncestor(relative->parent);
}

// siblings if parents are the same
bool TreeNode::isSibling(const TreeNode* potentialSibling) const {
    assert(potentialSibling);
    assert(this != potentialSibling);

    return (parent == potentialSibling->parent);
}

// no invariant for base class, just return true
bool TreeNode::satisfiesInvariant() const {
    return true;
}
