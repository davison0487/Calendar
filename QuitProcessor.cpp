//
//  QuitProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/19/21.
//

#include "QuitProcessor.hpp"


namespace ECE141 {

  QuitProcessor::QuitProcessor(CommandProcessor *aNext)
        : CommandProcessor(aNext) {}

  QuitProcessor::~QuitProcessor() {}

  bool QuitProcessor::recognizes(Tokenizer &aTokenizer) {
    return aTokenizer.skipTo(Keywords::quit_kw);
  }

  Command* QuitProcessor::makeCommand(Tokenizer &aTokenizer) {
    return new Command();
  }

  int QuitProcessor::run(Command *aCommand, Calendar &aCal) {
    return kTerminate;
  }

}
