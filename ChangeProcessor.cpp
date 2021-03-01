//
//  ChangeProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

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
      //TODO
      std::string title;
      std::string startDate;
      std::string endDate;
      std::string time;
      std::string with;

      aTokenizer.restart();

      while (aTokenizer.more()) {
          if (aTokenizer.current().type != TokenType::keyword) {
              aTokenizer.next();
              continue;
          }

          int curKeyWord = (int)aTokenizer.current().keyword;
          aTokenizer.next(); //eat the keyword

          switch (curKeyWord) {
          case (int)Keywords::add_kw: {
              if (aTokenizer.current().type == TokenType::string) {
                  title = aTokenizer.current().data;
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
              }
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
              break;
          }

          case (int)Keywords::with_kw: {
              if (aTokenizer.current().type == TokenType::string) {
                  with = aTokenizer.current().data;
              }
              break;
          }

          default:
              break;
          }

          aTokenizer.next();
      }

      return new Command(title, "", time, with);
  }

  int ChangeProcessor::run(Command *aCommand, Calendar &aCal) {
    return kNotImplemented;
  }

}
