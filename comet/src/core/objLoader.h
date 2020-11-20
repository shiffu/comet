#pragma once

#include <experimental/filesystem>
#include <vector>
#include <comet/vertex.h>

namespace comet
{
    namespace fs = std::experimental::filesystem;

    class ObjLoader
    {
    public:
        void loadFile(const fs::path& filepath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    };

} // namespace comet
