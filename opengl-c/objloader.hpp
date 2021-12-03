#ifndef OBJLOADER_H
#define OBJLOADER_H

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);

void createTexture(const char* texFile, int TEXTURE, const GLuint& texID);

void loadObject(const char* objFile, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, GLuint& vertexbuffer, GLuint& uvbuffer);

void applyAttribute(int attribute, const GLuint& buffer, int size);

void selectTexture(GLuint& TextureID, const GLuint& programID, const GLuint& texID, const GLint& unit);

void createTexture(char  texFile[8], int TEXTURE, const GLuint& texID);



bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif