//
//  Calendar.hpp
//  Assignment4
//
//  Created by rick gessner on 2/20/21.
//

#ifndef Calendar_hpp
#define Calendar_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

namespace ECE141 {
  
  class Event {
  public:
        
    Event(const std::string &aTitle="",
          const std::string &aDate="2021-01-01",
          const std::string &aTime="09:00am",
          const std::string &aPerson="")
      : title(aTitle), date(aPerson), time(aTime), with(aPerson) {
        ++leaks;
      }
    
    ~Event() {leaks--;}
    
    //just for testing...
    bool matches(const std::string &aField, const std::string &aValue) {
      if("title"==aField) {
        return title==aValue;
      }
      else if("date"==aField) {
        return date==aValue;
      }
      else if("time"==aField) {
        return time==aValue;
      }
      else if("with"==aField) {
        return with==aValue;
      }
      return false;
    }
    
    std::string title;
    std::string date;
    std::string time;
    std::string with;
    
    static int leaks;
  };

  //--------------------------------------

  using Events = std::vector<Event*>;

  class Calendar {
  public:
    Calendar(std::ostream &aStream);
    ~Calendar();
    
    bool addEvent(Event *anEvent);
    bool removeEvent(Event *anEvent);
    
    //------ STUDENT: Add what you want here...

    
    //-----------------------------------------
        
    Events&       getEvents() {return events;}
    std::ostream& getStream() {return stream;}

  protected:
    Events events; //REQUIRED: do not change...
    std::ostream &stream;
  };
}

#endif /* Calendar_hpp */
