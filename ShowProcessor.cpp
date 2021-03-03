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
              if (aCommand->date.substr(5, 2) != event->date.substr(5, 2))
                  continue;
              else {
                  int eventStartDay = std::stoi(event->date.substr(8, 2));
                  int eventEndDay = std::stoi(event->date.substr(event->date.find_last_of("-") + 1, 2));

                  int startDay = std::stoi(aCommand->date.substr(8, 2));
                  int endDay = std::stoi(aCommand->date.substr(aCommand->date.find_last_of("-") + 1, 2));

                  if (eventStartDay > endDay && eventEndDay < startDay)
                      continue;
              }
          }

          if (aCommand->time != "") {
              if (aCommand->time != event->time)
                  continue;
          }

          matchEvents[event->date].push_back(event);
      }

      if (matchEvents.size() == 0) {
          aCal.getStream() << "No scheduled events found.\n";
      }
      else {
          aCal.getStream()
              << "|--------------------|-----------------------|-----------------|-----------------|\n"
              << "| Title              | Date                  | Time            | With            |\n"
              << "|--------------------|-----------------------|-----------------|-----------------|\n";

          for (auto eventVec : matchEvents) {
              for (auto event : eventVec.second) {
                  aCal.getStream()
                      << "| " << std::setw(19) << std::left << event->title
                      << "| " << std::setw(22) << std::left << event->date
                      << "| " << std::setw(16) << std::left << event->time
                      << "| " << std::setw(16) << std::left << event->with << "|\n";
              }
              
          }
          aCal.getStream()
              << "|--------------------|-----------------------|-----------------|-----------------|\n";

      }

      View theView;
      theView.show(aCal.getStream());

      return kNoError;
  }

}
