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
#include <vector>

#include "AddProcessor.hpp"
#include "QuitProcessor.hpp"
#include "ChangeProcessor.hpp"
#include "DeleteProcessor.hpp"
#include "ShowProcessor.hpp"

namespace ECE141 {
  

  using StringMap = std::map<std::string, std::string>;

  class TestAutomatic {
  public:
    
    ~TestAutomatic() {std::cout << "Version 1.4\n";}

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
      size_t thePos1=(rand() % theSize);
      size_t thePos2=(rand() % theSize);
      theOutput << theWords[thePos1] << ' ' << theWords[thePos2];
      return theOutput.str();
    }
    
    std::string getRandomTime(bool addUntil=false) {
      std::stringstream theOutput;
      
      static const char* theAMPMs[]={"am","pm"};
      static int theTimes[]={9,10,1,3,5};
      static size_t theSize=sizeof(theTimes)/sizeof(theTimes[0]);
          
      bool theAM=rand() % 2;
      int theTime=theTimes[rand() % theSize];
      theOutput << theTime << ":00" << theAMPMs[theAM];
      if(addUntil) {
        theOutput<< " until " << theTime+1 << ":00" << theAMPMs[theAM];
      }
      return theOutput.str();
    }

    std::string getRandomDate(int aMo, int aDay, bool addUntil=false) {
      std::stringstream theOutput;
      theOutput << std::setfill('0');

      theOutput << "2021-" << std::setw(2) << aMo
        << "-" << std::setw(2) << aDay;
      
      if(addUntil) {
        theOutput << " until 2021-" << std::setw(2) << aMo
          << "-" << std::setw(2) << aDay+1;
      }
      return theOutput.str();
    }

    std::string getRandomPerson() {
      static const char *theNames[]=
        {"Bob","Alicia","Sandy","Ming","Shivi","Eshan","Medha","Lei","Travon"};
      static size_t theSize=sizeof(theNames)/sizeof(theNames[0]);
      std::stringstream theOutput;
      theOutput << theNames[rand() % (theSize-1)];
      return theOutput.str();
    }

