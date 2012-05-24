/*
*	File:		SFCEcho.cpp
*	
*	Version:	1.0
* 
*	Created:	06/08/28
*	
*	Copyright:  Copyright ˝ 2006 Vermicelli Magic, All Rights Reserved
*
*/

#include "SFCEcho.h"
#include <AudioToolbox/AudioToolbox.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(SFCEcho)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::SFCEcho
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SFCEcho::SFCEcho(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_mainvol_L, kDefaultValue_mainvol_L );
	SetParameter(kParam_mainvol_R, kDefaultValue_mainvol_R );
	SetParameter(kParam_echovol_L, kDefaultValue_echovol_L );
	SetParameter(kParam_echovol_R, kDefaultValue_echovol_R );
	SetParameter(kParam_echoFB, kDefaultValue_echoFB );
	SetParameter(kParam_echodelay, kDefaultValue_echodelay );
	SetParameter(kParam_fir0, kDefaultValue_fir0 );
	SetParameter(kParam_fir1, kDefaultValue_fir1 );
	SetParameter(kParam_fir2, kDefaultValue_fir2 );
	SetParameter(kParam_fir3, kDefaultValue_fir3 );
	SetParameter(kParam_fir4, kDefaultValue_fir4 );
	SetParameter(kParam_fir5, kDefaultValue_fir5 );
	SetParameter(kParam_fir6, kDefaultValue_fir6 );
	SetParameter(kParam_fir7, kDefaultValue_fir7 );
        
	for (int i=0; i<5; i++) {
		mFilterBand[i] = 1.0f;
	}
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult SFCEcho::Initialize(void)
{
    UInt32 in = GetInput(0)->GetStreamFormat().NumberChannels();
    UInt32 out = GetOutput(0)->GetStreamFormat().NumberChannels();
    if ((in != 2) || (out != 2)) {
        return kAudioUnitErr_FormatNotSupported;
    }	 
    return AUEffectBase::Initialize();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::SupportedNumChannels
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UInt32	SFCEcho::SupportedNumChannels (const AUChannelInfo** outInfo)
{
    static AUChannelInfo info[] = {{2, 2}};
    if (outInfo) {
        *outInfo = info;
    }
    return sizeof(info)/sizeof(*info);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::ChangeStreamFormat
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult SFCEcho::ChangeStreamFormat(AudioUnitScope iScope, AudioUnitElement iElem, const CAStreamBasicDescription& sOld, const CAStreamBasicDescription& sNew)
{
 //   const UInt32 iCha=sNew.NumberChannels();
    if (sOld.NumberChannels()!=sNew.NumberChannels())
    {
        const UInt32 iCha=sNew.NumberChannels();
        if ((iCha != 2) || (sNew.NumberInterleavedChannels()>1)) {
            return kAudioUnitErr_FormatNotSupported;
        }
    }
    return AUEffectBase::ChangeStreamFormat(iScope, iElem, sOld, sNew);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SFCEcho::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SFCEcho::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_mainvol_L:
                AUBase::FillInParameterName (outParameterInfo, kParameter_mainvol_L_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_mainvol_L;
                break;
            case kParam_mainvol_R:
                AUBase::FillInParameterName (outParameterInfo, kParameter_mainvol_R_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_mainvol_R;
                break;
            case kParam_echovol_L:
                AUBase::FillInParameterName (outParameterInfo, kParameter_echovol_L_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_echovol_L;
                break;
            case kParam_echovol_R:
                AUBase::FillInParameterName (outParameterInfo, kParameter_echovol_R_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_echovol_R;
                break;
            case kParam_echoFB:
                AUBase::FillInParameterName (outParameterInfo, kParameter_echoFB_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_echoFB;
                break;
			case kParam_echodelay:
                AUBase::FillInParameterName (outParameterInfo, kParameter_echodelay_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_UInt4;
                outParameterInfo.maxValue = kMaximumValue_UInt4;
                outParameterInfo.defaultValue = kDefaultValue_echodelay;
                break;
            case kParam_fir0:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC0_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir0;
                break;
            case kParam_fir1:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC1_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir1;
                break;
            case kParam_fir2:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC2_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir2;
                break;
            case kParam_fir3:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC3_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir3;
                break;
            case kParam_fir4:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC4_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir4;
                break;
            case kParam_fir5:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC5_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir5;
                break;
            case kParam_fir6:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC6_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir6;
                break;
            case kParam_fir7:
                AUBase::FillInParameterName (outParameterInfo, kParameter_firC7_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_SInt8;
                outParameterInfo.maxValue = kMaximumValue_SInt8;
                outParameterInfo.defaultValue = kDefaultValue_fir7;
                break;
			default:
                result = kAudioUnitErr_InvalidParameter;
                break;
            }
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    


	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SFCEcho::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	if (inScope == kAudioUnitScope_Global) {
		switch (inID) {
			
			case kAudioUnitProperty_CocoaUI:
				outWritable = false;
				outDataSize = sizeof(AudioUnitCocoaViewInfo);
				return noErr;
			
			case kAudioUnitCustomProperty_Band1:
			case kAudioUnitCustomProperty_Band2:
			case kAudioUnitCustomProperty_Band3:
			case kAudioUnitCustomProperty_Band4:
			case kAudioUnitCustomProperty_Band5:
				outWritable = false;
				outDataSize = sizeof(Float32);
				return noErr;
		}
	}
	return AUEffectBase::GetPropertyInfo(inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SFCEcho::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	if (inScope == kAudioUnitScope_Global) {
		switch (inID) {
			
			case kAudioUnitProperty_CocoaUI:
			{
				CFBundleRef bundle = CFBundleGetBundleWithIdentifier( CFSTR("com.VeMa.audiounit.SFCEcho") );
				
				if (bundle == NULL) return fnfErr;
                
				CFURLRef bundleURL = CFBundleCopyResourceURL( bundle, 
															  CFSTR("SFCEcho_ViewFactory"), 
															  CFSTR("bundle"), 
															  NULL);
                
                if (bundleURL == NULL) return fnfErr;
                
				CFStringRef className = CFSTR("SFCEcho_ViewFactory");
				AudioUnitCocoaViewInfo cocoaInfo = { bundleURL, className };
				*((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
				
				return noErr;
			}
			
			case kAudioUnitCustomProperty_Band1:
			case kAudioUnitCustomProperty_Band2:
			case kAudioUnitCustomProperty_Band3:
			case kAudioUnitCustomProperty_Band4:
			case kAudioUnitCustomProperty_Band5:
				*((Float32 *)outData) = mFilterBand[inID-kAudioUnitCustomProperty_Band1];
				return noErr;
		}
	}
	return AUEffectBase::GetProperty(inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::SetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SFCEcho::SetProperty(	AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 	inElement,
											const void *		inData,
											UInt32              inDataSize)
{
	float bandfactor[8][5] = {
	{0.0625,0.125,		0.12500,	0.25,		0.4375},
	{0.0625,0.11548,	0.08839,	0.04784,	-0.314209},
	{0.0625,0.08839,	0.00000,	-0.21339,	0.0625},
	{0.0625,0.04784,	-0.08839,	-0.11548,	0.093538},	
	{0.0625,0.00000,	-0.12500,	0.125,		-0.0625},
	{0.0625,-0.04784,	-0.08839,	0.11548,	-0.041761},
	{0.0625,-0.08839,	0.00000,	-0.03661,	0.0625},
	{0.0625,-0.11548,	0.08839,	-0.04784,	0.012432}
	};
	float	temp;
	AudioUnitEvent auEvent;
	
	if (inScope == kAudioUnitScope_Global) {
		switch (inID) {
			case kAudioUnitCustomProperty_Band1:
			case kAudioUnitCustomProperty_Band2:
			case kAudioUnitCustomProperty_Band3:
			case kAudioUnitCustomProperty_Band4:
			case kAudioUnitCustomProperty_Band5:
				mFilterBand[inID-kAudioUnitCustomProperty_Band1] = *((Float32*)inData);
				
				for (int j=0; j<8; j++) {
					temp = 0;
					for (int i=0; i<5; i++) {
						temp += mFilterBand[i] * bandfactor[j][i];
					}
					if (temp < 0)
						temp = ceil(temp*127);
					else
						temp = floor(temp*127);
					
					SetParameter(kParam_fir0+j, temp);
					auEvent.mEventType = kAudioUnitEvent_ParameterValueChange;
					auEvent.mArgument.mParameter.mAudioUnit = (AudioUnit)GetComponentInstance();
					auEvent.mArgument.mParameter.mScope = kAudioUnitScope_Global;
					auEvent.mArgument.mParameter.mParameterID = kParam_fir0+j;
					auEvent.mArgument.mParameter.mElement = 0;
					AUEventListenerNotify(NULL, NULL, &auEvent);
				}
				return noErr;
		}
	}
	return AUEffectBase::SetProperty(inID, inScope, inElement, inData, inDataSize);
}


#pragma mark ____SFCEchoEffectKernel

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::SFCEchoKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		SFCEcho::SFCEchoKernel::Reset()
{
	mEchoBuffer.Clear();
	mFIRbuf.Clear();
	mEchoIndex=0;
	mFIRIndex=0;
	mPrevFilterDigest=0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SFCEcho::SFCEchoKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SFCEcho::SFCEchoKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels,	//inNumChannelÇ…ÇÕÇPÇµÇ©Ç±Ç»Ç¢
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	Float32 gain=0.0,echo_vol=0.0,fb_lev,fir_taps[8];
	int		i, delay_samples, filter_digest=0;
	
	//ÉpÉâÉÅÅ[É^ÇÃì«Ç›çûÇ›
	if (GetChannelNum() == 0) {		//LÉ`ÉÉÉìÉlÉã
		gain = GetParameter( kParam_mainvol_L ) / 128.0;
		echo_vol = GetParameter( kParam_echovol_L ) / 128.0;
	}
	else if (GetChannelNum() == 1) {	//RÉ`ÉÉÉìÉlÉã
		gain = GetParameter( kParam_mainvol_R ) / 128.0;
		echo_vol = GetParameter( kParam_echovol_R ) / 128.0;
	}
	fb_lev = GetParameter( kParam_echoFB ) / 128.0;
	delay_samples = GetParameter( kParam_echodelay ) * mDelayUnit;
	if (delay_samples < 0) delay_samples=0;
	
	for (i=0; i<8; i++) {
		fir_taps[i] = GetParameter( kParam_fir0+i ) / 128.0;
		filter_digest ^= (int)(fir_taps[i]*128);
	}
	if (filter_digest != mPrevFilterDigest) {
		setWeights(fir_taps);
	}
	mPrevFilterDigest = filter_digest;
		
	while (nSampleFrames-- > 0) {
		Float32 input = *sourceP;
		sourceP += inNumChannels;
		
		Float32 output,temp,echo;
		UInt32	echoabs;
		
		echo = input;
		output = input * gain;
		mEchoIndex &= 0x7fff;
		
		//ÉfÉBÉåÉCêMçÜÇ…FIRÉtÉBÉãÉ^Çä|ÇØÇƒÇ©ÇÁèoóÕÇ…â¡éZÇ∑ÇÈ
		mFIRbuf[mFIRIndex] = mEchoBuffer[mEchoIndex];
		temp =0;
		for (i=0; i<mFIRLength-1; i++) {
			temp += mFIRbuf[mFIRIndex] * mFIRweights[i];
			mFIRIndex = (mFIRIndex + 1)%mFIRLength;
		}
		temp += mFIRbuf[mFIRIndex] * mFIRweights[i];
		//mFIRbufÇ÷ÇÃèëÇ´çûÇ›ÇÕÅAâEÇ©ÇÁç∂Ç÷Ç∆çsÇÌÇÍÇÈ
		output += temp*echo_vol;
		
		//ì¸óÕÇ…ÉtÉBÅ[ÉhÉoÉbÉNÇâ¡éZÇµÇΩÇ‡ÇÃÇÉoÉbÉtÉ@ÉLÉÖÅ[Ç…ì¸ÇÍÇÈ
		echo += temp*fb_lev;
		echoabs = (*(int*)&echo)&0x7fffffff;
		if (*(float*)&echoabs < 1.0/32768.0)
			echo = 0;
		mEchoBuffer[mEchoIndex++] = echo;
		if (mEchoIndex >= delay_samples)
			mEchoIndex=0;
		
		*destP = output;
		destP += inNumChannels;
	}
}

const float onepi = 3.14159265358979;

void SFCEcho::SFCEchoKernel::setWeights(Float32 *paramtaps)
{
	Float32	temp,pos,pix,flat=0;
	int		i,min,max;
	
	if (paramtaps[0] == 127.0/128.0) {
		for (i=1; i<8; i++) {
			flat += paramtaps[i];
		}
		if (flat == 0) {
			mFIRweights[0] = 1.0;
			for (i=1; i<mFIRLength; i++) {
				mFIRweights[i] = 0;
			}
			return;
		}
	}
	
	for (i=0; i<mFIRLength; i++) {
		pos=(i-mFIRLength/2)*mFilterStride;
		temp=0;
		min=pos+0.5;
		max=min+8;
		min-=8;
		for (int j=min; j<max; j++) {
			pix=(pos-j)*onepi;
			if (j>=0 && j<8) {
				if (pix == .0)
					temp+=paramtaps[j];
				else
					temp+=paramtaps[j]*sinf(pix)/pix;
			}
		}
		//temp*=0.5-0.5*cosf((2.0*onepi*i)/mFIRLength);
		temp*=expf(-4.5*((2*i-mFIRLength)/(float)mFIRLength)*((2*i-mFIRLength)/(float)mFIRLength));
		
		mFIRweights[i]=temp;
	}
}

