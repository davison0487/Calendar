//
//  AddProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/19/21.
//

#ifndef AddProcessor_hpp
#define AddProcessor_hpp

#include <stdio.h>

#include "CommandProcessor.hpp"

namespace ECE141 {

  class AddProcessor : public CommandProcessor {
  public:
    
    AddProcessor(CommandProcessor *aNext=nullptr);
    ~AddProcessor();

    virtual bool      recognizes(Tokenizer &aTokenizer);
    virtual Command*  makeCommand(Tokenizer &aTokenizer);
    virtual int       run(Command *aCommand, Calendar &aCal);

  };

}

#endif /* AddProcessor_hpp */
