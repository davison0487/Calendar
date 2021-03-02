//
//  Command.cpp
//  Assignment4
//
//  Created by rick gessner on 2/20/21.
//

#include "Command.hpp"

namespace ECE141 {

    Command::Command(const std::string& aTitle,
                     const std::string& aDate,
                     const std::string& aTime,
                     const std::string& aPerson) :
        title(aTitle), date(aDate), time(aTime), with(aPerson),
        newTitle(""), newDate(""), newTime(""), newWith("") {};

    Command::Command(const std::string& aTitle,
            const std::string& aDate,
            const std::string& aTime,
            const std::string& aPerson,
            const std::string& aNewTitle,
            const std::string& aNewDate,
            const std::string& aNewTime,
            const std::string& aNewPerson) :
        title(aTitle), date(aDate), time(aTime), with(aPerson),
        newTitle(aNewTitle), newDate(aNewDate),
        newTime(aNewTime), newWith(aNewPerson) {};

}
