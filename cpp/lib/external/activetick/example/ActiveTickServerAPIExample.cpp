#include "StdAfx.h"
#include "Helper.h"
#include "Requestor.h"
#include "Session.h"
#include "Streamer.h"
#include <ActiveTickServerAPI/ActiveTickServerAPI.h>

#include <iostream>
#include <fstream>
// #include <ifstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>
#include <sys/stat.h>

#include <unistd.h>

using namespace std;

// TODO: import file with symbols

string intToString(uint64_t i)	// convert int to string
{
  stringstream s;
  s << i;
  return s.str();
}

std::vector<std::string> getSymbols()
{
  string symbol;
  std::vector<std::string> symbols;
  ifstream symbol_file ("/home/william/projects/ActiveTickHttpInterface/ActiveTickHttpInterface/symbols/new_symbols.txt", ios::in);
  if (symbol_file.is_open()) {
    while ( getline(symbol_file, symbol)) {
      symbols.push_back(symbol);
    }
    symbol_file.close();
  }
  return symbols;
}

int FileExists(const char *path)
{
    struct stat fileStat;
    if ( stat(path, &fileStat) )
    {
        return 0;
    }
    if ( !S_ISREG(fileStat.st_mode) )
    {
        return 0;
    }
    return 1;
}

int main(int argc, char** argv)
{
	ATInitAPI();
  unsigned int seconds = 7;
  unsigned int microseconds = seconds * 1000000;

  //TODO: gonna have to have a "status struct" or whatever to pass to every class
  //      and grab the proper status of every command we need to know
  //      example: login status if it's logged in we can continue, etc. also for
  //      the tickers to know that we received all the data before leacving the program
    string date = string(argv[1]);

    // std::vector<string> temp_symbols = getSymbols();
    // // std::vector<string> temp_symbols{"CALI"};
    //
    string temp;
    // vector<string> symbols;
    // for (int i = 0; i < temp_symbols.size(); i++) {
    //   temp = "/home/william/projects/stocks/data/" + date + "/" + temp_symbols[i] + ".csv";
    //   if (!FileExists( temp.c_str() )) {
    //     symbols.push_back(temp_symbols[i]);
    //   }
    // }
    //
    // cout << symbols.size() << endl;
    // for (int i = 0; i < symbols.size(); i++) {
    //   cout << symbols[i] << endl;
    // }
    vector<string> temp_symbols{"BIOC"};
    vector<string> symbols{"BIOC"};

		APISession session;
		Requestor requestor(session, date);
		Streamer streamer(session, date);

		string serverIpAddress, apiUserid, userid, password;
		uint32_t serverPort = 0;
		serverIpAddress = "activetick1.activetick.com";
		serverPort = 443;
		apiUserid = "8c35815547ae49a8834dd7644025eb2a";
		userid = "108empire";
		password =  "zizou777";

		ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
		bool rc = session.Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());
		printf("init status: %d\n", rc);

    int count = 0;
    int limit = 100;
    bool wait = true;
    string pause = "";
    usleep(microseconds);
    uint64_t beginTime = stoi(string(argv[1]));
    cout << beginTime << endl;
    beginTime = beginTime*1000000;
    uint64_t endTime = beginTime + 235959;
    cout << beginTime << endl;
    cout << endTime << endl;

    while (pause != "q") {
      string symbol;
      limit += count;
      if (symbols.size() < limit) {
        limit = symbols.size();
      }
      if (limit <= count) {
        symbols.clear();
        usleep(microseconds);
        for (int i = 0; i < temp_symbols.size(); i++) {
          temp = "/home/william/projects/stocks/data/" + date + "/" + temp_symbols[i] + ".csv";
          if (!FileExists( temp.c_str() )) {
            symbols.push_back(temp_symbols[i]);
          }
        }
        if (symbols.size() == 0) {
          cout << "quitting" << endl;
          break;
        }
        count=0;
        limit = 100;
      }
			for (int i=count; i < limit;  i++) {
        symbol = symbols[i];
				ATTIME atBeginTime = Helper::StringToATTime(intToString(beginTime));
				ATTIME atEndTime = Helper::StringToATTime(intToString(endTime));
				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				uint64_t request = requestor.SendATTickHistoryDbRequest(atSymbol, true, false, atBeginTime, atEndTime, 3000000);
				printf("SEND (%lu): Tick history request [%s]\n", request, symbol.c_str());
        count++;
			}
      usleep(microseconds);
  	}

	ATShutdownAPI();

	return 0;
}
