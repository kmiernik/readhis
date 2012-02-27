/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#ifndef DEBUGH
#define DEBUGH

#include <sys/time.h>
#include <string>
#include <iostream>
#include <sstream>

#define DBG(x) std::cout << "#" << #x << " = " << (x) << std::endl;

namespace debug {
    /**
     * Counter for simple debugging.
     * Shows message on any action. Main counter is static so you may
     * create many instances across the code. The subindex helps to trace
     * problematic brake points. The optional text may be passed as a 
     * stringstream, so it's easy to pass variables etc.
     *
     * Example:
     *  debug::Counter cnt;
     *  stringstream ss;
     *
     *  //... some code
     *
     *  ss << "sz =  " << sz;
     *  cnt.set(ss);
     *
     *  for (int i = 0; i < sz; ++i) {
     *    cnt.add();
     *  //... code
     *  }
     *  
     */
    /** Class for time measurements. */
    class Timer {
        public:
            /** C'tor, gets clock at the moment of creation. */
            Timer() {gettimeofday(&t,NULL);}

            /** Resets timer to current time. */
            void set_time() { gettimeofday(&t,NULL);}

            /** Resetrs timer to given time.*/
            void set_time(timeval& x) { t = x;}

            /** Returns time in us. */
            long get_us() { 
                return (t.tv_sec * 1000000 + t.tv_usec);
            }

            /** Returns time in ms. */
            long get_ms() { 
                return (t.tv_sec * 1000 + 
                        long(t.tv_usec / 1000.0 + 0.5) );
            }

            /** Returns time in s. */
            long get_s() { return (t.tv_sec 
                                    + long (t.tv_usec / 1.0e6 + 0.5) );
            }

            /** Difference between timer x and this timer in us.*/
            long dt(Timer& x) { return x.get_us() - get_us();}

        private:
            /** Timestamp. */
            timeval t;
    };

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
            
            /** Adds one to subindex, sets text to passed 
             * stringstream and calls show().
             * Argument clear set to
             * true (default) clears stringstream and text field.*/
            void set(std::stringstream& text, bool clear = true) {
                ++index_;
                text_ = text.str();
                show();

                if (clear) {
                    text.str("");
                    text_ = "";
                }
            }

            /** Adds one to subindex, sets text to passed 
             * string and calls show(). 
             * Argument clear set to true (default) clears
             * text field.*/
            void set(const std::string& text, bool clear = true) {
                ++index_;
                text_ = text;
                show();

                if (clear)
                    text_ = "";
            }

            /** Prints to cout the counter number, index and message. */
            void show() const { 
                std::cout << "#C: " << counter_ << "." << index_ 
                          << " : " << text_ << std::endl;
            }
            ~Counter() {
                std::cout << "#C: " << counter_ << " "
                          << " destructed " << std::endl;
            }
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
