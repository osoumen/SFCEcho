/*
*	File:		SFCEcho_ViewFactory.h
*	
*	Version:	1.0
* 
*	Created:	ÇDATEÈ
*	
*	Copyright:  Copyright © ÇYEARÈ ÇORGANIZATIONNAMEÈ, All Rights Reserved
* 
*
*/

#import <Cocoa/Cocoa.h>
#import <AudioUnit/AUCocoaUIView.h>

@class SFCEcho_UIView;


@interface SFCEcho_ViewFactory : NSObject <AUCocoaUIBase>
{
    IBOutlet SFCEcho_UIView *	uiFreshlyLoadedView;
}

- (NSString *) description;	// string description of the view

@end