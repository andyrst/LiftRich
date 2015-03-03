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

#include "audio.h"
#include "s3e.h"
#include "IwHashString.h"

//Audio*  g_pAudio;

bool AudioSound::Load(const char* filename)
{
    m_NameHash = IwHashString(filename);
    m_SoundData = CIwSoundWAV::Create(filename, 0, 0);
    if (m_SoundData == 0)
        return false;
    m_SoundSpec = new CIwSoundSpec();
//	m_SoundInst = new CIwSoundInst();
    m_SoundSpec->SetData(m_SoundData);

    return true;
}

void AudioSound::Stop()
{
	if (m_SoundInst != 0) m_SoundInst->Stop();
}

AudioSound::~AudioSound()
{
    if (m_SoundSpec != 0)
        delete m_SoundSpec;
    if (m_SoundData != 0)
        delete m_SoundData;
//	if (m_SoundInst != 0)
//		delete m_SoundInst;
}

//
// Audio class
//
Audio::Audio()
{
    // Initialise IwSound
    IwSoundInit();

	m_iMusicMaxVolume = s3eAudioGetInt(S3E_AUDIO_VOLUME_DEFAULT);

	s3eAudioSetInt(S3E_AUDIO_VOLUME , m_iMusicMaxVolume * 0.7);
}


Audio::~Audio()
{

    for (std::list<AudioSound*>::iterator it = m_Sounds.begin(); it !=  m_Sounds.end(); it++)
        delete *it;
    IwSoundTerminate();
}

void Audio::StopAllSounds()
{
	for (std::list<AudioSound*>::iterator it = m_Sounds.begin(); it !=  m_Sounds.end(); it++)
		(*it)->Stop();
	
}

AudioSound* Audio::findSound(unsigned int name_hash)
{
    for (std::list<AudioSound*>::iterator it = m_Sounds.begin(); it !=  m_Sounds.end(); it++)
        if ((*it)->m_NameHash == name_hash)
            return *it;
    return 0;
}

void Audio::Update()
{
    IwGetSoundManager()->Update();
}

void Audio::PlayMusic(const char* filename, bool repeat)
{
    // We only support play back of MP3 music
    if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
        s3eAudioPlay(filename, repeat ? 0 : 1);
	
}

void Audio::PlayMusicFromBuffer(void *g_AudioData, int g_AudioSize)
{
	if (s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlayFromBuffer(g_AudioData, g_AudioSize, 0);
}

void Audio::StopMusic()
{
//	s3eAudioSetInt(S3E_AUDIO_VOLUME , 150);

    s3eAudioStop();
}

void Audio::ResumeMusic()
{
	s3eAudioResume();
}


void Audio::PauseMusic()
{
	
	s3eAudioPause();
}


AudioSound* Audio::PreloadSound(const char* filename)
{
    AudioSound* sound = findSound(IwHashString(filename));
    if (sound == 0)
    {
        sound = new AudioSound();
        if (!sound->Load(filename))
        {
            delete sound;
            return 0;
        }
        m_Sounds.push_back(sound);
    }

    return sound;
}

void Audio::StopSound(const char *filename)
{
	AudioSound* sound = PreloadSound(filename);

    if (sound != 0)
		sound->Stop();

}

void Audio::PlaySound(const char* filename)
{
    AudioSound* sound = PreloadSound(filename);


    if (sound != 0)
	{

		if (sound->m_SoundInst == 0)
		{
			sound->m_SoundInst = sound->m_SoundSpec->Play();
		}
		else
		{
			if (!sound->m_SoundInst->IsPlaying()) sound->m_SoundInst = sound->m_SoundSpec->Play();
		}

	}
}



