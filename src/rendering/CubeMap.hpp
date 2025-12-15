#pragma once
#include "Program.hpp"
#include "Texture2d.hpp"
#include <glad/gl.h>

namespace RR {
	class CubeMap {
	public:
		GLuint id = -1;
		
		CubeMap(const image_data faces[]);
		CubeMap(GLuint id);
		~CubeMap();
		CubeMap(CubeMap&& other) noexcept;
		CubeMap& operator=(CubeMap&& other) noexcept;

		void bindToSlot(int slot);
		void bindToSlotAndName(Program& program, int slot, const char* uniformName);
	};
}
