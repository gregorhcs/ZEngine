#pragma once

#include <glm/glm.hpp>

#include <string>

#include "ResourceManager.h"

namespace zn {

class ShaderProgram
{
    friend class ResourceManager;

    public:

        // -----------------------------
        // RAII

        ShaderProgram();

        // implement move constructor/ move assignment
        ShaderProgram(ShaderProgram&& other) noexcept;
        ShaderProgram& operator=(ShaderProgram&& other) noexcept;

        // delete copy constructor/ copy assignment
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        ~ShaderProgram() noexcept;

        // -----------------------------
        // use

        void Use() const noexcept;

        // -----------------------------
        // utilities for uniforms

        int GetUniformLoc(const std::string &name) const noexcept;

        void SetBool(const std::string &name, bool value) const noexcept;
        void SetInt(const std::string &name, int value) const noexcept;
        void SetFloat(const std::string &name, float value) const noexcept;

        void SetVec2f(const std::string &name, const glm::vec2 &value) const noexcept;
        void SetVec3f(const std::string &name, const glm::vec3 &value) const noexcept;
        void SetVec4f(const std::string &name, const glm::vec4 &value) const noexcept;

        void SetMat3f(const std::string &name, const glm::mat3 &value) const noexcept;
        void SetMat4f(const std::string &name, const glm::mat4 &value) const noexcept;

		void SetTextureUniforms(unsigned int numberOfUsedTextures) const noexcept;

    protected:

        /* Compiles and links shader sources into a shader program.
         * Throws runtime_error iff shader compilation or linking failed. */
        ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER };

		unsigned int programID_;

        /* Checks and outputs errors occurring during shader program linking.
         * Throws runtime_error if linking failed.
         */
        void CheckLinkingErrors(unsigned int ShaderProgramID) const;

        /* Checks and outputs errors occurring while compiling a shader.
         * Throws runtime_error if compilation failed.
         */
        void CheckCompilationErrors(unsigned int ShaderID, ShaderType type) const;

        bool bHoldsData_;

};

}
