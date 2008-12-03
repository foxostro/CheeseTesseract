#ifndef _SOUNDSYSTEM_H_
#define _SOUNDSYSTEM_H_

#include "ScopedEventHandler.h"
#include "ActionPlaySound.h"

// FMOD defined types
typedef struct FSOUND_SAMPLE FSOUND_SAMPLE;
typedef struct FSOUND_STREAM FSOUND_STREAM;

/** Controls the loading and playing of sounds */
class SoundSystem : public ScopedEventHandlerSubscriber
{
public:
	virtual string getTypeString() const { return "SoundSystem"; }

	/** Destructor */
	virtual ~SoundSystem();

	/** Constructor */
	SoundSystem(UID _uid, ScopedEventHandler *_parentScope);

	/**
	Plays a sound file
	@param fileName The name of the sound file
	@return Returns a handle to the sound
	*/
	void play(const FileName &fileName);

	/**
	Plays music
	@param fileName The name of a music file
	@return Returns a handle to the sound
	*/
	void playMusic(const FileName &fileName);

	/** Stop music stream immediately */
	void stopMusic();

    /**
    Set the volume level for sound effects
	@param volume Volume level [0.0, 1.0]
    */
    void setSoundEffectVolume(float volume);

    /**
    Set the volume level for music
	@param volume Volume level [0.0, 1.0]
    */
    void setMusicVolume(float volume);

    /**
    Mutes or unmutes sound
	@param mute true if sound is to be muted
    */
    void setMute(bool mute);

private:
	void handleActionPlaySound(const ActionPlaySound *action);

	FSOUND_SAMPLE * getSample( const FileName &fileName);

	static int FSOUND_Init(int mixrate, int maxsoftwarechannels, unsigned int flags);
	static FSOUND_SAMPLE * FSOUND_Sample_Load(int index, const char *name_or_data, unsigned int mode, int offset, int length);
	static void FSOUND_StopSound(int channel);
	static int FSOUND_PlaySound(int channel, FSOUND_SAMPLE *sptr);
	static void FSOUND_SetMute(int channel, signed char mute);
	static void FSOUND_SetSFXMasterVolume(int volume);
	static FSOUND_STREAM * FSOUND_Stream_Open(const char *name_or_data, unsigned int mode, int offset, int length);
	static int FSOUND_Stream_Play(int channel, FSOUND_STREAM *stream);
	static signed char FSOUND_SetVolume(int channel, int vol);
	static signed char FSOUND_Stream_Stop(FSOUND_STREAM *stream);
	static signed char FSOUND_Stream_Close(FSOUND_STREAM *stream);

private:
    /** Indicates that sound is muted */
    bool mute;

    /** Global sound effects volume [0.0, 1.0] */
    float soundVolume;

    /** Global music volume [0.0, 1.0] */
    float musicVolume;

	/** Quick reference to loaded sound effects */
	map<FileName, FSOUND_SAMPLE*> cache;

	/** Music stream */
	FSOUND_STREAM *musicStream;

	/** Channel that music plays on */
	int musicChannel;
};

#endif
