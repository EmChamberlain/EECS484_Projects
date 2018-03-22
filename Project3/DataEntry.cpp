#include "DataEntry.h"                                  // file-specific header
#include "Utilities.h"                                  // for Key, Record
#include <cassert>                                      // for assert


// constructor
DataEntry::DataEntry(const Key& key, const Record record)
    : key{ key }, record{ record } {

    assert(record == key);
}

// implicit Key converter
DataEntry::operator Key() const {
    return key;
}

// return record
const Record* DataEntry::getRecord() const {
    return &record;
}

// equality
bool DataEntry::operator==(const DataEntry& rhs) const {
    return (key == rhs.key);
}

// inequality
bool DataEntry::operator!=(const DataEntry& rhs) const {
    return (key != rhs.key);
}

// less than
bool DataEntry::operator<(const DataEntry& rhs) const {
    return (key < rhs.key);
}

// greater than
bool DataEntry::operator>(const DataEntry& rhs) const {
    return (key > rhs.key);
}

// less than or equal to
bool DataEntry::operator<=(const DataEntry& rhs) const {
    return (key <= rhs.key);
}

// greater than or equal to
bool DataEntry::operator>=(const DataEntry& rhs) const {
    return (key >= rhs.key);
}
