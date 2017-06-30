#include "entities/EntityGroup.h"

const std::map<std::string, EntityGroupId> EntityGroupIdHelper::m_stringToEnum{
    { "Friendly", EntityGroupId::Friendly },
    { "Unfriendly", EntityGroupId::Unfriendly },
    { "Neutral", EntityGroupId::Neutral },
    { "Aggressive", EntityGroupId::Aggressive }
};