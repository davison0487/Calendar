//
//  DeleteProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#include "DeleteProcessor.hpp"


namespace ECE141 {

  DeleteProcessor::DeleteProcessor(CommandProcessor *aNext)
    : CommandProcessor(aNext) {}

  DeleteProcessor::~DeleteProcessor() {}

  bool DeleteProcessor::recognizes(Tokenizer &aTokenizer) {
    return false;
  }

  Command* DeleteProcessor::makeCommand(Tokenizer &aTokenizer) {
    return nullptr;
  }

  int DeleteProcessor::run(Command *aCommand, Calendar &aCal) {
    return kNotImplemented;
  }

}
