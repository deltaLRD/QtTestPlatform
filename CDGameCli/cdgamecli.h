#ifndef CDGAMECLI_H
#define CDGAMECLI_H

#include <functional>
#include "DataStruct.h"

class CDGameCli
{
public:
    CDGameCli(int argc, char *argv[], std::function<void(DataStruct*data,KeyStruct*key)> aiPve,
              std::function<void(DataStruct*data,KeyStruct*key)> aiPvp);
};

#endif // CDGAMECLI_H
