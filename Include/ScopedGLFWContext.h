#pragma once

namespace zn {

class ScopedGLFWContext
{
	public:
		ScopedGLFWContext(int OpenGLVersionMajor, int OpenGLVersionMinor);
		~ScopedGLFWContext();
};

}