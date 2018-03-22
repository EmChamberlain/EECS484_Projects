#ifndef EECS484P3_DATA_ENTRY_H
#define EECS484P3_DATA_ENTRY_H

#include "Utilities.h"                                  // for Key, Record


class DataEntry {
    public:
        // [Constructor]
        // REQUIRES: <record> is not nullptr, <record> has the same value
        //   as <key>
        DataEntry(const Key& key, const Record record);

        // [To-Key Converter]
        // EFFECTS:  implicitly converts <this> DataEntry to its key
        operator Key() const;

        // [Record Obtainer]
        // EFFECTS:  returns a pointer to the record represented by <this>
        //   DataEntry
        const Record* getRecord() const;

        // [Binary Operators]
        // EFFECTS:  returns TRUE if and only if <this> DataEntry compares
        //   equal to, not equal to, less than, greater than, less than or
        //   equal to, or greater than or equal to <rhs> by comparing the
        //   keys of <this> DataEntry and <rhs>
        bool operator==(const DataEntry& rhs) const;
        bool operator!=(const DataEntry& rhs) const;
        bool operator<(const DataEntry& rhs) const;
        bool operator>(const DataEntry& rhs) const;
        bool operator<=(const DataEntry& rhs) const;
        bool operator>=(const DataEntry& rhs) const;

    private:
        Key key;
        Record record;
};

#endif
