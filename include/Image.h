#pragma once
#include <string>

struct Image {
	Image(const std::string& path, int& width, int& height, int& channels);
	~Image();

	static void flip_vertically(bool flip);

	unsigned char* data;
};