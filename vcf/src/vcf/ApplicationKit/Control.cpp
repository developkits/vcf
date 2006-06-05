//Control.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


/* Generated by Together */
#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/Action.h"
#include "vcf/ApplicationKit/ActionEvent.h"


using namespace VCF;

Control* Control::capturedMouseControl = NULL;
Control* Control::currentFocusedControl = NULL;
Control* Control::previousMouseOverControl = NULL;

Control::Control():
	peer_(NULL),
	context_(NULL),
	parent_(NULL),
	aligment_(AlignNone),
	anchor_(AnchorNone),
	bounds_(NULL),
	clientBounds_(NULL),
	border_(NULL),
	color_(NULL),
	font_(NULL),
	view_(NULL),
	popupMenu_(NULL),
	scrollable_(NULL),
	cursorID_(0),
	cursor_(NULL),
	tabOrder_(-1),
	container_(NULL),
	controlState_(Control::csDefaultControlState)
{
	//this (Font) cast is to avoid an internal compiler error on some vc6 versions
	font_ = new Font( (Font) UIToolkit::getUIMetricsManager()->getDefaultFontFor( UIMetricsManager::ftControlFont ) );

	context_ = new ControlGraphicsContext( this );

	bounds_ = new Rect();
	clientBounds_ = new Rect();
	color_ = new Color;

	setCursorID( (long)Cursor::SCT_DEFAULT );

	setColor( GraphicsToolkit::getSystemColor( SYSCOLOR_FACE ) );

	viewControl_ = this;

	anchorDeltas_[ANCHOR_DTOP] = 0.0f;
	anchorDeltas_[ANCHOR_DLEFT] = 0.0f;
	anchorDeltas_[ANCHOR_DBOTTOM] = 0.0f;
	anchorDeltas_[ANCHOR_DRIGHT] = 0.0f;

	minSize_.width_ = Control::mmIgnoreMinWidth;
	minSize_.height_ = Control::mmIgnoreMinHeight;

	maxSize_.width_ = Control::mmIgnoreMaxWidth;
	maxSize_.height_ = Control::mmIgnoreMaxHeight;
}

Control::~Control()
{
	componentState_ = Component::csDestroying;
	/**
	this shouldn't happen, but it's
	possible if an exception is thrown in a constructor
	and not handled. In this case the Control::destroy
	method will not be called, but rather the destructor
	will be called directly.
	So we double check here and delete the control peer
	*/
	if ( NULL != peer_ ) {
		peer_->setControl( NULL );
		peer_->destroyControl();
		delete peer_;
		peer_ = NULL;
	}
}

void Control::destroy()
{
	
	if ( isLightWeight() ) {
		//have to handle a Component::COMPONENT_DESTROYED event manually
		//as they will NOT recv notification from the underlying windowing system
		VCF::ComponentEvent event( this, Component::COMPONENT_DESTROYED );
		handleEvent( &event );
	}


	if ( NULL != peer_ ) {
		UIToolkit::removeAcceleratorKeysForControl( this );
	}

	if ( this == Control::previousMouseOverControl ) {
		Control::previousMouseOverControl = NULL;
	}

	delete bounds_;
	bounds_ = NULL;
	delete clientBounds_;
	clientBounds_ = NULL;
	delete color_;
	color_ = NULL;

	delete context_;
	context_ = NULL;

	if ( NULL != peer_ ) {
		peer_->destroyControl();
		delete peer_;
		peer_ = NULL;
	}

	if ( NULL != font_ ){
		font_->free();
		font_ = NULL;
	}

	if ( this == Control::currentFocusedControl ) {
		Control::currentFocusedControl = NULL;
	}

	if ( NULL != view_ ) {
		Object* obj = dynamic_cast<Object*>( view_ );
		if ( NULL != obj ) {
			obj->release();
		}
		else {
			delete view_;
		}
	}
	view_ = NULL;

	
	container_ = NULL;
	
	Component::destroy();
}

Border* Control::getBorder()
{
	return border_;
}

void Control::setBorder( Border* border )
{	
	border_ = border;

	if ( NULL != border_ ) {
		if ( NULL == border_->getOwner() ) {
			addComponent( border_ );
		}
	}
	
	peer_->setBorder( border_ );
}

Rect Control::getBounds()/**throw( InvalidPeer ); -JEC - FIXME later*/
{	

	Control* parent = getParent();
	bool lightweightParent = false;
	if ( NULL != parent ) {
		lightweightParent = parent->isLightWeight();
	}

	if ( lightweightParent && !isLightWeight() ) {
		*bounds_ = peer_->getBounds();

		Rect tmpBounds = *bounds_;

		tmpBounds.offset( -tmpBounds.left_, -tmpBounds.top_ );

		translateToScreenCoords( &tmpBounds );

		parent->translateFromScreenCoords( &tmpBounds );

		*bounds_ =  tmpBounds;
	}
	else {
		*bounds_ = peer_->getBounds();
		//peer_->setBounds( bounds_ );
	}
	return *bounds_;
}




Rect Control::getClientBounds( const bool& includeBorder ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{	
	Rect r = peer_->getBounds();
	clientBounds_->setRect( 0.0, 0.0, r.getWidth(), r.getHeight() );

	if ( (includeBorder) && (NULL != border_) ){
		r = *clientBounds_;
		*clientBounds_ = border_->getClientRect( &r, this );
	}

	return *clientBounds_;
}

double Control::getLeft() /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	
	//*bounds_ = peer_->getBounds();
	getBounds();

	return bounds_->left_;
}

double Control::getRight()
{

	getBounds();
	//*bounds_ = peer_->getBounds();

	return bounds_->right_;
}