    std::string getRandomAddCmd(const std::string &aTitle) {
      static int theParts[]={1,2,3,4};

      std::random_shuffle(std::begin(theParts), std::end(theParts));
      std::stringstream theOutput;
      
      for(auto thePart : theParts) {
        
        int theMon=1+(rand() % 11);
        int theDay=1+(rand() % 25);

        switch(thePart) {
          case 1: theOutput << " add \"" << aTitle << '"'; break;
          case 2:
            theOutput << " on "
              << getRandomDate(theMon, theDay, 2==theParts[0]);
            break;
          case 3:
            theOutput << " at "  << getRandomTime(2==theParts[0]);
            break;
          case 4: theOutput << " with \"" << getRandomPerson() << '"'; break;
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
        int theMon=1+(rand() % 11);
        int theDay=1+(rand() % 25);
        switch(thePart) {
          case 1: theOutput << " add " << '"' << aTitle << '"'; break;
          case 2:
            theOutput << " on "  <<
              getRandomDate(theMon, theDay, 2==theParts[0]);
            break;
          case 3:
            theOutput << " at "  << getRandomTime(3==theParts[0]);
            break;
          case 4: theOutput << " with \""<< getRandomPerson() << '"'; break;
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
 
    //------- called from main() -------------
    
    bool doShowTest() {
      bool theResult=false;

      std::stringstream theStream;
      {
        Calendar theCalendar(theStream); //for testing ...

        if(addTestEvents(theCalendar)) {
          std::stringstream theStream("show");
          ECE141::Tokenizer theTokenizer(theStream);
          theTokenizer.tokenize();

          ECE141::ShowProcessor theProc;
          if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
            theResult=theProc.run(theCmd, theCalendar)==kNoError;
            delete theCmd;
          }
        }
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
    
    int testShowCmd(Calendar &aCalendar, const std::string &anInput) {
      int theResult=kNoError;
      
      std::stringstream theStream(anInput);
      ECE141::Tokenizer theTokenizer(theStream);
      theTokenizer.tokenize();

      ECE141::ShowProcessor theProc;
      if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
        theResult=theProc.run(theCmd, aCalendar)==kNoError;
        delete theCmd;
      }
      return theResult;
    }

    int testShowAll(Calendar &aCalendar, size_t &aCount) {
      aCount=aCalendar.getEvents().size();
      return testShowCmd(aCalendar,"show");
    }
      
    using StringCount = std::map<std::string, int>;
    
    std::string theBusiest(StringCount &aMap, size_t &aCount) {
      std::string theKey;
      for(auto theElem : aMap) {
        if(theElem.second>aCount) {
          theKey=theElem.first;
          aCount=theElem.second;
        }
      }
      return theKey;
    }
    
    int testShowIfTime(Calendar &aCalendar, size_t &aCount) {
      auto &theEvents=aCalendar.getEvents();
      StringCount theCounts;
      for(auto theEvent : theEvents) {
        theCounts[theEvent->time]++;
      }
      
      std::string theBusy=theBusiest(theCounts, aCount);
      std::string theCmd("show if time=\""+theBusy+'"');
      return testShowCmd(aCalendar,theCmd);
    }

    int testShowIfTitle(Calendar &aCalendar, size_t &aCount) {
      auto &theEvents=aCalendar.getEvents();
      StringCount theCounts;
      for(auto theEvent : theEvents) {
        theCounts[theEvent->title]++;
      }
      std::string theBusy=theBusiest(theCounts,aCount);
      std::string theCmd("show if title=\""+theBusy+'"');
      return testShowCmd(aCalendar,theCmd);
    }

    int testShowDateRange(Calendar &aCalendar, size_t &aCount) {
      int theMon=1+(rand() % 3);
      int theDay=10+(rand() % 15);

      std::string theStart=getRandomDate(theMon, theDay);
      std::string theEnd=getRandomDate(theMon+1, theDay);
        
      aCount=0;
      auto &theEvents=aCalendar.getEvents();
      for(auto theEvent : theEvents) {
        if(theStart<=theEvent->date && theEvent->date<=theEnd) {
          aCount++;
        }
      }
      
      return testShowCmd(aCalendar,"show on "+theStart+" until "+theEnd);
    }

    //--------------------------------------------------

    size_t countLines(const std::string &aBuffer) {
      std::stringstream theStream(aBuffer);
      size_t theCount=0;
      const size_t kBufferSize=1024;
      char theBuffer[kBufferSize];

      //std::cout << aBuffer;
      static const char* theSubs[]={"|-----","|Title","| Titl"};
      while(!theStream.eof()) {
        theStream.getline(theBuffer, kBufferSize);
        std::string temp(theBuffer);
        if(temp.size()) {
          std::string theSub=temp.substr(0,6);
          theCount++;
          for(auto theStr: theSubs) {
            if(theSub==theStr) {
              theCount--;
              break;
            }
          }
        }
      }
      return theCount;
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
              
              std::stringstream theStream;
              theStream << "change " << '"' << theRE->title << '"';

              std::vector<int> theParts={0,1,2,3};
              while(theParts.size()>2) {
                size_t theIndex=rand() % theParts.size();
                theParts.erase(theParts.begin() + theIndex);
              }
              StringMap theMap;
              for(auto theValue : theParts) {
                int theMon=1+(rand() % 11);
                int theDay=1+(rand() % 25);
                switch(theValue) {
                  case 0:
                    theMap["title"]=getRandomTitle();
                    theStream << " title=\"" << theMap["title"] << '"';
                    break;
                  case 1:
                    theMap["date"]=getRandomDate(theMon, theDay);
                    theStream << " date=" << theMap["date"];
                    break;
                  case 2:
                    theMap["time"]=getRandomTime();
                    theStream << " time=" << theMap["time"];
                    break;
                  default:
                    theMap["with"]=getRandomPerson(); ;
                    theStream << " with=\"" << theMap["with"] << '"';
                    break;
                }
              }

              ECE141::ChangeProcessor theProc;
              std::string theStr=theStream.str();
              std::stringstream theInput(theStr);
              ECE141::Tokenizer theTokenizer(theInput);
              theTokenizer.tokenize();
              
              if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
                theResult=theProc.run(theCmd, aCalendar)==kNoError;
                delete theCmd;
                for(auto &thePair : theMap) {
                  if(theResult) {
                    theResult=the1st->matches(thePair.first,thePair.second);
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
    
    std::string makeRandomDeleteCmd(Calendar &aCalendar) {
      std::stringstream theStream;

      if(Event* theRE=getRandomEvent(aCalendar)) {
        if(Event* the1st=firstOf(aCalendar, "title",theRE->title)) {
          if(theRE==the1st) {
            theStream << "delete ";
            switch(rand() % 3) {
              case 0:
                theStream << " if date=" << theRE->date; break;
              case 1:
                theStream << " if time=" << theRE->time; break;
              default:
                theStream << " if title=\"" << theRE->title << '"';
                break;
            }
          }
        }
      }
      return theStream.str();
    }
    
    //-------------------------------------------

    bool deleteTestEvent(Calendar &aCalendar, const std::string &anInput) {
      bool theResult=true;
            
      ECE141::DeleteProcessor theProc;
      std::stringstream theStream(anInput);
      ECE141::Tokenizer theTokenizer(theStream);
      theTokenizer.tokenize();
      
      if(Command *theCmd=theProc.makeCommand(theTokenizer)) {
        size_t theCalSize=aCalendar.getEvents().size();
        theResult=theProc.run(theCmd, aCalendar);
        delete theCmd;
        theResult=theCalSize>aCalendar.getEvents().size();
      }
   
      return theResult;
    }
    
    bool deleteAllEvents(Calendar &aCalendar) {
      bool theResult=true;
      
      std::map<std::string, int> theDates;
      auto &theEvents=aCalendar.getEvents();
      for(auto theEvent : theEvents) {
        theDates[theEvent->date]++;
      }
      
      std::string thePrefix("delete on ");
      for(auto thePair: theDates) {
        std::string theCmd(thePrefix+thePair.first);
        theResult=deleteTestEvent(aCalendar, theCmd);
        if(!theResult) {
          std::cout << "(Delete) ";
          break;
        }
      }
            
      return theResult;
    }
    
    //-------------------------------------------
    // Stress test coming soon in testing update...
    //----------------------------------------------    
    bool doStressTest() {
      
      bool theResult=true;
      {
        std::stringstream theTestStream;
        Calendar theCalendar(theTestStream); //for testing ...
        Events&  theEvents=theCalendar.getEvents();

        size_t theCount=0;
        while((++theCount<1000) && theResult) {
          //std::cout << "stress " << theCount << "\n";
          if(theEvents.size()<100) {
            for(int i=0;i<20;i++) {
              theResult=addTestEvent(getRandomTitle(),theCalendar);
              if(!theResult) std::cout << " (Add) ";
            }
          }
          if(theCount % 50==0) {
            size_t theSCount=0;
            switch(rand() % 3) {
              case 0:
                theResult=testShowIfTitle(theCalendar,theSCount);
                break;
              case 1:
                theResult=testShowIfTime(theCalendar,theSCount);
                break;
              case 2:
                theResult=testShowAll(theCalendar,theSCount);
                break;
              default:
                theResult=testShowDateRange(theCalendar,theSCount);
                break;
            };
            if(theResult) {
              std::string temp=theTestStream.str();
              //std::cout << temp << "\n";
              size_t theLCount=countLines(temp);
              theResult=((theSCount<=theLCount) && (theLCount<=theSCount+2));
            }

            theTestStream.str("");
            theTestStream.clear();
          }
          else {
            switch(rand() % 3) {
              case 0:
                {
                  std::string theInput=makeRandomDeleteCmd(theCalendar);
                  if(theInput.size()) {
                    theResult=deleteTestEvent(theCalendar, theInput);
                    if(!theResult) std::cout << "(Delete) ";
                  }
                }
                break;
              default:
                theResult=changeTestEvent(theCalendar);
                if(!theResult) std::cout << "(Change) ";
                break;
            }
          }
        } //while...
               
        if(theResult) { //delete all
          theResult=deleteAllEvents(theCalendar);
        }
      }
      return theResult && Event::leaks==0;
    }
   
  };

}

#endif /* TestAutomatic_h */
