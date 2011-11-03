/*
*	File:		SFCEcho_UIView.h
*	
*	Version:	1.0
* 
*	Created:	ÇDATEÈ
*	
*	Copyright:  Copyright © ÇYEARÈ ÇORGANIZATIONNAMEÈ, All Rights Reserved
*
*/

#import <Cocoa/Cocoa.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioToolbox.h>

#include "SFCEchodefines.h"

@interface SFCEcho_UIView : NSView
{
    // IB Members
    IBOutlet NSSlider *				uiParamSlider0;
	IBOutlet NSSlider *				uiParamSlider1;
	IBOutlet NSSlider *				uiParamSlider2;
	IBOutlet NSSlider *				uiParamSlider3;
	IBOutlet NSSlider *				uiParamSlider4;
	IBOutlet NSSlider *				uiParamSlider5;
    IBOutlet NSTextField *			uiParamTextField0;
	IBOutlet NSTextField *			uiParamTextField1;
	IBOutlet NSTextField *			uiParamTextField2;
	IBOutlet NSTextField *			uiParamTextField3;
	IBOutlet NSTextField *			uiParamTextField4;
	IBOutlet NSTextField *			uiParamTextField5;
	IBOutlet NSTextField *			uiParamFIRTapTextField0;
	IBOutlet NSTextField *			uiParamFIRTapTextField1;
	IBOutlet NSTextField *			uiParamFIRTapTextField2;
	IBOutlet NSTextField *			uiParamFIRTapTextField3;
	IBOutlet NSTextField *			uiParamFIRTapTextField4;
	IBOutlet NSTextField *			uiParamFIRTapTextField5;
	IBOutlet NSTextField *			uiParamFIRTapTextField6;
	IBOutlet NSTextField *			uiParamFIRTapTextField7;
	IBOutlet NSSlider *				uiBandSlider0;
	IBOutlet NSSlider *				uiBandSlider1;
	IBOutlet NSSlider *				uiBandSlider2;
	IBOutlet NSSlider *				uiBandSlider3;
	IBOutlet NSSlider *				uiBandSlider4;
	IBOutlet NSTextField *			uiBandValue0;
	IBOutlet NSTextField *			uiBandValue1;
	IBOutlet NSTextField *			uiBandValue2;
	IBOutlet NSTextField *			uiBandValue3;
	IBOutlet NSTextField *			uiBandValue4;
	
    // Other Members
    AudioUnit 				mAU;
    AUEventListenerRef		mAUEventListener;
	AudioUnitParameter		parameter[kNumberOfParameters];
	AudioUnitProperty		property[kNumberOfProperties];
}

#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU;

#pragma mark ____ INTERFACE ACTIONS ____
- (IBAction)iaSliderParamChanged:(id)sender;
- (IBAction)iaBandValueChanged:(id)sender;

#pragma mark ____ PRIVATE FUNCTIONS
- (void)_synchronizeUIWithParameterValues;
- (void)_addListeners;
- (void)_removeListeners;

#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
- (void)_eventListener:(void *) inObject event:(const AudioUnitEvent *)inEvent value:(Float32)inValue;

@end
