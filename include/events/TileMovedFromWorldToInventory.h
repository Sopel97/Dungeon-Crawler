#pragma once

#include "TileTransferMediator.h"

class TileStack;

class TileMovedFromWorldToInventory
{
public:
    TileTransferMediator::FromWorld source;
    TileTransferMediator::ToInventory destination;
    const TileStack* tileAfterMove;
};