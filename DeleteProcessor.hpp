//
//  DeleteProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#ifndef DeleteProcessor_hpp
#define DeleteProcessor_hpp


#include <stdio.h>
#include "CommandProcessor.hpp"

namespace ECE141 {

  class DeleteProcessor : public CommandProcessor {
  public:
    
    DeleteProcessor(CommandProcessor *aNext=nullptr);
    ~DeleteProcessor();

    virtual bool      recognizes(Tokenizer &aTokenizer);
    virtual Command*  makeCommand(Tokenizer &aTokenizer);
    virtual int       run(Command *aCommand, Calendar &aCal);
  };

}


#endif /* DeleteProcessor_hpp */
