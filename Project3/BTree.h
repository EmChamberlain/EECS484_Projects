#ifndef EECS484P3_BTREE_H
#define EECS484P3_BTREE_H

#include "Utilities.h"                                  // for Key alias
#include <iosfwd>                                       // for ostream forward declaration
#include <vector>                                       // for vector (forward declaration is difficult)

class DataEntry;                                        // only used as function argument
class TreeNode;                                         // only used as pointer or function argument


class BTree {
    public:
        // [Constructor]
        BTree();

        // [Destructor]
        // MODIFIES: the root of <this> BTree, memory pool
        // EFFECTS:  deallocates the memory associated with the root of
        //   <this> BTree
        ~BTree();

        // [Statistic Accessors]
        // EFFECTS:  returns the height of or the number of data entries in
        //   <this> BTree
        size_t getHeight() const;
        size_t getSize() const;

        // [Inserter]
        // MODIFIES: <this>
        // EFFECTS:  inserts <newEntry> into <this> BTree if it has a unique
        //   key, otherwise does nothing
        void insertEntry(const DataEntry& newEntry);

        // [Deleter]
        // MODIFIES: <this>, memory pool
        // EFFECTS:  removes <newEntry> from <this> BTree if it exists,
        //   otherwise does nothing
        void deleteEntry(const DataEntry& entryToRemove);

        // [Range Value Finder]
        // REQUIRES: <end> >= <begin>
        // EFFECTS:  returns a sorted list of all data entries in <this> BTree
        //   whose key is in the range [<begin>, <end>] (both endpoints
        //   inclusive)
        std::vector<DataEntry> rangeFind(const Key& begin, const Key& end) const;

        // [Printer]
        // MODIFIES: <os>
        // EFFECTS:  prints <this> BTree to <os>
        void print(std::ostream& os) const;

    private:
        TreeNode* root;
        size_t height;
        size_t size;
};

#endif
