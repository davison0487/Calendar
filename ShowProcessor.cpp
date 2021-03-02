//
//  ShowProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#include <map>
#include <vector>
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
      std::string title;
      std::string date;
      std::string time;
      std::string with;

      std::map<std::string, std::string*> idMap{ {"title", &title}, {"date", &date}, {"time", &time}, {"with", &with} };
      
      aTokenizer.restart();

      while (aTokenizer.more()) {
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

          default: {
              aTokenizer.next();
              break;
          }
          }
      }

      return new Command(title, date, time, with);
  }

  int ShowProcessor::run(Command *aCommand, Calendar &aCal) {
      std::map<std::string, std::vector<Event*>> matchEvents;

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

          matchEvents[event->date].push_back(event);
      }

      if (matchEvents.size() == 0) {
          aCal.getStream() << "No scheduled events found.\n";
      }
      else {
          aCal.getStream()
              << "|---------------|---------------|-------------------|-------------------|\n"
              << "| Title         | Date          | Time              | With              |\n"
              << "|---------------|---------------|-------------------|-------------------|\n";

          for (auto eventVec : matchEvents) {
              for (auto event : eventVec.second) {
                  aCal.getStream()
                      << "| " << std::setw(14) << std::left << event->title;
                  if (aCommand->date != "")
                      aCal.getStream() << "| " << std::setw(14) << std::left << aCommand->date.substr(0, 10);
                  else
                      aCal.getStream() << "| " << std::setw(14) << std::left << event->date;
                  aCal.getStream()
                      << "| " << std::setw(18) << std::left << event->time
                      << "| " << std::setw(18) << std::left << event->with << "|\n";
              }
              
          }
          aCal.getStream()
              << "|---------------|---------------|-------------------|-------------------|\n";

      }

      View theView;
      theView.show(aCal.getStream());

      return kNoError;
  }

}
