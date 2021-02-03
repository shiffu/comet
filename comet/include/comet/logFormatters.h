#pragma once

#include <spdlog/fmt/ostr.h>

#include <glm/gtx/io.hpp>

// Define customized spdlog formats

// GLM: Those are actually already defined in: glm/gtx/io.hpp

// std::ostream& operator<<(std::ostream& os, const glm::mat4& val)
// {
//     return os << "\n" <<
//         "\t" << "[" << val[0][0] << ", " << val[1][0] << ", " << val[2][0] << ", " << val[3][0] << "]\n" <<
//         "\t" << "[" << val[0][1] << ", " << val[1][1] << ", " << val[2][1] << ", " << val[3][1] << "]\n" <<
//         "\t" << "[" << val[0][2] << ", " << val[1][2] << ", " << val[2][2] << ", " << val[3][2] << "]\n" <<
//         "\t" << "[" << val[0][3] << ", " << val[1][3] << ", " << val[2][3] << ", " << val[3][3] << "]\n";
// }

// std::ostream& operator<<(std::ostream& os, const glm::vec2& val)
// {
//     return os << "[" << val.x << ", " << val.y << "]";
// }

// std::ostream& operator<<(std::ostream& os, const glm::vec3& val)
// {
//     return os << "[" << val.x << ", " << val.y << ", " << val.z << "]";
// }

// std::ostream& operator<<(std::ostream& os, const glm::vec4& val)
// {
//     return os << "[" << val.x << ", " << val.y << ", " << val.z << ", " << val.w << "]";
// }