double Control::getBottom() /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	getBounds();
	//*bounds_ = peer_->getBounds();

	return bounds_->bottom_;
}

double Control::getWidth() /**throw( InvalidPeer ); -JEC - FIXME later*/
{

	getBounds();
	//*bounds_ = peer_->getBounds();

	return bounds_->getWidth();
}

double Control::getTop() /**throw( InvalidPeer ); -JEC - FIXME later*/
{

	getBounds();
	//*bounds_ = peer_->getBounds();

	return bounds_->top_;
}

double Control::getHeight() /**throw( InvalidPeer ); -JEC - FIXME later*/
{

	//double result = 0.0;

	getBounds();
	//*bounds_ = peer_->getBounds();
	//result = bounds_->getHeight();


	return bounds_->getHeight();
}

bool Control::getVisible() /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	return (controlState_ & Control::csVisible) ? true : false;//peer_->getVisible();
}

AlignmentType Control::getAlignment()
{
	return aligment_;
}

bool Control::isIgnoredForLayout()
{
	return (controlState_ & Control::csIgnoreForLayout) ? true : false;
}

void Control::setIgnoredForLayout( const bool& val )
{
	if ( val ) {
		controlState_ |= Control::csIgnoreForLayout;
	}
	else {
		controlState_ &= ~Control::csIgnoreForLayout;
	}

	Control* parent = getParent();
	if ( NULL != parent ) {
		parent->getContainer()->resizeChildren( NULL );
	}
}

void Control::setBounds( const double& x, const double& y, const double& width, const double& height )
{
	setBounds( &Rect( x, y, x+width, y+height ) );
}

void Control::setUsingRenderBuffer( const bool& useRenderBuffer )
{
	if ( useRenderBuffer ) {
		controlState_  |= Control::csUseRenderBuffer;
	}
	else {
		controlState_  &= ~Control::csUseRenderBuffer;
	}

	if ( useRenderBuffer ) {
		//if ( NULL == context_->getRenderArea() ) {
			//context_->setRenderArea( *bounds_ );
		//}
		repaint();
	}
	else {
		//if ( NULL != context_->getRenderArea() ) {
			context_->deleteRenderArea();
		//}
	}
}

void Control::setBounds( Rect* rect, const bool& anchorDeltasNeedUpdating ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	*bounds_ = *rect;

	/**
	Adjust the bounds to take into account a
	potentially lightweight parent
	*/
	Control* parent = getParent();
	bool lightweightParent = false;
	if ( NULL != parent ) {
		lightweightParent = parent->isLightWeight();
	}

	if ( lightweightParent && !isLightWeight() ) {
		Control* realParent = parent;
		while ( NULL != realParent ) {
			if ( !realParent->isLightWeight() ) {
				break;
			}
			realParent = realParent->getParent();
		}

		if ( NULL != realParent ) {


			Rect tmp = *bounds_;
			Rect tmp2 = *bounds_;

			tmp.offset( -tmp.left_, -tmp.top_ );

			parent->translateToScreenCoords( &tmp );

			realParent->translateFromScreenCoords( &tmp );

			tmp2.offset( tmp.left_, tmp.top_ );

			*bounds_ = tmp2;

			peer_->setBounds( &tmp2 );
		}
		else {
			peer_->setBounds( bounds_ );
		}
	}
	else {
		peer_->setBounds( bounds_ );
	}

	if ( isUsingRenderBuffer() ) {
//		if ( NULL == context_->getRenderArea() ) {
			//context_->setRenderArea( *bounds_ );
//		}
	}

	if ( true == anchorDeltasNeedUpdating ) {
		updateAnchorDeltas();
	}


	//check to see if we need to adjust the scrollable's virtual height/width
	//of our parent
	if ( (getAlignment() == AlignNone) && (!ignoreForParentScrolling()) ) {
		Control* parent = getParent();
		if ( NULL != parent ) {
			Container* container = parent->getContainer();
			Scrollable* scrollable = parent->getScrollable();
			if ( (NULL != container) && (NULL != scrollable) ) {
				Rect r = getBounds();

				if ( scrollable->getVirtualViewHeight() < r.bottom_ ) {
					scrollable->setVirtualViewHeight( r.bottom_ );
				}

				if ( scrollable->getVirtualViewWidth() < r.right_ ) {
					scrollable->setVirtualViewWidth( r.right_ );
				}
			}
		}
	}
}

void Control::setAlignment( const AlignmentType& alignment )
{
	aligment_ = alignment;
	/**
	Fix submitted by Marcello - see bug [ 789945 ] Control::setAlignment() does not allow only one align style
	*/
	if ( aligment_ != AlignNone ) {
		anchor_ = AnchorNone;
	}

	Control* parent = getParent();
	if ( NULL != parent ){
		Container* container = parent->getContainer();
		if ( NULL != container ){
			container->resizeChildren(NULL);//this);
		}
	}

	updateAnchorDeltas();
}

void Control::setLeft( const double& left ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	double dx = bounds_->getWidth();
	bounds_->left_ = left;
	bounds_->right_ = left + dx;
	setBounds( bounds_ );
}

void Control::setRight( const double& right ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	bounds_->right_ = right;
	setBounds( bounds_ );
}

void Control::setWidth( const double& width ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	bounds_->right_ = bounds_->left_ + width;
	setBounds( bounds_ );
}

void Control::setTop( const double& top ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	double dy = bounds_->getHeight();
	bounds_->top_ = top;
	bounds_->bottom_ = top + dy;
	setBounds( bounds_ );
}

