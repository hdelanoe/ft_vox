#include "Shader.hpp"
#include "ChunkManager.hpp"



Shader::Shader(const char* vertexPath, const char* fragmentPath)
:fogDensity(0.0040f)
{
	std::string vString = getFromFile(vertexPath);
    std::string fString = getFromFile(fragmentPath);

   	vShaderCode = vString.c_str();
    fShaderCode = fString.c_str();

  	compile();
  	use();
}

std::string Shader::getFromFile(const std::string &filename)
{
	std::ifstream   ifs(filename);
    std::string     content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return (content);
}

void			Shader::compile(void)
{
	char            info_log[512];
    int             success;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
		glGetShaderInfoLog(vertex, 512, NULL, info_log);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
		glGetShaderInfoLog(fragment, 512, NULL, info_log);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
		glGetProgramInfoLog(id, 512, NULL, info_log);
	glDeleteShader(vertex);
	glDeleteShader(fragment);


}

unsigned int	Shader::loadTexture(void)
{
	unsigned int texture;
	int width, height, nrChannels;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	unsigned char *data = stbi_load("texture/atlas.png", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
	return texture;
}

unsigned int	Shader::loadSkybox(std::vector<std::string> faces)
{
	unsigned int texture;
	int width, height, nrChannels;
	unsigned char *data;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for(unsigned int i = 0; i < faces.size(); i++)
	{
    	data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    	if (data)
    	{
    		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
    	}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			stbi_image_free(data);
			break;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return texture;
}

void		Shader::displayModel(Camera &camera)
{
	bool uw = camera.getPos().y > 129 ? false : true;
	setMat4("projection", camera.getProjection());
	setMat4("view", camera.getView());
	setVec3("playerPos", camera.getPos());
	if (camera.getPos().y > 129)
	{
		setFloat("fogDensity", fogDensity);
		setVec3("lightPos", glm::vec3(8192, 256, 8192));
	}
	else
	{
		setFloat("fogDensity", fogDensity);
		setVec3("lightPos", camera.getPos());
	}
	setVec3("viewDir", camera.getFront());
	setInt("underWater", uw);
	glm::mat4 model = glm::mat4(1.0f);
	setMat4("model", model);
}

void	Shader::displaySkybox(Camera &camera)
{
	setMat4("projection", camera.getProjection());
	setMat4("view", glm::mat4(glm::mat3(camera.getView())));
}

void		Shader::RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s.use();
	glUniform3f(glGetUniformLocation(s.id, "textColor"), color.x, color.y, color.z);
	glBindVertexArray(freeVAO);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, freeVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void			Shader::initFreetype(Shader &s)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(W), 0.0f, static_cast<GLfloat>(H));
	s.use();
	glUniformMatrix4fv(glGetUniformLocation(s.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "font/minecraft.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		GLuint freetypeID;
		glActiveTexture(GL_TEXTURE0);
		s.setInt("image", 0);
		glGenTextures(1, &freetypeID);
		glBindTexture(GL_TEXTURE_2D, freetypeID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, face->glyph->bitmap.width,
		face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character = {
			freetypeID,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	glGenVertexArrays(1, &freeVAO);
	glGenBuffers(1, &freeVBO);
	glBindVertexArray(freeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, freeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void		Shader::Debbug_screen(Camera &c, Shader &s, int f, ChunkManager &manager)
{
	std::string camPos = glm::to_string(c.getPos());
	std::string cursorPos = glm::to_string(c.getCursor());

	std::string chunkPos = glm::to_string(manager.getPosOnGrid(c));
	std::string fps = std::to_string(f);
	std::string active;
	int type = 0;
	std::string sType;

	try {
		active = manager.getBlockOnGrid(c).isActive() ? "Active" : "Inactive";
		type = manager.getBlockOnGrid(c).getType();
	} catch(std::exception & e) {
		active = "UNKNOWN";
	}
	if (active == "Active")
	{
		if (type == DIRT) sType = "DIRT";
		else if (type == GRASS) sType = "GRASS";
		else if (type == STONE) sType = "STONE";
		else if (type == COBBLE) sType = "COBBLE";
		else if (type == SAND) sType = "SAND";
		else if (type == SNOW) sType = "SNOW";
		else if (type == TREE) sType = "TREE";
		else if (type == WATER) sType = "WATER";
	}
	else sType = "VOID";

	std::string host = "login : ";
	host += getenv("LOGNAME");

	s.RenderText(s, camPos, 50.0f, H - 25, 0.4f, glm::vec3(1.f, 1.f, 1.f));
	s.RenderText(s, chunkPos, 50.0f, H - 50, 0.4f, glm::vec3(1.f, 1.f, 1.f));
	s.RenderText(s, host, 50.0f, H - 75, 0.4f, glm::vec3(1.f, 1.f, 1.f));
	s.RenderText(s, "FPS: " + fps, 50.0f, H - 100, 0.4f, glm::vec3(1.f, 1.f, 1.f));
	s.RenderText(s,"Camera facing : " + c.cameraCompass(), 50.0f, H - 150, 0.4f, glm::vec3(1.f, 1.f, 1.f));
	s.RenderText(s,"Active : " + active + " Type : " + sType, 50.0f, H - 175, 0.4f, glm::vec3(1.f, 1.f, 1.f));


}
