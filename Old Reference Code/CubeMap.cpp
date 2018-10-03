#include <glad/glad.h>
#include "CubeMap.h"


gp::CubeMap::CubeMap(unsigned int id) : _id(id)
{}


gp::CubeMap::CubeMap(CubeMap && c) : _id(c._id)
{
	c._id = 0;
}


gp::CubeMap::~CubeMap()
{
	glDeleteTextures(1, &_id);
}
