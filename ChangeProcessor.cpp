//
//  ChangeProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#include <map>
#include <vector>

#include "ChangeProcessor.hpp"

namespace ECE141 {

  ChangeProcessor::ChangeProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext) {}

  ChangeProcessor::~ChangeProcessor() {}

  bool ChangeProcessor::recognizes(Tokenizer &aTokenizer) {
      aTokenizer.restart();
      return aTokenizer.skipTo(Keywords::change_kw);
  }

  Command* ChangeProcessor::makeCommand(Tokenizer &aTokenizer) {
      std::string oldTitle;
      std::string oldDate;
      std::string oldTime;
      std::string oldWith;
      std::map<std::string, std::string*> oldIdMap{ {"title", &oldTitle}, {"date", &oldDate}, {"time", &oldTime}, {"with", &oldWith} };

      std::string newTitle;
      std::string newDate;
      std::string newTime;
      std::string newWith;
      std::map<std::string, std::string*> newIdMap{ {"title", &newTitle}, {"date", &newDate}, {"time", &newTime}, {"with", &newWith} };

      aTokenizer.restart();
      bool existIf = aTokenizer.skipTo(Keywords::if_kw);
      
      aTokenizer.restart();

      while (aTokenizer.more()) {
          //std::cout << "here";
          int curKeyWord = (int)aTokenizer.current().keyword;

          switch (curKeyWord) {
          case (int)Keywords::change_kw: {//old data
              aTokenizer.next(); //eat change
              if (existIf) {
                  while (aTokenizer.more() && aTokenizer.current().keyword != Keywords::if_kw) {
                      int secKeyWord = (int)aTokenizer.current().keyword;
                      switch (secKeyWord) {
                      case (int)Keywords::unknown_kw: {
                          if (aTokenizer.current().type == TokenType::identifier) {
                              std::string id = aTokenizer.current().data;
                              aTokenizer.next();
                              if (aTokenizer.current().type == TokenType::punctuation) {
                                  //expecting a '='
                                  aTokenizer.next();
                                  if (aTokenizer.current().type == TokenType::string) {
                                      *oldIdMap[id] = aTokenizer.current().data;
                                      aTokenizer.next();
                                  }
                              }
                          }
                          break;
                      }

                      case (int)Keywords::with_kw: {
                          aTokenizer.next();
                          if (aTokenizer.current().type == TokenType::string) {
                              oldWith = aTokenizer.current().data;
                              aTokenizer.next();
                          }
                          break;
                      }

                      case (int)Keywords::at_kw: {
                          aTokenizer.next();
                          if (aTokenizer.current().type == TokenType::timedate) {
                              oldTime = aTokenizer.current().data;
                              aTokenizer.next();
                              if (aTokenizer.more() && aTokenizer.current().keyword == Keywords::until_kw) {
                                  aTokenizer.next();
                                  if (aTokenizer.current().type == TokenType::timedate) {
                                      oldTime += "-" + aTokenizer.current().data;
                                      aTokenizer.next();
                                  }
                              }
                          }
                          break;
                      }

                      case (int)Keywords::on_kw: {
                          aTokenizer.next();
                          if (aTokenizer.current().type == TokenType::timedate) {
                              oldDate = aTokenizer.current().data;
                              aTokenizer.next();
                              if (aTokenizer.more() && aTokenizer.current().keyword != Keywords::until_kw) {
                                  aTokenizer.next();
                                  if (aTokenizer.current().type == TokenType::timedate) {
                                      oldDate += " " + aTokenizer.current().data;
                                      aTokenizer.next();
                                  }
                              }
                          }
                          break;
                      }

                      default: {
                          aTokenizer.next();
                          break;
                      }
                      }
                  }
              }
              else {
                  int secKeyWord = (int)aTokenizer.current().keyword;
                  switch (secKeyWord) {
                  case (int)Keywords::unknown_kw: {
                      if (aTokenizer.current().type == TokenType::identifier) {
                          std::string id = aTokenizer.current().data;
                          aTokenizer.next();
                          if (aTokenizer.current().type == TokenType::punctuation) {
                              //expecting a '='
                              aTokenizer.next();
                              *oldIdMap[id] = aTokenizer.current().data;
                              aTokenizer.next();
                          }
                      }
                      else if (aTokenizer.current().type == TokenType::string) {
                          oldTitle = aTokenizer.current().data;
                          aTokenizer.next();
                      }
                      break;
                  }

                  case (int)Keywords::with_kw: {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::string) {
                          oldWith = aTokenizer.current().data;
                          aTokenizer.next();
                      }
                      break;
                  }

                  case (int)Keywords::at_kw: {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          oldTime = aTokenizer.current().data;
                          aTokenizer.next();
                          if (aTokenizer.more() && aTokenizer.current().keyword == Keywords::until_kw) {
                              aTokenizer.next();
                              if (aTokenizer.current().type == TokenType::timedate) {
                                  oldTime += "-" + aTokenizer.current().data;
                                  aTokenizer.next();
                              }
                          }
                      }
                      break;
                  }

                  case (int)Keywords::on_kw: {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          oldDate = aTokenizer.current().data;
                          aTokenizer.next();
                          if (aTokenizer.more() && aTokenizer.current().keyword != Keywords::until_kw) {
                              aTokenizer.next();
                              if (aTokenizer.current().type == TokenType::timedate) {
                                  oldDate += " " + aTokenizer.current().data;
                                  aTokenizer.next();
                              }
                          }
                      }
                      break;
                  }

                  default: {
                      aTokenizer.next();
                      break;
                  }
                  }
                  break;
              }
          }
          default: {//new data
              while (aTokenizer.more() && aTokenizer.current().keyword != Keywords::change_kw) {
                  //std::cout << "here";
                  std::string id = aTokenizer.current().data;
                  if (id == "")
                      id = "with";
                  aTokenizer.next();
                  if (aTokenizer.current().type == TokenType::punctuation) {
                      //expecting a '='
                      aTokenizer.next();
                      *newIdMap[id] = aTokenizer.current().data;
                      aTokenizer.next();
                  }
                  /*
                  case (int)Keywords::with_kw: {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::string) {
                          newWith = aTokenizer.current().data;
                          aTokenizer.next();
                      }
                      break;
                  }

                  case (int)Keywords::at_kw: {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          newTime = aTokenizer.current().data;
                          aTokenizer.next();
                          if (aTokenizer.more() && aTokenizer.current().keyword == Keywords::until_kw) {
                              aTokenizer.next();
                              if (aTokenizer.current().type == TokenType::timedate) {
                                  newTime += "-" + aTokenizer.current().data;
                                  aTokenizer.next();
                              }
                          }
                      }
                      break;
                  }

                  case (int)Keywords::on_kw: {
                      aTokenizer.next();
                      if (aTokenizer.current().type == TokenType::timedate) {
                          newDate = aTokenizer.current().data;
                          aTokenizer.next();
                          if (aTokenizer.more() && aTokenizer.current().keyword != Keywords::until_kw) {
                              aTokenizer.next();
                              if (aTokenizer.current().type == TokenType::timedate) {
                                  newDate += " " + aTokenizer.current().data;
                                  aTokenizer.next();
                              }
                          }
                      }
                      break;
                  }
                  */

              }
          }
          }
      }

      return new Command(oldTitle, oldDate, oldTime, oldWith, newTitle, newDate, newTime, newWith);
  }

  int ChangeProcessor::run(Command *aCommand, Calendar &aCal) {
      std::vector<Event*> matchEvents;
      
      for (auto* event : aCal.getEvents()) {
          if (aCommand->title != "") {
              if (aCommand->title != event->title)
                  continue;
          }

          if (aCommand->with != "") {
              if (aCommand->with != event->with)
                  continue;
          }

          if (aCommand->date != "") {
              int endDate = 0, eventEndDate = 0;

              int startDate = stoi(aCommand->date.substr(5, 2)) * 100 + stoi(aCommand->date.substr(8, 2));
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
                          std::string newDate = "2021-" + std::to_string(eventEndDate / 100) + "-" + std::to_string(eventEndDate % 100);
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
          matchEvents.push_back(event);
      }

      for (auto event : matchEvents) {
          if (aCommand->newTitle != "")
              event->title = aCommand->newTitle;
          if (aCommand->newDate != "")
              event->date = aCommand->newDate;
          if (aCommand->newTime != "")
              event->time = aCommand->newTime;
          if (aCommand->newWith != "")
              event->with = aCommand->newWith;
      }

      return kNoError;
  }

}