void Control::setBottom( const double& bottom ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	bounds_->bottom_ = bottom;
	setBounds( bounds_ );
}

void Control::setHeight( const double& height ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	if ( NULL != peer_ ){
		bounds_->bottom_ = bounds_->top_ + height;
		setBounds( bounds_ );
	};
}

void Control::setVisible( const bool& visible ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	bool oldVisible = peer_->getVisible();
	
	if ( visible ) {
		controlState_  |= Control::csVisible;
	}
	else {
		controlState_  &= ~Control::csVisible;
	}

	if ( oldVisible != visible ) {	
		
		if ( !isLoading() ) {
			peer_->setVisible( visible );
		}

		Control* parent = getParent();
		if ( NULL != parent ) {
			Container* container = parent->getContainer();
			if ( NULL != container ) {
				if ( visible ) { //(oldVisible == false) && (true == visible) ) {
					container->resizeChildren(NULL);//this);
				}
				else {
					container->resizeChildren(NULL);
				}
			}
		}
	}
}

void Control::handleEventAndForwardToChildren( Event* event )
{
	handleEvent( event );
	Container* container = getContainer();
	if ( NULL != container && (!event->isConsumed())) {
		Enumerator<Control*>* children = container->getChildren();
		while ( children->hasMoreElements() )  {
			Control* child = children->nextElement();
			child->handleEventAndForwardToChildren( event );
			if ( event->isConsumed() ) {
				break;
			}
		}
	}

}

void Control::handleEvent( Event* event )
{
	Component::handleEvent( event );
	if ( NULL != event ){
		unsigned long eventType = event->getType();

		switch ( eventType ){
			case Action::UpdateEvent : {
				ActionEvent* actionEvent = (ActionEvent*)event;

				setEnabled( actionEvent->isEnabled() );
			}
			break;

			case CONTROL_SIZED:{
				ControlEvent* controlEvent = (ControlEvent*)event;

				//bounds_->right_ = bounds_->left_ + controlEvent->getNewSize().width_;
				//bounds_->bottom_ = bounds_->top_ + controlEvent->getNewSize().height_;
				ControlSized.fireEvent( (ControlEvent*)event );

				if (!event->isConsumed() && !isDesigning()) {
					sizeChange( controlEvent );
				}

				if ( isUsingRenderBuffer() ) {
					Rect bounds = getClientBounds(false);
					//context_->setRenderArea( bounds );						
				}				
			}
			break;

			case CONTROL_POSITIONED:{
				ControlEvent* controlEvent = (ControlEvent*)event;
				//double w = bounds_->getWidth();
				//double h = bounds_->getHeight();

				//bounds_->left_ = controlEvent->getNewPosition().x_;
				//bounds_->top_ = controlEvent->getNewPosition().y_;
				//bounds_->right_ = bounds_->left_ + w;
				//bounds_->bottom_ = bounds_->top_ + h;

				ControlPositioned.fireEvent( controlEvent );

				if (!event->isConsumed() && !isDesigning()) {
					positionChange( controlEvent );
				}
			}
			break;

			case CONTROL_PARENT_CHANGED:{
				ControlParentChanged.fireEvent( (ControlEvent*)event );

				if (!event->isConsumed() && !isDesigning()) {
					parentChange( (ControlEvent*)event );
				}
			}
			break;

			case MOUSE_ENTERED:{
				MouseEvent* mouseEvent = (MouseEvent*)event;


				MouseEnter.fireEvent( mouseEvent );
				if (!event->isConsumed() && !isDesigning()) {
					mouseEnter( mouseEvent );
				}
			}
			break;

			case MOUSE_DOWN:{
				MouseEvent* mouseEvent = (MouseEvent*)event;

				clickPt_ = *mouseEvent->getPoint();
				

				MouseDown.fireEvent( mouseEvent );

				//turn off normal mouse behaviour in design mode
				if ( !event->isConsumed() && !isDesigning() ) {
					mouseDown( mouseEvent );
				}
			}
			break;

			case MOUSE_MOVE:{
				MouseEvent* mouseEvent = (MouseEvent*)event;

				if (mouseEvent->hasLeftButton() || mouseEvent->hasRightButton() || mouseEvent->hasMiddleButton() ) {
					if ( (true == getAutoStartDragDrop()) ) { //&& (false == dragDropStarted_) ) {
						if ( true == canBeginDragDrop( mouseEvent->getPoint() ) ) {
							//dragDropStarted_ = true;							
							if ( beginDragDrop ( mouseEvent ) ) {
								return;
							}
						}
					}
				}
				
				peer_->setCursor( cursor_ );
				MouseMove.fireEvent( mouseEvent );
				if (!event->isConsumed() && !isDesigning()) {
					mouseMove( mouseEvent );
				}
			}
			break;

			case MOUSE_UP:{

				MouseEvent*  mouseEvent = (MouseEvent*)event;

				Point tmpPt = *mouseEvent->getPoint();
				Point origPt = *mouseEvent->getPoint();


				bool rightBtn = mouseEvent->hasRightButton();
				MouseUp.fireEvent( mouseEvent );
				if (!mouseEvent->isConsumed() && !isDesigning()) {
					mouseUp( mouseEvent );
				}
				if ( (true == rightBtn) && !isDesigning() ){
					//(NULL != popupMenu_) && 
					ControlPopupMenuMenuEvent e(this,Control::BEFORE_POPUP_MENU);
					e.popupMenu = popupMenu_;

					BeforePopupMenu.fireEvent( &e );

					if ( !e.cancelPopup && (NULL != e.popupMenu) ) {
						Point tmpPt = *mouseEvent->getPoint();
						if ( NULL != scrollable_ ) {
							tmpPt.x_ -= scrollable_->getHorizontalPosition();
							tmpPt.y_ -= scrollable_->getVerticalPosition();
						}

						e.popupMenu->popup( &tmpPt );
					}
				}


				if ( clickPt_.closeTo( tmpPt.x_, tmpPt.y_, 2 ) ){

					
					MouseEvent clickEvent( this,
											MOUSE_CLICK,
											mouseEvent->getButtonMask(),
											mouseEvent->getKeyMask(),
											&origPt );


					handleEvent( &clickEvent );
				}

			}
			break;

			case MOUSE_LEAVE:{
				MouseEvent*  mouseEvent = (MouseEvent*)event;


				MouseLeave.fireEvent( mouseEvent );
				if (!mouseEvent->isConsumed() && !isDesigning()) {
					mouseLeave( mouseEvent );
				}
			}
			break;

			case MOUSE_CLICK:{
				
				MouseEvent*  mouseEvent = (MouseEvent*)event;
				MouseClicked.fireEvent( mouseEvent );
				if (!mouseEvent->isConsumed() && !isDesigning() ) {
					mouseClick( mouseEvent );
				}
			}
			break;

			case MOUSE_DBLCLICK:{
				MouseEvent*  mouseEvent = (MouseEvent*)event;

				MouseDoubleClicked.fireEvent( mouseEvent );
				if (!mouseEvent->isConsumed() && !isDesigning() ) {
					mouseDblClick( mouseEvent );
				}
			}
			break;

			case KEYBOARD_DOWN:{
				KeyboardEvent* kbEvent = (KeyboardEvent*)event;
				
				if ( !isDesigning() || (NULL != getContainer()) ) {
					keyDown( kbEvent );					
				}
				KeyDown.fireEvent( kbEvent );
			}
			break;

			case KEYBOARD_PRESSED:{
				KeyboardEvent* kbEvent = (KeyboardEvent*)event;
				if ( !isDesigning() || (NULL != getContainer()) ) {
					keyPressed( kbEvent );
				}
				
				KeyPressed.fireEvent( kbEvent );
			}
			break;

			case KEYBOARD_UP:{
				KeyboardEvent* kbEvent = (KeyboardEvent*)event;
				if ( !isDesigning() || (NULL != getContainer()) ) {
					keyUp ( kbEvent );
				}
				KeyUp.fireEvent( kbEvent );
			}
			break;

			case FOCUS_LOST:{
				FocusEvent* focusEvent = (FocusEvent*)event;

				//StringUtils::traceWithArgs( "Control::FOCUS_LOST, this[%s]@ %p\n",
				//					getClassName().c_str(), this );

				FocusLost.fireEvent( focusEvent );

				if (!event->isConsumed() && !isDesigning()) {
					lostFocus( focusEvent );
				}
			}
			break;

			case FOCUS_GAINED:{
				FocusEvent* focusEvent = (FocusEvent*)event;

				//StringUtils::traceWithArgs( "Control::FOCUS_GAINED, this[%s]@ %p\n",
				//					getClassName().c_str(), this );

				FocusGained.fireEvent( focusEvent );

				if (!event->isConsumed() && !isDesigning()) {
					gotFocus( focusEvent );
				}
			}
			break;
		}
		//if ( !mouseEvent->isConsumed() )
			//mouseEvent->setConsumed(true);
	}
}


