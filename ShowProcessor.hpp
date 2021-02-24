//
//  ShowProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#ifndef ShowProcessor_hpp
#define ShowProcessor_hpp


#include <stdio.h>
#include "CommandProcessor.hpp"

namespace ECE141 {

  class ShowProcessor : public CommandProcessor {
  public:
    
    ShowProcessor(CommandProcessor *aNext=nullptr);
    ~ShowProcessor();

    virtual bool      recognizes(Tokenizer &aTokenizer);
    virtual Command*  makeCommand(Tokenizer &aTokenizer);
    virtual int       run(Command *aCommand, Calendar &aCal);
    
  };

}

#endif /* ShowProcessor_hpp */
