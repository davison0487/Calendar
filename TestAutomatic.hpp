//
//  TestAutomatic.hpp
//  Assignment4
//
//  Created by rick gessner on 2/20/21.
//

#ifndef TestAutomatic_h
#define TestAutomatic_h

#include "Calendar.hpp"
#include <sstream>
#include <map>
#include <random>
#include <algorithm>
#include <iomanip>

#include "AddProcessor.hpp"
#include "QuitProcessor.hpp"
#include "ChangeProcessor.hpp"
#include "DeleteProcessor.hpp"
#include "ShowProcessor.hpp"

namespace ECE141 {
  

  using StringMap = std::map<std::string, std::string>;

  class TestAutomatic {
  public:
    
    ~TestAutomatic() {std::cout << "Version 1.0\n";}

    bool doCompileTest() {return true;}

    //-----------------------------------------
    
    std::string getRandomTitle() {
      static const char* theWords[] = {
        "meet","fun","lunch","breakfast","dinner","snack","party",
        "study","train","work","school","after","before","holiday",
        "prepare","fix","update","group","team","family","repair"
      };
      static size_t theSize=sizeof(theWords)/sizeof(theWords[0]);
      
      std::stringstream theOutput;
      theOutput << theWords[rand() % theSize-1]
        << ' ' << theWords[rand() % theSize-1];
      return theOutput.str();
    }
    
    std::string getRandomTime() {
      switch(rand() % 5) {
        case 0:  return "9:00am";
        case 1:  return "11:00am";
        case 2:  return "1:00pm";
        case 3:  return "3:00pm";
        default: return "5:00pm";
      }
    }

    std::string getRandomDate() {
      std::stringstream theOutput;
      theOutput << std::setfill('0');
      theOutput << "2021-" << std::setw(2) << 1 + (rand() % 11)
        << "-" << std::setw(2) <<  1 + (rand() % 25);
      return theOutput.str();
    }

    std::string getRandomPerson() {
      static const char *theNames[]=
        {"Bob","Alicia","Sandy","Ming","Shivi","Eshan","Medha","Lei","Travon"};
      static size_t theSize=sizeof(theNames)/sizeof(theNames[0]);
      return theNames[rand() % (theSize-1)];
    }

    std::string getRandomAddCmd(const std::string &aTitle) {
      static int theParts[]={1,2,3,4};

      std::random_shuffle(std::begin(theParts), std::end(theParts));
      std::stringstream theOutput;
      
      for(auto thePart : theParts) {
        switch(thePart) {
          case 1: theOutput << " add \"" << aTitle <<'"'; break;
          case 2: theOutput << " on "  << getRandomDate(); break;
          case 3: theOutput << " at "  << getRandomTime(); break;
          case 4: theOutput << " with \""
            << getRandomPerson() << '"'; break;
          default: break;
        }
      }
      return theOutput.str();
    }
    
    std::string getRandomChangeCmd(const std::string &aTitle) {
      static int theParts[]={1,2,3,4};

      std::random_shuffle(std::begin(theParts), std::end(theParts));
      std::stringstream theOutput;
      
      for(auto thePart : theParts) {
        switch(thePart) {
          case 1: theOutput << " add \"" << aTitle <<'"'; break;
          case 2: theOutput << " on "  << getRandomDate(); break;
          case 3: theOutput << " at "  << getRandomTime(); break;
          case 4: theOutput << " with \""
            << getRandomPerson() << '"'; break;
          default: break;
        }
      }
      return theOutput.str();
    }
    
    bool addTestEvent(const std::string &aTitle, Calendar &aCalendar) {
      std::string theInput=getRandomAddCmd(aTitle);
      std::stringstream theStream(theInput);
      
      ECE141::Tokenizer theTokenizer(theStream);
      theTokenizer.tokenize();

      ECE141::AddProcessor theProc;
      if(theProc.recognizes(theTokenizer)) {
        if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
          int theResult=theProc.run(theCmd, aCalendar);
          delete theCmd;
          return theResult==kNoError;
        }
      }
      return false;
    }
    
    //put a known event on a calendar...
    bool addTestEvents(Calendar &aCalendar) {
      bool theResult=false;

      const char* theTitles[] = {"Lunch","Interview","StudyGroup"};
      for(auto theTitle: theTitles) {
        theResult=addTestEvent(theTitle,aCalendar);
      }
      
      return theResult;
    }
    
    //cover method to get calendar event by key...
    Event* firstOf(Calendar &aCalendar,
                   const std::string &aKey, const std::string &aVal) {
      for(auto theEvent : aCalendar.getEvents()) {
        if(aKey=="title" && theEvent->title==aVal) {
          return theEvent;
        }
      }
      return nullptr;
    }
        
    bool doAddTest() {
      bool theResult=false;
      {
        Calendar theCal(std::cout); //for testing ...
        if(addTestEvents(theCal)) {
          if(Event *theEvent=firstOf(theCal, "title","Interview")) {
            theResult=theCal.getEvents().size()==3;
          }
        }
      }
      return theResult && Event::leaks==0;
    }
        
