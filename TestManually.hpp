//
//  ManualTest.hpp
//  Assignment4
//
//  Created by rick gessner on 2/20/21.
//

#ifndef TestManually_h
#define TestManually_h

#include <map>
#include <sstream>

#include "AddProcessor.hpp"
#include "QuitProcessor.hpp"
#include "ChangeProcessor.hpp"
#include "DeleteProcessor.hpp"
#include "ShowProcessor.hpp"

namespace ECE141 {

  // USE: ---------------------------------------------

  static std::map<int, std::string> theErrorMessages = {
    {ECE141::kUnknownCommand, "Unknown command"},
    {ECE141::kSyntaxError,    "Syntax error"},
    {ECE141::kUnknownToken,   "Unknown token"}
  };

  void showError(int &aResult) {
    std::string theMessage="Unknown Error";
    if(theErrorMessages.count(aResult)) {
      theMessage=theErrorMessages[aResult];
    }
    std::cout << "Error (" << aResult << ") " << theMessage << "\n";
  }
    
  class TestManually {
  public:
    
    bool testIncrementally() {

        //STUDENT: Add other processors (change, delete, show)
        //         to the chain-of-responsibililty below...

      //insert more processors here...
      ECE141::AddProcessor theAddProc(nullptr);
      ECE141::QuitProcessor theQuitProc(&theAddProc);
      ECE141::ChangeProcessor theChangeProc(&theQuitProc);
      ECE141::DeleteProcessor theDeleteProc(&theChangeProc);
      ECE141::ShowProcessor theShowProc(&theDeleteProc);

      std::string theUserInput;
      Calendar theCalendar(std::cout); //for testing...
      bool running=true;
      while(running) {
        std::cout << "\n> ";
        if(std::getline(std::cin, theUserInput)) {
          if(theUserInput.length()) {
            std::stringstream theStream(theUserInput);
            ECE141::Tokenizer theTokenizer(theStream);
            int theResult=theTokenizer.tokenize();
            
            if(ECE141::kNoError==theResult) {
              if(auto *theHandler=theShowProc.findHandler(theTokenizer)) {
                if(auto *theCmd=theHandler->makeCommand(theTokenizer)) {
                  theResult=theHandler->run(theCmd, theCalendar);
                  if(ECE141::kNoError!=theResult) {
                    if(ECE141::kTerminate==theResult) running=false;
                    else showError(theResult);
                  }
                  delete theCmd;
                }
              }
              else showError(theResult);
            }
          }
        }
      } //while
      return true;
    } //manualtest
    
  };

} //ece141


#endif /* TestManually_h */
