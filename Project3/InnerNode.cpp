#include "DataEntry.h"                                  // for DataEntry
#include "InnerNode.h"                                  // file-specific header
#include "TreeNode.h"                                   // for TreeNode
#include "Utilities.h"                                  // for size constants, print prefix, Key alias
#include <algorithm>                                    // for any_of
#include <cassert>                                      // for assert
#include <iostream>                                     // for ostream
#include <string>                                       // for string
#include <vector>										// forvector
///////////////////////////////////////////////////////////////
#include <iterator>/*Added by me, not sure if allowed yet*/////
////////////////////*Included to support move semantic insert*/
///////////////////////////////////////////////////////////////


using std::any_of;
using std::vector; using std::string;
using std::ostream;
/////////////////////////////////////////////////////////
using std::back_inserter;/*included in iterator library*/
/////////////////////////////////////////////////////////



// value constructor
InnerNode::InnerNode(TreeNode* child1, const Key& key, TreeNode* child2, InnerNode* parent) 
    : TreeNode{ parent }, keys{ key }, children{ child1, child2 } {

    assert(child1 && child2);
    assert(*child1 < key && *child2 >= key);

    child1->updateParent(this);
    child2->updateParent(this);
}

InnerNode::InnerNode(vector<TreeNode*> &_children, vector<Key> &_keys, InnerNode* parent)
	: TreeNode{ parent }, keys{ std::move(_keys) }, children{ std::move(_children) } {
	for (auto & child : children) {
		child->updateParent(this);
	}
}

// deallocate all children
InnerNode::~InnerNode() {
    for (auto child : children) {
        delete child;
    }
}

// print keys, then each node on its own line
void InnerNode::print(ostream& os, int indent) const {
    assert(indent >= 0);

    os << kPrintPrefix << string(indent, ' ') << "[ ";
    for (const auto& key : keys) {
        if (key != keys[0]) {
            os << " | ";
        }
        os << key;
    }
    os << " ]\n";

    for (const auto child : children) {
        child->print(os, indent + kIndentIncr);
    }

    assert(satisfiesInvariant());
}

// ask first child, which must exist
Key InnerNode::minKey() const {
    return children.front()->minKey();
}

// ask last child, which must exist
Key InnerNode::maxKey() const {
    return children.back()->maxKey();
}

// ask the child where the data entry with that key would be
bool InnerNode::contains(const Key& key) const {
    return (
        any_of(children.cbegin(), children.cend(), [key](auto n)->bool { return n->contains(key); }));
}

// ask children if they contain
bool InnerNode::contains(const TreeNode* node) const {
    return ((this == node) ||
        any_of(children.cbegin(), children.cend(), [node](auto n)->bool { return n->contains(node); }));
}

// ask the child where the data entry with that key is
const DataEntry& InnerNode::operator[](const Key& key) const {
    assert(contains(key));

    for (const auto child : children) {
        if (child->contains(key)) {
            return child->operator[](key);
        }
    }
    assert(false);
}

vector<DataEntry> InnerNode::rangeFind(const Key& begin, const Key& end) const {
    // TO DO: implement this function
	assert(end >= begin);
	vector<DataEntry> result;
	for (unsigned i = 0; i < keys.size(); ++i) {
		if (begin < keys[i])
			moveInsert(children[i]->rangeFind(begin, end), result);
		if (begin <= keys[i] && keys[i] <= end)
			moveInsert(children[i + 1]->rangeFind(begin, end), result);
	}
    return result;
}

void InnerNode::updateKey(const TreeNode* rightDescendant, const Key& newKey) {
    // TO DO: implement this function
	assert(rightDescendant != nullptr);
	for (unsigned i = 0; i < children.size(); ++i) {
		if (this->contains(rightDescendant)) {
			keys[i] = newKey;
			break;
		}
	}
}

// use generic delete, then look at number of children to determine
// if height decreased
TreeNode* InnerNode::deleteFromRoot(const DataEntry& entryToRemove) {
    assert(!getParent());
    assert(contains(entryToRemove));

    deleteEntry(entryToRemove);
    if (children.size() == 1) {                 // one child means height has shrunk
        auto newRoot = children.front();
        children.clear();                       // clear children so not deallocated later
        assert(satisfiesInvariant());
        return newRoot;
    }
    assert(satisfiesInvariant());
    return this;
}

