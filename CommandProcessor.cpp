//
//  CommandProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 2/17/21.
//

#include "CommandProcessor.hpp"

namespace ECE141 {

  //----------------------------------------------------------

  CommandProcessor::CommandProcessor(CommandProcessor *aNext) {
    next=aNext;
  }

  CommandProcessor::~CommandProcessor() {
  }

  CommandProcessor* CommandProcessor::findHandler(Tokenizer &aTokenizer) {
    aTokenizer.restart();
    if(recognizes(aTokenizer)) {
      return this;
    }
    return next ? next->findHandler(aTokenizer) : nullptr;
  }

}
