//OSXUIToolkit.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/ApplicationKitPrivate.h"
#include "vcf/ApplicationKit/OSXDesktopPeer.h"
#include "vcf/ApplicationKit/OSXApplicationPeer.h"
#include "vcf/ApplicationKit/OSXControlContextPeer.h"
#include "vcf/ApplicationKit/OSXCursorPeer.h"
#include "vcf/ApplicationKit/OSXWindow.h"



#define kSleepTime	32767


namespace VCF {
//UIMetricsManager implementation for OSX
class OSXUIMetricsManager : public UIMetricsManager {
public:
	OSXUIMetricsManager(){}

	virtual ~OSXUIMetricsManager(){}

	virtual VCF::Font getDefaultFontFor( const UIMetricsManager::FontType& type ) {
		VCF::Font result("Arial", 10);

		result.setColor( GraphicsToolkit::getSystemColor( SYSCOLOR_CAPTION_TEXT ) );
		switch ( type ) {
			case UIMetricsManager::ftMenuItemFont : {

			}
			break;

			case UIMetricsManager::ftSelectedMenuItemFont : {

			}
			break;

			case UIMetricsManager::ftControlFont :  case UIMetricsManager::ftSystemFont : {

			}
			break;

			case UIMetricsManager::ftMessageFont : {

			}
			break;

			case UIMetricsManager::ftToolTipFont : {

			}
			break;
		}

		return result;
	}

	virtual double getDefaultHeightFor( const UIMetricsManager::HeightType& type )  {
		double result = 0.0;
		switch ( type ) {
			case UIMetricsManager::htLabelHeight : {
				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftControlFont );
				result = f.getHeight() * 1.75;
			}
			break;

			case UIMetricsManager::htComboBoxHeight : {
				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftControlFont );
				result = f.getHeight() * 2.0;
			}
			break;

			case UIMetricsManager::htListItemHeight : {
				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftControlFont );
				result = f.getHeight() * 1.65;
			}
			break;

			case UIMetricsManager::htButtonHeight : {
				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftControlFont );
				result = (f.getHeight() * 1.75) + 2.50;
			}
			break;

			case UIMetricsManager::htRadioBoxHeight : case UIMetricsManager::htCheckBoxHeight : {
				//in Win32 a radio box or check box is ALWAYS 10 dialog units high
				//dialog units are converted by
				//(2 * average char height dialog font / average char height system font pixels
				//where average char height dialog font = TEXTMETRIC.tmHeight field or a Font::getHeight()


				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftControlFont );
				result = (9.0 * ((2.0 * f.getHeight()) / f.getHeight())) - 4.0;//0.590909;
			}
			break;

			case UIMetricsManager::htToolTipHeight : {
				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftToolTipFont );
				result = f.getHeight() * 1.2222;
			}
			break;

			case UIMetricsManager::htSeparatorHeight : {
				result = 2.0;
			}
			break;

			case UIMetricsManager::htInformationalControl : {
				VCF::Font f = getDefaultFontFor( UIMetricsManager::ftControlFont );
				result = f.getHeight() * 1.75;
			}
			break;
		}
		return result;

	}

	virtual double getPreferredSpacingFor( const UIMetricsManager::SpacingType& type )  {
		double result = 0.0;

		//values largely derived from the Apple HIG at
		//http://developer.apple.com/techpubs/macosx/Essentials/AquaHIGuidelines/AHIGLayout/index.html
		switch ( type ) {
			case UIMetricsManager::stWindowBorderDelta : {
				result = 20.0;
			}
			break;

			case UIMetricsManager::stContainerBorderDelta : {
				result = 8.0;
			}
			break;

			case UIMetricsManager::stControlVerticalSpacing : {
				result = 14.0;
			}
			break;

			case UIMetricsManager::stControlHorizontalSpacing : {
				result = 10.0;
			}
			break;

			case UIMetricsManager::stInformationControlTopSpacer : {
				result = 2.0;
			}
			break;

			case UIMetricsManager::stInformationControlBottomSpacer : {
				result = 8.0;
			}
			break;
		}

		return result;
	}

	virtual Size getDefaultSliderThumbDimensions()  {
		Size result;

		//where the hell do we get these ????
		result.width_ = 22;
		result.height_ = 22;

		return result;
	}

	virtual Size getDefaultMenuItemDimensions( const String& caption )  {
		Size result;


		return result;
	}

	virtual Size getDefaultVerticalScrollButtonDimensions()  {
		Size result;

		result.width_ = 22;
		result.height_ = 22;

		return result;
	}

	virtual Size getDefaultHorizontalScrollButtonDimensions()  {
		Size result;

		result.width_ = 22;
		result.height_ = 22;

		return result;
	}

	virtual Size getDefaultTabDimensions( const String& caption )  {
		Size result;

		return result;
	}
};

};//end of VCF namespace


