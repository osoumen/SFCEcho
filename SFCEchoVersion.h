/*
*	File:		SFCEchoVersion.h
* 	
*	Version:	1.0
* 
*	Created:	06/08/28
*	
*	Copyright:  Copyright © 2006 __MyCompanyName__, All Rights Reserved
*
*/

#ifndef __SFCEchoVersion_h__
#define __SFCEchoVersion_h__


#ifdef DEBUG
	#define kSFCEchoVersion 0xFFFFFFFF
#else
	#define kSFCEchoVersion 0x00010000	
#endif

//~~~~~~~~~~~~~~  Change!!! ~~~~~~~~~~~~~~~~~~~~~//
#define SFCEcho_COMP_SUBTYPE		kAudioUnitSubType_Delay
#define SFCEcho_COMP_MANF			'VeMa'
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#endif

