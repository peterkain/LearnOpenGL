#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const std::string& path, int& width, int& height, int& channels) {
	data = stbi_load(path.data(), &width, &height, &channels, 0);
}

Image::~Image() {
	stbi_image_free(data);
}

void Image::flip_vertically(bool flip) {
	stbi_set_flip_vertically_on_load(flip);
}