#include "AggroGroup.h"

const std::map<std::string, AggroGroupId> AggroGroupIdHelper::m_stringToEnum{
    { "Friendly", AggroGroupId::Friendly },
    { "Unfriendly", AggroGroupId::Unfriendly },
    { "Neutral", AggroGroupId::Neutral },
    { "Aggressive", AggroGroupId::Aggressive }
};