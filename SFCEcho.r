/*
*	File:		SFCEcho.r
*	
*	Version:	1.0
* 
*	Created:	06/08/28
*	
*	Copyright:  Copyright © 2006 Vermicelli Magic, All Rights Reserved
*
*/
#include <AudioUnit/AudioUnit.r>

#include "SFCEchoVersion.h"

// Note that resource IDs must be spaced 2 apart for the 'STR ' name and description
#define kAudioUnitResID_SFCEcho				1000
#define kAudioUnitResID_SFCEchoView			2000

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SFCEcho~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define RES_ID			kAudioUnitResID_SFCEcho
#define COMP_TYPE		kAudioUnitType_Effect
#define COMP_SUBTYPE	SFCEcho_COMP_SUBTYPE
#define COMP_MANUF		SFCEcho_COMP_MANF	

#define VERSION			kSFCEchoVersion
#define NAME			"osoumen: SFCEcho"
#define DESCRIPTION		"SFCEcho AU"
#define ENTRY_POINT		"SFCEchoEntry"

#include "AUResources.r"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SFCEchoView~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define RES_ID			kAudioUnitResID_SFCEchoView
#define COMP_TYPE		kAudioUnitCarbonViewComponentType
#define COMP_SUBTYPE	SFCEcho_COMP_SUBTYPE
#define COMP_MANUF		SFCEcho_COMP_MANF	

#define VERSION			kSFCEchoVersion
#define NAME			"osoumen: SFCEchoView"
#define DESCRIPTION		"SFCEcho Carbon AU View"
#define ENTRY_POINT		"SFCEchoViewEntry"

#include "AUResources.r"
