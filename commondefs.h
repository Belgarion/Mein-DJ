/////////////////////////////////////////////////////////////////////////////
// Name:        commondefs.h
// Purpose:     Common definition of define, enum and structures.
//		This file required a generated header file config.h from
//		./configure.
// Author:      Yann Hamiaux
// Modified by: -
// Created:     03/19/2008
// Copyright:   Written for Guillemot Corporation by Yann Hamiaux
// Licence:     The GNU General Public License v2+.
/////////////////////////////////////////////////////////////////////////////

#include <alsa/asoundlib.h>
#include <linux/types.h>
#include <syslog.h>
#include "djdevioctls.h"

#ifndef _COMMON_DEFS__H_
#define _COMMON_DEFS__H_

enum MIDI_INPUT_SEL {MIDI_IN_PLUG = 0, DJ_CONSOLE_BUTTONS};
enum RECORD_INPUT   {LINE_IN = 0, MIC, DIGITAL_COAXIAL, DIGITAL_OPTICAL};
enum AUDIO_SOURCE   {USB_PC_OUT = 0, MK2_AUDIO_INPUT};
enum HEADPHONE_SEL  {DECK_A = 0, DECK_B, MIX, SPLIT};
enum JOG_WHEELS_DIS {ENABLE = 0, DISABLE_A, DISABLE_B, DISABLE_BOTH};
enum CROSSFADER_SEL {BEATMIX = 0, DIPPED, SCRATCH, LOCK_ON_A, LOCK_ON_B, LOCK_ON_CENTER};
enum JOGS_SENSITIVE {NORMAL = 0, LESS, LEAST};
enum OUT_HWARE_MIX  {NONE = 0, MIX_WITH_MIC, MIX_WITH_LINE_IN};

struct DJ_CONSOLE_STATE {
	bool		bEnableDJMode;			// Normal or DJ Switch
	int		nVolume[6];			// Volume for 6 channels
	bool		bMute[3];			// Mute for every Stereo Volume
	bool		bHeadphoneOnMaster;		// Headphone Selection (if true 1-2/Master, otherwise 3-4/Monitor)
	bool		bEnableDJMouse;			// Enable/Disable DJ Mouse
	int		nSampleRate;			// Sample rate 44100 Hz or 48000 Hz
	MIDI_INPUT_SEL	eMidiInput;			// MIDI Input: Midi-In Plug or DJ Console buttons
	RECORD_INPUT	eRecordingInput;		// Recording input: Line In, Mic, Digital Coax. or Digital Opt.
	bool		bTalkoverAttenuation;		// Enable/Disable Talkover Attenuation
	int		nTalkoverAttenuation;		// Set Talkover Attenuation to 0dB, -3dB or -6dB
	bool		bOnAir;				// ON AIR activated or not? [READ ONLY]
	long		lFirmwareVersion;		// Current Firmware Version
};

struct DJ_CONSOLE_MK2_STATE {
	int		nVolume[4];			// Volume for 4 channels
	bool		bMute[2];			// Mute for every Stereo Volume
	bool		bLockAudioSource[2];		// Audio source lock per deck
	AUDIO_SOURCE	eAudioSourceSelection[2];	// Audio source selection (PC Sound or Audio Input) [READ ONLY]
	bool		bEnableInputMic;		// Input type: Mic, otherwise Line-In.
	bool		bTalkoverAttenuation;		// Enable/Disable Talkover Attenuation
	int		nTalkoverAttenuation;		// Set Talkover Attenuation to 0dB, -3dB or -6dB
	bool		bOnAir;				// ON AIR activated or not? [READ ONLY]
	HEADPHONE_SEL	eHeadphoneSelection;		// Headphone Selection [READ ONLY]
	CROSSFADER_SEL	eCrossFaderCurve;		// Cross-Fader Curves
	bool		bUseExternalMixer;		// Use external mixer
	bool		bEnableDJMouse;			// Enable/Disable DJ Mouse
	long		lFirmwareVersion;		// Current Firmware Version
};

struct DJ_CONTROL_MP3_STATE {
	int		nMidiChannel;			// MIDI Channel 0-15
	bool		bEnableDJMouse;			// Enable/Disable DJ Mouse
	long		lFirmwareVersion;		// Current Firmware Version
};

struct DJ_CONSOLE_RMX_STATE {
	int		nVolume[4];			// Volume for 4 channels
	bool		bMute[2];			// Mute for every Stereo Volume
	bool		bVolumeLink[2];			// Volume are link or not
	bool		bEnableInputMic;		// Input type: Mic, otherwise Line-In.
	bool		bTalkoverAttenuation;		// Enable/Disable Talkover Attenuation
	int		nTalkoverAttenuation;		// Set Talkover Attenuation to 0dB, -3dB or -6dB
	JOG_WHEELS_DIS	eJogWheelsLock;			// Jog-Wheel enable or disable per deck.
	JOGS_SENSITIVE	eJogWheelsSensitivity;		// Jog-Wheel sensitivity.
	OUT_HWARE_MIX	eOutput12Mix;			// Output channel 1-2, Hardware Mix.
	int		nMidiChannel;			// MIDI Channel 0-15
	long		lFirmwareVersion;		// Current Firmware Version
};

struct DJ_CONTROL_STEEL_STATE {
	int		nMidiChannel;			// MIDI Channel 0-15
	JOG_WHEELS_DIS	eJogWheelsLock;			// Jog-Wheel enable or disable per deck.
	JOGS_SENSITIVE	eJogWheelsSensitivity;		// Jog-Wheel sensitivity.
	long		lFirmwareVersion;		// Current Firmware Version
};


struct DJ_DEVICE_STATE {
	// --- Device State and Interfaces Info. ---
	bool			 bPresent;
	bool			 bStateChange;		// Internal state has change (need GUI update).
	unsigned long		 ulProductCode;
	char			 szLocationID[LOCATION_ID_LEN];
	int			 nNetlinkUnit;

	bool			 bBULK_Init;		// --- BULK Interface ---
	int			 nBULK_FileIndex;
	int			 nBULK_FHandle;
	unsigned long		 ulBULK_Version;
	unsigned long		 ulBULK_ReadSize;
	unsigned long		 ulBULK_OutSize;

	bool			 bALSA_Init;		// --- ALSA Interface ---
	snd_ctl_t 		*pALSA_SndCtl;
	snd_mixer_t		*pALSA_Mixer;
	snd_mixer_elem_t 	*pALSA_elem_Volume;
	bool			 bALSA_Refresh;		// Need ALSA refresh.
	bool			 bVolumeToApply[6];	// Need to apply ALSA volumes.
	int			 nVolumeToApply[6];
	int			 nVolMin, nVolMax;

	// --- Device Current State ---
	DJ_CONSOLE_STATE	 djConsole;
	DJ_CONSOLE_MK2_STATE	 djConsoleMk2;
	DJ_CONTROL_MP3_STATE	 djControlMP3;
	DJ_CONSOLE_RMX_STATE	 djConsoleRmx;
	DJ_CONTROL_STEEL_STATE	 djControlSteel;
};

#endif


