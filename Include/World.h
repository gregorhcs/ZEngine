#pragma once

#include <vector>

#include "Object.h"

namespace zn {

class World
{
	public:

		std::vector<Object*> objects_;

		~World();

		Object* AddObject(Mesh* mesh);

};

}

