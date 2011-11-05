/*
 *  SFCEchoView.cpp
 *  SFCEcho
 *
 *  Created by 開発用 on 06/11/30.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "SFCEchoView.h"
#include "TDragTextControl.h"

COMPONENT_ENTRY(SFCEchoView)

void SFCEchoView::InitWindow(CFBundleRef sBundle) // Register custom controls...
{
	CAUCarbonViewNib::InitWindow(sBundle);
    
    // Register custom controls classes
    TViewNoCompositingCompatible::RegisterClassForBundleID<TDragTextControl>(GetBundleID());
}

void SFCEchoView::FinishWindow(CFBundleRef sBundle)
{
	HIViewRef	control;
	HIViewID	id = {'user', 0};
	OSStatus	result;
	EventTypeSpec events[] = {
	{ kEventClassControl, kEventControlValueFieldChanged }
	};
	for (int i=0; i<5; i++) {
		id.id=i;
		result = HIViewFindByID(mRootUserPane, id, &control);
		if (result == noErr) {
			WantEventTypes(GetControlEventTarget(control), GetEventTypeCount(events), events);
			RegisterPropertyChanges(kAudioUnitCustomProperty_Band1+i);
			PropertyHasChanged(kAudioUnitCustomProperty_Band1+i,kAudioUnitScope_Global,0);
		}
	}
}

bool SFCEchoView::HandleEventForView(EventRef event, HIViewRef view)
{
	TCarbonEvent theEvent = event;
    UInt32 eclass = theEvent.GetClass();
    UInt32 ekind = theEvent.GetKind();
	HIViewID	id;
	Float32		floatValue;
	SInt32		maximum,cval;
	
	if (eclass == kEventClassControl) {
		HIViewGetID(view,&id);
		switch (ekind) {
			case kEventControlValueFieldChanged:
				if (id.signature == 'user') {
					maximum = GetControl32BitMaximum(view);
					cval = GetControl32BitValue(view);
					floatValue = (float)cval / (float)maximum;
					AudioUnitSetProperty(mEditAudioUnit,kAudioUnitCustomProperty_Band1+id.id,
										 kAudioUnitScope_Global,0,&floatValue,sizeof(float));
				}
				return true;
		}
	}
	
	return false;
}

bool SFCEchoView::HandleCommand(EventRef inEvent, HICommandExtended &cmd)
{
	switch (cmd.commandID) {
		case 'copy':
		{
			CFStringRef param_str = CreateXMSNESText();
			
			HIViewID	id = {'text',0};
			HIViewRef	control;
			OSStatus	result;
			result = HIViewFindByID(mRootUserPane, id, &control);
			HIViewSetText( control, param_str );
			
			OSStatus err = noErr;
			PasteboardRef theClipboard;
			err = PasteboardCreate( kPasteboardClipboard, &theClipboard );
			err = PasteboardClear( theClipboard );
			
			char text[256];
			CFStringGetCString( param_str, text, 256, kCFStringEncodingUTF8 );
			CFDataRef   data = CFDataCreate( kCFAllocatorDefault, (UInt8*)text, (strlen(text)) * sizeof(char) );
			err = PasteboardPutItemFlavor( theClipboard, (PasteboardItemID)1, kUTTypeUTF8PlainText, data, 0 );
			
			CFRelease(theClipboard);
			CFRelease( data );
			CFRelease(param_str);
			
			return true;
		}
	}
	return false;
}

CFStringRef SFCEchoView::CreateXMSNESText()
{
	CFStringRef	param_str;
	
	Float32	echovol_L;
	Float32 echovol_R;
	Float32 fir0;
	Float32 fir1;
	Float32 fir2;
	Float32 fir3;
	Float32 fir4;
	Float32 fir5;
	Float32 fir6;
	Float32 fir7;
	Float32 echodelay;
	Float32 echoFB;
	
	AudioUnitGetParameter(mEditAudioUnit,kParam_echovol_L,kAudioUnitScope_Global,0,&echovol_L);
	AudioUnitGetParameter(mEditAudioUnit,kParam_echovol_R,kAudioUnitScope_Global,0,&echovol_R);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir0,kAudioUnitScope_Global,0,&fir0);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir1,kAudioUnitScope_Global,0,&fir1);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir2,kAudioUnitScope_Global,0,&fir2);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir3,kAudioUnitScope_Global,0,&fir3);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir4,kAudioUnitScope_Global,0,&fir4);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir5,kAudioUnitScope_Global,0,&fir5);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir6,kAudioUnitScope_Global,0,&fir6);
	AudioUnitGetParameter(mEditAudioUnit,kParam_fir7,kAudioUnitScope_Global,0,&fir7);
	AudioUnitGetParameter(mEditAudioUnit,kParam_echodelay,kAudioUnitScope_Global,0,&echodelay);
	AudioUnitGetParameter(mEditAudioUnit,kParam_echoFB,kAudioUnitScope_Global,0,&echoFB);
	
	int vol_l = echovol_L;
	int vol_r = echovol_R;
	if (vol_l >= 0) {
		vol_l = 32 + vol_l * 47 / 127;
	}
	else {
		vol_l = 80 - vol_l * 46 / 128;
	}
	if (vol_r >= 0) {
		vol_r = 32 + vol_r * 47 / 127;
	}
	else {
		vol_r = 80 - vol_r * 46 / 128;
	}
	
	param_str = CFStringCreateWithFormat(NULL,NULL,
										 CFSTR(">%c%c%02X%02X%02X%02X%02X%02X%02X%02X%1X%02X"),
										 vol_l,
										 vol_r,
										 (UInt8)fir0,
										 (UInt8)fir1,
										 (UInt8)fir2,
										 (UInt8)fir3,
										 (UInt8)fir4,
										 (UInt8)fir5,
										 (UInt8)fir6,
										 (UInt8)fir7,
										 (UInt8)echodelay,
										 (UInt8)echoFB
										 );
	return param_str;
}

void SFCEchoView::PropertyHasChanged(AudioUnitPropertyID inPropertyID, AudioUnitScope inScope,  
						AudioUnitElement inElement)
{
	OSStatus	result;
	HIViewRef	control;
	HIViewID	id = {'user', 0};
	UInt32		size;
	float		floatValue;
	SInt32		maximum,cval;
	
	switch (inPropertyID) {
		case kAudioUnitCustomProperty_Band1:
		case kAudioUnitCustomProperty_Band2:
		case kAudioUnitCustomProperty_Band3:
		case kAudioUnitCustomProperty_Band4:
		case kAudioUnitCustomProperty_Band5:
			size = sizeof(float);
			AudioUnitGetProperty(mEditAudioUnit,inPropertyID,inScope,inElement,&floatValue,&size);
			id.id=inPropertyID-kAudioUnitCustomProperty_Band1;
			result = HIViewFindByID(mRootUserPane, id, &control);
			if (result == noErr) {
				maximum = GetControl32BitMaximum(control);
				cval = SInt32(floatValue*maximum + 0.5);
				SetControl32BitValue(control, cval);
			}
			break;
	}
}
