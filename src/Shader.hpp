#ifndef SHADER_HPP
#define SHADER_HPP

#include "Camera.hpp"
#include "Env.hpp"
#include "ft_vox.hpp"

class ChunkManager;

class Shader
{
    public:
        struct Character {
        GLuint     TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        unsigned int     Advance;    // Offset to advance to next glyph
        };
        std::map<GLchar, Character> Characters;
        Shader(void) {}
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader(void) { glDeleteProgram(id); }
        unsigned int	getId(void) { return id; }
        void			use(void)  	{ glUseProgram(id); }
        void			displayModel(Camera &camera);
        void			displaySkybox(Camera &camera);
        void			compile(void);
        unsigned int	loadTexture(void);
        unsigned int	loadSkybox(std::vector<std::string> faces);
        void            RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
        void            initFreetype(Shader &s);
        void            Debbug_screen(Camera &c, Shader &s, int f, ChunkManager &manager);

        void setInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }
        void setFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }
        void setVec2(const std::string &name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }
        void setVec3(const std::string &name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }
        void setVec4(const std::string &name, const glm::vec4 &value) const { glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }
        void setMat3(const std::string &name, const glm::mat3 &mat) const { glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
        void setMat4(const std::string &name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    private:

    	float			fogDensity;
        unsigned int    id;
        unsigned int    vertex;
        unsigned int    fragment;
        GLuint          freeVBO;
        GLuint          freeVAO;
        const char*     vShaderCode;
        const char*     fShaderCode;      

        std::string   getFromFile(const std::string &filename);
};

#endif

