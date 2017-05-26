#pragma once

#include "TileTransferMediator.h"

class TileStack;

class TileMovedFromInventoryToWorld
{
public:
    TileTransferMediator::FromInventory source;
    TileTransferMediator::ToWorld destination;
    const TileStack* tileAfterMove;
};