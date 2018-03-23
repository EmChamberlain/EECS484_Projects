#ifndef EECS484P3_TREE_NODE_H
#define EECS484P3_TREE_NODE_H

#include "DataEntry.h"                                          // for DataEntry (template parameter)
#include "Utilities.h"                                          // for Key primitive alias
#include <iosfwd>                                               // for ostream forward declaration
#include <vector>                                               // for vector (forward declaration is difficult)

//Added by me
//#define BRANCHING_FACTOR 7

class InnerNode;                                                // only used as pointer or function argument


class TreeNode {
    public:
        // [Constructor]
        // REQUIRES: <parent> is not <this>
        explicit TreeNode(InnerNode* parent = nullptr);

        // [Destructor]
        // MODIFIES: memory pool
        // EFFECTS:  virtually destroys all memory managed by <this> TreeNode
        virtual ~TreeNode();

        // [Insert when Root Node]
        // REQUIRES: <this> TreeNode's parent is nullptr, no data entry in
        //   <this> TreeNode or any of <this> TreeNode's descendants has the
        //   same key as <newEntry>
        // MODIFIES: <this>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  inserts <newEntry> into the appropriate location in <this>
        //   TreeNode or one of <this> TreeNode's children, increasing the
        //   height of the BTree whose root is <this> if necessary; after the
        //   insertion is completed, returns the root of that BTree, which
        //   may have changed due to height increase
        TreeNode* insertIntoRoot(const DataEntry& newEntry);

        // [Delete when Root Node]
        // REQUIRES: <this> TreeNode's parent is nullptr, <entryToRemove> is a
        //   data entry in <this> TreeNode or one of <this> TreeNode's
        //   descendants
        // MODIFIES: <this>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  removes <entryToRemove> from <this> TreeNode or one of
        //   <this> TreeNode's children, decreasing the height of the BTree
        //   whose root is <this> if necessary; after the removal is complete,
        //   returns the root of that BTree, which may have changed due to
        //   height decrease
        virtual TreeNode* deleteFromRoot(const DataEntry& entryToRemove) = 0;

        // [Generic Insert]
        // REQUIRES: no data entry in <this> TreeNode or any of <this> TreeNode's
        //   descendants has the same key as <newEntry>
        // MODIFIES: <this>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  inserts <newEntry> into the appropriate location in <this>
        //   TreeNode or one of <this> TreeNode's children, increasing the
        //   height of the BTree whose root is <this> if necessary
        virtual void insertEntry(const DataEntry& newEntry) = 0;

        // [Generic Delete]
        // REQUIRES: <entryToRemove> is a data entry in <this> TreeNode or one
        //   of <this> TreeNode's descendants
        // MODIFIES: <this>, the TreeNodes in the same BTree as <this>
        // EFFECTS:  removes <entryToRemove> from <this> TreeNode or one of
        //   <this> TreeNode's children, decreasing the height of the BTree
        //   whose root is <this> if necessary
        virtual void deleteEntry(const DataEntry& entryToRemove) = 0;

        // [Comparators]
        // EFFECTS:  returns TRUE if and only if all data entries in <this>
        //   TreeNode or all of <this> TreeNode's descendants have keys that
        //   are less than (or greater than or equal to) <key>
        bool operator<(const Key& key) const;
        bool operator>=(const Key& key) const;

        // [Minimum/Maximum Accessors]
        // EFFECTS:  returns the minimum (or maximum) key of all data entries
        //   in <this> TreeNode or any of <this> TreeNode's descendants; returns
        //   the minimum (or maximum) possible key if <this> TreeNode is empty
        virtual Key minKey() const = 0;
        virtual Key maxKey() const = 0;

        // [Containment Checker]
        // EFFECTS:  returns TRUE if and only if there is a data entry in <this>
        //   TreeNode or one of <this> TreeNode's descendants whose key is <key>
        //   (key version); or if <this> TreeNode or one of <this> TreeNode's
        //   descendants is <node> (node version)
        virtual bool contains(const Key& key) const = 0;
        virtual bool contains(const TreeNode* node) const = 0;

        // [Single-Value Finder]
        // REQUIRES: there is a data entry in <this> TreeNode or one of <this>
        //   TreeNode's descendants whose key is <key>
        // EFFECTS:  returns the data entry in <this> TreeNode or one of <this>
        //   TreeNode's descendants whose key is <key>
        virtual const DataEntry& operator[](const Key& key) const = 0;

        // [Range Value Finder]
        // REQUIRES: <end> >= <begin>
        // EFFECTS:  returns a vector consisting of every data entry in <this>
        //   TreeNode or <this> TreeNode's descendants whose key is in the range
        //   [<begin>, <end>] (both endpoints inclusive)
        virtual std::vector<DataEntry> rangeFind(const Key& begin, const Key& end) const = 0;




        // [Printer]
        // REQUIRES: <indent> >= 0
        // MODIFIES: <os>
        // EFFECTS:  prints <this> TreeNode to <os>
        virtual void print(std::ostream& os, int indent = 0) const = 0;


        // [Parent Modifier]
        // EFFECTS:  updates the parent of <this> TreeNode to be <newParent>
        void updateParent(InnerNode* newParent);
		
		//returns true if node is full
		virtual bool full() const = 0;

    protected:
        // [Parent Accessor]
        // MODIFIES: <this>, <parent> (non-const version only)
        // EFFECTS:  returns the parent of <this> TreeNode
        InnerNode* getParent();
        const InnerNode* getParent() const;

        // [Common Ancestor Calculator]
        // REQUIRES: <relative> is not nullptr, <relative> is not the same as <this>,
        //   <relative> is at the same height and in the same BTree as <this> TreeNode
        // MODIFIES: <this>, <relative> the common ancestor of <this> and
        //   <relative> (non-const version only)
        // EFFECTS:  returns the common ancestor of <this> TreeNode and <relative>
        InnerNode* getCommonAncestor(const TreeNode* relative);
        const InnerNode* getCommonAncestor(const TreeNode* relative) const;

        // [Sibling Identifier]
        // REQUIRES: <potentialSibling> is not nullptr, <potentialSibling> is not
        //   the same as <this>
        // EFFECTS:  returns TRUE if and only if <this> TreeNode and
        //   <potentialSibling> are siblings (i.e. have the same parent)
        bool isSibling(const TreeNode* potentialSibling) const;

        // [Invariant Checker]
        // EFFECTS:  returns TRUE if and only if <this> TreeNode satisfies whatever
        //   derived-class-specific invariant it must satisfy; by default, always
        //   returns TRUE
        virtual bool satisfiesInvariant() const;

    private:
        InnerNode* parent;
};

#endif
