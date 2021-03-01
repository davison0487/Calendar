//
//  ShowProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#include <map>
#include <iomanip>

#include "ShowProcessor.hpp"
#include "IView.hpp"

namespace ECE141 {

  ShowProcessor::ShowProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext) {}

  ShowProcessor::~ShowProcessor() {}

  bool ShowProcessor::recognizes(Tokenizer &aTokenizer) {
      aTokenizer.restart();
      return aTokenizer.skipTo(Keywords::show_kw);
  }

  Command* ShowProcessor::makeCommand(Tokenizer &aTokenizer) {
      commandType cmdType(commandType::Show);      
      std::string title;
      std::string startDate;
      std::string endDate;
      std::string time;
      std::string with;
      
      aTokenizer.restart();

      while (aTokenizer.more()) {
          int curKeyWord = (int)aTokenizer.current().keyword;
          aTokenizer.next();
          switch (curKeyWord) {
          case (int)Keywords::if_kw: {
              if (aTokenizer.current().type == TokenType::string) {
                  std::string dataType = aTokenizer.current().data;
                  aTokenizer.next();
                  if (aTokenizer.current().type == TokenType::punctuation) {//expecting a '='
                      aTokenizer.next();
                      if (dataType == "title") {
                          if (aTokenizer.current().type == TokenType::string) {
                              title = aTokenizer.current().data;
                          }
                      }
                      else if (dataType == "date") {
                          if (aTokenizer.current().type == TokenType::timedate) {
                              startDate = aTokenizer.current().data;
                              if (!aTokenizer.more() || aTokenizer.peek().keyword != Keywords::until_kw) {
                                  endDate = startDate;
                              }
                              else {
                                  aTokenizer.next();
                                  if (aTokenizer.peek().type == TokenType::timedate) {
                                      aTokenizer.next();
                                      endDate = aTokenizer.current().data;
                                  }
                              }
                          }
                      }
                      else if (dataType == "time") {
                          if (aTokenizer.current().type == TokenType::timedate) {
                              time = aTokenizer.current().data;
                          }
                          if (aTokenizer.more()) {
                              if (aTokenizer.peek().keyword == Keywords::until_kw) {
                                  aTokenizer.next();
                                  if (aTokenizer.peek().type == TokenType::timedate) {
                                      aTokenizer.next();
                                      time += "-" + aTokenizer.current().data;
                                  }
                              }
                          }
                      }
                      else { //dataType == "with"

                      }
                  }
              }               
              break;
          }

          case (int)Keywords::with_kw: {
              if (aTokenizer.current().type == TokenType::string) {
                  with = aTokenizer.current().data;
              }
              break;
          }

          case (int)Keywords::at_kw: {
              if (aTokenizer.current().type == TokenType::timedate) {
                  time = aTokenizer.current().data;
              }
              if (aTokenizer.more()) {
                  if (aTokenizer.peek().keyword == Keywords::until_kw) {
                      aTokenizer.next();
                      if (aTokenizer.peek().type == TokenType::timedate) {
                          aTokenizer.next();
                          time += "-" + aTokenizer.current().data;
                      }
                  }
              }
              break;
          }

          case (int)Keywords::on_kw: {
              if (aTokenizer.current().type == TokenType::timedate) {
                  startDate = aTokenizer.current().data;
                  if (!aTokenizer.more() || aTokenizer.peek().keyword != Keywords::until_kw) {
                      endDate = startDate;
                  }
                  else {
                      aTokenizer.next();
                      if (aTokenizer.peek().type == TokenType::timedate) {
                          aTokenizer.next();
                          endDate = aTokenizer.current().data;
                      }
                  }
              }
              break;
          }

          default:
              break;
          }
      }

      return new Command(title, "", time, with);
  }

  int ShowProcessor::run(Command *aCommand, Calendar &aCal) {
      std::map<std::string, Event*> matchEvents;

      for (auto* event : aCal.getEvents()) {
          if (aCommand->title != "") 
              if (aCommand->title != event->title)
                  continue;

          if (aCommand->with != "")
              if (aCommand->with != event->with)
                  continue;
          
          /*if (aCommand->date != "") {
              int startDate = stoi(aCommand->startDate.substr(5, 2))*100 + stoi(aCommand->startDate.substr(8, 2));
              int endDate   = stoi(aCommand->endDate.substr(5, 2))*100 + stoi(aCommand->endDate.substr(8, 2));
              int eventDate = stoi(event->date.substr(5, 2)) * 100 + stoi(event->date.substr(8, 2));
              if (startDate > eventDate || eventDate > endDate)
                  continue;
          }*/

          if (aCommand->time != "") {
              int startTime = stoi(aCommand->time.substr(0, 2)) * 100 + stoi(aCommand->time.substr(3, 2));
              if (aCommand->time.substr(5, 2) == "pm")
                  startTime += 1200;

              int endTime = stoi(aCommand->time.substr(8, 2)) * 100 + stoi(aCommand->time.substr(11, 2));
              if (aCommand->time.substr(13, 2) == "pm")
                  endTime += 1200;

              int eventStartTime = stoi(event->time.substr(0, 2)) * 100 + stoi(event->time.substr(3, 2));
              if (event->time.substr(5, 2) == "pm")
                  eventStartTime += 1200;

              int eventEndTime = stoi(event->time.substr(8, 2)) * 100 + stoi(event->time.substr(11, 2));
              if (event->time.substr(13, 2) == "pm")
                  eventEndTime += 1200;

              if (eventEndTime < startTime || eventStartTime > endTime)
                  continue;
          }

          matchEvents[event->date] = event;
      }

      aCal.getStream()
          << "|---------------|---------------|-------------------|-------------------|\n"
          << "| Title         | Date          | Time              | With              |\n"
          << "|---------------|---------------|-------------------|-------------------|\n";
      
      for (auto event : matchEvents) {
          aCal.getStream()
              << "| " << std::setw(14) << std::left << event.second->title
              << "| " << std::setw(14) << std::left << event.second->date
              << "| " << std::setw(14) << std::left << event.second->time
              << "| " << std::setw(14) << std::left << event.second->with << '|\n';
      }
      aCal.getStream()
          << "|---------------|---------------|-------------------|-------------------|\n";

      View theView;
      theView.show(aCal.getStream());
      return kNoError;
  }

}
