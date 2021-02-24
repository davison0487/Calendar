//
//  ChangeProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/24/21.
//

#ifndef ChangeProcessor_hpp
#define ChangeProcessor_hpp


#include <stdio.h>
#include "CommandProcessor.hpp"

namespace ECE141 {

  class ChangeProcessor : public CommandProcessor {
  public:
        
    ChangeProcessor(CommandProcessor *aNext=nullptr);
    ~ChangeProcessor();

    virtual bool      recognizes(Tokenizer &aTokenizer);
    virtual Command*  makeCommand(Tokenizer &aTokenizer);
    virtual int       run(Command *aCommand, Calendar &aCal);
  };

}


#endif /* ChangeProcessor_hpp */
