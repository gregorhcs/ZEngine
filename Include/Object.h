#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"

namespace zn {

class Object
{
    public:

        Mesh* mesh_;

        glm::vec4 transform_ = glm::vec4(0.f);

        virtual bool CheckCollision(const Object* other) const noexcept;

};

}
