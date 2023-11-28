#pragma once
#ifndef SOUND_DATA
#define SOUND_DATA

#define CUTE_SOUND_IMPLEMENTATION
#include "External/cute_sound.h"

struct SoundData
{
	cs_context_t* ctx;
};

#endif