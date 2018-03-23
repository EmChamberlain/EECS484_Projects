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
#include "LeafNode.h"
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
    // this function might double count, in some circumstances
	assert(end >= begin);
	vector<DataEntry> result;

	// NOTE: void backInsert(vector<DataEntry>& dst, vector<DataEntry>&& src);
	
	for (unsigned i = 0; i < keys.size(); ++i) {
		if (begin < keys[i]) {
			backInsert(result, children[i]->rangeFind(begin, end));
		}
		if (begin <= keys[i] && keys[i] <= end && i == keys.size() - 1) {
			backInsert(result, children[i + 1]->rangeFind(begin, end));
		}
	}
    return result;
}


void InnerNode::updateKey(const TreeNode* rightDescendant, const Key& newKey) {
    // TO DO: implement this function
	/*  Not entirely sure what is meant by 'rightDescendant'...
	 *  if this were a BST, I would assume this means the right child,
	 *	but that is not the case for a B+ tree. Perhaps rightDescendant
	 *  is the right-most child?
	 */
	assert(rightDescendant != nullptr);
	for (unsigned i = 1; i < children.size(); ++i) {
		if (children[i]->contains(rightDescendant)) {
			keys[i-1] = newKey;
			return;
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
	Key comp = newEntry;
	auto lb = std::lower_bound(keys.begin(), keys.end(), comp);
	int index = std::distance(keys.begin(), lb);
	if (lb != keys.end() && *lb == comp)
		index++;
	children[index]->insertEntry(newEntry);
}

void InnerNode::deleteEntry(const DataEntry& entryToRemove) {
	Key comp = entryToRemove;
	auto lb = std::lower_bound(keys.begin(), keys.end(), comp);
	int index = std::distance(keys.begin(), lb);
	if (lb != keys.end() && *lb == comp)
		index++;
	children[index]->deleteEntry(entryToRemove);
}

void InnerNode::insertChild(TreeNode* newChild, const Key& key) {
	newChild->updateParent(this);
	if (this->full()) {
		//must split
		auto lb = std::lower_bound(keys.begin(), keys.end(), key);
		int index = std::distance(keys.begin(), lb);
		if (lb != keys.end() && *lb == key)
			index++;

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
		if (lb != keys.end() && *lb == key)
			index++;
		keys.insert(lb, key);
		children.insert(children.begin() + index, newChild);
		
	}
}
bool redistribute(vector<Key> &need, vector<Key> &has, Key deleted, Key parentKey, Key &erased) {

	
	if (has.size() > kInnerOrder)
	{
		if (deleted > *has.begin())
		{
			need.insert(need.begin(), parentKey);
			erased = *(has.end() - 1);
			has.erase(has.end() - 1);
		}
		else
		{
			need.insert(need.end(), parentKey);
			erased = *(has.begin());
			has.erase(has.begin());
		}

		return true;
	}
	return false;
}

vector<Key> merge(vector<Key> &left, vector<Key> &right) {
	auto l = left.begin();
	auto r = right.begin();

	auto le = left.end();
	auto re = right.end();

	std::vector<Key> result;
	result.reserve(left.size() + right.size());

	while (l != le && r != re) {
		if (*l < *r)
			result.push_back(*l++);
		else
			result.push_back(*r++);
	}

	while (l != le)
		result.push_back(*l++);

	while (r != re)
		result.push_back(*r++);

	return result;
}
vector<TreeNode*> mergeChildren(vector<TreeNode*> &left, vector<TreeNode*> &right) {
	auto l = left.begin();
	auto r = right.begin();

	auto le = left.end();
	auto re = right.end();

	std::vector<TreeNode*> result;
	result.reserve(left.size() + right.size());


	while (l != le)
		result.push_back(*l++);

	while (r != re)
		result.push_back(*r++);

	return result;
}


void InnerNode::deleteChild(TreeNode* childToRemove) {
    // TO DO: implement this function
	int childIndex = -1;
	for (unsigned i = 0; i < children.size(); ++i) {
		if (children[i] == childToRemove) {
			childIndex = i;
			break;
		}
	}
	assert(childIndex >= 0);
	//assert(!keys.empty());
	if (keys.empty())
		return;
	LeafNode* leaf = dynamic_cast<LeafNode*>(childToRemove);
	if (leaf)
		leaf->updateNeighborsDeletion();
	
	Key deleted;
	children.erase(children.begin() + childIndex);
	
	if (childIndex == 0)
	{
		deleted = *keys.begin();
		keys.erase(keys.begin());	
	}
	else
	{
		deleted = *(keys.begin() + (childIndex - 1));
		keys.erase(keys.begin() + (childIndex - 1));
	}
	


	if (keys.size() < kInnerOrder) {
		//need to redistribute or merge
		InnerNode *rightNeighbor = nullptr;
		InnerNode *leftNeighbor = nullptr;
		Key parentKeyLeft = 0;
		Key parentKeyRight = 0;
		if(getParent())
		{
			auto parentChildren = getParent()->children;
			for (unsigned int i = 0; i < parentChildren.size(); i++)
			{
				if (parentChildren[i] == this)
				{
					if (i != 0)
					{
						parentKeyLeft = getParent()->keys[i - 1];
						leftNeighbor = (InnerNode*)parentChildren[i - 1];
					}

					if (i != parentChildren.size() - 1)
					{
						parentKeyRight = getParent()->keys[i];
						rightNeighbor = (InnerNode*)parentChildren[i + 1];
					}

					break;
				}
			}
		}
		Key erased = 0;
		if (rightNeighbor && redistribute(keys, rightNeighbor->keys, deleted, parentKeyRight, erased))
		{
			auto beg = rightNeighbor->children.begin();
			(*beg)->updateParent(this);
			children.insert(children.end(), *beg);
			rightNeighbor->children.erase(beg);
			getParent()->updateKey(rightNeighbor, erased);
			
			return;
		}
		
		if (leftNeighbor && redistribute(keys, leftNeighbor->keys, deleted, parentKeyLeft, erased))
		{
			auto end = leftNeighbor->children.end() - 1;
			(*end)->updateParent(this);
			children.insert(children.begin(), *end);
			leftNeighbor->children.erase(end);
			getParent()->updateKey(this, erased);
			
			return;
		}
		if (rightNeighbor)
		{
			keys.push_back(parentKeyRight);
			vector<Key> mergedKeys = merge(keys, rightNeighbor->keys);
			vector<TreeNode*> mergedChildren = mergeChildren(children, rightNeighbor->children);
			for (TreeNode* child : mergedChildren)
			{
				child->updateParent(this);
			}
			keys = mergedKeys;
			//keys.insert(keys.begin(), parentKeyRight);
			children = mergedChildren;
			getParent()->deleteChild(rightNeighbor);
		}
		else if(leftNeighbor)
		{
			leftNeighbor->keys.push_back(parentKeyLeft);
			vector<Key> mergedKeys = merge(leftNeighbor->keys, keys);
			vector<TreeNode*> mergedChildren = mergeChildren(leftNeighbor->children, children);
			for (TreeNode* child : mergedChildren)
			{
				child->updateParent(leftNeighbor);
			}
			leftNeighbor->keys = mergedKeys;
			//leftNeighbor->keys.insert(leftNeighbor->keys.end(), parentKeyLeft);
			leftNeighbor->children = mergedChildren;
			getParent()->deleteChild(this);
		}
		else
		{
			int temp = 11;
		}
		

	}
	
	return;
}

bool InnerNode::full() const {
	return (int)keys.size() == kInnerOrder * 2;
}

void backInsert(vector<DataEntry>& dst, vector<DataEntry>&& src) {
	dst.reserve(dst.size() + src.size());
	for (unsigned i = 0; i < src.size(); ++i) {
		dst.emplace_back(src[i]);
	}
}


