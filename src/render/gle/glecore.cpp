#include "glecore.h"

#include GLE_HEADER


GLE_BEGIN

namespace Core {

	u32 openglContextVersion = 0;
	bool openglDebugContext = false;

	u32 maxTextureSize = 0;
	u32 maxArrayTextureLayers = 0;
	u32 maxMipmapLevels = 0;
	float maxAnisotropy = 1.0;
	u32 maxTextureUnits = 0;


	bool init() {

		glewExperimental = GL_TRUE;
		GLenum result = glewInit();

		if (result != GLEW_OK) {
			GLE::error("OpenGL context creation failed: %s", glewGetErrorString(result));
			return false;
		}

		i32 majorVersion = 0;
		i32 minorVersion = 0;
		i32 flags = 0;

		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

		openglContextVersion = majorVersion << 8 | minorVersion;
		openglDebugContext = flags & GL_CONTEXT_FLAG_DEBUG_BIT;
		
		GLE::info("OpenGL %d.%d context set up", majorVersion, minorVersion);
		GLE::info("Debug context %s", (openglDebugContext ? "enabled" : "disabled"));

		i32 tmp = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tmp);
		maxTextureSize = tmp;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &tmp);
		maxArrayTextureLayers = tmp;

		u32 texSize = maxTextureSize;
		maxMipmapLevels = 0;

		while (texSize != 1) {
			texSize /= 2;
			maxMipmapLevels++;
		}

		if (GLE_EXT_SUPPORTED(ARB_texture_filter_anisotropic)) {
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
		}

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &tmp);
		maxTextureUnits = tmp;

		return true;

	}


	void printErrors() {

#ifndef GLE_DISABLE_DEBUG

		GLenum errorNumber = 0;

		while ((errorNumber = glGetError()) != GL_NO_ERROR) {

			const char* errorMessage;

			switch (errorNumber) {

				case GL_INVALID_ENUM:                  
					errorMessage = "An invalid enum has been specified.";
					break;

				case GL_INVALID_VALUE:                 
					errorMessage = "An invalid value has been specified";
					break;

				case GL_INVALID_OPERATION:             
					errorMessage = "An invalid operation has been performed.";
					break;

				case GL_STACK_OVERFLOW:                
					errorMessage = "Stack overflown.";
					break;

				case GL_STACK_UNDERFLOW:               
					errorMessage = "Stack underflown.";
					break;

				case GL_OUT_OF_MEMORY:                 
					errorMessage = "Out of memory.";
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION: 
					errorMessage = "An invalid framebuffer operation has been performed.";
					break;

				default:
					errorMessage = "An unknown error occured.";
					break;

			}

			GLE::error("OpenGL error %d: %s", errorNumber, errorMessage);

		}

#endif

	}


	u32 getMaxTextureSize() {
		return maxTextureSize;
	}


	u32 getMaxArrayTextureLayers() {
		return maxArrayTextureLayers;
	}


	u32 getMaxMipmapLevels() {
		return maxMipmapLevels;
	}


	float getMaxTextureAnisotropy() {
		return maxAnisotropy;
	}


	u32 getMaxTextureUnits() {
		return maxTextureUnits;
	}

}


void setRowUnpackAlignment(Alignment a) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 << static_cast<u32>(a));
}



void setRowPackAlignment(Alignment a) {
	glPixelStorei(GL_PACK_ALIGNMENT, 1 << static_cast<u32>(a));
}


GLE_END