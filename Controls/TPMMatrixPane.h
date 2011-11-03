/*
 *  TPMMatrixPane.h
 *  NeuSynth
 *
 *  Created by Airy on Wed Mar 12 2003.
 *  Copyright (c) 2003 Airy ANDRE. All rights reserved.
 *
 */
#ifndef _TPMMatrixPane_h_
#define _TPMMatrixPane_h_

#include "TViewNoCompositingCompatible.h"

/*!
* \class TPMMatrixPane
 * \brief A Pane that switch its content according to its value.

 */
class TPMMatrixPane : public TViewNoCompositingCompatible
{
	friend class TViewNoCompositingCompatible;
public:
	static CFStringRef GetControlName() { return CFSTR("PMMatrix"); }
	static const OSType kControlKind = 'Mtrx';
        static const OSType kSubPanelSignature = 'SubP';
        static const OSType kSubPanelID = 'SbID';
protected:
	// Contstructor/Destructor
	TPMMatrixPane(HIViewRef inControl);
	virtual ~TPMMatrixPane();

    virtual bool UseNonblockingTracking() { return false; }
	virtual ControlKind GetKind();
//		virtual OSStatus StartTracking(TCarbonEvent& inEvent, HIPoint& from);
//		virtual OSStatus StopTracking(TCarbonEvent& inEvent, HIPoint& from);
//	virtual OSStatus StillTracking(TCarbonEvent&inEvent, HIPoint& from);

	virtual ControlPartCode HitTest(const HIPoint& inWhere);
	virtual OSStatus HandleEvent(EventHandlerCallRef    inCallRef, TCarbonEvent&        inEvent );

	virtual OSStatus Initialize(TCarbonEvent& inEvent);
	virtual OSStatus ControlHit(ControlPartCode inPart, UInt32 inModifiers );
	virtual void CompatibleDraw(RgnHandle	inLimitRgn, CGContextRef inContext, bool);
	virtual void Draw(RgnHandle	inLimitRgn, CGContextRef inContext);
	virtual void ValueChanged();

	virtual UInt32 GetBehaviors() { return TViewNoCompositingCompatible::GetBehaviors() | kControlSupportsEmbedding | kControlHandlesTracking; }
	static pascal OSStatus MouseEventHandler(EventHandlerCallRef	inCallRef,EventRef inEvent,void* inUserData );
	pascal OSStatus mouseDragged(TCarbonEvent&inEvent);
	pascal OSStatus mouseUp(TCarbonEvent&inEvent);
private:
        int mID;
	int mValue;
    EventHandlerRef      mMouseEventHandler;
    HIPoint mFromPoint;
};


#endif // _TPMMatrixPane_h_
