/*
*	File:		SFCEcho_UIView.m
*	
*	Version:	1.0
* 
*	Created:	ﾇDATEﾈ
*	
*	Copyright:  Copyright ｩ ﾇYEARﾈ ﾇORGANIZATIONNAMEﾈ, All Rights Reserved
*
*/

#import "SFCEcho_UIView.h"
#import <math.h>

#pragma mark ____ LISTENER CALLBACK DISPATCHER ____

void EventListenerDispatcher (void *inRefCon, void *inObject, const AudioUnitEvent *inEvent, UInt64 inHostTime, Float32 inValue)
{
	SFCEcho_UIView *SELF = (SFCEcho_UIView *)inRefCon;
    
    [SELF _eventListener:inObject event: inEvent value: inValue];
}

@implementation SFCEcho_UIView

#pragma mark ____ (INIT /) DEALLOC ____
- (void)dealloc {
    [self _removeListeners];
    [super dealloc];
}

#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU {
	// remove previous listeners
	if (mAU) [self _removeListeners];
	mAU = inAU;
    
	// add new listeners
	[self _addListeners];
	
	// initial setup
	[self _synchronizeUIWithParameterValues];
}

#pragma mark ____ INTERFACE ACTIONS ____
- (IBAction)iaSliderParamChanged:(id)sender {
	NSSlider *uiParamSlider[6] = {
		uiParamSlider0,
		uiParamSlider1,
		uiParamSlider2,
		uiParamSlider3,
		uiParamSlider4,
		uiParamSlider5
	};
	NSTextField *uiParamTextField[14] = {
		uiParamTextField0,
		uiParamTextField1,
		uiParamTextField2,
		uiParamTextField3,
		uiParamTextField4,
		uiParamTextField5,
		uiParamFIRTapTextField0,
		uiParamFIRTapTextField1,
		uiParamFIRTapTextField2,
		uiParamFIRTapTextField3,
		uiParamFIRTapTextField4,
		uiParamFIRTapTextField5,
		uiParamFIRTapTextField6,
		uiParamFIRTapTextField7
	};
	int	tag = [sender tag];
    float floatValue = [sender floatValue];
	floatValue = floor(floatValue);
	
	if (tag >= kParam_mainvol_L && tag < kParam_echodelay) {
		if (sender == uiParamSlider[tag]) {
			[uiParamTextField[tag] setFloatValue:floatValue];
		} else {
			[uiParamSlider[tag] setFloatValue:floatValue];
		}
	}
	else if (tag == kParam_echodelay) {
		[uiParamTextField[tag] setFloatValue:floatValue*16];
	}
	NSAssert(	AUParameterSet(mAUEventListener, sender, &parameter[tag], (Float32)floatValue, 0) == noErr,
                @"[SFCEcho_UIView iaSliderParamChanged:] AUParameterSet()");
}


- (IBAction)iaBandValueChanged:(id)sender {
	NSSlider *uiBandSlider[5] = {
		uiBandSlider0,
		uiBandSlider1,
		uiBandSlider2,
		uiBandSlider3,
		uiBandSlider4
	};
	NSTextField *uiBandValue[5] = {
		uiBandValue0,
		uiBandValue1,
		uiBandValue2,
		uiBandValue3,
		uiBandValue4
	};
	int tag = [sender tag];
	float floatValue = [sender floatValue];
	
	if (sender == uiBandSlider[tag]) {
		[uiBandValue[tag] setFloatValue:floatValue];
	} else {
		[uiBandSlider[tag] setFloatValue:floatValue];
	}
	AudioUnitSetProperty(mAU,kAudioUnitCustomProperty_Band1+tag,kAudioUnitScope_Global,0,&floatValue,sizeof(float));
}


#pragma mark ____ PRIVATE FUNCTIONS ____
- (void)_addListeners {
	AudioUnitEvent auEvent;
	
	NSAssert (	AUEventListenerCreate(	EventListenerDispatcher, self, 
										CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0.05, 0.05,
										&mAUEventListener	) == noErr,
                @"[SFCEcho_UIView _addListeners] AUEventListenerCreate()");
	
    int i;
    for (i = 0; i < kNumberOfParameters; ++i) {
        parameter[i].mAudioUnit = mAU;
		parameter[i].mParameterID = i;
		parameter[i].mScope = kAudioUnitScope_Global;
		parameter[i].mElement = 0;
		auEvent.mArgument.mParameter=parameter[i];
		auEvent.mEventType=kAudioUnitEvent_ParameterValueChange;
        NSAssert (	AUEventListenerAddEventType(mAUEventListener, self, &auEvent) == noErr,
                    @"[SFCEcho_UIView _addListeners] AUEventListenerAddEventType()");
    }
	
	for (i=0; i<kNumberOfProperties; i++) {
		property[i].mAudioUnit = mAU;
		property[i].mPropertyID = kAudioUnitCustomProperty_First + i;
		property[i].mScope = kAudioUnitScope_Global;
		property[i].mElement = 0;
		auEvent.mArgument.mProperty = property[i];
		auEvent.mEventType = kAudioUnitEvent_PropertyChange;
		NSAssert ( AUEventListenerAddEventType(mAUEventListener, self, &auEvent) == noErr,
				   @"[SFCEcho_UIView _addListeners] AUEventListenerAddEventType()");
	}
}

