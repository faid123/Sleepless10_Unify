
#include "ModelLoader.h"
#include <GL/glew.h>
#include <math.h>
#include <algorithm>    // std::max

using namespace modelLoader;

void mLoader::LoadModel()
{

	if (auto walkingMan = aiImportFile("Models/road.fbx", aiProcessPreset_TargetRealtime_MaxQuality))
	{
		std::cout << "Success!\n";
		std::cout << "Number of meshes found in file: " << walkingMan->mNumMeshes << std::endl;
		std::cout << "Number of vertices in first mesh: " << walkingMan->mMeshes[0]->mNumVertices << std::endl;

	}

	else
	{
		std::cout << "Fail!\n";
	}
}

GLuint mLoader::SOIL_LoadTextureDDS(const char* filename)
{
	int width, height, channels;
	unsigned char* data =
		SOIL_load_image
		(
			filename,
			&width,
			&height,
			&channels,
			SOIL_LOAD_AUTO
		);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (data)
	{
		
		if (channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (filename[0] == 'a')
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		}

		// copy image data from client memory to GPU texture buffer memory
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture at " << filename << std::endl;
		std::cout << "what went wrong: " << SOIL_last_result() << std::endl;
		std::exit(EXIT_FAILURE);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(data);
	return texture;
}
GLuint mLoader::LoadTextureDDS(const char* filename)
{
	const unsigned long FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
	const unsigned long FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
	const unsigned long FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

	//Logger::getInstance()->write(StringUtils::format("Loading DDS Image %s", filename));
	//std::cout << "Loading DDS Image: " << filename << "\n";

	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		//Logger::getInstance()->write("Failed to load Image: could not open the file");
		std::cout << "Failed to open file: " << filename << "\n";
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(fp);
		//Logger::getInstance()->write("Failed to load Image: not a direct draw surface file");
		std::cout << "Image not DDS! -> " << filename << "\n";
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char* buffer;
	unsigned int bufsize;

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	if (buffer)
		fread(buffer, 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);

	//unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		//Logger::getInstance()->write("Failed to load Image: dds file format not supported (supported formats: DXT1, DXT3, DXT5)");
		std::cout << "Failed to load Image: dds file format not supported (supported formats: DXT1, DXT3, DXT5)" << "\n";
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width = std::max(width / 2, (unsigned int)1);
		height = std::max(height / 2, (unsigned int)1);

		
		
	}

	free(buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Logger::getInstance()->write(StringUtils::format("Loaded DDS Image %s", filename));
	//std::cout << "OK! Loading DDS Image: " << filename << "\n";

	return textureID;
}

void modelLoader::mLoader::setTextureDDS(std::string filename)
{
	GLModel texture;
	texture.set_texId(modelLoader::mLoader::LoadTextureDDS(filename.c_str()));
}

bool modelLoader::mLoader::loadCubeMapTextureDDS(const char* filename, unsigned& num)
{
	const unsigned long FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
	const unsigned long FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
	const unsigned long FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

	//Logger::getInstance()->write(StringUtils::format("Loading DDS Image %s", filename));
	//std::cout << "Loading DDS Image: " << filename << "\n";

	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		//Logger::getInstance()->write("Failed to load Image: could not open the file");
		std::cout << "Failed to open file: " << filename << "\n";
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(fp);
		//Logger::getInstance()->write("Failed to load Image: not a direct draw surface file");
		std::cout << "Image not DDS! -> " << filename << "\n";
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char* buffer;
	unsigned int bufsize;

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	if (buffer)
		fread(buffer, 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);

	//unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		//Logger::getInstance()->write("Failed to load Image: dds file format not supported (supported formats: DXT1, DXT3, DXT5)");
		std::cout << "Failed to load Image: dds file format not supported (supported formats: DXT1, DXT3, DXT5)" << "\n";
		return 0;
	}


	// Create one OpenGL texture
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	unsigned int level = 0;
	unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + num, level, format, width, height,
		0, size, buffer + offset);

	free(buffer);

	//Logger::getInstance()->write(StringUtils::format("Loaded DDS Image %s", filename));
	//std::cout << "OK! Loading DDS Image: " << filename << "\n";
	return true;
	
}

