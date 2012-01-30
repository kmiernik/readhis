/*
 * K. Miernik (k.a.miernik@gmail.com) 2012
 *
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <iostream>
#include <cstdio>

/**
* Exception handler.
* Basic class for error handling.
* It consist of only one field (message) created with inlined
* constructor.
*/
class GenError {
    const std::string message; //! Error Message
public:
    GenError(const std::string msg = 0) : message(msg) {}
    std::string show() { return message;}
};

/**
* Class for IO error handling.
*/
class IOError : public GenError {
public:
    IOError(const std::string msg = 0) : GenError(msg) {} //! Constructor calling base class constructor in initializer list
};

/**
 * Class for errors related to arrays and vectors.
 */
class ArrayError : public GenError {
public:
    ArrayError(const std::string msg = 0) : GenError(msg) {} //! Constructor calling base class constructor in initializer list
};

namespace debug {

    /**
    * General function for requiriment check.
    */
    inline void require(bool requirement,
                        const std::string& msg = "Requirement failed"){
        if (!requirement) {
            // fputs(msg.c_str(), stderr);
            // fputs("\n", stderr);
            throw GenError(msg);
        }
    }
    /**
     * Counter for simple debugging.
     */
    class Counter {
        public:
            Counter(const std::string& text = "") : text_(text) {
                ++counter_;
                index_ = 0;
                show(); 
            }
            void add() {
                ++index_;
                show();
            }
            void setText(const std::string& text) {
                text_ = text;
            }
            void show() const { 
                std::cout << "#C: " << counter_ << "." << index_ 
                          << " : " << text_ << std::endl;
            }
            ~Counter() { }
        private:
            static int counter_;
            int index_;
            std::string text_;
    };
}

#endif
