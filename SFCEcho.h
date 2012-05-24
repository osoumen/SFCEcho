/*
*	File:		SFCEcho.h
*	
*	Version:	1.0
* 
*	Created:	06/08/28
*	
*	Copyright:  Copyright © 2006 Vermicelli Magic, All Rights Reserved
*
*/

#include "AUEffectBase.h"
#include "SFCEchoVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __SFCEcho_h__
#define __SFCEcho_h__

#include "SFCEchodefines.h"

static CFStringRef kParameter_mainvol_L_Name = CFSTR("Dry(L)");
static const float kDefaultValue_mainvol_L = 127;

static CFStringRef kParameter_mainvol_R_Name = CFSTR("Dry(R)");
static const float kDefaultValue_mainvol_R = 127;

static CFStringRef kParameter_echovol_L_Name = CFSTR("Wet(L)");
static const float kDefaultValue_echovol_L = 50;

static CFStringRef kParameter_echovol_R_Name = CFSTR("Wet(R)");
static const float kDefaultValue_echovol_R = -50;

static CFStringRef kParameter_echoFB_Name = CFSTR("FeedBack");
static const float kDefaultValue_echoFB = -70;

static CFStringRef kParameter_echodelay_Name = CFSTR("Delay");
static const float kDefaultValue_echodelay = 6;

static CFStringRef kParameter_firC0_Name = CFSTR("Filter a0");
static const float kDefaultValue_fir0 = 127;

static CFStringRef kParameter_firC1_Name = CFSTR("Filter a1");
static const float kDefaultValue_fir1 = 0;

static CFStringRef kParameter_firC2_Name = CFSTR("Filter a2");
static const float kDefaultValue_fir2 = 0;

static CFStringRef kParameter_firC3_Name = CFSTR("Filter a3");
static const float kDefaultValue_fir3 = 0;

static CFStringRef kParameter_firC4_Name = CFSTR("Filter a4");
static const float kDefaultValue_fir4 = 0;

static CFStringRef kParameter_firC5_Name = CFSTR("Filter a5");
static const float kDefaultValue_fir5 = 0;

static CFStringRef kParameter_firC6_Name = CFSTR("Filter a6");
static const float kDefaultValue_fir6 = 0;

static CFStringRef kParameter_firC7_Name = CFSTR("Filter a7");
static const float kDefaultValue_fir7 = 0;


#pragma mark ____SFCEcho
class SFCEcho : public AUEffectBase
{
public:
	SFCEcho(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~SFCEcho () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult		Initialize();
	virtual UInt32				SupportedNumChannels (	const AUChannelInfo**	outInfo);
	virtual ComponentResult		ChangeStreamFormat(AudioUnitScope iScope, AudioUnitElement iElem, const CAStreamBasicDescription& sOld, const CAStreamBasicDescription& sNew);
	virtual AUKernelBase *		NewKernel() { return new SFCEchoKernel(this); }
	
	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
	virtual ComponentResult		SetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 	inElement,
											const void *		inData,
											UInt32              inDataSize);
	
 	virtual	bool				SupportsTail () { return true; }
//	virtual Float64 GetTailTime() {return 0.25;}
	
	/*! @method Version */
	virtual ComponentResult	Version() { return kSFCEchoVersion; }
	
	//int		GetNumCustomUIComponents () { return 1; }
	/*
	void	GetUIComponentDescs (ComponentDescription* inDescArray) {
        inDescArray[0].componentType = kAudioUnitCarbonViewComponentType;
        inDescArray[0].componentSubType = SFCEcho_COMP_SUBTYPE;
        inDescArray[0].componentManufacturer = SFCEcho_COMP_MANF;
        inDescArray[0].componentFlags = 0;
        inDescArray[0].componentFlagsMask = 0;
	}
    */
	
protected:
		class SFCEchoKernel : public AUKernelBase		// most real work happens here
	{
public:
		SFCEchoKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit)
	{
			mDelayUnit=512.0*GetSampleRate()/32000.0;
			mEchoBuffer.AllocateClear(7680*GetSampleRate()/32000.0);
			mFIRLength=8*GetSampleRate()/32000.0+0.5;
			mFIRLength*=2;
			mFIRbuf.AllocateClear(mFIRLength);
			mFIRweights.AllocateClear(mFIRLength);
			mFilterStride=32000.0/GetSampleRate();
			mEchoIndex=0;
			mFIRIndex=0;
			mPrevFilterDigest=0;
	};
		
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
private:
		TAUBuffer<Float32>	mEchoBuffer;
		TAUBuffer<Float32>	mFIRbuf;
		TAUBuffer<Float32>	mFIRweights;
		int					mEchoIndex;
		int					mFIRIndex, mFIRLength;
		int					mPrevFilterDigest;
		Float32				mDelayUnit;
		Float32				mFilterStride;
		
		void setWeights(Float32 *paramtaps);
	};
	
		Float32				mFilterBand[5];
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif