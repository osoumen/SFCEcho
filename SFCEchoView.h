/*
 *  SFCEchoView.h
 *  SFCEcho
 *
 *  Created by 開発用 on 06/11/30.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SFCEchoView__H_
#define __SFCEchoView__H_

#include "SFCEchoVersion.h"
#include "AUCarbonViewNib.h"
#include "SFCEchodefines.h"

static const UInt32  kNumDisplayRows = 1;

class SFCEchoView: public CAUCarbonViewNib
{
public:
    // Construction ------------------------------
    SFCEchoView(AudioUnitCarbonView inInstance): CAUCarbonViewNib(inInstance) {};
    // Baseclass overrides -----------------------
    CFStringRef GetBundleID() { return CFSTR("com.VeMa.audiounit.SFCEcho"); };  // my Bundle ID 
    void InitWindow(CFBundleRef sBundle);
	virtual void FinishWindow(CFBundleRef sBundle);
	virtual bool HandleEventForView(EventRef event, HIViewRef view);
	virtual int GetFontsize() { return 10; }
	virtual int GetTextJustification() { return teJustRight; }
	virtual CFStringRef GetFontname() { return CFSTR("Monaco"); }
	
	virtual void PropertyHasChanged(AudioUnitPropertyID inPropertyID, AudioUnitScope inScope,  
                                    AudioUnitElement inElement);
};

#endif
