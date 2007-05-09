#ifndef _VCF_CONTROL_H__
#define _VCF_CONTROL_H__
//Control.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


//control properties
//that may use component contants
//when be written to a Visual Form file.
#define CONTROL_HEIGHT			"height"
#define CONTROL_WIDTH			"width"
#define CONTROL_FONTNAME		"font.name"
#define CONTROL_FONTSIZE		"font.pointsize"


//control constant's
#define CC_FONTNAME				"ControlFont"
#define CC_FONTSIZE				"ControlFontsize"

#define CC_BUTTONHEIGHT			"ButtonHeight"
#define CC_LABELHEIGHT			"LabelHeight"
#define CC_CHECKBOXHEIGHT		"CheckboxHeight"
#define CC_COMBOBOXHEIGHT		"ComboboxHeight"
#define CC_HEADERHEIGHT			"HeaderHeight"
#define CC_TEXTHEIGHT			"TextHeight"
#define CC_VPROGRESSWIDTH		"VProgressWidth"
#define CC_HPROGRESSHEIGHT		"HProgressHeight"
#define CC_RADIOBOXHEIGHT		"RadioboxHeight"
#define CC_SEPARATORHEIGHT		"SeparatorHeight"
#define CC_VSLIDERWIDTH			"VSliderWidth"
#define CC_HSLIDERHEIGHT		"HSliderHeight"


namespace VCF {


class Rect;
class Color;
class Border;
class Font;
class ControlPeer;
class GraphicsContext;
class ControlGraphicsContext;
class PopupMenu;
class Scrollable;
class Cursor;
class Model;
class AcceleratorKey;
class Container;
class Frame;
class ControlEvent;

typedef uint32 AnchorTypes;

#define CONTROL_CLASSID		"B91B1828-3639-4bcf-9882-342F16C90E21"


/**
The various enumerations of standard alignment types used by 
the default containers for control alignment. Note that
other custom control's may have their own types.
*/
enum AlignmentType{
	/**
	The default value for a control's alignment. The control's top, left, 
	width, and height are what determine it's position, regardless of 
	surrounding controls.
	*/
    AlignNone=0,
	
	/**
	The control moves to the top of the parent container and resizes to 
	fill in the width of the parent control. The height of the control 
	is not altered during parent dimension changes.
	*/
    AlignTop,
	
	/**
	The control moves to the bottom of the parent container and resizes 
	to fill in the width of the parent control. The height of the control 
	is not altered during parent dimension changes.
	*/
    AlignLeft,
	
	/**
	The control moves to the left side of the parent container and resizes 
	to fill in the height of the parent control. The width of the control 
	is not altered during parent dimension changes.
	*/
    AlignRight,
	
	/**
	The control moves to the right side of the parent container and resizes 
	to fill in the height of the parent control. The width of the control is 
	not altered during parent dimension changes.
	*/
    AlignBottom,
	
	/**
	The control resizes to fill in the remaining client area of a form (after 
	all other alignment positions of other controls are calculated).
	*/
    AlignClient
};









/**
An enum of anchor types for a control's anchor value.
These may be masked together.
*/
enum AnchorType {
	/**
	This is the default value for a control's anchor property. 
	No layout adjustments are performed on the control.
	*/
	AnchorNone = 0,
	
	/**
	The Control is anchored to the top edge of the parent control 
	it belongs to. Whatever the distance between the top edge and 
	the top coordinate of the control when this is set, is maintained 
	whenever the parent control's dimensions change. 
	*/
	AnchorTop = 1,
	
	/**
	The Control is anchored to the left edge of the parent 
	control it belongs to. Whatever the distance between the 
	left edge and the left coordinate of the control when this 
	is set, is maintained whenever the parent control's dimensions 
	change.
	*/
	AnchorLeft = 2,
	
	/**
	The Control is anchored to the bottom edge of the parent 
	control it belongs to. Whatever the distance between the 
	bottom edge and the bottom coordinate of the control when 
	this is set, is maintained whenever the parent control's 
	dimensions change. 
	*/
	AnchorBottom = 4,
	
	/**
	The Control is anchored to the right edge of the parent 
	control it belongs to. Whatever the distance between the 
	right edge and the right coordinate of the control when 
	this is set, is maintained whenever the parent control's 
	dimensions change.
	*/
	AnchorRight = 8
};


									 






enum TextAlignmentType {
	taTextLeft = 0,
	taTextCenter,
	taTextRight
};











/**
\class Control Control.h "vcf/ApplicationKit/Control.h"
The base class for all visual components in the Visual Component Framework.
In addition it implements the View interface. This means that the
Control is itself a specialized form of a View, though it may not have
have a model, and can hold an external view, separate from itself.

A control receives a wide variety of events from the user such as paint 
events, mouse events, keyboard events, help events, etc.

Controls can have parents, and can form a hierarchy of parent-child 
controls. Controls can have children if they have a valid Container 
instance set.

Controls can also be aligned or anchored, which can aid in laying out the
presentation of a UI.

Finally a Control may also be lightweight or heavyweight, which determines
how many  window system resources the control consumes. A heavyweight control
has a native window/widget and takes up resources accordingly. In addition a
heavyweight control also has a native graphics context associated with it that
also takes up resources. In contrast, a lightweight control shares the underlying
native window and graphics context resources with it's parent, and greatly
reduces the number of resources the application will consume. This is especially
useful when making complex custom controls that have many moving parts like 
a tree-list control, complete with a header and movable icons, etc.

To aid in the drawing of a control, all controls are double buffered by default
to prevent flicker, though this can be turned off and on at will.
 @delegates
	@del Control::ControlSized
	@del Control::ControlPositioned
	@del Control::ControlParentChanged
	@del Control::MouseDoubleClicked
	@del Control::MouseClicked
	@del Control::MouseMove
	@del Control::MouseUp
	@del Control::MouseDown
	@del Control::MouseEnter
	@del Control::MouseLeave
	@del Control::KeyPressed
	@del Control::KeyDown
	@del Control::KeyUp
	@del Control::ControlHelpRequested
	@del Control::HelpRequested
	@del Control::FocusLost
	@del Control::FocusGained
	@del Control::ToolTipRequested
	@del Control::ToolTip
	@del Control::ControlModelChanged
	@del Control::BeforeControlPainted
	@del Control::AfterControlPainted
 */
class APPLICATIONKIT_API Control : public UIComponent, public AbstractView {
public:
	//Anchor delta enums
	enum {
		ANCHOR_DTOP = 0,
		ANCHOR_DLEFT,
		ANCHOR_DBOTTOM,
		ANCHOR_DRIGHT
	};

