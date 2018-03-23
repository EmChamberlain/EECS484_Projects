#include "DataEntry.h"                                  // for DataEntry
#include "InnerNode.h"                                  // for InnerNode
#include "LeafNode.h"                                   // file-specific header
#include "TreeNode.h"                                   // for TreeNode
#include "Utilities.h"                                  // for size constants, print prefix, Key alias
#include <algorithm>                                    // for find
#include <cassert>                                      // for assert
#include <iostream>                                     // for ostream
#include <numeric>                                      // for numeric_limits
#include <string>                                       // for string
#include <vector>                                       // for vector

using std::find;
using std::vector;
using std::numeric_limits;
using std::ostream;
using std::string;


// constructor
LeafNode::LeafNode(InnerNode* parent)
    : TreeNode{ parent }, entries{}
{
	leftNeighbor = nullptr;
	rightNeighbor = nullptr;
}

// print keys of data entries surrounded by curly braces, ending
// newline
void LeafNode::print(ostream& os, int indent) const {
    assert(indent >= 0);

    os << kPrintPrefix << string(indent, ' ') << "{ ";
    for (const auto& entry : entries) {
        if (entry != entries[0]) {
            os << " | ";
        }
        os << entry;
    }
    os << " }\n";

    assert(satisfiesInvariant());
}

// data entries are sorted; minimum is first entry's key
Key LeafNode::minKey() const {
    if (entries.empty()) {
        return numeric_limits<Key>::min();
    }
    return entries.front();
}

// data entries are sorted; maximum is last entry's key
Key LeafNode::maxKey() const {
    if (entries.empty()) {
        return numeric_limits<Key>::max();
    }
    return entries.back();
}

// TRUE if key is the key of any entry
bool LeafNode::contains(const Key& key) const {
    return (find(entries.cbegin(), entries.cend(), key) != entries.cend());
}

// TRUE if this node is the target
bool LeafNode::contains(const TreeNode* node) const {
    return (this == node);
}

// return the data entry with given key
const DataEntry& LeafNode::operator[](const Key& key) const {
    assert(contains(key));

    return *find(entries.cbegin(), entries.cend(), key);
}

vector<DataEntry> LeafNode::rangeFind(const Key& begin, const Key& end) const {
	vector<DataEntry> result;
	result.reserve(entries.size());
	for (unsigned i = 0; i < entries.size(); ++i) {
		if (entries[i] >= begin) {
			if (entries[i] <= end)
				result.push_back(entries[i]);
			else break;
		}
	}
    return result;
}



// use generic delete; height can't decrease
TreeNode* LeafNode::deleteFromRoot(const DataEntry& entryToRemove) {
    assert(contains(entryToRemove));
    assert(!getParent());

    deleteEntry(entryToRemove);
    assert(satisfiesInvariant());
    return this;
}

void LeafNode::insertEntry(const DataEntry& newEntry) {
	assert(!contains(newEntry));
	if (this->full()) {
		//must split
		auto lb = std::lower_bound(entries.begin(), entries.end(), newEntry);
		//int index = std::distance(entries.begin(), lb);

		
		int threshold = kLeafOrder;
		
		LeafNode * newSibling = new LeafNode{};

		newSibling->leftNeighbor = this;
		newSibling->rightNeighbor = this->rightNeighbor;
		if(this->rightNeighbor)
			this->rightNeighbor->leftNeighbor = newSibling;
		this->rightNeighbor = newSibling;

		entries.insert(lb, newEntry);
		DataEntry temp = entries[threshold];

		for (int i = 0; i < (int)entries.size() - threshold; ++i) {
			newSibling->insertEntry(entries[threshold + i]);
		}
		entries.erase(entries.begin() + threshold, entries.end());
		
		

		if (this->getParent()) {
			InnerNode* parent = this->getParent();
			parent->insertChild(newSibling, temp);
		}
		else {
			InnerNode* newRoot = new InnerNode(this, temp, newSibling);
			if (newRoot)
			{
				return;
			}
			
		}
	}
	else {
		//normal insert
		auto lb = std::lower_bound(entries.begin(), entries.end(), newEntry);
		entries.insert(lb, newEntry);
	}
}
bool redistribute(vector<DataEntry> &need, vector<DataEntry> &has, DataEntry deleted) {
	

	if (has.size() > kLeafOrder)
	{
		if(deleted > *has.begin())
		{
			need.insert(need.begin(), *(has.end() - 1));
			has.erase(has.end() - 1);
		}
		else
		{
			need.insert(need.end(), *(has.begin()));
			has.erase(has.begin());
		}
		
		return true;
	}
	return false;
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

	while (l != le)
		result.push_back(*l++);

	while (r != re)
		result.push_back(*r++);

	return result;
}

void LeafNode::deleteEntry(const DataEntry& entryToRemove) {
    // TO DO: implement this function
	if (entries.empty())
		return;

	auto lb = std::lower_bound(entries.begin(), entries.end(), entryToRemove);
	if (lb == entries.end())
		return;
	DataEntry deleted = *lb;
	if (deleted != entryToRemove)
		return;
	entries.erase(lb);

	if (entries.size() < kLeafOrder) {
		//need to redistribute or merge
		
		if (rightNeighbor && redistribute(entries, rightNeighbor->entries, deleted))
		{
			getCommonAncestor(rightNeighbor)->updateKey(rightNeighbor, (rightNeighbor->entries[0]));
			return;
		}
			
		if (leftNeighbor && redistribute(entries, leftNeighbor->entries, deleted))
		{
			getCommonAncestor(leftNeighbor)->updateKey(this, entries[0]);
			return;
		}
		if (rightNeighbor)
		{
			vector<DataEntry> merged = merge(entries, rightNeighbor->entries);
			entries = merged;
			Key minkey = (rightNeighbor->getParent())->minKey();
			getCommonAncestor(rightNeighbor)->updateKey(rightNeighbor, minkey);
			rightNeighbor->getParent()->deleteChild(rightNeighbor);
			
		}
		else if (leftNeighbor)
		{
			vector<DataEntry> merged = merge(entries, leftNeighbor->entries);
			leftNeighbor->entries = merged;
			Key minkey = minKey();
			getCommonAncestor(leftNeighbor)->updateKey(this, minkey);
			leftNeighbor->getParent()->deleteChild(this);
			
		}
		else
		{
			return;
		}
		

	}
	
}



bool LeafNode::full() const {
	return (int)entries.size() == kLeafOrder * 2;
}

void LeafNode::updateNeighborsDeletion()
{
	if (leftNeighbor)
		leftNeighbor->rightNeighbor = rightNeighbor;
	if (rightNeighbor)
		rightNeighbor->leftNeighbor = leftNeighbor;
}