- (void)_removeListeners {
    int i;
	AudioUnitEvent auEvent;
	
	for (i=0; i<kNumberOfProperties; i++) {
		auEvent.mEventType = kAudioUnitEvent_PropertyChange;
		auEvent.mArgument.mProperty = property[i];
		NSAssert (	AUEventListenerRemoveEventType(mAUEventListener, self, &auEvent) == noErr,
					@"[SFCEcho_UIView _removeListeners] AUEventListenerRemoveEventType()");
	}
	
	for (i=0; i<kNumberOfParameters; ++i) {
        NSAssert (	AUListenerRemoveParameter(mAUEventListener, self, &parameter[i]) == noErr,
                    @"[SFCEcho_UIView _removeListeners] AUListenerRemoveParameter()");
    }
	
	NSAssert (	AUListenerDispose(mAUEventListener) == noErr,
                @"[SFCEcho_UIView _removeListeners] AUListenerDispose()");
	mAUEventListener = NULL;
	mAU = NULL;
}

- (void)_synchronizeUIWithParameterValues {
	AudioUnitEvent auEvent;
	Float32 value;
    int i;
    
	//パラメータの同期
    for (i=0; i<kNumberOfParameters; ++i) {
        NSAssert (	AudioUnitGetParameter(mAU, parameter[i].mParameterID, kAudioUnitScope_Global, 0, &value) == noErr,
                    @"[SFCEcho_UIView synchronizeUIWithParameterValues] (x.1)");
		auEvent.mArgument.mParameter=parameter[i];
		auEvent.mEventType=kAudioUnitEvent_ParameterValueChange;
		[self _eventListener:self event:&auEvent value:value];
    }
	
	//プロパティの同期
	for (i=0; i<kNumberOfProperties; i++) {
		auEvent.mEventType = kAudioUnitEvent_PropertyChange;
		auEvent.mArgument.mProperty = property[i];
		[self _eventListener:self event:&auEvent value:.0f];
	}
}

#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
- (void)_eventListener:(void *) inObject event:(const AudioUnitEvent *)inEvent value:(Float32)inValue
{
	UInt32	size;
	float	floatValue;
	
    //inObject ignored in this case.
	NSSlider *uiParamSlider[6] = {
		uiParamSlider0,
		uiParamSlider1,
		uiParamSlider2,
		uiParamSlider3,
		uiParamSlider4,
		uiParamSlider5
	};
	NSTextField *uiParamTextField[14] = {
		uiParamTextField0,
		uiParamTextField1,
		uiParamTextField2,
		uiParamTextField3,
		uiParamTextField4,
		uiParamTextField5,
		uiParamFIRTapTextField0,
		uiParamFIRTapTextField1,
		uiParamFIRTapTextField2,
		uiParamFIRTapTextField3,
		uiParamFIRTapTextField4,
		uiParamFIRTapTextField5,
		uiParamFIRTapTextField6,
		uiParamFIRTapTextField7
	};
	switch (inEvent->mEventType) {
		case kAudioUnitEvent_ParameterValueChange:
			switch (inEvent->mArgument.mParameter.mParameterID) {
				case kParam_mainvol_L:
				case kParam_mainvol_R:
				case kParam_echovol_L:
				case kParam_echovol_R:
				case kParam_echoFB:
				case kParam_echodelay:
					[uiParamSlider[inEvent->mArgument.mParameter.mParameterID] setFloatValue:inValue];
				case kParam_fir0:
				case kParam_fir1:
				case kParam_fir2:
				case kParam_fir3:
				case kParam_fir4:
				case kParam_fir5:
				case kParam_fir6:
				case kParam_fir7:
					if (inEvent->mArgument.mParameter.mParameterID == kParam_echodelay)
						inValue *= 16;
					[uiParamTextField[inEvent->mArgument.mParameter.mParameterID] setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
					break;
			}
			break;
			
		case kAudioUnitEvent_PropertyChange:
			size = sizeof(float);
			AudioUnitGetProperty(mAU,inEvent->mArgument.mProperty.mPropertyID,kAudioUnitScope_Global,0,&floatValue,&size);
			
			switch (inEvent->mArgument.mProperty.mPropertyID) {
				case kAudioUnitCustomProperty_Band1:
					[uiBandSlider0 setFloatValue:floatValue];
					[uiBandValue0 setStringValue:[[NSNumber numberWithFloat:floatValue] stringValue]];
					break;
				case kAudioUnitCustomProperty_Band2:
					[uiBandSlider1 setFloatValue:floatValue];
					[uiBandValue1 setStringValue:[[NSNumber numberWithFloat:floatValue] stringValue]];
					break;
				case kAudioUnitCustomProperty_Band3:
					[uiBandSlider2 setFloatValue:floatValue];
					[uiBandValue2 setStringValue:[[NSNumber numberWithFloat:floatValue] stringValue]];
					break;
				case kAudioUnitCustomProperty_Band4:
					[uiBandSlider3 setFloatValue:floatValue];
					[uiBandValue3 setStringValue:[[NSNumber numberWithFloat:floatValue] stringValue]];
					break;
				case kAudioUnitCustomProperty_Band5:
					[uiBandSlider4 setFloatValue:floatValue];
					[uiBandValue4 setStringValue:[[NSNumber numberWithFloat:floatValue] stringValue]];
					break;
			}
			break;
			
		default:
			break;
	}
}


@end
