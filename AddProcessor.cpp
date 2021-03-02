//
//  AddProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/19/21.
//

#include "AddProcessor.hpp"

namespace ECE141 {

  AddProcessor::AddProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext) {}

  AddProcessor::~AddProcessor() {}

  bool AddProcessor::recognizes(Tokenizer &aTokenizer) {
      aTokenizer.restart();
      return aTokenizer.skipTo(Keywords::add_kw);
  }

  Command* AddProcessor::makeCommand(Tokenizer &aTokenizer) {
      std::string title;
      std::string date; 
      std::string time;
      std::string with;

      aTokenizer.restart();

      while (aTokenizer.more()) {
          int curKeyWord = (int)aTokenizer.current().keyword;        

          switch (curKeyWord) {
          case (int)Keywords::add_kw: {
              aTokenizer.next();
              if (aTokenizer.current().type == TokenType::string) {
                  title = aTokenizer.current().data;
                  aTokenizer.next();
              }
              break;
          }

          case (int)Keywords::at_kw: {
              aTokenizer.next();
              if (aTokenizer.current().type == TokenType::timedate) {
                  time = aTokenizer.current().data;
                  if (time.size() != 7)
                      time = '0' + time;
                  aTokenizer.next();
                  if (aTokenizer.more() && aTokenizer.current().keyword == Keywords::until_kw) {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          std::string endTime = aTokenizer.current().data;
                          if(endTime.size() != 7)
                              endTime = '0' + endTime;
                          time += "-" + endTime;
                          aTokenizer.next();
                      }
                  }
              }              
              break;
          }

          case (int)Keywords::on_kw: {
              aTokenizer.next();
              if (aTokenizer.current().type == TokenType::timedate) {
                  date = aTokenizer.current().data;
                  aTokenizer.next();
                  if (aTokenizer.more() && aTokenizer.current().keyword == Keywords::until_kw) {                      
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          date += " " + aTokenizer.current().data;
                          aTokenizer.next();                          
                      }
                  }
              }
              break;
          }

          case (int)Keywords::with_kw: {
              aTokenizer.next();
              if (aTokenizer.current().type != TokenType::string) {
                  //is a '='
                  aTokenizer.next();
              }
              with = aTokenizer.current().data;
              aTokenizer.next();
              break;
          }

          default: {
              aTokenizer.next();
              break;
          }
          }
      }
      return new Command(title, date, time, with);
  }

  int AddProcessor::run(Command *aCommand, Calendar &aCal) {
      int month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

      Event* newEvent = new Event(aCommand->title, aCommand->date,
          aCommand->time, aCommand->with);

      if (!aCal.addEvent(newEvent))
          delete newEvent;

      //std::cout << "Event added\n";
      return kNoError;
  }

}
