#ifndef EECS484P3_LEAF_NODE_H
#define EECS484P3_LEAF_NODE_H

#include "DataEntry.h"                                          // for DataEntry
#include "TreeNode.h"                                           // for TreeNode (base class)
#include "Utilities.h"                                          // for size constants
#include <iosfwd>                                               // for ostream forward declaration
#include <vector>                                               // for vector

class InnerNode;                                                // only used as pointer or function argument


class LeafNode final : public TreeNode {
    public:
        // [Constructor]
        // REQUIRES: <parent> is not <this>
        explicit LeafNode(InnerNode* parent = nullptr);

        // [Copy/Move Constructors and Assignment Operators]
        // EFFECTS:  disables the copying or moving of LeafNodes
        LeafNode(const LeafNode& rhs) = delete;
        LeafNode(LeafNode&& rhs) = delete;
        LeafNode& operator=(const LeafNode& rhs) = delete;
        LeafNode& operator=(LeafNode&& rhs) = delete;

        // [Delete when Root Node]
        // REQUIRES: <this> LeafNode's parent is nullptr, <entryToRemove> is a
        //   data entry in <this> LeafNode
        // MODIFIES: <this>
        // EFFECTS:  removes <entryToRemove> from <this> LeafNode and returns
        //   <this>
        TreeNode* deleteFromRoot(const DataEntry& entryToRemove) override;

        // [Generic Insert]
        // REQUIRES: no data entry in <this> LeafNode has the same key as
        //   <newEntry>
        // MODIFIES: <this>
        // EFFECTS:  inserts <newEntry> into the appropriate location in <this>
        //   LeafNode, increasing the height of the BTree whose root is <this>
        //   if necessary
        void insertEntry(const DataEntry& newEntry) override;

        // [Generic Delete]
        // REQUIRES: <entryToRemove> is a data entry in <this> LeafNode
        // MODIFIES: <this>
        // EFFECTS:  removes <entryToRemove> from <this> LeafNode, decreasing the
        //   height of the BTree whose root is <this> if necessary
        void deleteEntry(const DataEntry& entryToRemove) override;

        // [Minimum/Maximum Accessors]
        // EFFECTS:  returns the minimum (or maximum) key of all data entries
        //   in <this> LeafNode
        Key minKey() const override;
        Key maxKey() const override;

        // [Containment Checker]
        // EFFECTS:  returns TRUE if and only if there is a data entry in <this>
        //   LeafNode whose key is <key> (key version); or if <this> LeafNode is
        //   <node> (node version)
        bool contains(const Key& key) const override;
        bool contains(const TreeNode* node) const override;

        // [Single-Value Finder]
        // REQUIRES: there is a data entry in <this> LeafNode whose key is <key>
        // EFFECTS:  returns the data entry in <this> LeafNode whose key is <key>
        const DataEntry& operator[](const Key& key) const override;

        // [Range Value Finder]
        // REQUIRES: <end> >= <begin>
        // EFFECTS:  returns a vector consisting of every data entry in <this>
        //   LeafNode whose key is in the range [<begin>, <end>] (both endpoints
        //   inclusive)
        std::vector<DataEntry> rangeFind(const Key& begin, const Key& end) const override;

        // [Printer]
        // REQUIRES: <indent> >= 0
        // MODIFIES: <os>
        // EFFECTS:  prints <this> TreeNode to <os>
        void print(std::ostream& os, int indent = 0) const override;

		//EFFECTS: returns if the node is full
		bool full() const;

		// REQUIRES: 
		// MODIFIES: <leftNeighbor> and <rightNeighbor>
		// EFFECTS:  unlinks <this> from the linked list
		void updateNeighborsDeletion();

		// REQUIRES: 
		// MODIFIES: <leftNeighbor> and <rightNeighbor>
		// EFFECTS:  unlinks <this> from the linked list
		void updateNeighborsInsertion();

    private:
        LeafNode* leftNeighbor;
        LeafNode* rightNeighbor;
        std::vector<DataEntry> entries;
};

#endif
