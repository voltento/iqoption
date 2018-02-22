//
// Created by v on 22.02.18.
//

#ifndef RECIVER_STORAGE_H
#define RECIVER_STORAGE_H

#include "DataProvider.h"

class Storage {
public:
   explicit Storage(DataProvider* dataProvider);
private:
    DataProvider* dataProvider;
};


#endif //RECIVER_STORAGE_H