	enum ControlState {
		csVisible					= 0x00000001,
		csEnabled					= 0x00000002,
		csUseParentFont				= 0x00000004,
		csDoubleBuffered			= 0x00000008,
		csAutoStartDragDrop			= 0x00000010,
		csTabStop					= 0x00000020,
		csIgnoreForLayout			= 0x00000040,
		csIgnoreForParentScrolling  = 0x00000080,
		csAllowPaintNotification	= 0x00000100,
		csHasMouseCapture			= 0x00000200,
		csUseRenderBuffer			= 0x00000400,
		csAllowMouseFocus			= 0x00000800,
		csAllowFocus				= 0x00001000,
		csDefaultControlState = csEnabled | csTabStop | csDoubleBuffered | csAllowMouseFocus | csAllowFocus
	};

	enum MinMaxSizeDefaults{
		mmIgnoreMinWidth = -1,
		mmIgnoreMinHeight = -1,
		mmIgnoreMaxWidth = -1,
		mmIgnoreMaxHeight = -1
	};

	enum ControlEvents {
		CONTROL_EVENTS = COMPONENT_EVENTS_LAST + 200,
		CONTROL_SIZED,
		CONTROL_POSITIONED,
		CONTROL_PARENT_CHANGED,
		CONTROL_MODELCHANGED,
		FOCUS_GAINED,
		FOCUS_LOST,
		HELP_REQUESTED,
		WHATS_THIS_HELP_REQUESTED,
		KEYBOARD_DOWN,
		KEYBOARD_PRESSED,
		KEYBOARD_UP,
		KEYBOARD_ACCELERATOR,
		MOUSE_DOWN,
		MOUSE_MOVE,
		MOUSE_UP,
		MOUSE_CLICK,
		MOUSE_DBLCLICK,
		MOUSE_ENTERED,
		MOUSE_LEAVE,
		BEFORE_CONTROL_PAINTED,
		AFTER_CONTROL_PAINTED,
		BEFORE_POPUP_MENU,
		CONTROL_EVENTS_LAST
	};


	Control();
	virtual ~Control();

	/**
	@delegate ControlSized fires an ControlEvent, with a type set to Control::CONTROL_SIZED.
	Fired whenever the control's width or height changes. Calls to setBounds(), setHeight(), or
	setWidth() can trigger this.
	@event ControlEvent
	@eventtype Control::CONTROL_SIZED
	*/
	DELEGATE(ControlSized);

	/**
	@delegate ControlPositioned fires an ControlEvent, with a type set to Control::CONTROL_POSITIONED.
	Fired whenever the control's position (top or left coordinates) changes. Calls to setBounds(), setLeft(), or
	setTop() can trigger this.
	@event ControlEvent
	@eventtype Control::CONTROL_POSITIONED
	*/
	DELEGATE(ControlPositioned);

	/**
	@delegate ControlParentChanged fires an ControlEvent, with a type set to Control::CONTROL_PARENT_CHANGED.
	Fired whenever the control's parent changes. Calls to setParent() trigger this.
	@event ControlEvent
	@eventtype Control::CONTROL_PARENT_CHANGED
	*/
	DELEGATE(ControlParentChanged);

	/**
	@delegate MouseDoubleClicked fires an MouseEvent, with a type set to Control::MOUSE_DBLCLICK.
	Fired whenever the control receives a double click from the mouse. This is triggered by the
	underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_DBLCLICK
	*/
	DELEGATE(MouseDoubleClicked);

	/**
	@delegate MouseClicked fires an MouseEvent, with a type set to Control::MOUSE_CLICK.
	Fired whenever the control receives a click from the mouse. This is triggered by the
	underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_CLICK
	*/
	DELEGATE(MouseClicked);

	/**
	@delegate MouseMove fires an MouseEvent, with a type set to Control::MOUSE_MOVE.
	Fired whenever the control receives a move event from the mouse. This is triggered by the
	underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_MOVE
	*/
	DELEGATE(MouseMove);

	/**
	@delegate MouseUp fires an MouseEvent, with a type set to Control::MOUSE_UP.
	Fired whenever the control receives notice that a mouse button has been released, or lifted up.
	This is triggered by the underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_UP
	*/
	DELEGATE(MouseUp);

	/**
	@delegate MouseDown fires an MouseEvent, with a type set to Control::MOUSE_DOWN.
	Fired whenever the control receives notice a mouse button has been pressed down.
	This is triggered by the underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_DOWN
	*/
	DELEGATE(MouseDown);

	/**
	@delegate MouseEnter fires an MouseEvent, with a type set to Control::MOUSE_ENTERED.
	Fired whenever the control receives notice the mouse has first entered the control's bounds.
	This is triggered by the underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_ENTERED
	*/
	DELEGATE(MouseEnter);

	/**
	@delegate MouseLeave fires an MouseEvent, with a type set to Control::MOUSE_LEAVE.
	Fired whenever the control receives notice the mouse has left the control's bounds.
	This is triggered by the underlying windowing system.
	@event MouseEvent
	@eventtype Control::MOUSE_LEAVE
	*/
	DELEGATE(MouseLeave);

	/**
	@delegate KeyPressed fires an KeyboardEvent, with a type set to Control::KEYBOARD_PRESSED.
	Fired whenever the control receives a keypress. This is triggered by the
	underlying windowing system.
	@event KeyboardEvent
	@eventtype Control::KEYBOARD_PRESSED
	*/
	DELEGATE(KeyPressed);

	/**
	@delegate KeyDown fires an KeyboardEvent, with a type set to Control::KEYBOARD_DOWN.
	Fired whenever the control receives notice a key has been pressed down. This is triggered by the
	underlying windowing system.
	@event KeyboardEvent
	@eventtype Control::KEYBOARD_DOWN
	*/
	DELEGATE(KeyDown);

	/**
	@delegate KeyUp fires an KeyboardEvent, with a type set to Control::KEYBOARD_UP.
	Fired whenever the control receives notice a key has been released. This is triggered by the
	underlying windowing system.
	@event KeyboardEvent
	@eventtype Control::KEYBOARD_UP
	*/
	DELEGATE(KeyUp);

	/**
	@delegate ControlHelpRequested fires an WhatsThisHelpEvent, with a type set to Control::WHATS_THIS_HELP_REQUESTED.
	Fired whenever the control receives notice of a context sensitive help event.
	This is triggered by the underlying windowing system. On Win32 this is frequently triggered
	by right clicking on a control, and clicking on the "Whats This?" context menu item.
	@event WhatsThisHelpEvent
	@eventtype Control::WHATS_THIS_HELP_REQUESTED
	*/
	DELEGATE(ControlHelpRequested);

	/**
	@delegate HelpRequested fires an HelpEvent, with a type set to Control::HELP_REQUESTED.
	Fired whenever the control receives notice the F1 key (or some other acknowledged help key)
	has been pressed. This is triggered by the underlying windowing system.
	@event HelpEvent
	@eventtype Control::HELP_REQUESTED
	*/
	DELEGATE(HelpRequested);

