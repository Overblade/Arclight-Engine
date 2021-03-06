#pragma once

#include "texture.h"
#include "gc.h"

GLE_BEGIN


class CubemapTexture : public Texture {

public:

	constexpr CubemapTexture() : Texture(TextureType::CubemapTexture) {}

	inline void setData(u32 face, u32 s, ImageFormat format, TextureSourceFormat srcFormat, TextureSourceType srcType, void* data) {
		setData(getCubemapFace(face), s, format, srcFormat, srcType, data);
	}

	inline void setMipmapData(u32 face, u32 level, TextureSourceFormat srcFormat, TextureSourceType srcType, void* data) {
		setMipmapData(getCubemapFace(face), level, srcFormat, srcType, data);
	}

	inline void update(u32 face, u32 x, u32 y, u32 w, u32 h, TextureSourceFormat srcFormat, TextureSourceType srcType, void* data, u32 level = 0) {
		update(getCubemapFace(face), x, y, w, h, srcFormat, srcType, data, level);
	}

	void setData(CubemapFace face, u32 s, ImageFormat format, TextureSourceFormat srcFormat, TextureSourceType srcType, void* data);
	void setMipmapData(CubemapFace face, u32 level, TextureSourceFormat srcFormat, TextureSourceType srcType, void* data);
	void update(CubemapFace face, u32 x, u32 y, u32 w, u32 h, TextureSourceFormat srcFormat, TextureSourceType srcType, void* data, u32 level = 0);

	using Texture::setWrapU;
	using Texture::setWrapV;
	using Texture::setWrapW;
	using Texture::setBorderColor;
	using Texture::setMipmapBaseLevel;
	using Texture::setMipmapMaxLevel;
	using Texture::setMipmapRange;
	using Texture::setAnisotropy;
	using Texture::setMinFilter;
	using Texture::setMagFilter;
	using Texture::generateMipmaps;
	using Texture::enableComparisonMode;
	using Texture::disableComparisonMode;

};


GLE_END