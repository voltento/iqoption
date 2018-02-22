//
// Created by v on 22.02.18.
//

#include "Storage.h"

Storage::Storage(DataProvider* dataProvider) : dataProvider(dataProvider)
{
    if(dataProvider == nullptr)
        throw std::runtime_error("Nullptr was sent as dataProvider'");
}
