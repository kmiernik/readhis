#ifndef HEXCEPTIONS
#define HEXCEPTIONS

#include <string>
#include <cstdio>
/// Exception handler
/**
* Basic class for error handling
* it consist of only one field (message) created with inlined
* constructor
*/
using namespace std;
class GenError {
    const string message; //! Error Message
public:
    GenError(const string msg = 0) : message(msg) {}
    string show() { return message;}
};
/// I/O Exception handler
/**
* class for IO error handling
*/
class IOError : public GenError {
public:
    IOError(const string msg = 0) : GenError(msg) {} //! Constructor calling base class constructor in initializer list
};

class BadIndex : public GenError {
public:
    BadIndex(const string msg = 0) : GenError(msg) {} //! Constructor calling base class constructor in initializer list
};

inline void require(bool requirement, const std::string& msg = "Requirement failed"){
    using namespace std;
    if (!requirement) {
        fputs(msg.c_str(), stderr);
        fputs("\n", stderr);
        throw GenError(msg);
    }
}
#endif
