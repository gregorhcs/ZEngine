#include "World.h"

#include "Object.h"

namespace zn {

Object* zn::World::AddObject(Mesh* mesh)
{
	objects_.push_back(new Object());
	objects_.back()->mesh_ = mesh;
	return objects_.back();
}

World::~World()
{
	for (Object* object : objects_)
		free(object);
}

}