using namespace VCF;



OSXUIToolkit::OSXUIToolkit():
    quitEventLoop_(false),
    eventHandlerRef_(NULL),
    handlerUPP_(NULL),
    timerUPP_(NULL),
    idleTimerUPP_(NULL),
    idleTimerRef_(NULL)
{
    metricsMgr_ = new OSXUIMetricsManager();

    //install event loop callbacks
    handlerUPP_ = NewEventHandlerUPP(OSXUIToolkit::handleOSXApplicationEvents);
    static EventTypeSpec eventsToHandle[] ={
                            { kEventClassCommand, kEventCommandProcess },
                            { kEventClassCommand, kEventCommandUpdateStatus },
                            { OSXUIToolkit::CustomEventClass, OSXUIToolkit::EventPosted} };

    OSStatus err = InstallEventHandler( GetApplicationEventTarget(),
                                        handlerUPP_,
                                        sizeof(eventsToHandle)/sizeof(eventsToHandle[0]),
                                        eventsToHandle,
                                        this,
                                        &eventHandlerRef_ );

    timerUPP_ = NewEventLoopTimerUPP(OSXUIToolkit::handleTimerEvent);
    idleTimerUPP_ = NewEventLoopIdleTimerUPP(OSXUIToolkit::handleIdleTimer);

    InstallEventLoopIdleTimer( GetCurrentEventLoop(), 0, 0.25,
                                idleTimerUPP_, this, &idleTimerRef_ );


    if ( err != noErr ) {
        printf( "InstallEventHandler failed" );
    }
}

OSXUIToolkit::~OSXUIToolkit()
{

    std::map<EventLoopTimerRef,TimeOutHandler>::iterator found = timeoutHandlers_.begin();
    while ( !timeoutHandlers_.empty() ) {
        internal_unregisterTimerHandler( found->second.handler_ );
        found = timeoutHandlers_.begin();
    }


    DisposeEventHandlerUPP(handlerUPP_);
    DisposeEventLoopTimerUPP(timerUPP_);

    RemoveEventLoopTimer( idleTimerRef_ );
    DisposeEventLoopIdleTimerUPP(idleTimerUPP_);
    printf( "OSXUIToolkit destroyed\n");
}

ApplicationPeer* OSXUIToolkit::internal_createApplicationPeer()
{
    return new OSXApplicationPeer();
}

TextPeer* OSXUIToolkit::internal_createTextPeer( TextControl* component, const bool& isMultiLineControl, ComponentType componentType)
{
    return NULL;
}

TreePeer* OSXUIToolkit::internal_createTreePeer( TreeControl* component, ComponentType componentType)
{
    return NULL;
}

ListviewPeer* OSXUIToolkit::internal_createListViewPeer( ListViewControl* component, ComponentType componentType)
{
    return NULL;
}

DialogPeer* OSXUIToolkit::internal_createDialogPeer( Control* owner, Dialog* component, ComponentType componentType )
{
    return NULL;
}

DialogPeer* OSXUIToolkit::internal_createDialogPeer()
{
    return NULL;
}

ControlPeer* OSXUIToolkit::internal_createControlPeer( Control* control, ComponentType componentType)
{
    return NULL;
}

