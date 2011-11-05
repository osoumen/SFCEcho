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
			UpdateXMSNESText();
			return true;
		}
	}
	return false;
}

void SFCEchoView::UpdateXMSNESText()
{
	HIViewID	id = {'text',0};
	HIViewRef	control;
	OSStatus	result;
	result = HIViewFindByID(mRootUserPane, id, &control);
	CFStringRef	param_str;
	
	int vol_l = mEditAudioUnit->GetParameter( kParam_echovol_L );
	int vol_r = mEditAudioUnit->GetParameter( kParam_echovol_R );
	if (vol_l >= 0) {
		vol_l = 32 + vol_l * 48 / 127;
	}
	else {
		vol_l = 80 - vol_l * 47 / 128;
	}
	if (vol_r >= 0) {
		vol_r = 32 + vol_r * 48 / 127;
	}
	else {
		vol_r = 80 - vol_r * 47 / 128;
	}
	
	param_str = CFStringCreateWithFormat(NULL,NULL,
										 CFSTR(">%c%c%02x%02x%02x%02x%02x%02x%02x%02x%1x%02x"),
										 vol_l,
										 vol_r,
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir0 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir1 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir2 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir3 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir4 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir5 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir6 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_fir7 ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_echodelay ),
										 (UInt8)mEditAudioUnit->GetParameter( kParam_echoFB )
										 );
	
	HIViewSetText( control, param_str );
	CFRelease(param_str);
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
