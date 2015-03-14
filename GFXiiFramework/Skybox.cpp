#include "Skybox.h"
#include "ImageIO.h"
#include "GLEW/include/glew.h"

Skybox::Skybox()
{

}

Skybox::~Skybox()
{

}

void Skybox::Init()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	// Load the skybox texture files for each face.
	m_textures[0] = new OGLTexture();
	m_textures[0]->CreateTextureFromFile("../asset/texture/map2.tga");
}


void Skybox::Render()
{
	SimpleVertex corners[] =
	{
		// Face: Back
		{ m_position[0] + -dimensions, m_position[1] + -dimensions, m_position[2] + dimensions, 0.0f, 0.0f, 0.0f, 0.25f, 0.5f },// 0
		{ m_position[0] + dimensions, m_position[1] + -dimensions, m_position[2] + dimensions, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f }, // 1
		{ m_position[0] + dimensions, m_position[1] + dimensions, m_position[2] + dimensions, 1.0f, 1.0f, 0.0f, 0.5f, 0.75f }, // 2
		{ m_position[0] + -dimensions, m_position[1] + dimensions, m_position[2] + dimensions, 0.0f, 1.0f, 0.0f, 0.25f, 0.75f }, // 3

		// Face: Right
		{ m_position[0] + dimensions, m_position[1] + -dimensions, m_position[2] + dimensions, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f }, // 4
		{ m_position[0] + dimensions, m_position[1] + -dimensions, m_position[2] + -dimensions, 1.0f, 0.0f, 1.0f, 0.75f, 0.5f }, // 5
		{ m_position[0] + dimensions, m_position[1] + dimensions, m_position[2] + -dimensions, 1.0f, 1.0f, 1.0f, 0.75f, 0.75f }, // 6
		{ m_position[0] + dimensions, m_position[1] + dimensions, m_position[2] + dimensions, 1.0f, 1.0f, 0.0f, 0.5f, 0.75f }, // 7

		// Face: Front
		{ m_position[0] + dimensions, m_position[1] + -dimensions, m_position[2] + -dimensions, 1.0f, 0.0f, 1.0f, 0.75f, 0.5f }, // 8
		{ m_position[0] + -dimensions, m_position[1] + -dimensions, m_position[2] + -dimensions, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f }, //9
		{ m_position[0] + -dimensions, m_position[1] + dimensions, m_position[2] + -dimensions, 0.0f, 1.0f, 1.0f, 1.0f, 0.75f }, // 10
		{ m_position[0] + dimensions, m_position[1] + dimensions, m_position[2] + -dimensions, 1.0f, 1.0f, 1.0f, 0.75f, 0.75f }, // 11

		// Face: Left
		{ m_position[0] + -dimensions, m_position[1] + -dimensions, m_position[2] + -dimensions, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f }, //12
		{ m_position[0] + -dimensions, m_position[1] + -dimensions, m_position[2] + dimensions, 0.0f, 0.0f, 0.0f, 0.25f, 0.5f },// 13
		{ m_position[0] + -dimensions, m_position[1] + dimensions, m_position[2] + dimensions, 0.0f, 1.0f, 0.0f, 0.25f, 0.75f }, // 14
		{ m_position[0] + -dimensions, m_position[1] + dimensions, m_position[2] + -dimensions, 0.0f, 1.0f, 1.0f, 0.0f, 0.75f }, // 15

		// Face: Top
		{ m_position[0] + -dimensions, m_position[1] + dimensions, m_position[2] + dimensions, 0.0f, 1.0f, 0.0f, 0.25f, 0.75f }, // 16
		{ m_position[0] + dimensions, m_position[1] + dimensions, m_position[2] + dimensions, 1.0f, 1.0f, 0.0f, 0.5f, 0.75f }, // 17
		{ m_position[0] + dimensions, m_position[1] + dimensions, m_position[2] + -dimensions, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f }, // 18
		{ m_position[0] + -dimensions, m_position[1] + dimensions, m_position[2] + -dimensions, 0.0f, 1.0f, 1.0f, 0.25f, 1.0f }, // 19

		// Face: Bottom
		{ m_position[0] + -dimensions, m_position[1] + -dimensions, m_position[2] + -dimensions, 0.0f, 0.0f, 1.0f, 0.25f, 0.25f }, //20
		{ m_position[0] + dimensions, m_position[1] + -dimensions, m_position[2] + -dimensions, 1.0f, 0.0f, 1.0f, 0.5f, 0.25f }, // 21
		{ m_position[0] + dimensions, m_position[1] + -dimensions, m_position[2] + dimensions, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f }, // 22
		{ m_position[0] + -dimensions, m_position[1] + -dimensions, m_position[2] + dimensions, 0.0f, 0.0f, 0.0f, 0.25f, 0.5f }// 23
	};

	unsigned int indices[] =
	{
		3, 1, 0,
		3, 2, 1,

		7, 5, 4,
		7, 6, 5,

		11, 9, 8,
		11, 10, 9,

		15, 14, 12,
		14, 13, 12,

		19, 18, 16,
		18, 17, 16,

		23, 22, 20,
		22, 21, 20
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(SimpleVertex), corners, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 12));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 24));
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &m_vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]->m_syshandle);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}