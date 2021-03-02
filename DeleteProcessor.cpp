//
//  DeleteProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#include "DeleteProcessor.hpp"
#include <map>


namespace ECE141 {

  DeleteProcessor::DeleteProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext) {}

  DeleteProcessor::~DeleteProcessor() {}

  bool DeleteProcessor::recognizes(Tokenizer &aTokenizer) {
      aTokenizer.restart();
      return aTokenizer.skipTo(Keywords::delete_kw);
  }

  Command* DeleteProcessor::makeCommand(Tokenizer &aTokenizer) {
      std::string title;
      std::string date;
      std::string time;
      std::string with;

      std::map<std::string, std::string*> idMap{ {"title", &title}, {"date", &date}, {"time", &time}, {"with", &with} };

      aTokenizer.restart();
      while (aTokenizer.more()) {
          //std::cout << "here";
          int curKeyWord = (int)aTokenizer.current().keyword;

          switch (curKeyWord) {
          case (int)Keywords::unknown_kw: {
              if (aTokenizer.current().type == TokenType::identifier) {
                  std::string id = aTokenizer.current().data;
                  aTokenizer.next();
                  if (aTokenizer.current().type == TokenType::punctuation) {
                      //expecting a '='
                      aTokenizer.next();
                      *idMap[id] = aTokenizer.current().data;
                      aTokenizer.next();
                      if (aTokenizer.more() && aTokenizer.current().type == TokenType::timedate) {
                          if (id == "date") {
                              *idMap[id] += ' ' + aTokenizer.current().data;
                          }
                          else {
                              std::string endTime = aTokenizer.current().data;
                              if (endTime.size() != 7)
                                  endTime = '0' + endTime;
                              *idMap[id] += '-' + endTime;
                          }
                          aTokenizer.next();
                      }
                  }
              }
              break;
          }

          case (int)Keywords::date_kw: {
              aTokenizer.next();
              //expecting a '='
              aTokenizer.next();
              date = aTokenizer.current().data;
              aTokenizer.next();
              if (aTokenizer.more() && aTokenizer.current().type == TokenType::timedate) {
                  date += ' ' + aTokenizer.current().data;
                  aTokenizer.next();
              }
              break;
          }

          case (int)Keywords::time_kw: {
              aTokenizer.next();
              //expecting a '='
              aTokenizer.next();
              time = aTokenizer.current().data;
              if (time.size() != 7)
                  time = '0' + time;
              aTokenizer.next();
              if (aTokenizer.more() && aTokenizer.current().type == TokenType::timedate) {
                  std::string endTime = aTokenizer.current().data;
                  if (endTime.size() != 7)
                      endTime = '0' + endTime;
                  time += '-' + endTime;
                  aTokenizer.next();
              }
              break;
          }

          case (int)Keywords::at_kw: {
              aTokenizer.next();
              if (aTokenizer.current().type == TokenType::timedate) {
                  time = aTokenizer.current().data;
                  aTokenizer.next();
                  if (aTokenizer.more() && aTokenizer.current().keyword == Keywords::until_kw) {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          time += "-" + aTokenizer.current().data;
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
                  if (aTokenizer.more() && aTokenizer.current().keyword != Keywords::until_kw) {
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

  int DeleteProcessor::run(Command *aCommand, Calendar &aCal) {
      Events toBeDelete;

      for (auto* event : aCal.getEvents()) {
          if (aCommand->with != "" && aCommand->with != event->with) {
              continue;
          }
          
          if (aCommand->title != "" && aCommand->title != event->title) {
              continue;
          }

          if (aCommand->time != "") {
              int endTime = 0, eventEndTime = 0;

              int startTime = stoi(aCommand->time.substr(0, 2)) * 100 + stoi(aCommand->time.substr(3, 2));
              if (aCommand->time.substr(5, 2) == "pm")
                  startTime += 1200;
              if (aCommand->time.size() > 8) {
                  endTime = stoi(aCommand->time.substr(8, 2)) * 100 + stoi(aCommand->time.substr(11, 2));
                  if (aCommand->time.substr(13, 2) == "pm")
                      endTime += 1200;
              }
              else 
                  endTime = startTime;

              int eventStartTime = stoi(event->time.substr(0, 2)) * 100 + stoi(event->time.substr(3, 2));
              if (event->time.substr(5, 2) == "pm")
                  eventStartTime += 1200;
              if (event->time.size() > 8) {
                  eventEndTime = stoi(event->time.substr(8, 2)) * 100 + stoi(event->time.substr(11, 2));
                  if (event->time.substr(13, 2) == "pm")
                      eventEndTime += 1200;
              }
              else
                  eventEndTime = eventStartTime;

              if (eventEndTime < startTime || eventStartTime > endTime)
                  continue;
          }
          
          if (aCommand->date != "") {
              int endDate = 0, eventEndDate = 0;

              int startDate = stoi(aCommand->date.substr(5, 2))*100 + stoi(aCommand->date.substr(8, 2));
              if (aCommand->date.size() > 11)
                  endDate = stoi(aCommand->date.substr(16, 2)) * 100 + stoi(aCommand->date.substr(19, 2));              
              else
                  endDate = startDate;
              
              int eventStartDate = stoi(event->date.substr(5, 2)) * 100 + stoi(event->date.substr(8, 2));
              if (event->date.size() > 11)
                  eventEndDate = stoi(event->date.substr(16, 2)) * 100 + stoi(event->date.substr(19, 2));
              else
                  eventEndDate = eventStartDate;

              if (eventStartDate > endDate || eventEndDate < startDate)
                  continue;
              else {
                  if (eventStartDate < startDate) {
                      if (eventStartDate == startDate - 1) {
                          std::string newDate = event->date.substr(0, 10);
                          Event* newEvent = new Event(event->title, newDate, event->time, event->with);
                          aCal.addEvent(newEvent);
                      }
                      else {
                          std::string newDate = event->date.substr(0, 10) + " 2021-";
                          --startDate;

                          if (startDate / 100 < 10)
                              newDate += '0' + std::to_string(startDate / 100) + '-';
                          else
                              newDate += std::to_string(startDate / 100) + '-';

                          if (startDate % 100 < 10)
                              newDate += '0' + std::to_string(startDate % 100);
                          else
                              newDate += std::to_string(startDate % 100);

                          Event* newEvent = new Event(event->title, newDate, event->time, event->with);
                          aCal.addEvent(newEvent);
                      }
                  }
                  if (eventEndDate > endDate) {
                      if (eventEndDate == endDate + 1) {
                          std::string newDate = "2021-" + std::to_string(eventEndDate/100) + "-" + std::to_string(eventEndDate % 100);
                          Event* newEvent = new Event(event->title, newDate, event->time, event->with);
                          aCal.addEvent(newEvent);
                      }
                      else {
                          ++endDate;
                          std::string newDate = "2021-" + std::to_string(endDate / 100) + "-" + std::to_string(endDate % 100);
                          
                          if (endDate / 100 < 10)
                              newDate += '0' + std::to_string(endDate / 100) + '-';
                          else
                              newDate += std::to_string(endDate / 100) + '-';

                          if (endDate % 100 < 10)
                              newDate += '0' + std::to_string(endDate % 100);
                          else
                              newDate += std::to_string(endDate % 100);

                          newDate += " 2021-" + std::to_string(eventEndDate / 100) + "-" + std::to_string(eventEndDate % 100);
                          Event* newEvent = new Event(event->title, newDate, event->time, event->with);
                          aCal.addEvent(newEvent);
                      }
                  }
              }
          }
          toBeDelete.push_back(event);
      }
      
      for (auto* currDelete : toBeDelete)
          aCal.removeEvent(currDelete);

      return kNoError;
  }

}
