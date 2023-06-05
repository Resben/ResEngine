#pragma once

#include "AEngine/Math/Math.h"

namespace AEngine
{
    class Collider
    {
    public:
        virtual ~Collider() = default;
        virtual void SetIsTrigger(bool isTrigger) = 0;
        virtual bool GetIsTrigger() const = 0;
    };
}