WindowPeer* OSXUIToolkit::internal_createWindowPeer( Control* control, Control* owner, ComponentType componentType)
{
    return new OSXWindow( control, owner );
}

ToolbarPeer* OSXUIToolkit::internal_createToolbarPeer( Toolbar* toolbar )
{
    return NULL;
}

MenuItemPeer* OSXUIToolkit::internal_createMenuItemPeer( MenuItem* item )
{
    return NULL;
}

MenuBarPeer* OSXUIToolkit::internal_createMenuBarPeer( MenuBar* menuBar )
{
    return NULL;
}

PopupMenuPeer* OSXUIToolkit::internal_createPopupMenuPeer( PopupMenu* popupMenu )
{
    return NULL;
}

ButtonPeer* OSXUIToolkit::internal_createButtonPeer( CommandButton* component, ComponentType componentType)
{
    return NULL;
}

HTMLBrowserPeer* OSXUIToolkit::internal_createHTMLBrowserPeer( Control* control )
{
    return NULL;
}

ContextPeer* OSXUIToolkit::internal_createContextPeer( Control* control )
{
    return new OSXControlContextPeer( control->getPeer() );
}

CommonFileDialogPeer* OSXUIToolkit::internal_createCommonFileOpenDialogPeer( Control* owner )
{
    return NULL;
}

CommonFileDialogPeer* OSXUIToolkit::internal_createCommonFileSaveDialogPeer( Control* owner )
{
    return NULL;
}

CommonColorDialogPeer* OSXUIToolkit::internal_createCommonColorDialogPeer( Control* owner )
{
    return NULL;
}

CommonFolderBrowseDialogPeer* OSXUIToolkit::internal_createCommonFolderBrowseDialogPeer( Control* owner )
{
    return NULL;
}

CommonFontDialogPeer* OSXUIToolkit::internal_createCommonFontDialogPeer( Control* owner )
{
    return NULL;
}

DragDropPeer* OSXUIToolkit::internal_createDragDropPeer()
{
    return NULL;
}

DataObjectPeer* OSXUIToolkit::internal_createDataObjectPeer()
{
    return NULL;
}

DropTargetPeer* OSXUIToolkit::internal_createDropTargetPeer()
{
    return NULL;
}

DesktopPeer* OSXUIToolkit::internal_createDesktopPeer( Desktop* desktop )
{
    return new OSXDesktopPeer(desktop);
}

ScrollPeer* OSXUIToolkit::internal_createScrollPeer( Control* control )
{
    return NULL;
}

CursorPeer* OSXUIToolkit::internal_createCursorPeer( Cursor* cursor )
{
    return new OSXCursorPeer(cursor);
}

ClipboardPeer* OSXUIToolkit::internal_createClipboardPeer()
{
    return NULL;
}

bool OSXUIToolkit::internal_createCaret( Control* owningControl, Image* caretImage  )
{
    return false;
}

bool OSXUIToolkit::internal_destroyCaret( Control* owningControl )
{
    return false;
}

void OSXUIToolkit::internal_setCaretVisible( const bool& caretVisible )
{

}

void OSXUIToolkit::internal_setCaretPos( Point* point )
{

}

void OSXUIToolkit::internal_postEvent( VCF::EventHandler* eventHandler, Event* event, const bool& deleteHandler )
{
    EventRef osxEvent = OSXUIToolkit::createUserCarbonEvent(OSXUIToolkit::EventPosted);


    SetEventParameter( osxEvent, OSXUIToolkit::EventHandler,
                        typeUInt32, OSXUIToolkit::SizeOfEventHandler, &eventHandler );

    SetEventParameter( osxEvent, OSXUIToolkit::EventHandlerEvent,
                        typeUInt32, OSXUIToolkit::SizeOfEventHandlerEvent, &event );

    Boolean val = deleteHandler;
    SetEventParameter( osxEvent, OSXUIToolkit::DeletePostedEvent,
                        typeBoolean, OSXUIToolkit::SizeOfDeletePostedEvent, &val );

    OSStatus err = PostEventToQueue( GetCurrentEventQueue(), osxEvent, kEventPriorityStandard );
    if ( err != noErr ) {
        printf( "PostEventToQueue failed\n" );
    }

}