bool Control::canBeginDragDrop( Point* point )
{
	bool result = false;

	Size dragDropDelta = UIToolkit::getDragDropDelta();

	/**
	clickPt_ gets set on a mousedown
	so make a rect around it
	r1 will be the inner  rect
	r2 will be the outer rect
	if the drag is outside the bounds of r1 and within r2
	then we can do a drag, otherwise forget it
	*/
	Rect r1( clickPt_.x_ - dragDropDelta.width_ /2.0,
			clickPt_.y_ - dragDropDelta.height_ /2.0,
			clickPt_.x_ + dragDropDelta.width_ /2.0,
			clickPt_.y_ + dragDropDelta.height_ /2.0 );

	Rect r2 = r1;

	r2.inflate( dragDropDelta.width_, dragDropDelta.height_ );

	//we are only ready to to begin a drag drop operation
	//if the point is NOT, I repeat, NOT, within the bounds of the
	//drag-drop rect. This prevents us from starting too early.
	//changing the dragDropDelta value allows us to control the 
	//sensitivity of how we reacto to drag-drop starts
	result = (!r1.containsPt( point )) && (r2.containsPt( point ));	

	return result;
}

void Control::setAutoStartDragDrop(const bool& canAutoStartDragDrop)
{
	if ( canAutoStartDragDrop ) {
		controlState_  |= Control::csAutoStartDragDrop;
	}
	else {
		controlState_  &= ~Control::csAutoStartDragDrop;
	}
}

bool Control::getAutoStartDragDrop()
{
	return (controlState_ & Control::csAutoStartDragDrop) ? true : false;
}

bool Control::beginDragDrop( MouseEvent* event )
{
	return false;
}

ControlPeer* Control::getPeer()
{
	return peer_;
}

void Control::setParent( Control* parent ) /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	if ( parent != parent_ ) {
		ControlEvent event( this, parent );
		ControlParentChanged.fireEvent( &event );
	}

	parent_ = parent;
	peer_->setParent( parent );


	if ( NULL != parent_ ) {

		if ( NULL == getOwner() ) {
			parent->addComponent( this );
		}

		if ( parent_->isLightWeight() && !isLightWeight() ) {

			Rect tmp = peer_->getBounds();
			Rect tmp2 = tmp;
			tmp.offset( -tmp.left_, -tmp.top_ );

			translateToScreenCoords( &tmp );
			parent->translateFromScreenCoords( &tmp );

			tmp2.offset( tmp2.left_ - tmp.left_, tmp2.top_ - tmp.top_ );

			*bounds_ = tmp2;
			peer_->setBounds( &tmp2 );
		}
	}

	updateAnchorDeltas();
}

