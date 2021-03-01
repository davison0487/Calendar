//
//  View.hpp
//  Assignment4
//
//  Created by rick gessner on 2/22/21.
//

#ifndef IView_hpp
#define IView_hpp

#include <stdio.h>
#include <iostream>
#include <sstream>

namespace ECE141 {
    
  class IView {
  public:
    virtual void show(std::ostream &anOutput)=0;
  };

  class View : public IView {
  public:
      virtual void show(std::ostream& anOutput) {
         
      };

  };

}

#endif /* IView_hpp */
