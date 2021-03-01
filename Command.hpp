//
//  Command.hpp
//  Assignment4
//
//  Created by rick gessner on 2/20/21.
//

#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include <string>

namespace ECE141 {
    
    //Store cmd-related information in this object..
    //Make any changes you want...

    enum class commandType {Add, Change, Delete, Show, Quit};
    
    class Command {
    public:
        Command() {};
        Command(const std::string& aTitle,
                const std::string& aDate,
                const std::string& aTime,
                const std::string& aPerson);
    
        std::string title;
        std::string date;
        std::string time;
        std::string with;
    };

}

#endif /* Command_hpp */