Control* Control::getParent() /**throw( InvalidPeer ); -JEC - FIXME later*/
{
	return parent_;
}

void Control::removeFromParent( const bool& freeInstance )
{
	Control* parent = getParent();
	if ( NULL != parent ) {
		Container* container = parent->getContainer();
		VCF_ASSERT( NULL != container );

		container->remove( this );		
	}

	removeFromOwner( freeInstance );
}


bool Control::isFocused()
{
	bool result = (this == currentFocusedControl);

	return result;//peer_->isFocused();
}

Control* Control::setFocused()
{
	Control* result = NULL;

	if ( isNormal() || isCreated() || isDesigning() ) {	//JC added this so that a control recv's focus
														//only under these conditions

			if ( NULL != currentFocusedControl ) {
				if ( this != currentFocusedControl ) {
					//currentFocusedControl->setFocus( false ); //is this neccessary ?

					// do this to get the non focused control to repaint it's state
					currentFocusedControl->repaint();
				}
			}

		result = currentFocusedControl;
		currentFocusedControl = this;

		peer_->setFocused();
	}

	repaint();

	return result;
}

bool Control::isEnabled()
{
	return (controlState_ & Control::csEnabled) ? true : false;//peer_->isEnabled();
}

bool Control::areParentsEnabled()
{
	if ( !isEnabled() ) {
		return false;
	}

	bool result = true;

	Control* parent = getParent();
	while ( result && (NULL != parent) ) {
		result = parent->isEnabled();
		parent = parent->getParent();
	}

	return result;
}

void Control::setEnabled( const bool& enabled )
{
	if ( enabled ) {
		controlState_  |= Control::csEnabled;
	}
	else {
		controlState_  &= ~Control::csEnabled;
	}

	if ( ! isDesigning() ) {
		peer_->setEnabled( enabled );
	}

	repaint();
}

void Control::mouseEnter( MouseEvent* event )
{

}

void Control::mouseDown( MouseEvent* event )
{
	setFocused();
}

void Control::mouseMove( MouseEvent* event )
{
	//probably want to put a modifier here for the scrollable to modify the coords ?

}

void Control::mouseUp( MouseEvent* event )
{

}

void Control::mouseLeave( MouseEvent* event )
{

}

void Control::keyDown( KeyboardEvent* event )
{

}

void Control::mouseClick(  MouseEvent* event )
{

}

void Control::mouseDblClick(  MouseEvent* event )
{

}

void Control::keyPressed( KeyboardEvent* event )
{

}

void Control::keyUp( KeyboardEvent* event )
{

}

void Control::sizeChange( ControlEvent* event )
{

}

void Control::positionChange( ControlEvent* event )
{

}

void Control::parentChange( ControlEvent* event )
{

}

void Control::gotFocus( FocusEvent* event )
{

}

void Control::lostFocus( FocusEvent* event )
{

}

void Control::translateToParent( Point* point )
{
	Control* parent = getParent();
	if ( NULL != parent ){
		Rect bounds = parent->getBounds();
		point->x_ += bounds.left_;
		point->y_ += bounds.top_;
	}
	else{
		//throw exception ?
	}
}

void Control::translateToLocal( Point* point )
{
	Control* parent = getParent();
	if ( NULL != parent ){
		Rect bounds = getBounds();
		point->x_ -= bounds.left_;
		point->y_ -= bounds.top_;
	}
	else{
		//throw exception ?
	}
}

void Control::beforeDestroy( ComponentEvent* event )
{
	Component::beforeDestroy( event );
}

bool Control::isLightWeight()
{
	return false;
}

Control* Control::getHeavyweightParent()
{
	Control* parent = NULL;
	Control* result = NULL;
	result = getParent();
	if ( result != NULL ) {
		while ( true == result->isLightWeight() ){
			parent = result;
			result = parent->getParent();
		}
	}

	return result;
}

Color* Control::getColor()
{
	return color_;
}

void Control::setColor( Color* color )
{
	color_->copy( color );
	if ( (isNormal() || isDesigning()) && (NULL != peer_) ) {
		repaint();
	}
}


Font* Control::getFont()
{
	return font_;
}

void Control::setFont( Font* font )
{
	if ( NULL != font ){
		font_->copy( font );
	}
	
	peer_->setFont( font_ );

}

bool Control::useParentFont()
{
	return (controlState_ & Control::csUseParentFont) ? true : false;
}

void Control::setUseParentFont( const bool& useParentFont )
{
	if ( useParentFont ) {
		controlState_  |= Control::csUseParentFont;
	}
	else {
		controlState_  &= ~Control::csUseParentFont;
	}
}

void Control::afterCreate( ComponentEvent* event )
{	
	
}

void Control::repaint( Rect* repaintRect )
{	
	peer_->repaint( repaintRect, false );
	//if ( isUsingRenderBuffer() ) {
	//	context_->markRenderAreaDirty();
	///}
}

void Control::repaintNow( Rect* repaintRect )
{
	peer_->repaint( repaintRect, true );
	//if ( isUsingRenderBuffer() ) {
	//	context_->markRenderAreaDirty();
	//}
}

bool Control::isDoubleBuffered()
{
	return (controlState_ & Control::csDoubleBuffered) ? true : false;
}

