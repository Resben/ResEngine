#pragma once

#include "AEngine/Math/Math.hpp"

namespace AEngine
{
    class Collider
    {
    public:
        virtual ~Collider() = default;
        virtual void SetTrigger(bool isTrigger) = 0;
        virtual bool IsTrigger() const = 0;
    };
}
