#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "ft_vox.hpp"

class Block
{
	public:

		Block(void) { 	active = true;
						type = 0;
						activeQuad[0] = false;
						activeQuad[1] = false;
						activeQuad[2] = false;
						activeQuad[3] = false;
						activeQuad[4] = false;
						activeQuad[5] = false;
					}
					
		~Block(void) {};

		const int			getType(void) const			{ return type; }
		bool				isActive(void)				{ return active; }
		bool				getQuad(int q)				{ return activeQuad[q]; }
		void				setActive(bool a = true) 	{ active = a; }
		void				setType(int t)				{ type = t; }
		void				setBack(bool b = true)		{ activeQuad[BACK] = b; }
		void				setFront(bool b = true)		{ activeQuad[FRONT] = b; }
		void				setLeft(bool b = true)		{ activeQuad[LEFT] = b; }
		void				setRight(bool b = true)		{ activeQuad[RIGHT] = b; }
		void				setBottom(bool b = true)	{ activeQuad[BOTTOM] = b; }
		void				setTop(bool b = true)		{ activeQuad[TOP] = b; }


	private:

		bool				active;
		bool				activeQuad[6];
		int					type;

		enum { BACK = 0, FRONT, LEFT, RIGHT, BOTTOM, TOP };
};

#endif
