#pragma once

#include "TileTransferMediator.h"

class TileStack;

class TileMovedFromWorldToWorld
{
public:
    TileTransferMediator::FromWorld source;
    TileTransferMediator::ToWorld destination;
    const TileStack* tileAfterMove;
};