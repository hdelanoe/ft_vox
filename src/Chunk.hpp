#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Block.hpp"
#include "World.hpp"
#include "ft_vox.hpp"

class Shader;

class Chunk
{
	public:

		Chunk(void) {};
		Chunk(float x, float y, float z, unsigned int id);

		~Chunk(void) {};

		glm::vec3			const	&getPos(void)	const	{ return pos; }
		std::vector<float>	const	&getData(void)	const	{ return data; }
		unsigned int		const	&getId(void)	const	{ return id; }

		
		Block				&getBlock(int x, int y, int z)	{ if (x < CS && z < CS && y < CH && x >= 0 && y >= 0 && z >= 0) return blocks[x][z][y]; throw NoBlockAtPosException(); }
		void				setPos(glm::vec3 p)		{ pos = p; }
		void				setData(void);
		void				setType(Block* block, int nx, int ny, int nz, float noise);

		void				bind(void);
		void				remove(void);
		void				render(unsigned int textureId);

		void				checkBlock(int x, int y, int z);
		void				checkQuad(int x, int y, int z);
		glm::vec2			checkType(int blockType, glm::vec3 normal);

		std::vector<float>	addBack(float x1, float y1, float x2, float y2, float z, int t);
		std::vector<float>	addFront(float x1, float y1, float x2, float y2, float z, int t);
		std::vector<float>	addLeft(float x, float y1, float y2, float z1, float z2, int t);
		std::vector<float>	addRight(float x, float y1, float y2, float z1, float z2, int t);
		std::vector<float>	addBottom(float x1, float x2, float y, float z1, float z2, int t);
		std::vector<float>	addTop(float x1, float x2, float y, float z1, float z2, int t);

		class NoBlockAtPosException: public std::exception { virtual const char* what() const throw(); };

	private:

		unsigned int		id;
		glm::vec3			pos;
		std::vector<float>	data;
		Block***			blocks;
		World				biome;
		GLuint				VAO;
		GLuint				VBO;

		enum { BACK = 0, FRONT, LEFT, RIGHT, BOTTOM, TOP };



};

#endif
