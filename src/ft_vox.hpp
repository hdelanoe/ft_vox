#pragma once

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <sstream>
#include <map>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <fastnoise/FastNoise.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stb_image.h>

//Setting
#define W 		1920
#define H 		1080
#define CS 		8
#define CH 		128
#define SIZE 	16384
#define	DIST	256
#define	DEEP	256
#define SEA		128

#define DIRT		1
#define GRASS		2
#define STONE		3
#define SAND		4
#define GRAVEL		5
#define DIAMOND		6
#define COBBLEGRASS	7
#define COBBLESAND	8
#define COBBLE		9
#define BEDROCK		10
#define DESERT		11
#define ICE			12
#define SNOW		13
#define WATER		14
#define TREE		15
#define BLANK		16
