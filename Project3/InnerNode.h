#ifndef EECS484P3_INNER_NODE_H
#define EECS484P3_INNER_NODE_H

#include "DataEntry.h"                                          // for DataEntry
#include "TreeNode.h"                                           // for TreeNode (base class)
#include "Utilities.h"                                          // for size constants
#include <iosfwd>                                               // for ostream forward declaration


class InnerNode final : public TreeNode {
    public:
        // [Value Constructor]
        // REQUIRES: neither <child1> nor <child2> is nullptr, the maximum key
        //   in <child1> is strictly less than <key>, the minimum key in <child2>
        //   is greater than or equal to <key>, <parent> is not <this>
        // EFFECTS:  transfers the ownership of <child1> and <child2> to <this>
        InnerNode(TreeNode* child1, const Key& key, TreeNode* child2, InnerNode* parent = nullptr);

		InnerNode(std::vector<TreeNode*> &children, std::vector<Key> &keys, InnerNode* parent = nullptr);
        
		// [Destructor]
        // MODIFIES: the children of <this> InnerNode, memory pool
        // EFFECTS:  deallocates all memory associated with each child of <this>
        //   InnerNode
        ~InnerNode() override;

        // [Copy/Move Constructors and Assignment Operators]
        // EFFECTS:  disables the copying or moving of InnerNodes
        InnerNode(const InnerNode& rhs) = delete;
        InnerNode(InnerNode&& rhs) = delete;
        InnerNode& operator=(const InnerNode& rhs) = delete;
        InnerNode& operator=(InnerNode&& rhs) = delete;


        // [Delete when Root Node]
        // REQUIRES: <this> InnerNode's parent is nullptr, <entryToRemove> is a
        //   data entry in one of <this> InnerNode's descendants
        // MODIFIES: <this>, the InnerNodes in the same BTree as <this>
        // EFFECTS:  removes <entryToRemove> from the descendant of <this>
        //   InnerNode where it is located, decreasing the height of the BTree
        //   whose root is <this> if necessary; after the removal is complete,
        //   returns the root of that BTree, which may have changed due to
        //   height decrease
        TreeNode* deleteFromRoot(const DataEntry& entryToRemove) override;

        // [Generic Insert]
        // REQUIRES: no data entry in any of <this> InnerNode's descendants has
        //   the same key as <newEntry>
        // MODIFIES: <this>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  inserts <newEntry> into the appropriate location in one of
        //   <this> InnerNode's children, increasing the height of the BTree whose
        //   root is <this> if necessary
        void insertEntry(const DataEntry& newEntry) override;

        // [Generic Delete]
        // REQUIRES: <entryToRemove> is a data entry in one of <this> InnerNode's
        //   descendants
        // MODIFIES: <this>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  removes <entryToRemove> from the descendant of <this>
        //   InnerNode where it is located, decreasing the height of the BTree
        //   whose root is <this> if necessary
        void deleteEntry(const DataEntry& entryToRemove) override;
        
        // [Child Adder]
        // REQUIRES: <newChild> is not nullptr, the minimum key of <newChild> is
        //   greater than or equal to <key>, the keys of <newChild> or the
        //   descendants of <newChild> are unique among the keys of the descendants
        //   of <this> InnerNode
        // MODIFIES: <this>, <newChild>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  inserts <child> in the appropriate location of <this> InnerNode
        //   as the right child of a key with value <key>, increasing the height of
        //   the BTree containing <this> InnerNode if necessary; ownership of
        //   <newChild> is transferred
        void insertChild(TreeNode* newChild, const Key& key);

        // [Child Deleter]
        // REQUIRES: <childToRemove> is not nullptr, <childToRemove> is a child of
        //   <this> InnerNode
        // MODIFIES: <this>, <childToRemove>, memory pool, the TreeNodes in the
        //   same BTree as <this>
        // EFFECTS:  removes <childToRemove> from the children of <this> InnerNode
        //   and deallocates the memory associated with that child, decreasing the
        //   height of the BTree containing <this> InnerNode if necessary
        void deleteChild(TreeNode* childToRemove);

        // [Minimum/Maximum Accessors]
        // EFFECTS:  returns the minimum (or maximum) key of all data entries
        //   in <this> InnerNode's descendants
        Key minKey() const override;
		Key maxKey() const override;

        // [Containment Checker]
        // EFFECTS:  returns TRUE if and only if there is a data entry in one of 
        //   <this> InnerNode's descendants whose key is <key> (key version); or
        //   if <this> InnerNode or any of <this> InnerNode's descendants is
        //   <node> (node version)
        bool contains(const Key& key) const override;
        bool contains(const TreeNode* node) const override;

        // [Single-Value Finder]
        // REQUIRES: there is a data entry in one of <this> InnerNode's descendants
        //   whose key is <key>
        // EFFECTS:  returns the data entry in one of <this> InnerNode's descendants
        //   whose key is <key>
        const DataEntry& operator[](const Key& key) const override;

        // [Range Value Finder]
        // REQUIRES: <end> >= <begin>
        // EFFECTS:  returns a vector consisting of every data entry in all of <this>
        //   InnerNode's descendants whose key is in the range [<begin>, <end>] (both
        //   endpoints inclusive)
        std::vector<DataEntry> rangeFind(const Key& begin, const Key& end) const override;

        // [Printer]
        // REQUIRES: <indent> >= 0
        // MODIFIES: <os>
        // EFFECTS:  prints <this> TreeNode to <os>
        void print(std::ostream& os, int indent = 0) const override;

        // [Key Updater]
        // REQUIRES: <rightDescendant> is not nullptr
        // MODIFIES: <this>
        // EFFECTS:  changes the key in <this> InnerNode whose right subtree would
        //   contain <rightDescendant> to be the value of <newKey>
        void updateKey(const TreeNode* rightDescendant, const Key& newKey);

		bool full() const;

    private:
        std::vector<Key> keys;
        std::vector<TreeNode*> children;


};

void backInsert(std::vector<DataEntry>& vec1, std::vector<DataEntry>&& vec2);

std::vector<DataEntry> merge(std::vector<DataEntry> &left, std::vector<DataEntry> &right);

#endif
