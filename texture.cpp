#include "texture.hpp"

#include "lodepng.hpp"
//#include <SOIL/SOIL.h>

#include <iostream>

GLuint TextureFromFile(const std::string& path)
{
	GLubyte *image = NULL;
	GLuint imageW = 0, imageH = 0;
	GLuint imageID = 0;
	unsigned int error = lodepng_decode32_file(&image, &imageW, &imageH, path.c_str());

	/* gen texture and upload to videocard */
	glGenTextures(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, imageID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageW, imageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	/* texture paramters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	/* generate mipmap */
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] image;

	return imageID;
}
/*
GLuint SOILTextureFromFile(const std::string& path)
{
	GLubyte *image = NULL; // pixel data
	GLint imageW = 0, imageH = 0;
	GLuint imageID = 0;
	image = SOIL_load_image(path.c_str(), &imageW, &imageH, 0, SOIL_LOAD_RGBA); // SOIL_LOAD_L     SOIL_LOAD_RGBA
	if (!image) // error loading image
		printf("Could not load image!\n");

	// gen texture and upload to videocard 
	glGenTextures(1, &imageID);
	glBindTexture(GL_TEXTURE_2D, imageID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageW, imageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// texture paramters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// generate mipmap 
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return imageID;
}*/