void InnerNode::insertEntry(const DataEntry& newEntry) {
	// TO DO: implement this function
	Key comp = newEntry;
	auto lb = std::lower_bound(keys.begin(), keys.end(), comp);
	int index = std::distance(keys.begin(), lb);
	/*if (index >= children.size()) {
		children.resize(index + 1);
		//LeafNode* newLeaf = new LeafNode{};
	}*/
	children[index]->insertEntry(newEntry);
}

void InnerNode::deleteEntry(const DataEntry& entryToRemove) {
    // TO DO: implement this function
	Key comp = entryToRemove;
	auto lb = std::lower_bound(keys.begin(), keys.end(), comp);
	int index = std::distance(keys.begin(), lb);
	children[index]->deleteEntry(entryToRemove);
}

void InnerNode::insertChild(TreeNode* newChild, const Key& key) {
    // TO DO: implement this function
	newChild->updateParent(this);
	if (this->full()) {
		//must split
		auto lb = std::lower_bound(keys.begin(), keys.end(), key);
		int index = std::distance(keys.begin(), lb);

		//threshold marks the lower bound of the halfway point
		int threshold = kInnerOrder;

		//inserts new key in order
		keys.insert(lb, key);
		children.insert(children.begin() + index + 1, newChild);
		Key temp = keys[threshold];

		vector<Key> newKeys;
		newKeys.reserve(kInnerOrder * 2);
		vector<TreeNode*> newChildren;
		newChildren.reserve(kInnerOrder * 2 + 1);

		//This loop moves greater half of keys and children into temps for insertion into newSibling 
		for (int i = 1; i < (int)keys.size() - threshold; ++i) {
			newKeys.emplace_back(std::move(keys[threshold + i]));
			newChildren.emplace_back(std::move(children[threshold + i]));
		}
		newChildren.emplace_back(std::move(children[children.size() - 1]));
		//deletes keys and children that were just moved
		keys.erase(keys.begin() + threshold, keys.end());
		children.erase(children.begin() + threshold + 1, children.end());

		if (getParent()) {
			//node has parent
			InnerNode* newSibling = new InnerNode(newChildren, newKeys, getParent());
			getParent()->insertChild(newSibling, temp);
		}
		else {
			//new root must be created
			InnerNode* newSibling = new InnerNode(newChildren, newKeys);
			InnerNode* newRoot = new InnerNode(this, temp, newSibling);
			newSibling->updateParent(newRoot);
			updateParent(newRoot);
		}
	}
	else {
		//keys.reserve(kInnerOrder * 2);
		//children.reserve(kInnerOrder * 2 + 1);
		auto lb = std::lower_bound(keys.begin(), keys.end(), key);
		int index = std::distance(keys.begin(), lb) + 1;
		keys.insert(lb, key);
		children.insert(children.begin() + index, newChild);
	}
}

void InnerNode::deleteChild(TreeNode* childToRemove) {
    // TO DO: implement this function
	return;
}

bool InnerNode::full() const {
	return (int)keys.size() == kInnerOrder * 2;
}

void moveInsert(vector<DataEntry>& vec1, vector<DataEntry>& vec2) {
	if (vec2.empty())
		vec2 = vec1;
	else {
		vec2 = merge(vec1, vec2);
	}
}

vector<DataEntry> merge(vector<DataEntry> &left, vector<DataEntry> &right) {
	auto l = left.begin();
	auto r = right.begin();

	auto le = left.end();
	auto re = right.end();

	std::vector<DataEntry> result;
	result.reserve(left.size() + right.size());

	while (l != le && r != re) {
		if (*l < *r)
			result.push_back(*l++);
		else
			result.push_back(*r++);
	}

	// copy rest of left array
	while (l != le)
		result.push_back(*l++);

	// copy rest of right array
	while (r != re)
		result.push_back(*r++);

	return result;
}