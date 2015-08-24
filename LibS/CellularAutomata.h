#ifndef CELLULARAUTOMATA_H_INCLUDED
#define CELLULARAUTOMATA_H_INCLUDED

#include "Shapes.h"

namespace ls
{

    enum class OriginalCellularAutomatonStates;
    template <int I = 0> class ElementaryCellularAutomatonT;
    template <class Rules> class CellularAutomaton;

#include "include/ElementaryCellularAutomaton.h"
#include "include/CellularAutomaton.h"
}

#endif // CELLULARAUTOMATA_H_INCLUDED
