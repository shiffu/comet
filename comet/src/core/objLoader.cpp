#include <core/objLoader.h>
#include <comet/log.h>
#include <comet/timer.h>
#include <core/stringUtils.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>


namespace comet
{

    void ObjLoader::loadFile(const fs::path& filepath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        CM_PROFILE_FUNC();
		std::ifstream ifs(filepath.string());
		if (ifs.fail())
        {
            CM_CORE_LOG_ERROR("Failed loading file {}", filepath.string());
            return;
		}

		if (ifs)
        {
            std::string line;
            uint32_t lineNumber{0};
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> texCoords;
            std::vector<std::string> pos;
            pos.reserve(32);
            std::vector<std::string> tex;
            tex.reserve(32);
            std::vector<std::string> norm;
            norm.reserve(32);

            while (getline(ifs, line))
            {
                lineNumber++;
                auto sub2 = line.substr(0, 2);
                auto sub3 = line.substr(0, 3);
                // Vertex
                if (sub2 == "v ")
                {
                    pos.clear();
                    StringUtils::split(pos, line.substr(2));
                    if (pos.size() != 3)
                    {
                        CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                        break;
                    }
                    positions.emplace_back(std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2]));
                }
                // Texture coordinates
                else if (sub3 == "vt ")
                {
                    tex.clear();
                    StringUtils::split(tex, line.substr(3));
                    if (tex.size() != 3)
                    {
                        CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                        break;
                    }
                    texCoords.emplace_back(std::stof(tex[0]), std::stof(tex[1]));
                }
                // Normals
                else if (sub3 == "vn ")
                {
                    norm.clear();
                    StringUtils::split(norm, line.substr(3));
                    if (norm.size() != 3)
                    {
                        CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                        break;
                    }
                    normals.emplace_back(std::stof(norm[0]), std::stof(norm[1]), std::stof(norm[2]));
                }
                // Faces (indices)
                else if (sub2 == "f ")
                {
                    auto size = positions.size();
                    vertices.reserve(size);
                    indices.reserve(size);
                    break;
                }
            }

            // Now that we have all vertices, normals and texture coordinates
            // we can build the final Vertices from the faces
            std::unordered_map<std::string, uint32_t> usedVertices;
            std::vector<std::string> verticesDef;
            verticesDef.reserve(64);
            std::vector<std::string> vertexAttributes;
            vertexAttributes.reserve(32);

            do
            {
                verticesDef.clear();
                vertexAttributes.clear();
                if (line.substr(0, 2) == "f ")
                {
                    StringUtils::split(verticesDef, line.substr(2));
                    for (auto vertexDef : verticesDef)
                    {
                        StringUtils::split(vertexAttributes, vertexDef, '/');

                        auto positionIdx = std::stoi(vertexAttributes[0]) - 1;
                        int32_t texCoordIdx{-1};
                        if (!vertexAttributes[1].empty())
                        {
                            texCoordIdx = std::stoi(vertexAttributes[1]) - 1;
                        }

                        int32_t normalIdx{-1};
                        if (!vertexAttributes[2].empty())
                        {
                            normalIdx = std::stoi(vertexAttributes[2]) - 1;
                        }

                        // The vertex index has already been used before, so get the index from it
                        if (usedVertices.find(vertexDef) != usedVertices.end())
                        {
                            indices.push_back(usedVertices[vertexDef]);
                        }
                        // Vertex index has not been used before, so we need to create a new Vertex
                        else
                        {
                            auto position = positions[positionIdx];

                            glm::vec2 texCoord{0.0f};
                            if (texCoordIdx != -1)
                            {
                                texCoord = texCoords[texCoordIdx];
                            }

                            glm::vec3 normal{0.0f};
                            if (normalIdx != -1)
                            {
                                normal = normals[normalIdx];
                            }

                            vertices.emplace_back(position, normal, texCoord);
                            uint32_t newIndex = vertices.size() - 1;
                            indices.push_back(newIndex);
                            usedVertices.insert({vertexDef, newIndex});
                        }
                    }
                }
                lineNumber++;
                
            } while (getline(ifs, line));
        }
    }
} // namespace comet