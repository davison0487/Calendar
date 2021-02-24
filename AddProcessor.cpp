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
    return false; //gotta improve this!
  }

  Command* AddProcessor::makeCommand(Tokenizer &aTokenizer) {
    return nullptr;
  }

  int AddProcessor::run(Command *aCommand, Calendar &aCal) {
    return kNoError;
  }

}
