#pragma once

#include "ResourceManager.h"

namespace zn {

class Resource
{
	friend class ResourceManager;

	public:
		Resource() : resourceID_(-1) {};

	protected:
		unsigned int resourceID_;

};

}
