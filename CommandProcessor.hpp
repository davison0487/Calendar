//
//  CommandProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 2/17/21.
//

#ifndef CommandProcessor_hpp
#define CommandProcessor_hpp

#include <stdio.h>
#include "Command.hpp"
#include "Tokenizer.hpp"
#include "Calendar.hpp"

namespace ECE141 {
  
  //make subclasses for your own commands...
  class CommandProcessor  {
  public:
                      CommandProcessor(CommandProcessor *aNext=nullptr);
              virtual ~CommandProcessor();
    
    //return ptr to self if you can handle command; or let next try...
    CommandProcessor* findHandler(Tokenizer &aTokenizer); //general...
    
    virtual bool      recognizes(Tokenizer &aTokenizer)=0;
    virtual Command*  makeCommand(Tokenizer &aTokenizer)=0;
    virtual int       run(Command *aCommand, Calendar &aCal)=0;

    CommandProcessor *next;
  };

}    

#endif /* CommandProcessor_hpp */
