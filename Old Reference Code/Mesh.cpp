#include "Mesh.h"


gp::Mesh::Mesh(const Vertices &v, const Indices &i) :
    _numIndices((unsigned int)i.size())
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// vertex buffer
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vertex) * v.size(),
		&v[0],
		GL_STATIC_DRAW);

	// element buffer
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * i.size(),
		&i[0],
		GL_STATIC_DRAW);

	// attributes - position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, position));

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// tangent
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// uv
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}


gp::Mesh::Mesh(const Positions &v, const Indices &i) :
	_numIndices((unsigned int)i.size())
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// vertex buffer
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Position) * v.size(),
		&v[0],
		GL_STATIC_DRAW);

	// element buffer
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * i.size(),
		&i[0],
		GL_STATIC_DRAW);

	// attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Position), (void*)0);

	glBindVertexArray(0);
}


gp::Mesh::Mesh(Mesh &&m)
{
    _vao = m._vao;
    _vbo = m._vbo;
    _ebo = m._ebo;
    _numIndices = m._numIndices;
    
    m._vao = 0;
    m._vbo = 0;
    m._ebo = 0;
}


gp::Mesh::~Mesh()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
}


void gp::Mesh::draw()
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


gp::InstancedMesh::InstancedMesh(const Vertices & v, const Indices & i) : 
	Mesh(v, i), _numInstances(0)
{}


gp::InstancedMesh::InstancedMesh(const Positions & v, const Indices & i) : 
	Mesh(v, i), _numInstances(0)
{}


void gp::InstancedMesh::setMatrices(const std::vector<glm::mat4>& ms)
{
	// alloc the matrix buffer
	glGenBuffers(1, &_mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _mBuffer);
	glBufferData(GL_ARRAY_BUFFER, ms.size() * sizeof(glm::mat4), &ms[0], GL_STATIC_DRAW);

	// add to mesh
	const unsigned int v4SZ = sizeof(glm::vec4);
	const unsigned int STRIDE = v4SZ * 4;
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(v4SZ));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(v4SZ*2));

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, STRIDE, (void*)(v4SZ*3));

	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);

	_numInstances = ms.size();
}


void gp::InstancedMesh::draw()
{
	glBindVertexArray(_vao);
	glDrawElementsInstanced(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0, _numInstances);
	glBindVertexArray(0);
}