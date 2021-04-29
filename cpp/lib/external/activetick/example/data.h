#include <string>
#include <map>
#include <vector>
#include <ActiveTickServerAPI/ActiveTickServerAPI.h>

#ifndef DATA_H
#define DATA_H

struct SYMBOL_STRUCT {
  uint16_t count;
  float price;
	uint64_t volume;
};

class Data {
public:
    Data(std::vector<std::string>);

public:
		std::map<std::string, SYMBOL_STRUCT> SYMB;
};

#endif /* DATA_H */
