//
//  ShowProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#include "ShowProcessor.hpp"


namespace ECE141 {

  ShowProcessor::ShowProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext) {}

  ShowProcessor::~ShowProcessor() {}

  bool ShowProcessor::recognizes(Tokenizer &aTokenizer) {
    return false;
  }

  Command* ShowProcessor::makeCommand(Tokenizer &aTokenizer) {
    return nullptr;
  }

  int ShowProcessor::run(Command *aCommand, Calendar &aCal) {
    return kNotImplemented;
  }

}
