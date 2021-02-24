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
      if (findEvent(anEvent) != nullptr) {
          std::cerr << "Event already exist!\n";
          return false;
      }
      events.push_back(anEvent);
      return true;
  }

  bool Calendar::removeEvent(Event *anEvent) {
      for (int i = 0; i < events.size(); ++i) {
          if (anEvent->title == events[i]->title &&
              anEvent->date  == events[i]->date  &&
              anEvent->time  == events[i]->time  &&
              anEvent->with  == events[i]->with) {
              auto* toBeDelete = events[i];
              events.erase(events.begin() + i);
              delete toBeDelete;
              return true;
          }
      }
      std::cerr << "Event not found!\n";
      return false;
  }

  Event* Calendar::findEvent(Event* anEvent) {
      for (int i = 0; i < events.size(); ++i) {
          if (anEvent->title == events[i]->title &&
              anEvent->date == events[i]->date &&
              anEvent->time == events[i]->time &&
              anEvent->with == events[i]->with) {              
              return events[i];
          }
      }
      std::cerr << "Event not found!\n";
      return nullptr;
  };

}