void Control::setDoubleBuffered( const bool& doubleBuffered )
{
	if ( doubleBuffered ) {
		controlState_  |= Control::csDoubleBuffered;
	}
	else {
		controlState_  &= ~Control::csDoubleBuffered;
	}
}

void Control::keepMouseEvents()
{
	controlState_  |= Control::csHasMouseCapture;
	Control::setCapturedMouseControl( this );
	peer_->keepMouseEvents();
}

void Control::releaseMouseEvents()
{
	controlState_  &= ~Control::csHasMouseCapture;
	Control::setCapturedMouseControl( NULL );
	peer_->releaseMouseEvents();
}

Control* Control::getCapturedMouseControl()
{
	return Control::capturedMouseControl;
}

void Control::setCapturedMouseControl( Control* control )
{
	Control::capturedMouseControl = control;
}

GraphicsContext* Control::getContext()
{
	return context_;
}

PopupMenu* Control::getPopupMenu()
{
	return popupMenu_;
}

void Control::setPopupMenu( PopupMenu* popupMenu )
{
	if ( NULL != popupMenu_ ){
		popupMenu_->setControl( NULL );
	}

	popupMenu_ = popupMenu;

	if ( NULL != popupMenu_ ){
		popupMenu_->setControl( this );
	}
}

View* Control::getView()
{
	return view_;
}

void Control::setView( View* view )
{
	if ( NULL != view_ ) {
		Object* obj = dynamic_cast<Object*>( view_ );
		if ( NULL != obj ) {
			obj->release();
		}
	}

	view_ = view;

	if ( NULL != view_ ) {
		view_->setViewControl( this );
		Object* obj = dynamic_cast<Object*>( view_ );
		if ( NULL != obj ) {
			obj->addRef();
		}
	}
}

void Control::setScrollable( Scrollable* scrollable )
{
	scrollable_ = scrollable;
	if ( NULL != scrollable_ ) {
		scrollable_->setScrollableControl( this );
		scrollable_->recalcScrollPositions();
	}
}

void Control::setToolTipText( const String& tooltip )
{
	toolTip_ = tooltip;
}

void Control::setCursorID( const long& cursorID )
{
	cursorID_ = cursorID;
	cursor_ = CursorManager::getCursorManager()->getCursor( cursorID_ );
}

void Control::setAnchor( const unsigned long& anchor )
{
	anchor_ = anchor;
	aligment_ = AlignNone;
	updateAnchorDeltas();

	Control* parent = getParent();
	if ( NULL != parent ){
		Container* container = parent->getContainer();
		if ( NULL != container ){
			container->resizeChildren(NULL);//this);
		}
	}
}

void Control::updateAnchorDeltas() {
	if ( AnchorNone == anchor_ ) {
		anchorDeltas_[ANCHOR_DTOP] = 0.0f;
		anchorDeltas_[ANCHOR_DLEFT] = 0.0f;
		anchorDeltas_[ANCHOR_DBOTTOM] = 0.0f;
		anchorDeltas_[ANCHOR_DRIGHT] = 0.0f;
	}
	else {
		Control* parent = getParent();
		if ( NULL != parent ) {
			Rect parentBounds = parent->getClientBounds();
			Rect bounds = getBounds();
			anchorDeltas_[ANCHOR_DTOP] = (float) bounds.top_;
			anchorDeltas_[ANCHOR_DLEFT] = (float) bounds.left_;
			anchorDeltas_[ANCHOR_DBOTTOM] = (float) parentBounds.bottom_ -  bounds.bottom_;
			anchorDeltas_[ANCHOR_DRIGHT] = (float) parentBounds.right_ -  bounds.right_;
		}
	}
}

AcceleratorKey* Control::getAccelerator( const VirtualKeyCode& keyCode, const ulong32& modifierMask )
{
	return UIToolkit::getAccelerator( keyCode, modifierMask, this );
}

void Control::addAcceleratorKey( const VirtualKeyCode& keyCode, const ulong32& modifierMask, EventHandler* eventHandler )
{
	if ( NULL == eventHandler ) {
		throw InvalidPointerException( MAKE_ERROR_MSG_2("The Event handler passed in for the accelerator is NULL!") );
	}

	AcceleratorKey* newAccelKey = new AcceleratorKey( this, keyCode, modifierMask, eventHandler );

	addAcceleratorKey( newAccelKey );
}

void Control::addAcceleratorKey( const VirtualKeyCode& keyCode, const ulong32& modifierMask, Action* action )
{
	if ( NULL == action ) {
		throw InvalidPointerException( MAKE_ERROR_MSG_2("The action instance passed in for the accelerator is NULL!") );
	}

	addAcceleratorKey( keyCode, modifierMask, action->getAcceleratorEventHandler() );
}

void Control::addAcceleratorKey( AcceleratorKey* accelerator )
{
	UIToolkit::registerAccelerator( accelerator );
}

void Control::setTabStop( const bool& tabStop )
{
	if ( tabStop ) {
		controlState_  |= Control::csTabStop;
	}
	else {
		controlState_  &= ~Control::csTabStop;
	}
}

void Control::setTabOrder( const long& tabOrder )
{
	if ( tabOrder_ == tabOrder ) {
		return ;
	}

	Control* parent = getParent();
	if ( NULL != parent ) {
		Container* container = parent->getContainer();
		if ( NULL != container ) {
			ulong32 tmp = tabOrder;
			container->updateTabOrder( this, tmp );

			tabOrder_ = tmp;
		}
	}
}


Control* Control::getCurrentFocusedControl()
{
	return Control::currentFocusedControl;
}

void Control::mnemonicActivate()
{
	setFocused();
}

