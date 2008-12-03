#ifndef _WIN32
#include <unistd.h>
#endif

#include <fmod.h>

#ifndef NDEBUG
#include <fmod_errors.h>
#endif

#include "stdafx.h"
#include "SoundSystem.h"


SoundSystem::SoundSystem(UID uid, ScopedEventHandler *parentScope)
: ScopedEventHandlerSubscriber(uid, parentScope)
{
	REGISTER_HANDLER(SoundSystem::handleActionPlaySound);

	cache.clear();
	soundVolume = 1.0f;
	musicVolume = 0.5f;
	mute = false;
	musicStream = 0;
	musicChannel = -1;

	int mixrate = 44100;
	int maxsoftwarechannels = 32;
	unsigned int flags = 0;
	FSOUND_Init(mixrate, maxsoftwarechannels, flags);

	TRACE("Sound System initialized");
}

void SoundSystem::handleActionPlaySound( const ActionPlaySound *action )
{
	play(action->sound);
}

SoundSystem::~SoundSystem()
{
	stopMusic();
	FSOUND_Close();
}

void SoundSystem::play(const FileName &fileName)
{
	if(!mute && fileName!=FileName("none") && fileName!=FileName(""))
	{
		FSOUND_PlaySound(FSOUND_FREE, getSample(fileName));
	}
}

void SoundSystem::playMusic(const FileName &fileName)
{
	stopMusic();

	if(!mute && fileName!=FileName("none") && fileName!=FileName(""))
	{
		musicStream = FSOUND_Stream_Open(fileName.c_str(),
		                                 FSOUND_LOOP_NORMAL,
										 0, 0);
		musicChannel = FSOUND_Stream_Play(FSOUND_FREE, musicStream);
		TRACE("Playing music: " + fileName.str());
	}
}

void SoundSystem::setMusicVolume(float volume)
{
	ASSERT(volume >= 0.0f, "Volume is negative: " + ftos(volume));
	ASSERT(volume <= 1.0f, "Volume is greater than 100%: " + ftos(volume));

	musicVolume = volume;

	if(musicChannel>=0)
	{
		FSOUND_SetVolume(musicChannel, (int)(musicVolume * 255.0f));
	}
}

void SoundSystem::setSoundEffectVolume(float volume)
{
    ASSERT(volume >= 0.0f, "Volume is negative: " + ftos(volume));
    ASSERT(volume <= 1.0f, "Volume is greater than 100%: " + ftos(volume));

    soundVolume = volume;
	FSOUND_SetSFXMasterVolume((int)(soundVolume * 255.0f));
}

void SoundSystem::setMute(bool _mute)
{
	mute = _mute;
	FSOUND_SetMute(FSOUND_ALL, mute ? 1 : 0);
}

int SoundSystem::FSOUND_PlaySound(int channel, FSOUND_SAMPLE *sptr)
{
	/*
	I have experienced times when the return value id -1 and the reported
	error code is "No Errors." So it seems that failing silently might be the
	best solution.
	*/
	return ::FSOUND_PlaySound(channel, sptr);
}

void SoundSystem::FSOUND_SetSFXMasterVolume(int volume)
{
	::FSOUND_SetSFXMasterVolume(volume);

#ifndef NDEBUG
	int fmod_err = FMOD_ERR_NONE;
	if((fmod_err = FSOUND_GetError()) != FMOD_ERR_NONE)
	{
		FAIL("Failed to set master sfx volume: " + itos(volume) + ": " + FMOD_ErrorString(fmod_err));
	}
#endif
}

