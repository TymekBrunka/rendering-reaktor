#pragma once
#include "Texture2d.hpp"
#include "BufferBase.hpp"
#include <vector>

namespace RR {
	class FrameBuffer : BufferBase {
	public:
		std::vector<Texture2d> textures;
		GLuint depth_texture;

		FrameBuffer(int width, int height, int num_outputs);
		// ~FrameBuffer();
		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;
	};
}
