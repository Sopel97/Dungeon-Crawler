#pragma once

class GameTime
{
private:
    double m_time;
public:
    static GameTime& instance();

    void update(double dt);
    double now() const;

private:
    GameTime();
};