    //Create an event, change it, confirm it updated...
    bool doChangeTest() {
      bool theResult=false;
      {
        Calendar theCal(std::cout); //for testing ...
        if(addTestEvents(theCal)) {

          const char *theInput= "change \"Interview\" time=2:00pm";
                  
          std::stringstream theStream(theInput);
          ECE141::Tokenizer theTokenizer(theStream);
          theTokenizer.tokenize();

          ECE141::ChangeProcessor theProc;
          if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
            theResult=theProc.run(theCmd, theCal)==kNoError;
            delete theCmd;
            if(theResult) {
              if(auto theE=firstOf(theCal, "title","Interview")) {
                theResult=theE->matches("time","2:00pm");
              }
            }
          }
        }
      }
      return theResult && Event::leaks==0;
    }
    
    bool doDeleteTest() {
      bool theResult=false;
      {
        Calendar theCalendar(std::cout); //for testing ...
        if(addTestEvents(theCalendar)) {

          const char *theInput= "delete if title=\"Interview\"";
                  
          std::stringstream theStream(theInput);
          ECE141::Tokenizer theTokenizer(theStream);
          theTokenizer.tokenize();

          ECE141::DeleteProcessor theProc;
          if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
            theResult=theProc.run(theCmd, theCalendar)==kNoError;
            delete theCmd;
            if(theResult) {
              theResult=theCalendar.getEvents().size()==2;
            }
          }
        }
      }
      return theResult && Event::leaks==0;
    }
    
    //-----------------------------------------
    
    bool doShowTestWith(Calendar &aCalendar) {
      //select a set of events...
      //run show cmd...
      
      bool theResult=false;
      {
        if(addTestEvents(aCalendar)) {

          const char *theInput= "show if title=\"Interview\"";
                  
          std::stringstream theStream(theInput);
          ECE141::Tokenizer theTokenizer(theStream);
          theTokenizer.tokenize();

          ECE141::ShowProcessor theProc;
          if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
            theResult=theProc.run(theCmd, aCalendar)==kNoError;
            delete theCmd;
          }
        }
      }
      return theResult;
    }
      
    //------- called from main() -------------
    
    bool doShowTest() {
      bool theResult=false;
      std::stringstream theStream;
      {
        Calendar theCalendar(theStream); //for testing ...
        theResult=doShowTestWith(theCalendar);
      }
      if(theResult && Event::leaks==0) {
        std::stringstream theStream2(theStream.str());
        const size_t kBufferSize=1024;
        char theBuffer[kBufferSize];
        size_t theCount=0;

        const char* theTitles[] = {"Lunch","Inter","Study"};
        while(!theStream2.eof()) {
          theStream2.getline(theBuffer, kBufferSize);
          std::string temp(theBuffer);
          for(auto theTitle : theTitles) {
            if ( temp.find(theTitle)!= std::string::npos) {
              theCount++;
            }
          }
        }
        theResult=theCount==3;
      }
      return theResult;
    }
    
    //--------------------------------------------------
    
    Event* getRandomEvent(Calendar &aCalendar) {
      if(size_t theSize=aCalendar.getEvents().size()) {
        size_t theIndex=rand() % (theSize-1);
        return aCalendar.getEvents()[theIndex];
      }
      return nullptr;
    }
    
    //--------------------------------------------------
          
    bool changeTestEvent(Calendar &aCalendar) {
      bool theResult=false;
      bool done=false;
      
      while(!done) {
        if(Event* theRE=getRandomEvent(aCalendar)) {
          if(Event* the1st=firstOf(aCalendar, "title",theRE->title)) {
            if(theRE==the1st) {
              done=true;
              StringMap theMap;
              for(int i=0;i<2;i++) {
                switch(rand() % 4) {
                  case 0: theMap["title"]=getRandomTitle(); break;
                  case 1: theMap["date"]=getRandomDate(); break;
                  case 2: theMap["time"]=getRandomTime(); break;
                  default: theMap["with"]=getRandomPerson(); break;
                }
              }

              std::stringstream theStream;
              theStream << "change \"" << theRE->title << '"';
              for(auto &thePair : theMap) {
                theStream << " " << thePair.first << "=" << thePair.second;
              }

              ECE141::ChangeProcessor theProc;
              ECE141::Tokenizer theTokenizer(theStream);
              theTokenizer.tokenize();
              
              if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
                theResult=theProc.run(theCmd, aCalendar)==kNoError;
                delete theCmd;
                for(auto &thePair : theMap) {
                  if(theResult) {
                    theResult=
                      the1st->matches(thePair.first,thePair.second);
                  }
                }
              }
            }
          }
        }
      }
      return theResult;
    }
    
    //-------------------------------------------

    bool deleteTestEvent(Calendar &aCalendar) {
      bool theResult=false;
      bool done=false;
        
      while(!done) {
        if(Event* theRE=getRandomEvent(aCalendar)) {
          if(Event* the1st=firstOf(aCalendar, "title",theRE->title)) {
            if(theRE==the1st) {
              done=true;
              
              size_t theCalSize=aCalendar.getEvents().size();
              std::stringstream theStream;
              theStream << "DELETE ";
              
              switch(rand() % 3) {
                case 0:
                  theStream << " if date=" << theRE->date; break;
                  
                case 1:
                  theStream << " if time=" << theRE->time; break;

                case 2:
                  theStream << " if title=" << theRE->title; break;
              }

              ECE141::DeleteProcessor theProc;
              ECE141::Tokenizer theTokenizer(theStream);
              theTokenizer.tokenize();
              
              if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
                theResult=theProc.run(theCmd, aCalendar);
                delete theCmd;
                theResult=theCalSize>aCalendar.getEvents().size();
              }
            }
          }
        }
      } //while...
   
      return theResult;
    }
    
    //-------------------------------------------
    // Stress test coming soon in testing update...
    //----------------------------------------------
    bool doStressTest() {
      return false;
    }
   
  };

}

#endif /* TestAutomatic_h */