	/**
	@delegate FocusGained fires an FocusEvent, with a type set to Control::FOCUS_GAINED.
	Fired whenever the control receives notice that it has become the focused control. This is
	triggered by the underlying windowing system.
	@event FocusEvent
	@eventtype Control::FOCUS_GAINED
	*/
	DELEGATE(FocusGained);

	/**
	@delegate FocusLost fires an FocusEvent, with a type set to Control::FOCUS_LOST.
	Fired whenever the control receives notice that it has lost focus. This is
	triggered by the underlying windowing system.
	@event FocusEvent
	@eventtype Control::FOCUS_LOST
	*/
	DELEGATE(FocusLost);

	/**
	@delegate ToolTipRequested fires an ToolTipEvent, with a type set to TOOLTIP_EVENT_TIP_REQESTED.
	This is fired if the VCF framework determines a tooltip can be displayed for this
	control, and no tootip text was defined for the control.
	@event ToolTipEvent
	@eventtype TOOLTIP_EVENT_TIP_REQESTED
	@see ToolTipEvent.h for more on TOOLTIP_EVENT_TIP_REQESTED #define
	@see ToolTipEvent
	*/
	DELEGATE(ToolTipRequested);

	/**
	@delegate ToolTip fires an ControlEvent.
	Fired by the framework when it determins a tooltip can be displayed for a control.
	Typically this is caused by hovering the mouse over a control for some fixed amount
	of time, usually determine by the underlying windowing system's setting.
	@event ToolTipEvent
	*/
	DELEGATE(ToolTip);

	/**
	@delegate ControlModelChanged fires an ControlEvent.
	Fired by the control when the control's setViewModel is called. This event
	indicates that the control's model pointer has changed, and anyone interested in
	listening to the control's model should update accordingly.
	@event ControlEvent
	@eventtype Control::CONTROL_MODELCHANGED
	*/
	DELEGATE(ControlModelChanged);

	/**
	@delegate BeforeControlPainted fires an ControlEvent.
	Fired by the control just before the control's
	paint() method is called. This will only happen if the
	control's getAllowPaintNotification() returns true.	The
	ControlEvent's getPaintGraphicsContext() method will
	return the same GraphicsContext that passed to the
	control's paint() method.
	@event ControlEvent
	@eventtype Control::BEFORE_CONTROL_PAINTED
	*/
	DELEGATE(BeforeControlPainted);

	/**
	@delegate AfterControlPainted fires an ControlEvent.
	Fired by the control after the control's
	paint() method is called. This will only happen if the
	control's getAllowPaintNotification() returns true. The
	ControlEvent's getPaintGraphicsContext() method will
	return the same GraphicsContext that passed to the
	control's paint() method.
	@event ControlEvent
	@eventtype Control::AFTER_CONTROL_PAINTED
	*/
	DELEGATE(AfterControlPainted);

	/**
	@delegate AfterControlPainted fires an ControlPopupMenuMenuEvent
	before the popup menu is displayed, allowing for customization
	of the popup menu.
	@event ControlPopupMenuMenuEvent
	@eventtype Control::BEFORE_POPUP_MENU
	*/
	DELEGATE(BeforePopupMenu);
	
	

	/**
	This gets called by the ControlPeer for any windowing system mouse events,
	as well as for any windowing system keyboard events and for any
	windowing system events like size changes, position changes, etc.
	Once inside the event the Control determines the type, and behaves accordingly,
	as well as notifying any appropriate listeners.
	*/
	virtual void handleEvent( Event* event );

	/**
	calls handleEvent on this control, and then
	calls handleEventToChildren() on any child controls the
	container of this control may have.
	*/
	void handleEventAndForwardToChildren( Event* event );


	virtual Component* findComponent( const String& componentName, const bool& recursive=false );

	virtual void addNewComponent( Component* component );

	virtual void preLoading();

	virtual void postLoaded( const bool& visible );

	virtual bool bindVariable( Component** variablePtr, const String& variableName );

	virtual bool generatePropertyValue( const String& fullPropertyName, Property* property, VariantData* value, String& strValue );

	/**
	*Returns the control's current border object, if any
	*/
	Border* getBorder();

	/**
	\p
	Sets the border for this control. The control will check the
	border's owner value, and if it's NULL then it will set itself
	as the component owner of the border.
	*/
	void setBorder( Border* border );

