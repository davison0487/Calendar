//
//  Tokenizer.cpp
//  Assignment4
//
//  Created by rick gessner on 2/17/21.
//


#include "Tokenizer.hpp"
#include <exception>
#include <algorithm>
#include <map>

namespace ECE141 {
    
  static std::map<TokenType, const char*> kTypes {
    {TokenType::identifier, "id"},
    {TokenType::keyword, "keyword"},
    {TokenType::number, "num"},
    {TokenType::punctuation, "punc"},
    {TokenType::string, "str"},
    {TokenType::timedate, "time/date"},
    {TokenType::unknown, "unknown"}
  };

  static std::map<std::string,  Keywords> kDictionary = {
    {"add", Keywords::add_kw},
    {"at", Keywords::at_kw},
    {"change", Keywords::change_kw},
    {"create", Keywords::create_kw},
    {"delete", Keywords::delete_kw},
    {"if", Keywords::if_kw},
    {"it", Keywords::it_kw},
    {"on", Keywords::on_kw},
    {"quit", Keywords::quit_kw},
    {"show", Keywords::show_kw},
    {"until", Keywords::until_kw},
    {"with", Keywords::with_kw},
    {"unknown", Keywords::unknown_kw}
  };
  

  bool isWhitespace(char aChar) {
    static const char* theWS = " \t\r\n\b";
    return strchr(theWS,aChar);
  }

  bool isNumber(char aChar) {
    return isdigit(aChar) || '.'==aChar;
  }

  bool isTimeDate(char aChar) {
    return strchr("01234567890:-apm", aChar);
  }

  bool isAlphaNum(char aChar) {
    return isalnum(aChar) || '_'==aChar;
  }

  bool isQuote(char aChar) {
    return quote==aChar;
  }

  bool isOperator(char aChar) {
    return strchr("+-/*%=>!<.", aChar);
  }

  bool isPunctuation(char aChar) {
    return strchr("()[]{}:;,=", aChar);
  }
  
  //-----------------------------------------------------

  Tokenizer::Tokenizer(std::istream &anInput)
    : Parser(anInput), index(0)  {
      skipWhile(isWhitespace);
  }

  Token& Tokenizer::tokenAt(size_t anOffset) {
    if(anOffset>=0 && anOffset<tokens.size()) {
      return tokens[anOffset];
    }
    throw std::out_of_range("invalid offset");
  }

  bool Tokenizer::next(int anOffset) {
    if(index+anOffset<=size()) {
      index+=anOffset;
      return true;
    }
    return false;
  }

  Token& Tokenizer::current() {
    if(more()) {
      return tokens[index];
    }
    throw std::out_of_range("invalid offset");
  }

  Token& Tokenizer::peek(int anOffset) {
    return tokenAt(index+anOffset);
  }

  bool Tokenizer::skipTo(TokenType aType) {
    while (more()) {
      Token &theToken=current();
      if(theToken.type==aType) {
        return true;
      }
      next();
    }
    return false;
  }

  bool Tokenizer::skipTo(Keywords aKeyword) {
    while (more()) {
      Token &theToken=current();
      if(TokenType::keyword==theToken.type && aKeyword==theToken.keyword) {
        return true;
      }
      next();
    }
    return false;
  }

  bool Tokenizer::skipIf(TokenType aType) {
    if (more() && (aType==current().type)) {
      next(); //eat the target...
      return true;
    }
    return false;
  }

  bool Tokenizer::skipIf(Keywords aKeyword) {
    if (more() && (aKeyword==current().keyword)) {
      next(); //eat the target...
      return true;
    }
    return false;
  }

  // USE: skip a regular char, usually punctuation....
  bool Tokenizer::skipIf(char aChar) {
    if(more() && (aChar==current().data[0])) {
      next(); //eat the target...
      return true;
    }
    return false;
}


  //run on input provided in constructor; produce vector of tokens...
  int Tokenizer::tokenize() {
    
    int   theResult{kNoError};
    
    while(!input.eof()) {
      skipWhile(isWhitespace);
      char theChar = input.peek();
      if(isPunctuation(theChar)) {
        Token theToken{TokenType::punctuation, Keywords::unknown_kw, Operators::unknown_op};
        theToken.data.push_back(input.get());
        tokens.push_back(theToken);
      }
      else if(isNumber(theChar)) {
        Token theToken{TokenType::timedate, Keywords::unknown_kw, Operators::unknown_op};
        theToken.data=readWhile(isTimeDate);
        tokens.push_back(theToken);
      }
      else if(isQuote(theChar) || (apostrophe==theChar)) {
        input.get(); //skip first quote...
        Token theToken{TokenType::string, Keywords::unknown_kw, Operators::unknown_op};
        theToken.data=readUntil(theChar,false);
        tokens.push_back(theToken);
      }
      else if(isAlphaNum(theChar)) {
        std::string theString = readWhile(isAlphaNum);
        std::string temp(theString);
        std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if(kDictionary.count(temp)) {
          Token theToken{TokenType::keyword, kDictionary[temp]};
          tokens.push_back(theToken);
        } else {
          Token theToken{TokenType::identifier,Keywords::unknown_kw};
          theToken.data=temp;
          tokens.push_back(theToken);
        }
      }
      else return kSyntaxError;
    }
    
    return theResult;
  }
  
  void Tokenizer::dump() {
    for(auto theToken : tokens) {
                  
      std::cout << kTypes[theToken.type] << " ";
      
      if(TokenType::keyword==theToken.type) {
        for(auto &theIter : kDictionary) {
          if(theIter.second == theToken.keyword) {
            std::cout << theIter.first << "";
            break;
          }
        }
      }
      
      if(TokenType::string==theToken.type || TokenType::timedate==theToken.type) {
        std::cout << '"' << theToken.data << '"';
      }
      std::cout << "\n";

    }
  }
}
