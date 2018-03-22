#include "BTree.h"                                      // for BTree
#include "DataEntry.h"                                  // for DataEntry
#include <exception>                                    // for exception, bad_alloc
#include <iostream>                                     // for cin, cout, istream, ostream
#include <string>                                       // for string
#include <unordered_map>                                // for unordered_map
#include <vector>                                       // for vector

using std::istream; using std::ostream; using std::cin; using std::cout;
using std::string; using std::unordered_map;
using std::exception; using std::bad_alloc;

using ReadException = class : public exception {};
using CommandException = class : public exception {};
using ExecFunc_t = void(*)(istream&, BTree&);
using CommandMap_t = unordered_map<string, ExecFunc_t>;

static const string kInsertCmd = "insert";
static const string kDeleteCmd = "delete";
static const string kPrintCmd = "print";
static const string kRangeFindCmd = "find";
static const string kQuitCmd = "quit";
static ostream* const outStream = &cout;


// MODIFIES: <is>
// EFFECTS:  attempts to read a Key value out of <is> and returns the
//   read key if successful; throws a ReadException if the read fails
Key readKey(istream& is);

// MODIFIES: contents in the range [<begin>, <end>), memory pool
// EFFECTS:  deallocates the contents in the range [<begin>, <end>)
template <typename Iter>
void dellocateAll(Iter start, Iter end);

// MODIFIES: <is>
// EFFECTS:  reads characters out of <is> up to and including the
//   first newline character
void clearLine(istream& is);

// MODIFIES: <is>, <tree>
// EFFECTS:  reads a single integer from <is> and inserts a new
//   data entry with that key/value into <tree>
void performInsert(istream& is, BTree& tree);

// MODIFIES: <is>, <tree>
// EFFECTS:  reads a single integer from <is> and deletes the data
//   entry from <tree> with that key
void performDelete(istream& is, BTree& tree);

// MODIFIES: <is>, <tree>, <outStream>
// EFFECTS:  reads two integers from <is> and and performs a range
//   find on <tree> using those endpoints, printing the results of
//   the range find to <outStream>
void performRangeFind(istream& is, BTree& tree);

// MODIFIES: <outStream>
// EFFECTS:  prints <tree> to <outStream>
void performPrint(istream&, BTree& tree);


// application driver
int main() {
    BTree tree{};
    CommandMap_t cmdMap{                                    // map of command keywords to execution functions
        { kInsertCmd, &performInsert },
        { kDeleteCmd, &performDelete },
        { kPrintCmd, &performPrint },
        { kRangeFindCmd, &performRangeFind }
    };
    auto& err = *outStream;                                 // where to print error messages

    string command{ "" };
    while (true) {
        cin >> command;
        if (command == kQuitCmd) {                          // continue until QUIT command produced
            return 0;
        }

        try {
            if (command[0] == '#') {
                clearLine(cin);
                continue;
            }

            auto funcIter = cmdMap.find(command);
            if (funcIter == cmdMap.end()) {
                throw CommandException{};
            }
            (*funcIter->second)(cin, tree);
        }
        catch (CommandException&) {                                             // unrecognized command, keep going
            err << "\nUnrecognized command '" << command << "'\n\n";
            clearLine(cin);
        }
        catch (ReadException&) {                                                // failed integer read, keep going
            err << "\nUnable to read integer where expected\n\n";
            clearLine(cin);
        }
        catch (bad_alloc&) {                                                    // allocation failure, abort execution
            err << "\nMemory pool exceeded by over-allocation\n\n";
            return 1;
        }
        catch (exception& e) {                                                  // other exception, abort execution
            err << "\nException Encountered: " << e.what() << "\n\n";
            return 1;
        }
        catch (...) {                                                           // other throwable, abort execution
            err << "\nUnidentified object throw and caught\n\n";
            return 1;
        }
    }

    return 0;
}

template <typename Iter>
void dellocateAll(Iter start, Iter end) {
    while (start != end) {
        delete *start;
    }
}

// try to read a Key, throw ReadException if read fails
Key readKey(istream& is) {
    Key key{};
    if (!(is >> key)) {
        throw ReadException{};
    }
    return key;
}

// read single character until and including first newline character
void clearLine(istream& is) {
    is.clear();
    while (is.get() != '\n');
}

// try to read an integer and perform insert
void performInsert(istream& is, BTree& tree) {
    Key key = readKey(is);
    Record record{ key };
    tree.insertEntry(DataEntry{ key, record });
}

// try to read and integer and perform delete
void performDelete(istream& is, BTree& tree) {
    Key key = readKey(is);
    Record record{ key };
    tree.deleteEntry(DataEntry{ key, record });
}

// try to read two integers and perform range find, print results of
// range find to designated output stream
void performRangeFind(istream& is, BTree& tree) {
    Key begin = readKey(is);
    Key end = readKey(is);
    auto results = tree.rangeFind(begin, end);
    
    auto& os = *outStream;
    os << kPrintPrefix << "[ ";
    for (const auto& val : results) {
        if (val != results[0]) {
            os << " | ";
        }
        os << val;
    }
    os << " ]\n\n";
}

// print tree to designated output stream
void performPrint(istream&, BTree& tree) {
    auto& out = *outStream;

    out << "\n";
    tree.print(out);                    // ends with a newline by LeafNode print
    out << "\n";
}
