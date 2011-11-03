/*
 *  TPMMatrixPane.cpp
 *  NeuSynth
 *
 *  Created by Airy on Wed Mar 12 2003.
 *  Copyright (c) 2003 Airy ANDRE. All rights reserved.
 *
 */

#include "AUGUIUtilities.h"
#include "TPMMatrixPane.h"

const EventTypeSpec kHIViewRadioEvents[] =
{{ kEventClassControl, kEventControlInterceptSubviewClick }};

// -----------------------------------------------------------------------------
//	TPMMatrixPane constructor
// -----------------------------------------------------------------------------
//
TPMMatrixPane::TPMMatrixPane(
									 HIViewRef			inControl )
:	TViewNoCompositingCompatible( inControl), mID(-1),mValue(1)
{
}

pascal OSStatus TPMMatrixPane::MouseEventHandler(
                                                                EventHandlerCallRef	inCallRef,
                                                                EventRef			inEvent,
                                                                void*				inUserData )
{
    OSStatus			result = eventNotHandledErr;
    TPMMatrixPane*				view = (TPMMatrixPane*) inUserData;
    TCarbonEvent		event( inEvent );
    
    switch ( event.GetClass() )
    {
        case kEventClassMouse:
            switch ( event.GetKind() )
            {
                case kEventMouseUp:
                    result = view->mouseUp(event);
                    break;
                    
                case kEventMouseDragged:
                    result = view->mouseDragged(event);
                    break;
            }
            break;
    }
    return result;
}
//-----------------------------------------------------------------------------------
//	mMouseEventHandler
//-----------------------------------------------------------------------------------
//!	Our static mouseDragged event handler proc.
//
OSStatus TPMMatrixPane::mouseDragged(TCarbonEvent&inEvent)
{
	HIPoint				where;
	ControlPartCode		part;
	inEvent.GetParameter<HIPoint>( kEventParamWindowMouseLocation, typeHIPoint, &where );
    HIViewConvertPoint(&where,NULL , GetViewRef());
	
	part = HitTest( where );
	inEvent.SetParameter<ControlPartCode>( kEventParamControlPart, typeControlPartCode, part );
	return noErr;
}

//!	Our static mouseUp event handler proc.
OSStatus TPMMatrixPane::mouseUp(TCarbonEvent&inEvent)
{
    // Remove mouse handler...
    if (mMouseEventHandler) {
        RemoveEventHandler(mMouseEventHandler);
        mMouseEventHandler = 0;
    }
    
    HIPoint mouse;
    inEvent.GetParameter<HIPoint>(kEventParamWindowMouseLocation, typeHIPoint, &mouse);
    HIViewConvertPoint(&mouse,NULL , GetViewRef());
    ControlPartCode wherePart = HitTest(mouse);
	if(wherePart == kControlNoPart)
		wherePart = mValue;

    inEvent.SetParameter<ControlPartCode>(kEventParamControlPart, typeControlPartCode, wherePart);
	OSStatus result = noErr;

    // create a control hit event to simulate a normal Track call
    if (result == noErr)
    {
        TCarbonEvent hitEvent(kEventClassControl, kEventControlHit);
        
        // get values from the in event to pass to the new event
        ControlPartCode hitPart = kControlNoPart;
        inEvent.GetParameter(kEventParamControlPart, &hitPart);
        UInt32 modifiers = 0;
        inEvent.GetParameter(kEventParamKeyModifiers, &modifiers);
        
        // set up parameters
        hitEvent.SetParameter<ControlRef>(kEventParamDirectObject, typeControlRef, GetViewRef());
        hitEvent.SetParameter(kEventParamControlPart, hitPart);
        hitEvent.SetParameter(kEventParamKeyModifiers, modifiers);
        
        // post to the current event queue
        hitEvent.PostToQueue(GetCurrentEventQueue());
    }
    return result;
}

