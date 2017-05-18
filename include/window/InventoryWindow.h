#pragma once

#include "PlayerUi.h"
#include "WindowParams.h"

class InventoryView;

class InventoryWindow : public PlayerUi::PanelWindow
{
public:
    static WindowParams defaultParams();

    InventoryWindow(WindowSpaceManager& wsm, const std::string& name);
    InventoryWindow(WindowSpaceManager& wsm, const std::string& name, const WindowParams& params);

    virtual int verticalScroll() const;

    void setContentHeightToMax();

    void update();

    void updateMaxContentHeight(const InventoryView& view);

protected:
    static constexpr int m_minSensibleHeight = 46;

    int m_scroll;
};
