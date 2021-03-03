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
                          date += "-" + aTokenizer.current().data;
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
              int startTime = std::stoi(event->time.substr(0, event->time.find_first_of(":"))) * 100 +
                  std::stoi(event->time.substr(event->time.find_first_of(":") + 1, 2));
              int endTime = startTime;

              int eventStartTime = std::stoi(aCommand->time.substr(0, aCommand->time.find_first_of(":"))) * 100 +
                  std::stoi(aCommand->time.substr(aCommand->time.find_first_of(":") + 1, 2));
              int eventEndTime = eventStartTime;

              if (eventEndTime < startTime || eventStartTime > endTime)
                  continue;
          }
          
          if (aCommand->date != "") {
              if (aCommand->date.substr(5, 2) != event->date.substr(5, 2))
                  continue;
              else {
                  /*int eventStartDay = std::stoi(event->date.substr(8, 2));
                  int eventEndDay = std::stoi(event->date.substr(event->date.find_last_of("-") + 1, 2));

                  int startDay = std::stoi(aCommand->date.substr(8, 2));
                  int endDay = std::stoi(aCommand->date.substr(aCommand->date.find_last_of("-") + 1, 2));*/

                  if (aCommand->date != event->date)
                      continue;
              }
          }
          toBeDelete.push_back(event);
      }
      
      for (auto* currDelete : toBeDelete)
          aCal.removeEvent(currDelete);

      return kNoError;
  }

}