	/**
	returns the bounds in parent coordinates of the Control. The Control derived class must call it's Peer's getBounds() method
	*/
	virtual Rect getBounds() ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	Returns the bounds in terms of the control's coordinates. Thus the top and
	left will typically be 0.0, 0.0, and the right and bottom of the bounds
	will typically equal the width and height of the control.
	*/
	virtual Rect getClientBounds( const bool& includeBorder = true ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	Returns the left position in parent coordinates
	*/
	virtual double getLeft() ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	Returns the right position in parent coordinates
	*/
	virtual double getRight() ;

	/**
	*returns the width of the control
	*/
	virtual double getWidth() ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*returns the top position in parent coordinates
	*/
	virtual double getTop() ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*returns the bottom position in parent coordinates
	*/
	virtual double getBottom() ;

	/**
	*returns the height of the control
	*/
	virtual double getHeight() ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*is this control visible to the user.
	*@return bool true if the control is visible, otherwise
	*false.
	*/
	virtual bool getVisible() ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	\p
	Returns the control's alignment. These alignment values are used by
	the basic container type, the StandardContainer class, which uses these
	values to determine how to layout the controls. Other layout implementations
	may choose to ignore these values. For example, the HorizontalLayoutContainer
	doesn't pay any attention to these value, it just lays out the controls
	as they are added.
	@see isIgnoredForLayout()
	@see setIgnoredForLayout()
	*/
	AlignmentType getAlignment();

	/**
	\p
	Returns whether this control must be ignored in the control container's
	layout implementation. Unlike the alignment values returned by getAlignment(),
	which are optional for the container to use, this \em must be honored.
	Failure to do so may result in dire intergalatic consequences, or
	worse, result in a shortage of SUVs.
	\p
	If this is false, the default value, then the control is laid out however the
	container is implemented to accomplish this. If the value is true, then the
	container should simply place the control wherever the controls bounds
	indicate. Container implementations that have some sort of layout that processes
	controls sequentially, such as the HorizontalLayoutContainer, \em must
	ignore a control that returns false.
	*/
	bool isIgnoredForLayout();

	void setIgnoredForLayout( const bool& val );

	/**
	sets the bounds of the control. You can pass in left, top, width and height as the
	new position for the control.
	@param double x the new left coordinate of the control
	@param double y the new top coordinate of the control
	@param double width the new width of the control
	@param double height the new height of the control
	*/
	void setBounds( const double& x, const double& y, const double& width, const double& height );

	/**
	*sets the bounds of the control. The values in the rect parameter must be
	*in the corrdinate system of the parent control
	*@param Rect the new bounds of the control. If the control
	*implements the Container interface, then any child controls
	*wil be resized via the Container::resizeChildren() automatically.
	*/
	virtual void setBounds( Rect* rect, const bool& anchorDeltasNeedUpdating=true ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*sets the alignment of the control
	*/
	void setAlignment( const AlignmentType& alignment );

	/**
	*sets the left value for the control, immediately updates the
	*controls position.
	*/
	virtual void setLeft( const double& left ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*sets the right value of the control. This value is in the
	*coordinates of the parent control.
	*/
	virtual void setRight( const double& right ) ;

	/**
	*sets the width of the control.
	*If the control implements the Container interface,
	*then any child controls will be resized via the
	*Container::resizeChildren() automatically.
	*/
	virtual void setWidth( const double& width ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*sets the top value of the control. This value is in the
	*coordinates of the parent control.
	*/
	virtual void setTop( const double& top ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*sets the bottom value of the control. This value is in the
	*coordinates of the parent control.
	*/
	virtual void setBottom( const double& bottom ) ;

	/**
	*sets the height of the control.
	*If the control implements the Container interface,
	*then any child controls will be resized via the
	*Container::resizeChildren() automatically.
	*/
	virtual void setHeight( const double& height ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*determines whether or not the control is visible,
	* and set it visible if it wasn't.
	*@param bool true to show the control, false to hide it
	*/
	virtual void setVisible( const bool& visible ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/


	/**
	*if autoStartDragDrop_ is true then this method checks to see if
	*the Controls dragging is with the windowing system's tolernace
	*for a drag drop operation to start. other wise it returns false.
	*If  autoStartDragDrop_ is false then it returns false unless some
	*derived class overides the functionality.
	*/
	virtual bool canBeginDragDrop( Point* point );

	/**
	*sets auto start dragging to true or false
	*/
	void setAutoStartDragDrop(const bool& canAutoStartDragDrop);

	/**
	*returns whether or not auto dragging is on or off.
	*@return bool true if auto dragging is on, otherwise false.
	*/
	bool getAutoStartDragDrop();

	/**
	*Override this to provide functionality for handling the
	*start of drag-drop operation
	*This will get called automatically by the framework if it
	*detects that a mouse-move has occured that indicates a
	*drag-drop operation COULD occur.
	*@return bool must return true if a drag-drop operation
	*was started, otherwise should return false
	*/
	virtual bool beginDragDrop( MouseEvent* event );

	/**
	*returns the control's native peer interface
	*/
	virtual ControlPeer* getPeer();

	/**
	*sets the parent for the control
	*/
	virtual void setParent( Control* parent ) ;/**throw( InvalidPeer ); -JEC - FIXME later*/

	/**
	*returns the control's current parent
	*/
	virtual Control* getParent() ;/**throw( InvalidPeer ); -JEC - FIXME later*/


	void removeFromParent( const bool& freeInstance=true );

	/**
	*does this control currently have focus?
	*@return bool true if the control has focus, otehr wise false. If a control
	*has focus, it will receive keyboard events. Currently all controls
	*get focused when they receive a mouse down event.
	*/
	bool isFocused();

	/**
	Makes this control the currently focused control. A control with focus
	will receive keyboard events.
	@return Control - the previously focused control (which will receive a
	FocusLost event).
	*/
	Control* setFocused();

	bool getAllowsFocus();

	void setAllowsFocus( const bool& val );

	bool getAllowsMouseFocus();

	void setAllowsMouseFocus( const bool& val );

	/**
	*returns whether the control is enabled or not. A control that is
	*enabled can receive mouse events. A control that is not enabled
	*will not respond to either mouse events or keyboard events.
	*/
	bool isEnabled();

	/**
	This checks not only to see if the control itself is
	enabled, but will return false if any parent control is
	not enabled. This function traverse all the parent child
	relationships till the frame window is hit, and checks the
	enabled status of each control. The first control that is
	\em not enabled stops the search and the methods returns
	false, otherwise it continues till it finds a NULL
	parent and returns true.
	*/
	bool areParentsEnabled();

	/**
	*sets whether the control is enabled or not.
	*@param bool true if the control is enabled, otherwise false.
	*/
	void setEnabled( const bool& enabled );

	/**
	
	Paints the control. Called by the underlying windowing system whenever
	the control needs to be painted. Note that in some cases the GraphicsContext
	passed in to this method may not be the same pointer as the GraphicsContext
	that the control holds itself. During the paint() method you should only
	use the context value for all your drawing and not the one returned in
	getContext(). The value returned by getContext() should be used for drawing
	that takes place outside of the paint() method.
	
	If the control allows paint notification, then the framework will fire an 
	event to the BeforeControlPainted delegate \em prior to calling the 
	control's paint() method. After the paint() method has returned, if
	the control allows paint notification, the framework will fire an event to
	the AfterControlPainted delegate. This allows outside observers to take 
	part in the paint cycle, \em but beware that this does come at a bit of a 
	cost, so use this feature sparingly.
	
	\em Note: This should \em NEVER be called
	by programmers using the VCF, it will be called for you during the course 
	of your applications native event loop, and is only here for providing 
	custom drawing routines for controls. In other words: you implement it, 
	you never call it yourself.
	@see getAllowPaintNotification
	*/
	virtual void paint( GraphicsContext * context )=0;

	/**
	Paints the border for the control. This is called by the framework during the
	painting of the control. Do not count on this being called at the same time as the
	paint() method, as the order in which this is called is dependant on the
	underlying windowing system. For example, on Win32 systems this call may
	be made by the framework outside of the WM_PAINT message handling, while on
	Mac OS X this will be called in the same event handler that dispatches
	the control's paint() method.
	*/
	void paintBorder( GraphicsContext * context );
	/**
	*Called when the mouse first enters the control's bounds.
	*override this to provide specific behaviours when the control is
	*first entered, like highlighting the control
	*/
	virtual void mouseEnter( MouseEvent* event );

	/**
	*Called when one (or more) of the mouse buttons are held down.
	*override this to provide specific behaviours when
	*the mouse buttons are held down
	*/
	virtual void mouseDown( MouseEvent* event );

	/**
	*Called when the mouse moves over the control.
	*override this to provide specific behaviours when
	*the mouse moves over the control.
	*/
	virtual void mouseMove( MouseEvent* event );

	/**
	*Called when one (or more) of the mouse buttons are released.
	*override this to provide specific behaviours when
	*the mouse buttons are released.
	*/
	virtual void mouseUp( MouseEvent* event );

	/**
	*Called when one or mouse buttons are held down and
	*immediately released ( a "mouse click" ).
	**override this to provide specific behaviours when
	*a mouse click occurs
	*/
	virtual void mouseClick(  MouseEvent* event );

	/**
	*Called when one or mouse buttons are held down and
	*immediately released ( a "mouse double click" ) twice, within
	*some specific time period, as determined on the windowing
	*system.
	**override this to provide specific behaviours when
	*a mouse double click occurs
	*/
	virtual void mouseDblClick(  MouseEvent* event );

	/**
	*Called when the mouse leaves the control's bounds.
	*override this to provide specific behaviours when the control is
	*left, like highlighting the control
	*/
	virtual void mouseLeave( MouseEvent* event );

	/**
	*called when the user presses the key down
	*/
	virtual void keyDown( KeyboardEvent* event );

	/**
	*called when the user simply presses the key and represents a
	*full key cycle having occurred, in other words the key has been
	*pressed down and then released
	*/
	virtual void keyPressed( KeyboardEvent* event );

	/**
	*called when the user releases the key
	*/
	virtual void keyUp( KeyboardEvent* event );

	/**
	called when the control's bounds change. Override this
	for your own custom behaviour if you need to.
	*/
	virtual void sizeChange( ControlEvent* event );

	/**
	called when the control's coordinates change. Override this
	for your own custom behaviour if you need to.
	*/
	virtual void positionChange( ControlEvent* event );

	/**
	Called when the parent of the control is changed. Override this
	for your own custom behaviour if you need to.
	*/
	virtual void parentChange( ControlEvent* event );

	/**
	Called when the control gains keyboard focus. Override this
	for your own custom behaviour if you need to.
	*/
	virtual void gotFocus( FocusEvent* event );

	/**
	Called when the control loses keyboard focus. Override this
	for your own custom behaviour if you need to.
	*/
	virtual void lostFocus( FocusEvent* event );

	/**
	*translate the point from this controls coordinate system to
	*the parent controls coordinate system
	*/
	void translateToParent( Point* point );

	/**
	*translate the point from parent controls coordinate system
	*to this controls coordinate system
	*/
	void translateToLocal( Point* point );

	/**
	Takes the coordinates in pt, which are in the coordinate system of
	this control, and translates them into the coordinate system of the
	Screen.
	@param Point the initial point value, in the coordinate system of the
	control this method is called on.
	*/
	void translateToScreenCoords( Point* pt );

	/**
	Takes the coordinates in rect, which are in the coordinate system of
	this control, and translates them into the coordinate system of the
	Screen.
	@param Rect the initial rect value, in the coordinate system of the
	control this method is called on.
	*/
	void translateToScreenCoords( Rect* rect );

	/**
	Takes the coordinates in pt, which are in the coordinate system of
	the Screen, and translates them into the coordinate system of this
	control.
	@param Point the initial point value, in the coordinate system of the
	Screen.
	*/
	void translateFromScreenCoords( Point* pt );

	/**
	Takes the coordinates in rect, which are in the coordinate system of
	the Screen, and translates them into the coordinate system of this
	control.
	@param Rect the initial rect value, in the coordinate system of the
	Screen.
	*/
	void translateFromScreenCoords( Rect* rect );


	/**
	*identifies the control as a lightweight control. Lightweight control's
	*do not take up windowing system resources, instead, they rely on all events
	*and paint notification be sent to them via their parents. The default return for
	*this is false, so only those controls specifically actign as light weight Controls
	*need to return true
	*/
	virtual bool isLightWeight();

	/**
	*Returns the first parent of the Control that is a heavweight Control, i.e it's isLighweight()
	*method return false.
	*/
	virtual Control* getHeavyweightParent();

	/**
	*returns the color used to fill the background of this control
	*@see CustomControl::setUseColorForBackground
	*/
	Color* getColor();

	/**
	*sets the color to fill this control's background with
	*/
	void setColor( Color* color );

	/**
	* called just prior to completely destroying the control
	* and it's associated memory.
	*@see Component::beforeDestroy()
	*/
	virtual void beforeDestroy( ComponentEvent* event );

	/**
	* Special initializations may then happen here.
	*@see Component::afterCreate()
	*/
	virtual void afterCreate( ComponentEvent* event );

	/**
	*Returns the font associated with this control
	*/
	Font* getFont();

	/**
	*sets the font attributes. Note the internal font is <b><i>not</i></b>
	*reassigned, but rather the data is copied over from the font instance
	*passed in to the method.
	*The caller completely owns the font instance that is passed in, and is
	*responsible for cleaning up it's memory.
	*@param Font the font to copy data from in replacing attributes of the
	*control's font.
	*/
	void setFont( Font* font );

	/**
	*returns whether this control will use the font settings of the
	*parent's font.
	*@return bool true if the control does use it's parent's font settings
	*otherwise false.
	*/
	bool useParentFont();

	/**
	*sets whether the control should use it's parent font's settigns or
	*use it's own independent of it's parent's font.
	*/
	void setUseParentFont( const bool& useParentFont );

	/**
	*repaint the control. This post a message to the windowing systems
	*message queue.
	*@param Rect a rectangle may be specified indicating the precise
	*region of the control that should be repainted. If this is
	*NULL, then the entire visible region of the control is repainted
	*/
	void repaint( Rect* repaintRect=NULL );

	void repaintNow( Rect* repaintRect=NULL );

	/**
	*is this component double buffered.
	*@return bool true if the component is double buffered, otherwise
	*false.
	*/
	bool isDoubleBuffered();

	/**
	*sets whether or not this control is double buffered. A control
	*that is double buffered will automatically use a memory graphics
	*context to draw on, and then blit the contents of this on the
	*actual graphics context for the control. Doing this results in
	*completely flicker free drawing of your controls, but is slighty
	*slower. Without double buffering, the painting of the control
	*takes place directly on the control's graphics context, and can
	*result in flicker, but is slighlty faster. Controls have this
	*set to true by default, with the exception of the OpenGL control,
	*which lets the underlying OpenGL graphics engine take care of
	*the double buffering for it.
	*@param bool true to enable the double buffered drawing, otherwise
	*false
	*/
	void setDoubleBuffered( const bool& doubleBuffered );

	/**
	Returns true if the Control should take advantage of the GraphicsContexts'
	render buffer for anti-aliased vector graphics (based on the AGG library).
	*/
	bool isUsingRenderBuffer() {
		return (controlState_ & Control::csUseRenderBuffer) ? true : false;
	};

	/**
	sets whether or not the control is using the render buffer of it's
	GraphicsContext. Using the render buffer allows the control to
	take advantage of the GraphicsContext's advanced anti-aliasing vector
	graphics. This will create snapshot image that is retained and used
	to draw into. When the drawing is done (i.e. paint() returns), the image
	contents are copied back to the actual GraphicsContext. If the
	control is resized the image is resized as well
	*/
	void setUsingRenderBuffer( const bool& useRenderBuffer );

	/**
	*this keeps the mouse events being sent to this control, even if the
	*mouse leaves the physical bounds of the control
	*/
	void keepMouseEvents();

	/**
	*releases mouse events - goes back to normal event handling
	*/
	void releaseMouseEvents();

	/**
	*return a pointer to the graphics context of the control.
	*/
	GraphicsContext* getContext();

	/**
	*returns the Control's PopupMenu, if any.
	*The popupmenu will be displayed whenever the user
	*right clicks the mouse over the control.
	*/
	PopupMenu* getPopupMenu();

	/**
	*sets the popup menu for this control.
	*/
	void setPopupMenu( PopupMenu* popupMenu );

	/**
	*returns the view associated with this control
	*/
	View* getView();

	/**
	*sets the view to associate with this control
	*/
	void setView( View* view );

	/**
	*returns the preferred width for this control. This is used
	*when determing the width of the control when it is first created.
	*Override it to provide a different value that is more acceptable
	*for your control's initial display size.
	*/
	virtual double getPreferredWidth(){
		return 100.0;
	};

	/**
	*returns the preferred height for this control. This is used
	*when determing the height of the control when it is first created.
	*Override it to provide a different value that is more acceptable
	*for your control's initial display size.
	*/
	virtual double getPreferredHeight() {
		return 25.0;
	}

	/**
	*set the preferred width. This is used when determing the height
	*of the control when it is first created.
	*/
	virtual void setPreferredWidth( const double& width ){};

	/**
	*set the preferred height. This is used when determing the height
	*of the control when it is first created.
	*/
	virtual void setPreferredHeight( const double& height ){};


	/**
	Returns the minimum size for this control. At the moment this is only
	used for Frames, to control the minimum size a user can resize the
	Frame to.
	@return Size the minimum size. If the width_ field is set equal to 
	Control::mmIgnoreMinWidth then the minimum width is ignored. If the 
	height_ field is set equal to Control::mmIgnoreMinHeight then the 
	minimum height is ignored. 
	*/
	Size getMinSize() {
		return minSize_;
	}

	/**
	Sets the minimum size of the control.
	@param Size the minimum size. If the width_ field is set equal to 
	Control::mmIgnoreMinWidth then the minimum width is ignored. If the 
	height_ field is set equal to Control::mmIgnoreMinHeight then the 
	minimum height is ignored.
	*/
	void setMinSize( const Size& val ) {
		minSize_ = val;
	}

	/**
	Returns the minimum width of the control.
	@see getMinSize
	*/
	double getMinWidth() {
		return minSize_.width_;
	}

	/**
	Sets the minimum width of the control.
	@see setMinSize
	*/
	void setMinWidth( const double& val ) {
		minSize_.width_ = val;
	}

	/**
	Returns the minimum height of the control.
	@see getMinSize
	*/
	double getMinHeight() {
		return minSize_.height_;
	}

	/**
	Sets the minimum height of the control.
	@see setMinSize
	*/
	void setMinHeight( const double& val ) {
		minSize_.height_ = val;
	}

	/**
	Returns the maximum size for this control. At the moment this is only
	used for Frames, to control the maximum size a user can resize the
	Frame to.
	@return Size the maximum size. If the width_ field is set equal to 
	Control::mmIgnoreMaxWidth then the maximum width is ignored. If the 
	height_ field is set equal to Control::mmIgnoreMaxHeight then the 
	maximum height is ignored. 
	*/
	Size getMaxSize() {
		return maxSize_;
	}

	/**
	Sets the maximum size of the control.
	@param Size the maximum size. If the width_ field is set equal to 
	Control::mmIgnoreMaxWidth then the maximum width is ignored. If the 
	height_ field is set equal to Control::mmIgnoreMaxHeight then the 
	maximum height is ignored.
	*/
	void setMaxSize( const Size& val ) {
		maxSize_ = val;
	}

	double getMaxWidth() {
		return maxSize_.width_;
	}

	void setMaxWidth( const double& val ) {
		maxSize_.width_ = val;
	}

	double getMaxHeight() {
		return maxSize_.height_;
	}

	void setMaxHeight( const double& val ) {
		maxSize_.height_ = val;
	}

	/**
	*returns an object implementing the Scrollable interface
	*The default value is NULL, indicating the control does not support
	*scrolling behaviour
	*/
	Scrollable* getScrollable() {
		return scrollable_;
	}

	/**
	*sets the scrollable for the control. Control's with a
	*scrollable object will be able to provide scroll bars
	*when neccessary.
	*@see Scrollable
	*@see Scrollable::setVirtualViewHeight
	*@see Scrollable::setVirtualViewWidth
	*/
	virtual void setScrollable( Scrollable* scrollable );

	/**
	Call this method to adjust the view bounds (i.e. what the GraphicsContext::getViewableBounds()
	method returns) to take into consideration the presence of scroll bars
	*/
	void adjustViewableBoundsAndOriginForScrollable( GraphicsContext* context, Rect& viewBounds, Point& origin );

	/**
	*returns a string that is used for context sensitive help
	*for the control.
	*/
	String getWhatThisHelpString() {
		return whatThisHelpString_;
	}

	/**
	*sets the string that is used for context sensitive help
	*for the control.
	*/
	void setWhatThisHelpString( const String& whatThisHelpString ) {
		whatThisHelpString_ = whatThisHelpString;
	}

	/**
	*returns a string that is used to display in the tooltip
	*for the control.
	*/
	String getToolTipText() {
		return toolTip_;
	}

	/**
	*sets the string that is used to display in the tooltip
	*for the control.
	*/
	void setToolTipText( const String& tooltip );

	/**
	*returns the cursor ID for the control. The cursor id represents an id tag that
	*identifies a Cursor object to be used to for controling the display of the
	*mouse cursor. To access the Cursor object directly call the CursorManager::getCursor()
	*method.
	*@return int32 the id of the cursor
	*@see CursorManager
	*@see CursorManager::getCursor()
	*@see Cursor
	*/
	int32 getCursorID() {
		return cursorID_;
	}

	/**
	*sets the cursor ID for the control. This will change the
	*cursor appearance whenever the mouse is over the control.
	*/
	void setCursorID( const int32& cursorID );

	/**
	*returns the anchor mask value for this control.
	*Anchors allow you to customize the sizing behavior of a particular control.
	*Setting the alignment may be done at any time after the control has been instantiated.
	*By anchoring a control you can have finer control over how the control gets resized
	*when it's parent is resized than simply specifying an alignment value. However,
	*changing the anchor value of a control will automatically set the control's alignment
	*to ALIGN_NONE, while changing the control's alignment will automatically set the control's
	*anchor value to ANCHOR_NONE. The two values are mutually exclusive, you can get one or
	*the other, but not both. The following table describes the meanings of the various
	*mask values for the anchor value, which can have either the value of ANCHOR_NONE
	*or any combination of the other four mask types.
	*<table width="100%" cellpadding="2" cellspacing="0" border="1" bordercolor="#C0C0C0">
	*<tr>
	*<td width="20%" bgcolor="#C0C0C0" valign=TOP>
	*Value</td>
	*<td width="80%" bgcolor="#C0C0C0" valign=TOP>
	*Meaning</td>
	*</tr>
	*<tr>
	* <td width="20%" valign=TOP>
	*	<code>ANCHOR_NONE</code></td>
	* <td width="80%" valign=TOP>
	*   This is the default value for a control's anchor property. No layout
	*	adjustments are performed on the control.</td>
	*</tr>
	*<tr>
	* <td width="20%" valign=TOP>
	*   <code>ANCHOR_TOP</code></td>
	* <td width="80%" valign=TOP>
	*   The Control is anchored to the top edge of the parent control it
	*	belongs to. Whatever the distance between the top edge and the top
	*	coordinate of the control when this is set, is maintained whenever
	*	the parent control's dimensions change. </td>
	*</tr>
	*<tr>
	* <td width="20%" valign=TOP>
	*   <code>ANCHOR_LEFT</code></td>
	* <td width="80%" valign=TOP>
	*   The Control is anchored to the left edge of the parent control it
	*	belongs to. Whatever the distance between the left edge and the left
	*	coordinate of the control when this is set, is maintained whenever
	*	the parent control's dimensions change. </td>
	*</tr>
	*<tr>
	* <td width="20%" valign=TOP>
	*   <code>ANCHOR_BOTTOM</code></td>
	* <td width="80%" valign=TOP>
	*   The Control is anchored to the bottom edge of the parent control it belongs
	*	to. Whatever the distance between the bottom edge and the bottom coordinate
	*	of the control when this is set, is maintained whenever the parent control's
	*	dimensions change. </td>
	*</tr>
	*<tr>
	* <td width="20%" valign=TOP>
	*   <code>ANCHOR_RIGHT</code></td>
	* <td width="80%" valign=TOP>
	*   The Control is anchored to the right edge of the parent control it belongs to.
	*	Whatever the distance between the right edge and the right coordinate of the
	*	control when this is set, is maintained whenever the parent control's dimensions
	*	change.</td>
	*</tr>
	*</table>
	*/
	AnchorTypes getAnchor() {
		return anchor_;
	}

	/**
	*sets the anchor mask value for the control. See
	*getAnchor() for a complete description of the various values
	*and what they mean for the control's alignment.
	*@see getAnchor()
	*/
	void setAnchor( const AnchorTypes& anchor );

	/**
	*returns the current delta for each of the anchors.
	*There will be a delta for the left, the top, the right,
	*and the bottom sides. These deltas are recalculated whenever
	*the controls bounds are set with the anchorDeltasNeedUpdating
	*param set to true.
	*/
	float* getAnchorDeltas() {
		return anchorDeltas_;
	}

	/**
	*this function determines if the control keeps the key event
	*when the return key is pressed, or if it will relinquish the event
	*for the system to process. Most controls will return false with the
	*exception of keyboard entry controls. If the control does keep the
	*event, then any default buttons will NOT receive their enter key
	*event until they receive focus.
	*@see Button::setDefault()
	*/
	virtual bool keepReturnCharacter() {
		return false;
	}

	/**
	*This function determines whether or not the
	*control should have precedence and prevent the
	*the tab key from switching focus
	*to another control. Typically this will return
	*false, with the exception of keyboard entry controls
	*like the TextControl. To prevent the tab key from
	*causing a the framework to switch to another control,
	*override this and return true.
	*@return bool returns false, indicating the framework can
	*go ahead and switch the focus to the next tabbed control
	*when appropriate, otherwise if it returns true the control
	*will keep the tabbed key event and process it
	*/
	virtual bool keepsTabKey() {
		return false;
	}

	/**
	*This function determines whether or not the
	*control should have precedence and prevent the
	*the arrow keys from switching focus
	*to another control. Typically this will return
	*false, with the exception of keyboard entry controls
	*like the TextControl. To prevent the tab key from
	*causing a the framework to switch to another control,
	*override this and return true.
	*@return bool returns false, indicating the framework can
	*go ahead and switch the focus to the next tabbed control
	*when appropriate, otherwise if it returns true the control
	*will keep the tabbed key event and process it
	*/
	virtual bool keepsArrowKeys() {
		return false;
	}


	/**
	*Indicates whether the control responds to the TAB key
	*to set it's focus when tabbing from control to control.
	*@return bool whether the control responds to the Tab key. If the
	*return value is true then the control will allow itself to become
	*focused, while a value of false indicates the control does not respond
	*to the tab key and will not become focused.
	*/
	bool getTabStop() {
		return (controlState_ & Control::csTabStop) ? true : false;
	}

	/**
	*sets the tab stop value of the control. The default value
	*is true.
	*@param bool the new value for the tab stop property
	*@see getTabStop()
	*/
	void setTabStop( const bool& tabStop );

	/**
	*Returns the tab order of the control. Tab order may be specified
	*independently of the controls child order within it's parent
	*Container.
	*@return
	*/
	int32 getTabOrder() {
		return tabOrder_;
	}

	/**
	*
	*/
	void setTabOrder( const int32& tabOrder );


	/**
	Returns the AcceleratorKey instance that is associated with this control and
	the specified key code and modifier mask. The AcceleratorKey will be activated
	whenever the corresponding keyboard combination is pressed.
	@param VirtualKeyCode the key code that represents this AcceleratorKey. For
	example, vkLetterV is used to indicated an accelerator that is triggered whenever
	the "V" key is pressed.
	@param uint32 a mask of special keys that can be pressed together with the
	specified key code, such as Shift, Alt or Ctrl.
	@see AcceleratorKey
	*/
	AcceleratorKey* getAccelerator( const VirtualKeyCode& keyCode, const uint32& modifierMask );

	/**
	Associates a new AcceleratorKey with the Control.
	The key code, modifier mask and event handler
	all become the attributes of the new AcceleratorKey
	*/
	void addAcceleratorKey( const VirtualKeyCode& keyCode, const uint32& modifierMask, EventHandler* eventHandler );

	/**
	Associates a new AcceleratorKey with the Control.
	The key code, modifier mask and the action's
	accelerator event handler all become the attributes
	of the new AcceleratorKey instance.
	*/
	void addAcceleratorKey( const VirtualKeyCode& keyCode, const uint32& modifierMask, Action* action );

	/**
	This should generally not be called. It's for framework usage only at this
	point.
	*/
	void addAcceleratorKey( AcceleratorKey* accelerator );

	/**
	Pressing the accelerator character will cause the control's
	mnemonicActivate() method to be fired, which by default
	sets focus to the control
	*/
	virtual void mnemonicActivate();

	/**
	This returns a pointer to the top level frame that this control is associated with
	@return Frame the top level froma instance for this control. This may not be the
	parent instance as the control's current parent.
	@see Control::getParent()
	*/
	Frame* getParentFrame();

	/**
	This returns a value that indicates whether or not hte control is considered to
	be active. A control is considered active if it belongs to, i.e. is a child
	eitehr directly or indirectly, (or is) the active frame. This is
	determined by checking if the control's getParentFrame() equals the Frame::getActiveFrame().
	@return bool true if the control is active, otherwise false.
	*/
	bool isActive();

	/**
	Can the control accept focus?
	This method determines if the control can recieve focus.
	@return bool returns true if the control is enabled and all
	of it's parents are enabled as well. Otherwise it returns
	false.
	*/
	bool canAcceptFocus();

	/**
	returns the current container for this control.
	The container determines whether or not the control
	can contain child controls. By default a control
	container is NULL, indicating it cannot hold any
	child controls.
	@return Container returns the current container for
	the control. If the value returned is NULL, then
	the control has no container, and cannot have any child
	controls.
	@see Container
	*/
	Container* getContainer() {
		return container_;
	}

	/**
	sets the current container for a control. Setting
	a control's container, will enable (or disable, if
	the container value passed in is NULL) the control's
	ability to handle child controls
	@see Container
	*/
	void setContainer( Container* container );

	/**
	This returns whether or not the control has 
	any child controls associated with it. 

	@return bool returns true if the control has children. 
	A control with children, by definition, \em must 
	have a Container instance. Returns false if the
	control has no children. Note that a false
	return value doesn't neccessarily mean that the 
	control has no container (just that the container
	has no child controls yet).

	@see getContainer()
	*/
	bool hasChildren();

	/**
	Returns whether or not this control is a 
	child of some other control. Equivalent 
	to checking the return value of getParent()
	for a non-null value.
	@return bool returns true if the control has a 
	parent. Otherwise it returns false.

	@see getParent()
	*/
	bool isChild();

	/**
	Returns whether or not the control will
	allow paint notification. 
	@see paint()
	@see BeforeControlPainted
	@see AfterControlPainted
	*/
	bool getAllowPaintNotification() {
		return (controlState_ & Control::csAllowPaintNotification) ? true : false;
	}

	/**
	Sets whether or not the control will allow 
	paint notification.
	@see paint()
	@see BeforeControlPainted
	@see AfterControlPainted
	*/
	void setAllowPaintNotification( const bool& val ) {
		if ( val ) {
			controlState_  |= Control::csAllowPaintNotification;
		}
		else {
			controlState_  &= ~Control::csAllowPaintNotification;
		}
	}


	bool ignoreForParentScrolling() {
		return (controlState_ & Control::csIgnoreForParentScrolling) ? true : false;		
	}

	void setIgnoreForParentScrolling( const bool& val ) {
		if ( val ) {
			controlState_  |= Control::csIgnoreForParentScrolling;
		}
		else {
			controlState_  &= ~Control::csIgnoreForParentScrolling;
		}
	}

	/**
	Called by the container instance of the child's parent control during it's
	paintChildren() function. The implementation should do whatever is neccessary
	to seup up the child control's cliprect. A pre-calculated cliprect is passed
	in that was generated by the paintChildren() function. 
	
	This function is called \em prior the child control's paint() or paintBorder()
	functions.
	*/
	void preChildPaint( GraphicsContext* graphicsContext, Control* child, Rect* childClipRect );
	
	/**
	Called by the container instance of the child's parent control during it's
	paintChildren() function. Called after the child control's paint() and paintBorder()
	functions.
	*/
	void postChildPaint( GraphicsContext* graphicsContext, Control* child, Rect* oldClipRect );

	/**
	sets a new model for the view
	Once set, the control fires a ControlModelChanged event.
	*/
	virtual void setViewModel( Model* viewModel );
	
	/**
	*returns the current control that has captured the mouse input.
	*This may return NULL if no control has expressly captured the
	*mouse input through the Control::keepMouseEvents() method.
	*/
	static Control* getCapturedMouseControl();

	/**
	*sets the control that has captured the mouse. Used internally
	*only. <b>DO NOT<b> call, this is for internal framework use only.
	*/
	static void setCapturedMouseControl( Control* control );

	/**
	*returns the currently focused control.
	*/
	static Control* getCurrentFocusedControl();

	/**
	*returns the control the mouse was previously over, if any.
	*/
	static Control* getPreviousMouseOverControl();

	static void setPreviousMouseOverControl( Control* control );

	static void buildTabList( Control* control, std::vector<Control*>& tabList );

	/**
	called by the internals of the framework - DO NOT CALL!
	*/
	void internal_beforePaint( GraphicsContext* context );

	/**
	called by the internals of the framework - DO NOT CALL!
	*/
	void internal_afterPaint( GraphicsContext* context );
protected:
	void updateAnchorDeltas();

	virtual void getAppNameAndKey( String& appName, String& key );

	virtual void destroy();

protected:
	static Control* currentFocusedControl;
	static Control* previousMouseOverControl;
	static Control* capturedMouseControl;

	ControlPeer* peer_;
	ControlGraphicsContext* context_;
	Control * parent_;
	AlignmentType aligment_;
	uint32 anchor_;
	Rect* bounds_;
	Rect* clientBounds_;
	Border* border_;
	Color* color_;
	Font* font_;
	View* view_;
	//bool useParentFont_;
	//bool doubleBuffered_;
	//bool hasMouseCapture_;
	//bool autoStartDragDrop_;
	PopupMenu* popupMenu_;
	Scrollable* scrollable_;
	String whatThisHelpString_;
	String toolTip_;
	int32 cursorID_;
	Cursor* cursor_;
	float anchorDeltas_[4];
	//bool tabStop_;
	int32 tabOrder_;
	Point clickPt_;
	//bool useRenderBuffer_;
	Container* container_;
	Size minSize_;
	Size maxSize_;

	/*
	bool ignoredForLayout_;
	bool allowPaintNotification_;
	bool enabled_;
	bool visible_;
	*/
	uint32 controlState_;

};


}; // namespace VCF 


#endif // _VCF_CONTROL_H__

/**
$Id$
*/
