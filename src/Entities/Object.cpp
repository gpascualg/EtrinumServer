#include "Object.h"
#include "Puller.h"
#include "Player.h"
#include "Character.h"
#include "Grid.h"
#include "Server.h"
#include "Tools.h"

#ifdef SERVER_FRAMEWORK_TESTING

    #include "debugging.h"

#endif

Object::Object():
    _GUID(GuidManager::MAX_GUID)
{
    // Reset all flags
    for (Poco::UInt8 i = 0; i < MAX_FLAGS_TYPES; i++)
        _flags[i] = 0;

    // Reset Position
    _position.x = 0;
    _position.y = 0;
    _grid = NULL;
}

Object::~Object()
{
#if defined(SERVER_FRAMEWORK_TESTING)
    printf("Object %s deleted\n", Poco::NumberFormatter::formatHex(GetGUID()).c_str());
#endif
}

bool Object::update(const Poco::UInt32 diff)
{
    bool updatedGrid = false;
    if (hasFlag(FLAGS_TYPE_MOVEMENT, FLAG_MOVING))
    {
        // Check if movement has finalized, in which case, remove flag
        Vector2D newPos;
        if (motionMaster.evaluate(diff, newPos))
            clearFlag(FLAGS_TYPE_MOVEMENT, FLAG_MOVING);

        // Update grid if we have to
        Vector2D currentPos = GetPosition();
        if (Tools::GetCellFromPos(newPos.x) != Tools::GetCellFromPos(currentPos.x) || Tools::GetCellFromPos(newPos.y) != Tools::GetCellFromPos(currentPos.y))
        {                    
            // Add it to GridLoader move list, we can't add it directly from here to the new Grid
            // We would end up being deadlocked, so the GridLoader handles it
            sGridLoader.addObject(this);
            _grid = NULL;
            updatedGrid = true;
        }

        // Relocate Object
        Relocate(newPos);

        // Update LoS
        UpdateLoS();
    }

    return !updatedGrid;
}

void Object::UpdateLoS()
{
    if (!_grid)
        return;

    std::list<Poco::UInt64> newObjectsInSight = sGridLoader.ObjectsInGridNear(this->ToObject(), 35.0f);

    // Send spawn packets
    for (std::list<Poco::UInt64>::iterator itr = newObjectsInSight.begin(); itr != newObjectsInSight.end(); itr++)
    {
        // Is the object already in the LOS list? If not, send an update packet
        std::list<Poco::UInt64>::iterator found = std::find(_objectsInSight.begin(), _objectsInSight.end(), *itr);
        if (found == _objectsInSight.end())
        {
            // Send update packet to only players
            SharedPtr<Object> object = _grid->getObject(*itr);
            if (object.isNull() || object->GetHighGUID() != HIGH_GUID_PLAYER)
                continue;

            sServer->UpdateVisibilityOf(this->ToObject(), object, true);
        }
    }

    // Send despawn packets
    for (std::list<Poco::UInt64>::iterator itr = _objectsInSight.begin(); itr != _objectsInSight.end(); itr++)
    {
        // Is the object no longer in sight? Send Despawn packet
        std::list<Poco::UInt64>::iterator found = std::find(newObjectsInSight.begin(), newObjectsInSight.end(), *itr);
        if (found == newObjectsInSight.end())
        {
            // Send update packet to only players
            SharedPtr<Object> object = _grid->getObject(*itr);
            if (object.isNull() || object->GetHighGUID() != HIGH_GUID_PLAYER)
                continue;

            sServer->UpdateVisibilityOf(this->ToObject(), object, false);
        }
    }

    _objectsInSight = newObjectsInSight;
}

void Object::Despawn()
{
    // Send despawn packets
    for (std::list<Poco::UInt64>::iterator itr = _objectsInSight.begin(); itr != _objectsInSight.end(); itr++)
    {
        // Send update packet to only players
        SharedPtr<Object> object = _grid->getObject(*itr);
        if (object.isNull() || object->GetHighGUID() != HIGH_GUID_PLAYER)
            continue;

        sServer->UpdateVisibilityOf(this->ToObject(), object, false);
    }
}

void Object::SetGUID(Poco::UInt64 GUID)
{
    _GUID = GUID;
}

Poco::UInt64 Object::GetGUID()
{
    return _GUID;
}

Poco::UInt32 Object::GetHighGUID()
{
    return (Poco::UInt32)(_GUID >> 32);
}

Poco::UInt32 Object::GetLowGUID()
{
    return (Poco::UInt32)(_GUID & 0xFFFFFFFF);
}

Player* Object::ToPlayer()
{
    if (GetHighGUID() & HIGH_GUID_PLAYER)
        return (Player*)this;
    return NULL;
}
   
Character* Object::ToCharacter()
{
    if ((GetHighGUID() & HIGH_GUID_PLAYER) || (GetHighGUID() & HIGH_GUID_MONSTER))
        return (Character*)this;
    return NULL;
}

Object* Object::ToObject()
{
    return (Object*)this;
}