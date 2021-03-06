#include "multisamplearraytexture2d.h"

#include "glecore.h"
#include GLE_HEADER


GLE_BEGIN


void MultisampleArrayTexture2D::init(u32 w, u32 h, u32 layers, u32 samples, ImageFormat format, bool fixedSamples) {

	gle_assert(isBound(), "Texture %d has not been bound (attempted to initialize)", id);

	if (w > Limits::getMaxTextureSize() || h > Limits::getMaxTextureSize()) {
		error("2D multisample array texture dimension of size %d exceeds maximum texture size of %d", (w > h ? w : h), Limits::getMaxTextureSize());
		return;
	}

	if (layers > Limits::getMaxArrayTextureLayers()) {
		error("2D multisample array texture layer count of %d exceeds maximum array layer count of %d", w, Limits::getMaxArrayTextureLayers());
		return;
	}

	if (!samples) {
		error("2d multisample array texture cannot have 0 samples");
		return;
	}

	if (samples > Image::getMaxSamples(format)) {
		error("2D multisample array texture with given format cannot hold more than %d samples (got %d)", Image::getMaxSamples(format), samples);
		return;
	}

	width = w;
	height = h;
	depth = layers;
	texFormat = format;

	glTexImage3DMultisample(getTextureTypeEnum(type), samples, Image::getImageFormatEnum(format), w, h, layers, fixedSamples);

}


GLE_END