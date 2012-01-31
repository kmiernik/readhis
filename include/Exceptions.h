/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>

/**
* Exception handler.
* Basic class for error handling.
* It consist of only one field (message) created with inlined
* constructor.
*/
class GenError {
public:
    /** Error message is passed in the ctor.*/
    GenError(const std::string msg = "") : message(msg) {}
    /** Shows error message. */
    std::string show() { return message;}
private:
    /** Error message. */
    const std::string message;
};

/**
* Class for IO error handling.
*/
class IOError : public GenError {
public:
    /** Constructor calling base class constructor in initializer list */
    IOError(const std::string msg = 0) : GenError(msg) {}
};

/**
 * Class for errors related to arrays and vectors.
 */
class ArrayError : public GenError {
public:
    /** Constructor calling base class constructor in initializer list */
    ArrayError(const std::string msg = 0) : GenError(msg) {}
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
            /** Ctor, text message passed as an option. */
            Counter(const std::string& text = "") : text_(text) {
                ++counter_;
                index_ = 0;
                show(); 
            }

            /** Adds one to subindex, then calls show().*/
            void add() {
                ++index_;
                show();
            }
            
            /** Adds one to subindex, sets text to passed stringstream and calls show(). */
            void set(const std::stringstream& text) {
                ++index_;
                text_ = text.str();
                show();
            }

            /** Adds one to subindex, sets text to passed string and calls show(). */
            void set(const std::string& text) {
                ++index_;
                text_ = text;
                show();
            }

            /** Prints to cout the counter number, index and message. */
            void show() const { 
                std::cout << "#C: " << counter_ << "." << index_ 
                          << " : " << text_ << std::endl;
            }
            ~Counter() { }
        private:
            /** Counter number. */
            static int counter_;

            /** Sub index of counter, incremented by each call of add()
             * and set.*/
            int index_;

            /** Optional text stored by counter. In a form of stringstream
             * for easier passing values of variables etc.
             */
            std::string text_;
    };
}

#endif
