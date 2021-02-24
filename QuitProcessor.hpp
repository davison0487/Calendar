//
//  QuitProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/19/21.
//

#ifndef QuitProcessor_hpp
#define QuitProcessor_hpp

#include <stdio.h>

#include "CommandProcessor.hpp"

namespace ECE141 {

  class QuitProcessor : public CommandProcessor {
  public:
    
    QuitProcessor(CommandProcessor *aNext=nullptr);
    ~QuitProcessor();

    virtual bool      recognizes(Tokenizer &aTokenizer);
    virtual Command*  makeCommand(Tokenizer &aTokenizer);
    virtual int       run(Command *aCommand, Calendar &aCal);

  };

}

#endif /* QuitProcessor_hpp */