void OSXUIToolkit::handleIdleTimer( EventLoopTimerRef inTimer, EventLoopIdleTimerMessage inState, void *inUserData )
{
    OSXUIToolkit* toolkit = (OSXUIToolkit*)inUserData;

    printf( "idle handler...\n" );

    switch ( inState ) {
        case kEventLoopIdleTimerStarted : {

        }
        break;

        case kEventLoopIdleTimerIdling : {
            printf( "\tkEventLoopIdleTimerIdling...\n" );
            Application* app = Application::getRunningInstance();
            if ( NULL != app ) {
				app->idleTime();
			}

			//check library apps;
			Enumerator<LibraryApplication*>* registeredLibs = LibraryApplication::getRegisteredLibraries();
			while ( true == registeredLibs->hasMoreElements() ) {
				LibraryApplication* libraryApp = registeredLibs->nextElement();
				libraryApp->idleTime();
			}
        }
        break;

        case kEventLoopIdleTimerStopped : {

        }
        break;
    }
}

void OSXUIToolkit::handleTimerEvent( EventLoopTimerRef inTimer, void * inUserData )
{
   OSXUIToolkit* toolkit = (OSXUIToolkit*)inUserData;
   std::map<EventLoopTimerRef,TimeOutHandler>::iterator found =
        toolkit->timeoutHandlers_.find( inTimer );
    if ( found != toolkit->timeoutHandlers_.end() ) {
        TimeOutHandler& toh = found->second;
        TimerEvent event( toh.source_, TIMER_EVENT_PULSE );
		toh.handler_->invoke( &event );
    }
}


void OSXUIToolkit::internal_registerTimerHandler( Object* source, VCF::EventHandler* handler, const ulong32& timeoutInMilliSeconds )
{
    TimeOutHandler toh;
    toh.source_ = source;
    toh.handler_ = handler;


    std::map<EventLoopTimerRef,TimeOutHandler>::iterator found = timeoutHandlers_.begin();
    while ( found != timeoutHandlers_.end() ) {
        TimeOutHandler& tmHandler = found->second;
        if ( tmHandler.handler_ == handler ) {
            RemoveEventLoopTimer( tmHandler.timerRef_ );
            timeoutHandlers_.erase( found );
            break;
        }
        found ++;
    }

    double timeout = ((double)timeoutInMilliSeconds/1000.0);

    /*
    CFRunLoopTimerContext ctx;
    memset( &ctx, 0, sizeof(ctx));
    ctx.info = this;

    toh.timerRef_ = CFRunLoopTimerCreate( kCFAllocatorDefault,
                            CFAbsoluteTimeGetCurrent(),
                            timeout,
                            0,
                            0,
                            OSXUIToolkit::handleTimerEvent,
                            &ctx );
    */
    InstallEventLoopTimer( GetCurrentEventLoop(),
                            timeout,
                            timeout,
                            timerUPP_,
                            this,
                            &toh.timerRef_ );

    timeoutHandlers_[toh.timerRef_] = toh;

    //CFRunLoopAddTimer( CFRunLoopGetCurrent(), toh.timerRef_, kCFRunLoopCommonModes );
}

void OSXUIToolkit::internal_unregisterTimerHandler( VCF::EventHandler* handler )
{
    std::map<EventLoopTimerRef,TimeOutHandler>::iterator found = timeoutHandlers_.begin();
    while ( found != timeoutHandlers_.end() ) {
        TimeOutHandler& tmHandler = found->second;
        if ( tmHandler.handler_ == handler ) {
            RemoveEventLoopTimer( tmHandler.timerRef_ );
            timeoutHandlers_.erase( found );
            break;
        }
        found ++;
    }
}


