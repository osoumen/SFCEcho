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
	
	//HIViewSetText( control, CFSTR("test") );
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
