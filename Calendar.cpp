//
//  Calendar.cpp
//  Assignment4
//
//  Created by rick gessner on 2/20/21.
//

#include "Calendar.hpp"

namespace ECE141 {

  Calendar::Calendar(std::ostream &aStream)
    : stream(aStream) {}

  Calendar::~Calendar() {
    if(Event::leaks) {
      std::cout << "(" << Event::leaks << " events leaked) ";
    }
  }

  bool Calendar::addEvent(Event *anEvent) {
    //STUDENT: Add code to save event in calendar...
    return true;
  }

  bool Calendar::removeEvent(Event *anEvent) {
    //STUDENT: Add code to remove event from calendar...
    return true;
  }

}
