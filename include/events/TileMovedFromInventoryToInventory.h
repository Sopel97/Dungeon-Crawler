#pragma once

#include "TileTransferMediator.h"

class TileStack;

class TileMovedFromInventoryToInventory
{
public:
    TileTransferMediator::FromInventory source;
    TileTransferMediator::ToInventory destination;
    const TileStack* tileAfterMove;
};