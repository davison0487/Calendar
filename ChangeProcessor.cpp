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
    return false;
  }

  Command* ChangeProcessor::makeCommand(Tokenizer &aTokenizer) {
    return nullptr;
  }

  int ChangeProcessor::run(Command *aCommand, Calendar &aCal) {
    return kNotImplemented;
  }

}
