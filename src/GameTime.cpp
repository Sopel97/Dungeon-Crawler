#include "GameTime.h"

GameTime& GameTime::instance()
{
    static GameTime gt;
    return gt;
}

void GameTime::update(double dt)
{
    m_time += dt;
}
double GameTime::now() const
{
    return m_time;
}

GameTime::GameTime() :
    m_time(0.0)
{

}