Control* Control::getPreviousMouseOverControl()
{
	return Control::previousMouseOverControl;
}

void Control::setPreviousMouseOverControl( Control* control )
{
	Control::previousMouseOverControl = control;
}


void Control::translateToScreenCoords( Point* pt )
{
	peer_->translateToScreenCoords( pt );
}

void Control::translateToScreenCoords( Rect* rect )
{
	Point pt ( rect->left_, rect->top_ );

	peer_->translateToScreenCoords( &pt );
	rect->left_ = pt.x_;
	rect->top_ = pt.y_;

	pt.x_ = rect->right_;
	pt.y_ = rect->bottom_;

	peer_->translateToScreenCoords( &pt );

	rect->right_ = pt.x_;
	rect->bottom_ = pt.y_;
}

void Control::translateFromScreenCoords( Point* pt )
{
	peer_->translateFromScreenCoords( pt );
}

void Control::translateFromScreenCoords( Rect* rect )
{
	Point pt ( rect->left_, rect->top_ );

	peer_->translateFromScreenCoords( &pt );
	rect->left_ = pt.x_;
	rect->top_ = pt.y_;

	pt.x_ = rect->right_;
	pt.y_ = rect->bottom_;

	peer_->translateFromScreenCoords( &pt );

	rect->right_ = pt.x_;
	rect->bottom_ = pt.y_;
}

void Control::setContainer( Container* container )
{
	Container* oldContainer = container_;

	container_ = container;	

	if ( NULL != container_ ) {
		
		if ( NULL == container_->getOwner() ) {
			addComponent(container_) ;
		}

		container_->setContainerControl( this );
	}

	//transfer over container controls!
	if ( NULL != container_ && NULL != oldContainer ) {
		container_->clear();

		int count = oldContainer->getChildCount();
		for ( int i=0;i<count;i++ ) {
			Control* control = oldContainer->getControlAtIndex( 0 );

			oldContainer->remove( control );

			container_->add( control );
		}

		oldContainer->clear();
	}
}

void Control::buildTabList( Control* control, std::vector<Control*>& tabList )
{
	tabList.push_back( control );

	Container* container = control->getContainer();
	if ( NULL != container ) {
		container->getTabList( tabList );
	}
}

Frame* Control::getParentFrame()
{
	Control* result = NULL;

	Control* parent = getParent();
	while ( parent != NULL ) {
		result = parent;
		parent = result->getParent();
	}

	if ( NULL == result ) {
		result = this;
	}

	return (Frame*)result;
}

bool Control::canAcceptFocus()
{
	bool result = true;

	if ( isEnabled() ) {
		Control* parent = getParent();
		while ( parent != NULL ) {
			if ( !parent->isEnabled() ) {
				result = false;
				break;
			}
			parent = parent->getParent();
		}
	}
	else {
		result = false;
	}

	return result;
}

