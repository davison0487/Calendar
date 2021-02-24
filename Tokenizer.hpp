//
//  Tokenizer.hpp
//  Assignment4
//
//  Created by rick gessner on 2/17/21.
//


#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <iostream>
#include <vector>
#include <cstring>
#include "Parser.hpp"

namespace ECE141 {

  const int kNoError=0;
  const int kNotImplemented = 1000;
  const int kTerminate = 1100;
  const int kSyntaxError = 1200;
  const int kUnknownCommand = 1300;
  const int kUnknownToken = 1400;

  //----------------------------------------------------


  //This enum defines operators that will be used in SQL commands...
  enum class Operators {
    equal_op=1, unknown_op
  };

  enum class Keywords {
    add_kw=1, at_kw, change_kw, create_kw, date_kw, delete_kw,
    if_kw, it_kw, on_kw, show_kw, time_kw, quit_kw, until_kw,
    with_kw, unknown_kw
  };

  enum class TokenType {
    identifier, keyword, number, punctuation, string, timedate, unknown
  };
  
  //-----------------
  
  struct Token {
    
    Token(TokenType aType, Keywords aKeyWord, Operators anOp=Operators::unknown_op)
    : type(aType), keyword(aKeyWord), op(anOp) {}
    
    Token& operator=(const Token &aCopy) {
      type=aCopy.type;
      keyword=aCopy.keyword;
      data=aCopy.data;
      return *this;
    }
    
    TokenType   type;
    Keywords    keyword;
    Operators   op;
    std::string data;
  };
 
  //-----------------
  
  class Tokenizer : public Parser {
  public:
    Tokenizer(std::istream &anInputStream);
    
    int           tokenize();
    Token&        tokenAt(size_t anOffset);

    Token&        current();
    bool          more() {return index<size();}
    bool          next(int anOffset=1);
    Token&        peek(int anOffset=1);
    void          restart() {index=0;}
    size_t        size() {return tokens.size();}
    size_t        remaining() {return index<size() ? size()-index :0;}

                          //these might consume a token...
    bool          skipTo(Keywords aKeyword);
    bool          skipTo(TokenType aTokenType);

    bool          skipIf(Keywords aKeyword);
    bool          skipIf(Operators anOperator);
    bool          skipIf(TokenType aTokenType);
    bool          skipIf(char aChar);
    
    void          dump();

  protected:
    
    std::vector<Token>    tokens;    
    size_t                index;
  };
  
}

#endif /* Tokenizer_hpp */