FSOUND_SAMPLE * SoundSystem::FSOUND_Sample_Load( int index, const char *name_or_data, unsigned int mode, int offset, int length )
{
	FSOUND_SAMPLE *sound = 0;

	sound = ::FSOUND_Sample_Load(index,name_or_data,mode,offset,length);

#ifndef NDEBUG
	if(sound==0)
	{
		FAIL(string("Failed to load sound file ") + name_or_data + ": " +
			FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return sound;
}

void SoundSystem::FSOUND_StopSound(int channel)
{
#ifndef NDEBUG
	signed char r = ::FSOUND_StopSound(channel);
	if(!r)
	{
		FAIL(string("Failed to stop all sounds: ") +
			FMOD_ErrorString(FSOUND_GetError()));
	}
#else
	::FSOUND_StopSound(channel);
#endif
}

int SoundSystem::FSOUND_Init(int mixrate,
							 int maxsoftwarechannels,
							 unsigned int flags)
{
	int r = ::FSOUND_Init(mixrate, maxsoftwarechannels, flags);

#ifndef NDEBUG
	if(!r)
	{
		FAIL(string("Failed to initialize FMOD: ") +
			FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return r;
}
void SoundSystem::FSOUND_SetMute(int channel, signed char mute)
{
#ifndef NDEBUG
	signed char r = ::FSOUND_SetMute(channel, mute);
	if(!r)
	{
		if(mute)
		{
			FAIL("Failed to mute channel " + itos(channel) + ": " + FMOD_ErrorString(FSOUND_GetError()));
		}
		else
		{
			FAIL("Failed to unmute channel " + itos(channel) + ": " + FMOD_ErrorString(FSOUND_GetError()));
		}
	}
#else
	::FSOUND_SetMute(channel, mute);
#endif
}

FSOUND_STREAM * SoundSystem::FSOUND_Stream_Open( const char *name_or_data, unsigned int mode, int offset, int length )
{
	FSOUND_STREAM *stream = ::FSOUND_Stream_Open(name_or_data, mode, offset, length);

#ifndef NDEBUG
	if(!stream)
	{
		FAIL(string("Failed to open stream ") + name_or_data + ": "
			+ FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return stream;
}

int SoundSystem::FSOUND_Stream_Play( int channel, FSOUND_STREAM *stream )
{
	int r = ::FSOUND_Stream_Play(channel, stream);

#ifndef NDEBUG
	if(r == -1)
	{
		FAIL(string("Failed to play stream: ")
			+ FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return r;
}

signed char SoundSystem::FSOUND_SetVolume(int channel, int vol)
{
	signed char r = ::FSOUND_SetVolume(channel, vol);

#ifndef NDEBUG
	if(!r)
	{
		FAIL(string("Failed to set channel volume: ")
			+ FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return r;
}

signed char SoundSystem::FSOUND_Stream_Stop(FSOUND_STREAM *stream)
{
	signed char r = ::FSOUND_Stream_Stop(stream);

#ifndef NDEBUG
	if(!r)
	{
		FAIL(string("Failed to stop stream: ")
			+ FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return r;
}

signed char SoundSystem::FSOUND_Stream_Close(FSOUND_STREAM *stream)
{
	signed char r = ::FSOUND_Stream_Close(stream);

#ifndef NDEBUG
	if(!r)
	{
		FAIL(string("Failed to close stream: ")
			+ FMOD_ErrorString(FSOUND_GetError()));
	}
#endif

	return r;
}

FSOUND_SAMPLE * SoundSystem::getSample( const FileName &fileName )
{
	map<FileName, FSOUND_SAMPLE*>::const_iterator i;
	FSOUND_SAMPLE *sound = 0;

	i = cache.find(fileName); // check for it in the cache

	if(i == cache.end())
	{
		sound = FSOUND_Sample_Load(FSOUND_FREE, fileName.c_str(), 0, 0, 0);
		cache.insert(make_pair(fileName, sound)); // cache the sound chunk
		TRACE("Loaded and cached sound file:" + fileName.str());
	}
	else
	{
		sound = i->second;
	}

	return sound;
}

void SoundSystem::stopMusic()
{
	if(musicStream)
	{
		FSOUND_Stream_Stop(musicStream);
		FSOUND_Stream_Close(musicStream);
		musicStream=0;
	}
}
