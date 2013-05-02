#ifndef BASIC_SERVER_ENTITIES_CHARACTER_H
#define BASIC_SERVER_ENTITIES_CHARACTER_H

#include "Object.h"

class Character: public Object
{
public:
    Character(std::string name);

    inline std::string GetName()
    {
        return _name;
    }

    inline Poco::UInt32 GetMaxHP()
    {
        return _maxhp;
    }

    inline void SetMaxHP(Poco::UInt32 maxhp)
    {
        _maxhp = maxhp;
    }

    inline Poco::UInt32 GetHP()
    {
        return _hp;
    }

    inline void SetHP(Poco::UInt32 hp)
    {
        _hp = hp;
    }

    inline Poco::UInt32 GetMaxMP()
    {
        return _maxmp;
    }

    inline void SetMaxMP(Poco::UInt32 maxmp)
    {
        _maxmp = maxmp;
    }

    inline Poco::UInt32 GetMP()
    {
        return _mp;
    }

    inline void SetMP(Poco::UInt32 mp)
    {
        _mp = mp;
    }

    inline Poco::UInt8 GetLVL()
    {
        return _lvl;
    }

    inline void SetLVL(Poco::UInt8 lvl)
    {
        _lvl = lvl;
    }

    inline float GetSPeed(Poco::UInt8 movementType)
    {
        return _speed[movementType];
    }

    inline Poco::UInt8 MovementType()
    {
        return _movementType;
    }

private:
    std::string _name;
    Poco::UInt32 _maxhp;
    Poco::UInt32 _maxmp;
    Poco::UInt32 _hp;
    Poco::UInt32 _mp;
    Poco::UInt8 _lvl;

    Poco::UInt8 _movementType;
    float _speed[2];
};

#endif