void Control::adjustViewableBoundsAndOriginForScrollable( GraphicsContext* context, Rect& viewBounds, Point& origin )
{
	Scrollable* scrollable = getScrollable();
	if ( NULL != scrollable ) {
		Rect innerBounds = getClientBounds(true);

		//account for any children that overlap
		if ( NULL != container_ ) {
			Enumerator<Control*>* children = container_->getChildren();
			Rect childBounds;
			while ( children->hasMoreElements() ) {
				Control* child = children->nextElement();

				
				if ( child->getAlignment() != AlignNone && child->getVisible() ) {
					childBounds = child->getBounds();
					switch( child->getAlignment() ) {
						case AlignLeft : {
							innerBounds.left_ += childBounds.getWidth();
						}
						break;

						case AlignRight : {
							innerBounds.right_ -= childBounds.getWidth();
						}
						break;

						case AlignTop : {
							innerBounds.top_ += childBounds.getHeight();
						}
						break;

						case AlignBottom : {
							innerBounds.bottom_ -= childBounds.getHeight();
						}
						break;
					}
				}
			}
		}


		Point scrollPos;
		scrollPos.x_ = scrollable->getHorizontalPosition();
		scrollPos.y_ = scrollable->getVerticalPosition();

		//clip for border
		viewBounds.left_ = maxVal<>( innerBounds.left_+1,viewBounds.left_ );
		viewBounds.top_ = maxVal<>( innerBounds.top_+1,viewBounds.top_ );

		viewBounds.right_ = minVal<>( innerBounds.right_-1.0,viewBounds.right_ );
		viewBounds.bottom_ = minVal<>( innerBounds.bottom_-1.0,viewBounds.bottom_ );
		
		double dx = scrollable->getVirtualViewWidth() - innerBounds.getWidth();
		double dy = scrollable->getVirtualViewHeight() - innerBounds.getHeight();
		
		/* 
		* we need to do a comparison of virtualViewWidth and virtualViewHeight with bounds that have
		* accounted for the presence of scrollbars if they exist. We need to do this so that origin and
		* viewable bounds offset are calculated correctly so that when we are scrolled all the way to bottom
		* and right, the very bottom and right of bounds defined by virtualViewWidth and virtualViewHeight 
		* are visible. These 'adjusted' bounds are initially set to innerBounds dimensions, and then modified
		* if scrollbars present. (We alternatively could have increased virtualViewWidth/Height if scrollbars
		* were present, and compared these to actual innerBounds.)
		* NOTE: These adjusted values go hand-in-hand with adjustment to SCROLLINFO::nMax in scrollPeer when
		* both scrollbars present. 
		*/
		double scrollAdjustedWidth  = innerBounds.getWidth();
		double scrollAdjustedHeight = innerBounds.getHeight();	
		
		// can't use hasVerticalScrollbar here, we need to know if they are actually visible.
		bool isVertScrollbarVisible = scrollable->isVerticalScrollbarVisible();
		bool isHorzScrollbarVisible = scrollable->isHorizontalScrollbarVisible();		
		
		/*
		* since we are no longer adjusting virtualViewWidth and Height for presence of both scrollbars,
		* we need to tack on the extra to dx and dy here.
		*/
		if ( isHorzScrollbarVisible ) {
			dy += scrollable->getHorizontalScrollbarHeight();
			scrollAdjustedHeight -= scrollable->getHorizontalScrollbarHeight();
		}
		if ( isVertScrollbarVisible ) {
			dx += scrollable->getVerticalScrollbarWidth();
			scrollAdjustedWidth -= scrollable->getVerticalScrollbarWidth();
		}	
		
		/*
		Just a note: this assumes the scroll position units are same as GraphicsContext units (I think).
		When we implement a user-defined scroll increment, such as by the height of a line of text based
		on current Context, you may need a conversion here depending on how you implement that technique.
		*/
		origin.x_ -= scrollPos.x_;
		origin.y_ -= scrollPos.y_;

		//offset the viewBounds by the scrollable's offset
		viewBounds.offset( scrollPos.x_, scrollPos.y_ );

		if ( isHorzScrollbarVisible && ( scrollable->getVirtualViewWidth() > scrollAdjustedWidth ) ) {		
			Size horzSize =  UIToolkit::getUIMetricSize( UIMetricsManager::mtHorizontalSliderThumbSize );

			//viewBounds.bottom_ = minVal<>( viewBounds.bottom_-horzSize.height_,viewBounds.bottom_ );
						
			if ( dx < scrollPos.x_ ) {
			
				origin.x_ -= ( dx - scrollPos.x_ );

				viewBounds.offset( dx - scrollPos.x_, 0 );
			}
		}
		else {
			Size horzSize = UIToolkit::getUIMetricSize( UIMetricsManager::mtHorizontalSliderThumbSize );

			//viewBounds.bottom_ = minVal<>( viewBounds.bottom_-horzSize.height_,viewBounds.bottom_ );

			origin.x_ += scrollPos.x_;
			viewBounds.offset( scrollPos.x_, 0 );

			if ( scrollPos.x_ > 0 ) {
				scrollable->setHorizontalPosition( 0 );
			}
		}

		
		if ( isVertScrollbarVisible && ( scrollable->getVirtualViewHeight() > scrollAdjustedHeight ) ) {
			Size vertSize = UIToolkit::getUIMetricSize( UIMetricsManager::mtVerticalSliderThumbSize );

			//viewBounds.right_ = minVal<>( viewBounds.right_-vertSize.width_,viewBounds.right_ );

			if ( dy < scrollPos.y_ ) {
				origin.y_ -= ( dy - scrollPos.y_ );
				viewBounds.offset( 0, dy - scrollPos.y_ );
			}
		}
		else {
			Size vertSize = UIToolkit::getUIMetricSize( UIMetricsManager::mtVerticalSliderThumbSize );

			//viewBounds.right_ = minVal<>( viewBounds.right_-vertSize.width_,viewBounds.right_ );

			origin.y_ += scrollPos.y_;
			viewBounds.offset( 0, scrollPos.y_ );


			if ( scrollPos.y_ > 0 ) {
				scrollable->setVerticalPosition( 0 );
			}
		}


	}
}


bool Control::isActive()
{
	Frame* parentFrame = getParentFrame();
	//printf( "parentFrame: %p, Frame::getActiveFrame(): %p\n",
	//			parentFrame, Frame::getActiveFrame() );
	return (parentFrame == Frame::getActiveFrame()) && (parentFrame->isActiveFrame());
}

void Control::setViewModel( Model* viewModel )
{
	bool modelChanged = (viewModel != getViewModel()) ? true : false;
	
	/**
	NOTE!!
	Control's will assume that the model is added as a component
	some where along the line. Failure to do this will result in
	a memory leak. It will also result in the model NOT being written
	out for storage
	*/
	AbstractView::setViewModel( viewModel );
	
	if ( modelChanged ) {
		ControlEvent event( this, Control::CONTROL_MODELCHANGED );
		ControlModelChanged.fireEvent(&event);
	}
}

void Control::paintBorder( GraphicsContext * context )
{
	Border* border = getBorder();
	if ( NULL != border ) {
		border->paint( this, context );
	}
}

bool Control::hasChildren()
{
	bool result = false;
	Container* container = getContainer();
	if ( NULL != container ) {
		result = container->getChildCount() > 0;
	}
	return result;
}

bool Control::isChild()
{
	return getParent() != NULL;
}

void Control::internal_beforePaint( GraphicsContext* context )
{
	if ( getAllowPaintNotification() ) {
		ControlEvent e(this,Control::BEFORE_CONTROL_PAINTED,context);
		BeforeControlPainted.fireEvent( &e );
	}	
}

void Control::internal_afterPaint( GraphicsContext* context )
{
	if ( getAllowPaintNotification() ) {
		ControlEvent e(this,Control::AFTER_CONTROL_PAINTED,context);
		AfterControlPainted.fireEvent( &e );
	}
}

void Control::preChildPaint( GraphicsContext* graphicsContext, Control* child, Rect* childClipRect )
{
	peer_->preChildPaint( graphicsContext, child, childClipRect );
}

void Control::postChildPaint( GraphicsContext* graphicsContext, Control* child, Rect* oldClipRect )
{
	peer_->postChildPaint( graphicsContext, child, oldClipRect );
}


/**
$Id$
*/