// -----------------------------------------------------------------------------
//	TPMMatrixPane destructor
// -----------------------------------------------------------------------------
//	Clean up after yourself.
//
TPMMatrixPane::~TPMMatrixPane()
{
}

// -----------------------------------------------------------------------------
//	GetKind
// -----------------------------------------------------------------------------
//
ControlKind TPMMatrixPane::GetKind()
{
    const ControlKind kMyKind = { '  PM', kControlKind };
    return kMyKind;
}


//-----------------------------------------------------------------------------------
//	Initialize
//-----------------------------------------------------------------------------------
//	The control is set up.  Do the last minute stuff that needs to be done like
//	installing EventLoopTimers.
//
OSStatus TPMMatrixPane::Initialize(
									   TCarbonEvent&		inEvent )
{
#pragma unused( inEvent )
	AddEventTypesToHandler(fHandler, GetEventTypeCount( kHIViewRadioEvents ), kHIViewRadioEvents);
    return noErr;
}

OSStatus TPMMatrixPane::HandleEvent(EventHandlerCallRef    inCallRef, TCarbonEvent&        inEvent )
{
#pragma unused( inCallRef )
	
    OSStatus            result = eventNotHandledErr;
	

    switch ( inEvent.GetClass() )
    {
        case kEventClassControl:
            switch ( inEvent.GetKind() )
            {
                case kEventControlInterceptSubviewClick :
					{
						static const EventTypeSpec kHIMouseEvents[] = {{ kEventClassMouse, kEventMouseUp },{ kEventClassMouse, kEventMouseDragged }};
							
						InstallEventHandler(
												GetWindowEventTarget(GetOwner()),
												MouseEventHandler,
												GetEventTypeCount(kHIMouseEvents),
												kHIMouseEvents,
												this,
												&mMouseEventHandler);
						result = noErr;
					}
					break;

				default:
					result = TView::HandleEvent(inCallRef, inEvent);
					break;
					
                    // We should return noErr if we want the click
            }
			break;
		default:
			result = TView::HandleEvent(inCallRef, inEvent);
			break;
    }
	return result;
}

OSStatus TPMMatrixPane::ControlHit(
									   ControlPartCode		inPart,
									   UInt32				inModifiers )
{
#pragma unused( inModifiers )
	SetValue( inPart );
	mValue = inPart;
	return noErr;
}
//-----------------------------------------------------------------------------------
//	ValueChanged
//-----------------------------------------------------------------------------------
void TPMMatrixPane::ValueChanged()
{
	Invalidate();
}
//-----------------------------------------------------------------------------------
//	Draw
//-----------------------------------------------------------------------------------
//	The fun part of the control
//
void TPMMatrixPane::Draw(
									   RgnHandle				inLimitRgn,
									   CGContextRef			inContext)
{
	UInt16 count = 0;
	OSErr status = CountSubControls(GetViewRef(), &count);
	for (int i = 1; i <= count; i++) {
		ControlRef child;
		status = GetIndexedSubControl(GetViewRef(), i, &child);
		if(status == noErr)
			SetControlValue(child, i == GetValue());
		
	}
}

void TPMMatrixPane::CompatibleDraw(
						 RgnHandle				inLimitRgn,
						 CGContextRef			inContext,
						 bool inCompositing)
{
}

ControlPartCode TPMMatrixPane::HitTest(const HIPoint& inWhere)
{
#pragma unused( status )

	ControlPartCode		part = kControlNoPart;
	HIRect		bounds = Bounds();
		
		UInt16 count = 0;
		OSErr status = CountSubControls(GetViewRef(), &count);

	// Is the mouse in the view?
    if (CGRectContainsPoint(bounds, inWhere))
	{

		float segSize = Bounds().size.width / count;
		part = (SInt16)floor((inWhere.x - Bounds().origin.x )/segSize)+1;
		if(part > count)
		   part = count;
	} else {
		printf("Not in control\n");
		part = mValue;
	}
	SetValue(part);

	return part;
}
