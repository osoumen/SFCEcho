/*
 *  SFCEchodefines.h
 *  SFCEcho
 *
 *  Created by 開発用 on 06/11/29.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SFCEchodefines__
#define __SFCEchodefines__


#pragma mark ____SFCEcho Parameters

// parameters

enum {
	kParam_mainvol_L =0,
	kParam_mainvol_R,
	kParam_echovol_L,
	kParam_echovol_R,
	kParam_echoFB,
	kParam_echodelay,
	kParam_fir0,
	kParam_fir1,
	kParam_fir2,
	kParam_fir3,
	kParam_fir4,
	kParam_fir5,
	kParam_fir6,
	kParam_fir7,
	kNumberOfParameters
};

// properties
enum
{
	kAudioUnitCustomProperty_First = 64000,
	
	kAudioUnitCustomProperty_Band1 = kAudioUnitCustomProperty_First,
	kAudioUnitCustomProperty_Band2,
	kAudioUnitCustomProperty_Band3,
	kAudioUnitCustomProperty_Band4,
	kAudioUnitCustomProperty_Band5,
	
	kNumberOfProperties = 5
};

static const float kMinimumValue_SInt8 = -128;
static const float kMaximumValue_SInt8 = 127;

static const float kMinimumValue_UInt4 = 0;
static const float kMaximumValue_UInt4 = 15;


#endif
