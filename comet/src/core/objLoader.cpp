#include <core/objLoader.h>
#include <comet/log.h>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <fstream>
#include <regex>

namespace comet
{
    
    void ObjLoader::loadFile(const fs::path& filepath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
		std::ifstream ifs(filepath.string());
		if (ifs.fail())
        {
            CM_CORE_LOG_ERROR("Failed loading file {}", filepath.string());
            return;
		}

		if (ifs)
        {
            const std::regex vec2Regex("([-0-9]+\\.[0-9]+)\\s+([-0-9]+\\.[0-9]+)");
            const std::regex vec3Regex("([-0-9]+\\.[0-9]+)\\s+([-0-9]+\\.[0-9]+)\\s+([-0-9]+\\.[0-9]+)");
            const std::regex facesRegex("([0-9]+)/([0-9]*)/([0-9]*)");
            std::smatch match;
            std::string line;
            uint32_t lineNumber{0};
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> texCoords;

            while (getline(ifs, line))
            {
                lineNumber++;
                // Vertex
                if (line.substr(0, 2) == "v ")
                {
                    std::regex_search(line, match, vec3Regex);
                    if (match.size() != 4)
                    {
                        CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                        break;
                    }
                    positions.emplace_back(std::stof(match[1]), std::stof(match[2]), std::stof(match[3]));
                }
                // Texture coordinates
                else if (line.substr(0, 3) == "vt ")
                {
                    std::regex_search(line, match, vec2Regex);
                    if (match.size() != 3)
                    {
                        CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                        break;
                    }
                    texCoords.emplace_back(std::stof(match[1]), std::stof(match[2]));
                }
                // Normals
                else if (line.substr(0, 3) == "vn ")
                {
                    std::regex_search(line, match, vec3Regex);
                    if (match.size() != 4)
                    {
                        CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                        break;
                    }
                    normals.emplace_back(std::stof(match[1]), std::stof(match[2]), std::stof(match[3]));
                }
                // Faces (indices)
                else if (line.substr(0, 2) == "f ")
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
            do
            {
                auto linePart = line;
                if (line.substr(0, 2) == "f ")
                {
                    while (!linePart.empty())
                    {
                        std::regex_search(linePart, match, facesRegex);
                        if (match.size() != 4)
                        {
                            CM_CORE_LOG_ERROR("Format of obj file '{}' is incorrect (line {})", lineNumber);
                            break;
                        }

                        auto positionIdx = std::stoi(match[1]) - 1;
                        int32_t texCoordIdx{-1};
                        if (match[2].length() != 0)
                        {
                            texCoordIdx = std::stoi(match[2]) - 1;
                        }

                        int32_t normalIdx{-1};
                        if (match[3].length() != 0)
                        {
                            normalIdx = std::stoi(match[3]) - 1;
                        }

                        auto key = match[1].str() + "/" + match[2].str() + "/" + match[3].str();

                        // The vertex index has already been used before, so get the index from it
                        if (usedVertices.find(key) != usedVertices.end())
                        {
                            indices.push_back(usedVertices[key]);
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
                            usedVertices.insert({key, newIndex});
                        }

                        linePart = match.suffix();
                    }
                }

                lineNumber++;
                
            } while (getline(ifs, line));
        }
    }

} // namespace comet