void OSXUIToolkit::internal_runEventLoop()
{
    //set to false to begin with
    quitEventLoop_ = false;

    RunApplicationEventLoop();

    //reset back to false when finished
    quitEventLoop_ = false;
}

UIToolkit::ModalReturnType OSXUIToolkit::internal_runModalEventLoopFor( Control* control )
{
    return UIToolkit::mrFalse;
}

void OSXUIToolkit::internal_quitCurrentEventLoop()
{
    EventLoopRef currentLoop = GetCurrentEventLoop();
    QuitEventLoop( currentLoop );
    quitEventLoop_ = true;
}

OSStatus OSXUIToolkit::handleOSXApplicationEvents( EventHandlerCallRef nextHandler, EventRef osxEvent, void* userData )
{
    OSXUIToolkit * toolkit = (OSXUIToolkit*)userData;
    return toolkit->handleAppEvents( nextHandler, osxEvent );
}

OSStatus OSXUIToolkit::handleAppEvents( EventHandlerCallRef nextHandler, EventRef osxEvent )
{
    OSStatus result = eventNotHandledErr;
    switch ( GetEventClass( osxEvent ) ) {
        case OSXUIToolkit::CustomEventClass : {
            switch ( GetEventKind( osxEvent ) ) {
                case OSXUIToolkit::EventPosted : {

                    UInt32 val;
                    Boolean deleteHandler;
                    GetEventParameter( osxEvent,
                                        OSXUIToolkit::EventHandler,
                                        typeUInt32,
                                        NULL,
                                        OSXUIToolkit::SizeOfEventHandler,
                                        NULL,
                                        &val );

                    VCF::EventHandler* eventHandler  = (VCF::EventHandler*)val;

                    GetEventParameter( osxEvent,
                                        OSXUIToolkit::EventHandlerEvent,
                                        typeUInt32,NULL,
                                        OSXUIToolkit::SizeOfEventHandlerEvent,NULL,
                                        &val );
                    Event* e = (Event*)val;

                    GetEventParameter( osxEvent,
                                        OSXUIToolkit::DeletePostedEvent,
                                        typeUInt32,NULL,
                                        OSXUIToolkit::SizeOfDeletePostedEvent,NULL,
                                        &deleteHandler );
                    if ( (NULL != eventHandler) && (NULL != e ) ) {
                        printf( "event handler (%p) and event (%p) recv'd! Calling event handler's invoke()\n",
                                    eventHandler, e );
                        eventHandler->invoke( e );

                        if ( deleteHandler ) {
                            delete eventHandler;
                        }
                        delete e;
                    }

                    result = noErr;
                }
                break;
            }
        }
        break;

        default : {
            return ::CallNextEventHandler( nextHandler, osxEvent );
        }
        break;
    }

    return result;
}


