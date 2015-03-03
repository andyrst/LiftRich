/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#if !defined(__AUDIO_H__)
#define __AUDIO_H__

#include "IwSound.h"
#include <list>

/**
 * @struct    AudioSound
 *
 * @brief Audio sound effect class
 *
 */
class AudioSound
{
public:
	AudioSound() : m_SoundData(0), m_SoundSpec(0) {isplay = false; m_SoundInst = 0;}
    ~AudioSound();
	void Stop();
    unsigned int    m_NameHash;         // Hashed string name of sound effect
    CIwSoundData*   m_SoundData;        // Sound effect data
    CIwSoundSpec*   m_SoundSpec;        // Sound effect specification
	CIwSoundInst   *m_SoundInst;
	bool isplay;
    /**
     * @fn    bool AudioSound::Load(const char* filename)
     *
     * @brief Loads the given sound effect file.
     *
     * Loads a sound effect file stored in ADPCM IMA mono format and creates a sound spec that can be used to play the sound effect
     *
     * @param filename    The filename to load.
     *
     * @return    true if it succeeds, false if it fails.
     */
    bool            Load(const char* filename);
};

/**
 * @class Audio
 *
 * @brief Audio manager
 *
 * The audio manager is responsible for management of sound effects, update of the audio system and play back of sound effects and background music.
 *
 * Example usage:
 * @code
 *    // Set up audio systems
 *    g_pAudio = new Audio();
 *
 *    // Update
 *    while (!s3eDeviceCheckQuitRequest())
 *    {
 *        // Update audio system
 *        g_pAudio->Update();
 *    }
 *
 *    // Cleanup
 *    delete g_pAudio;
 *
 * @endcode
 *
 */
class Audio
{
protected:
public:
    Audio();
    ~Audio();

    std::list<AudioSound*> m_Sounds;                // List of sound effects

    AudioSound* findSound(unsigned int name_hash);
    void        Update();

    static void PlayMusic(const char* filename, bool repeat = true);
	void PlayMusicFromBuffer(void *g_AudioData, int g_AudioSize);
	void PauseMusic();
	void ResumeMusic();
	void StopAllSounds();
	void StopSound(const char *filename);

    static void StopMusic();
    AudioSound* PreloadSound(const char* filename);
    void        PlaySound(const char* filename);

private:
	int m_iMusicMaxVolume;
};

/**
 * @brief A pointer to the global audio object.
 */
extern Audio*   g_pAudio;



#endif  // __AUDIO_H__


