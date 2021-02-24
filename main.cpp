//
//  main.cpp
//  Assignment4
//
//  Created by rick gessner on 2/17/21.
//

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>

#include "Tokenizer.hpp"
#include "TestManually.hpp"
#include "TestAutomatic.hpp"


//----------------------------------------------------------

int ECE141::Event::leaks=0;

int main(int argc, const char * argv[]) {
  
  const char* theStatus[]={"FAIL","PASS"};
        
  // insert code here...
  if(argc>1) {
        
    ECE141::TestAutomatic theTests;
    
    srand(static_cast<uint>(time(0)));

    std::string temp(argv[1]);
    //temp="compile";
    
    if("compile"==temp) {
      std::cout << "Compile test " << theStatus[theTests.doCompileTest()] << ".\n";
    }
    else if("add"==temp) {
      std::cout << "Add test "
        << theStatus[theTests.doAddTest()] << ".\n";
    }
    else if ("change"==temp) {
      std::cout << "Change test "
        << theStatus[theTests.doChangeTest()] << ".\n";
    }
    else if ("delete"==temp) {
      std::cout << "Delete test "
        << theStatus[theTests.doDeleteTest()] << ".\n";
    }
    else if ("show"==temp) {
      std::cout << "Show test "
        << theStatus[theTests.doShowTest()] << ".\n";
    }    
    else if ("stress"==temp) {
      std::cout << "Stress test "
        << theStatus[theTests.doStressTest()] << ".\n";
    }

  }
  else {
    ECE141::TestManually theTest;
    theTest.testIncrementally();
  }
  
}