VCF::Event* OSXUIToolkit::internal_createEventFromNativeOSEventData( void* eventData )
{
    VCF::Event* result = NULL;
    OSXEventMsg* msg = (OSXEventMsg*)eventData;
    UInt32 whatHappened = GetEventKind( msg->osxEvent_ );

    UInt32 type = GetEventClass( msg->osxEvent_ );

    switch ( type ) {
        case kEventClassMouse : {
            switch ( whatHappened ) {
                case kEventMouseDown : {

                }
                break;

                case kEventMouseUp : {

                }
                break;

                case kEventMouseMoved : {

                }
                break;

                case kEventMouseDragged : {

                }
                break;

                case kEventMouseEntered : {

                }
                break;

                case kEventMouseExited : {

                }
                break;

                case kEventMouseWheelMoved : {

                }
                break;
            }
        }
        break;

        case kEventClassKeyboard : {
            switch ( whatHappened ) {
                case kEventRawKeyDown : {

                }
                break;

                case kEventRawKeyRepeat : {

                }
                break;

                case kEventRawKeyUp : {

                }
                break;

                case kEventRawKeyModifiersChanged : {

                }
                break;

                case kEventHotKeyPressed : {

                }
                break;

                case kEventHotKeyReleased : {

                }
                break;
            }
        }
        break;

        case kEventClassTextInput : {
            switch ( whatHappened ) {

            }
        }
        break;

        case kEventClassApplication : {
            switch ( whatHappened ) {
                case kEventAppActivated : {

                }
                break;

                case kEventAppDeactivated : {

                }
                break;

                case kEventAppQuit : {

                }
                break;

                case kEventAppLaunchNotification : {

                }
                break;

                case kEventAppLaunched : {

                }
                break;

                case kEventAppTerminated : {

                }
                break;

                case kEventAppFrontSwitched : {

                }
                break;

                case kEventAppFocusMenuBar : {

                }
                break;

                case kEventAppFocusNextDocumentWindow : {

                }
                break;

                case kEventAppFocusNextFloatingWindow : {

                }
                break;

                case kEventAppFocusToolbar : {

                }
                break;

                case kEventAppGetDockTileMenu : {

                }
                break;

                case kEventAppHidden : {

                }
                break;

                case kEventAppShown : {

                }
                break;

                case kEventAppSystemUIModeChanged : {

                }
                break;
            }
        }
        break;

        case kEventClassAppleEvent : {

        }
        break;

        case kEventClassMenu : {
            switch ( whatHappened ) {
                case kEventMenuBeginTracking : {

                }
                break;

                case kEventMenuEndTracking : {

                }
                break;

                case kEventMenuChangeTrackingMode : {

                }
                break;

                case kEventMenuOpening : {

                }
                break;

                case kEventMenuClosed : {

                }
                break;

                case kEventMenuTargetItem : {

                }
                break;

                case kEventMenuMatchKey : {

                }
                break;

                case kEventMenuEnableItems : {

                }
                break;

                case kEventMenuPopulate : {

                }
                break;

                case kEventMenuMeasureItemWidth : {

                }
                break;

                case kEventMenuMeasureItemHeight : {

                }
                break;

                case kEventMenuDrawItem : {

                }
                break;

                case kEventMenuDrawItemContent : {

                }
                break;

                case kEventMenuDispose : {

                }
                break;
            }
        }
        break;

        case kEventClassWindow : {
            switch ( whatHappened ) {
                case kEventWindowCollapse : {

                }
                break;

                case kEventWindowCollapseAll : {

                }
                break;

                case kEventWindowExpand : {

                }
                break;

                case kEventWindowExpandAll : {

                }
                break;

                case kEventWindowClose : {

                }
                break;

                case kEventWindowCloseAll : {

                }
                break;

                case kEventWindowZoom : {

                }
                break;

                case kEventWindowZoomAll : {

                }
                break;

                case kEventWindowContextualMenuSelect : {

                }
                break;

                case kEventWindowPathSelect : {

                }
                break;

                case kEventWindowGetIdealSize : {

                }
                break;

                case kEventWindowGetMinimumSize : {

                }
                break;

                case kEventWindowGetMaximumSize : {

                }
                break;

                case kEventWindowConstrain : {

                }
                break;

                case kEventWindowHandleContentClick : {

                }
                break;

                case kEventWindowGetDockTileMenu : {

                }
                break;

                case kEventWindowProxyBeginDrag : {

                }
                break;

                case kEventWindowProxyEndDrag : {

                }
                break;

                case kEventWindowToolbarSwitchMode : {

                }
                break;
            }
        }
        break;

        case kEventClassControl : {
            switch ( whatHappened ) {

                case kEventControlInitialize : {

                }
                break;

                case kEventControlDispose : {

                }
                break;

                case kEventControlGetOptimalBounds : {

                }
                break;

                case kEventControlHit : {

                }
                break;

                case kEventControlSimulateHit : {

                }
                break;

                case kEventControlHitTest : {

                }
                break;

                case kEventControlDraw : {

                }
                break;

                case kEventControlApplyBackground : {

                }
                break;

                case kEventControlApplyTextColor : {

                }
                break;

                case kEventControlSetFocusPart : {

                }
                break;

                case kEventControlGetFocusPart : {

                }
                break;

                case kEventControlActivate : {

                }
                break;

                case kEventControlDeactivate : {

                }
                break;

                case kEventControlSetCursor : {

                }
                break;

                case kEventControlContextualMenuClick : {

                }
                break;

                case kEventControlClick : {

                }
                break;

                case kEventControlGetNextFocusCandidate : {

                }
                break;

                case kEventControlGetAutoToggleValue : {

                }
                break;

                case kEventControlInterceptSubviewClick : {

                }
                break;

                case kEventControlGetClickActivation : {

                }
                break;

                case kEventControlDragEnter : {

                }
                break;

                case kEventControlDragWithin : {

                }
                break;

                case kEventControlDragLeave : {

                }
                break;

                case kEventControlDragReceive : {

                }
                break;

                case kEventControlTrack : {

                }
                break;

                case kEventControlGetScrollToHereStartPoint : {

                }
                break;

                case kEventControlGetIndicatorDragConstraint : {

                }
                break;

                case kEventControlIndicatorMoved : {

                }
                break;

                case kEventControlGhostingFinished : {

                }
                break;

                case kEventControlGetActionProcPart : {

                }
                break;

                case kEventControlGetPartRegion : {

                }
                break;

                case kEventControlGetPartBounds : {

                }
                break;

                case kEventControlSetData : {

                }
                break;

                case kEventControlGetData : {

                }
                break;

                case kEventControlGetSizeConstraints : {

                }
                break;

                case kEventControlValueFieldChanged : {

                }
                break;

                case kEventControlAddedSubControl : {

                }
                break;

                case kEventControlRemovingSubControl : {

                }
                break;

                case kEventControlBoundsChanged : {

                }
                break;

                case kEventControlTitleChanged : {

                }
                break;

                case kEventControlOwningWindowChanged : {

                }
                break;

                case kEventControlHiliteChanged : {

                }
                break;

                case kEventControlEnabledStateChanged : {

                }
                break;

                case kEventControlArbitraryMessage : {

                }
                break;
            }
        }
        break;

        case kEventClassCommand : {
            switch ( whatHappened ) {
                case kEventProcessCommand : {

                }
                break;

                case kEventCommandUpdateStatus : {

                }
                break;
            }
        }
        break;

        case kEventClassTablet : {

        }
        break;

        case kEventClassVolume : {

        }
        break;

        case kEventClassAppearance : {
            switch ( whatHappened ) {
                case kEventParamNewScrollBarVariant : {

                }
                break;
            }
        }
        break;

        case kEventClassService : {
            switch ( whatHappened ) {
                case kEventServiceCopy : {

                }
                break;

                case kEventServicePaste : {

                }
                break;

                case kEventServiceGetTypes : {

                }
                break;

                case kEventServicePerform : {

                }
                break;
            }
        }
        break;

        case kEventClassToolbar : {
            switch ( whatHappened ) {
                case kEventParamToolbar : {

                }
                break;

                case kEventParamToolbarItem : {

                }
                break;

                case kEventParamToolbarItemIdentifier : {

                }
                break;

                case kEventParamToolbarItemConfigData : {

                }
                break;
            }
        }
        break;

        case kEventClassToolbarItem : {

        }
        break;

        case kEventClassAccessibility : {

        }
        break;
    }
    return result;
}

EventRef OSXUIToolkit::createUserCarbonEvent( UInt32 eventType )
{
    EventRef result = NULL;

    OSStatus err = CreateEvent( NULL,
                                OSXUIToolkit::CustomEventClass,
                                eventType,
                                GetCurrentEventTime(),
                                kEventAttributeUserEvent,
                                &result );

    if ( err != noErr ) {
        printf( "OSXUIToolkit::createUserCarbonEvent CreateEvent failed!\n" );
    }
    else {

    }

    return result;
}

VCF::Size OSXUIToolkit::internal_getDragDropDelta()
{
    Size result;
    return result;
}


/**
*CVS Log info
*$Log$
*Revision 1.1.2.2  2004/04/29 03:43:14  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*/


