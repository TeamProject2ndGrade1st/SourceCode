#pragma once
#include <memory>
#include "ArResource.h"


namespace Argent::Resource
{
	class Shader:
		public Resource::ArResource
	{
	public:
		Shader(const char* filepath);
		
	public:
		~Shader() = default;
		bool Read(const char* filename);
		
		[[nodiscard]] void* GetData() const { return data.get(); }
		[[nodiscard]] uint64_t GetSize() const { return size; }
			
	private:
		std::unique_ptr<uint8_t[]> data;
		uint64_t size;
	};
}
