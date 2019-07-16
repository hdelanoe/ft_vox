#ifndef CHUNKMANAGER_HPP
# define CHUNKMANAGER_HPP

# include "Camera.hpp"
# include "Chunk.hpp"
# include "World.hpp"
# include "Shader.hpp"

# include "ft_vox.hpp"

class Block;
class Chunk;


class ChunkManager
{
	public:

		ChunkManager(glm::vec3 player = glm::vec3(0.0f, 0.0f,  0.0f));
		~ChunkManager(void) {}

		std::vector<Chunk> &getList(void) { return setupList; }

		void		update(Camera &camera);
		void		updateLoading(glm::vec3 ptr);
		void		updateUnloading(Camera &camera);
		void		updateSetupList(Camera &camera);
		void		updateRenderList(Camera &camera);

		glm::vec3	getPosOnGrid(Camera &camera);
		Block		getBlockOnGrid(Camera &camera);
		bool		sameAlti(float a, float b) { return (a >= SEA && b >= SEA) || (a < SEA - 1 && b < SEA - 1) ? true : false; }

		void	bind(void);
		void	render(Camera &camera);

		void	updateFps(void);

		class NoBlockAtPosException: public std::exception { virtual const char* what() const throw(); };

	private:

		Shader					shader;
		World					world;
		glm::vec4***			grid;
		std::vector<Chunk>		loadingList;
		std::vector<Chunk>		setupList;
		std::vector<Chunk>		renderList;
		unsigned int			texture;
		int						id;
		int						cpf;
		float					renderDistance;
		glm::vec3				lastPos;
		glm::vec3				lastView;
		
};

#endif
