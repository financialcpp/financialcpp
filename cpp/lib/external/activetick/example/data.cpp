#include "data.h"
#include <iostream>

Data::Data(std::vector<std::string> symbols) {
  	// std::cout << symbols[1] << std::endl;
  std::vector<std::string>::iterator it;
  int i = 0;
  for (it=symbols.begin(); it < symbols.end(); ++it) {
    SYMB[*it].count = 0;
    SYMB[*it].price = 0;
    SYMB[*it].volume = 0;
  }
}
