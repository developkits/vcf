//ListBoxControl.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


/* Generated by Together */

#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/ListBoxControl.h"
#include "vcf/ApplicationKit/DefaultListModel.h"
#include "vcf/GraphicsKit/DrawUIState.h"


using namespace VCF;


static ListItem* previouslySelectedListItem = NULL;


ListBoxControl::ListBoxControl():
	listModel_(NULL),
	currentMaxWidth_(0),
	currentMaxHeight_(0),
	leftGutter_(2),
	rightGutter_(2),
	textBounded_(false),
	imageList_(NULL),
	stateImageList_(NULL),
	stateItemIndent_(19)
{
	setListModel( new DefaultListModel() );

	addComponent( getViewModel() );

	init();
}


ListBoxControl::ListBoxControl( ListModel* listModel ):
	listModel_(NULL),
	currentMaxWidth_(0),
	currentMaxHeight_(0),
	leftGutter_(2),
	rightGutter_(2),
	textBounded_(false),
	imageList_(NULL),
	stateImageList_(NULL),
	stateItemIndent_(19)
{
	setListModel( listModel );
	init();
}

void ListBoxControl::init()
{
	allowsMultiSelect_ = false;

	allowsExtendedSelect_ = true;

	setColor( GraphicsToolkit::getSystemColor( SYSCOLOR_WINDOW ) );

	GraphicsContext* context = getContext();
	
	defaultItemHeight_ = UIToolkit::getUIMetricValue( UIMetricsManager::mtListItemHeight );

	EventHandler* lmh = (EventHandler*)
		new ClassProcedure1<ListModelEvent*,ListBoxControl>( this, &ListBoxControl::onItemAdded, "ListBoxControl::onItemAdded" );

	listModel_->ItemAdded += lmh;

	lmh = (EventHandler*)
		new ClassProcedure1<ListModelEvent*,ListBoxControl>( this, &ListBoxControl::onItemDeleted, "ListBoxControl::onItemDeleted" );

	listModel_->ItemRemoved += lmh;

	lmh = (EventHandler*)
		new ClassProcedure1<ListModelEvent*,ListBoxControl>( this, &ListBoxControl::onListModelContentsChanged, "ListBoxControl::onListModelContentsChanged" );

	listModel_->ContentsChanged += lmh;

	setUseColorForBackground( true );
}

ListBoxControl::~ListBoxControl()
{
	
}

void ListBoxControl::destroy()
{
	if ( NULL != listModel_ ) {
		EventHandler* ev = (EventHandler*)getCallback( "ListBoxControl::onItemAdded" );
		if ( NULL != ev ) {
			listModel_->ItemAdded -= ev;
		}

		ev = (EventHandler*)getCallback( "ListBoxControl::onItemDeleted" );
		if ( NULL != ev ) {
			listModel_->ItemRemoved -= ev;
		}

		ev = (EventHandler*)getCallback( "ListBoxControl::onListModelContentsChanged" );
		if ( NULL != ev ) {
			listModel_->ContentsChanged -= ev;
		}
	}

	items_.clear();

	CustomControl::destroy();
}

ListModel* ListBoxControl::getListModel()
{
	return listModel_;
}

void ListBoxControl::setListModel( ListModel * model )
{
	if ( model == listModel_ ) {
		return;
	}

	if ( NULL != listModel_ ) {
		EventHandler* ev = (EventHandler*)getCallback( "ListBoxControl::onItemAdded" );
		if ( NULL != ev ) {
			listModel_->ItemAdded -= ev;
		}

		ev = (EventHandler*)getCallback( "ListBoxControl::onItemDeleted" );
		if ( NULL != ev ) {
			listModel_->ItemRemoved -= ev;
		}

		ev = (EventHandler*)getCallback( "ListBoxControl::onListModelContentsChanged" );
		if ( NULL != ev ) {
			listModel_->ContentsChanged -= ev;
		}		
	}

	listModel_ = model;


	if ( NULL != listModel_ ) {
		
	}

	setViewModel( listModel_ );

	repaint();
}

void ListBoxControl::onListModelContentsChanged( ListModelEvent* event )
{
	repaint();



	if ( NULL != event ){
		switch ( event->getType() ){
			case lmeContentsDeleted: {
				selectedItems_.clear();
			}
			break;
/*
			case LIST_MODEL_ITEM_CHANGED: {
				ListItem* item = event->getListItem();
				if ( NULL != item ){
					listBoxPeer_->removeItem( item );
					listBoxPeer_->addItem( item );
				}
			}
			break;
			*/
		}

	}
}

ListItem* ListBoxControl::getListItem( const uint32& index )
{
	ListItem* result = NULL;

	if ( index < items_.size() ) {
		result = items_[index];
	}

	return result;
}

void ListBoxControl::setListItem( const uint32& index, ListItem* item )
{
	if ( index < items_.size() ) {
		items_[index]->free();
		items_[index] = item;
		repaint();
	}
}

void ListBoxControl::onItemAdded( ListModelEvent* event )
{

	ListItem* item = new ListItem();
	addComponent( item );
	items_.push_back( item );
	item->setModel( getViewModel() );
	item->setControl( this );
	item->setIndex( items_.size() - 1 );


	//event->item 


	Scrollable* scrollable = getScrollable();


	double width = getWidth();

	Rect itemRect;

	double scrollW = 0;

	if ( NULL != scrollable ) {
		if ( scrollable->getVirtualViewHeight() > getHeight() ) {
			scrollW = scrollable->getVerticalScrollbarWidth();
		}
	}

	GraphicsContext* ctx = getContext();

	itemRect.setRect( 0, currentMaxHeight_, width-scrollW, currentMaxHeight_ + defaultItemHeight_ );
	item->setBounds( itemRect );	

	currentMaxHeight_ += item->getBounds().getHeight();

	double imageWidth = 0.0;

	if ( NULL != imageList_ ) {
		imageWidth += imageList_->getImageWidth();
	}

	currentMaxWidth_ = maxVal<double>( ctx->getTextWidth( item->getCaption() ) + leftGutter_ + rightGutter_ + imageWidth, currentMaxWidth_ ); 

	if ( NULL != scrollable ) {
		if ( (getHeight() > currentMaxHeight_) && (scrollable->getVerticalPosition() > 0.0) ) {
			scrollable->setVerticalPosition( 0.0 );
		}

		if ( (getWidth() > currentMaxWidth_) && (scrollable->getHorizontalPosition() > 0.0) ) {
			scrollable->setHorizontalPosition( 0.0 );
		}

		scrollable->setVirtualViewSize( currentMaxWidth_, currentMaxHeight_ );
	}

	repaint();
}

void ListBoxControl::onItemDeleted( ListModelEvent* event )
{

	Array<ListItem*>::iterator found = items_.begin() + event->index ;
	if ( found != items_.end() ) {
		ListItem* item = *found;

		
		if ( item->isSelected() ) {
			eraseFromSelectedItems( item );
		}
		
		currentMaxHeight_ -= item->getBounds().getHeight();
		//need to recalc currentMaxWidth_ here also if item removed was the widest item. DT
		
		Scrollable* scrollable = getScrollable();
		
		if ( NULL != scrollable ) {
			if ( (getHeight() > currentMaxHeight_) && (scrollable->getVerticalPosition() > 0.0) ) {
				scrollable->setVerticalPosition( 0.0 );
			}
			
			if ( (getWidth() > currentMaxWidth_) && (scrollable->getHorizontalPosition() > 0.0) ) {
				scrollable->setHorizontalPosition( 0.0 );
			}		
			
			scrollable->setVirtualViewSize( currentMaxWidth_, currentMaxHeight_ );	
		}
		
		items_.erase( found );
	}
	repaint();
}

void ListBoxControl::rangeSelect( const bool & isSelected, ListItem * first, ListItem * last )
{
	if ( NULL == last ) {
		setSelectedItem( first ); //this will select it
		if ( !isSelected ) { // we'll deselect it
			first->setSelected( isSelected );
			setSelectedItem( NULL );
		}
		repaint();
	}
	else {
		ListItem* selected = getSelectedItem();
		if ( NULL != selected ) {
			selected->setSelected( false );
		}

		selectedItems_.clear();

		ListModel* lm = getListModel();
		if ( NULL != lm ) {
			uint32 start = first->getIndex();
			uint32 end = last->getIndex();
			for ( uint32 i=start;i<=end;i++) {
				ListItem* item =  items_[i];
				item->setSelected( isSelected );
				if ( true == isSelected ) {
					selectedItems_.push_back( item );
				}
			}

			selectionChanged( NULL );
		}
	}
}


Rect ListBoxControl::getStateRect( ListItem* item )
{
	Rect result;

	if ( item->getState() != Item::idsNone ) {
		Rect itemBounds = item->getBounds();

		result = itemBounds;

		result.left_ += leftGutter_;
		if ( NULL != stateImageList_ ) {
			result.right_ = result.left_ + stateImageList_->getImageWidth();
			result.top_ = itemBounds.top_ + (itemBounds.getHeight()/2.0 - stateImageList_->getImageHeight()/2.0);
			result.bottom_ = result.top_ + stateImageList_->getImageHeight();
		}
		else {
			result.right_ = result.left_ + stateItemIndent_;
			result.top_ = itemBounds.top_ + (itemBounds.getHeight()/2.0 - (minVal<double>(stateItemIndent_,defaultItemHeight_)/2.0));
			result.bottom_ = result.top_ + (minVal<double>(stateItemIndent_,defaultItemHeight_));
		}

		if ( NULL != imageList_ ) {
			if ( NULL != stateImageList_ ) {
				result.offset( (maxVal<double>( imageList_->getImageWidth(), stateImageList_->getImageWidth() )), 0 );
			}
			else {
				result.offset( (maxVal<double>( imageList_->getImageWidth(), stateItemIndent_ )), 0 );
			}
		}
		else {
			if ( NULL != stateImageList_ ) {
				result.offset( (maxVal<double>( stateItemIndent_, stateImageList_->getImageWidth() )), 0 );
			}
			else {
				//result.offset( -stateItemIndent_, 0 );
			}
		}

	}
	return result;
}

void ListBoxControl::paintItemState( GraphicsContext* ctx, Rect& itemRect, ListItem* item )
{
	Rect stateRect = getStateRect( item );

	stateRect.inflate( -1, -1 );

	if ( NULL != stateImageList_ ) {
		itemRect.left_ = stateRect.right_;
		stateImageList_->draw( ctx, item->getStateImageIndex(), &stateRect );
	}
	else {		
		stateRect.inflate( -1, -1 );
		
		itemRect.left_ = stateRect.right_;

		int32 state = item->getState();

		ButtonState buttonState;
		buttonState.setActive( true );

		if ( (state & Item::idsChecked) || 
				(state & Item::idsUnChecked) ) {

			buttonState.setPressed( state == Item::idsChecked ? true : false );
			ctx->drawThemeCheckboxRect( &stateRect, buttonState );
		}
		else if ( (state & Item::idsRadioPressed) || 
				(state & Item::idsRadioUnpressed) ) {

			buttonState.setPressed( state == Item::idsRadioPressed ? true : false );
			ctx->drawThemeRadioButtonRect( &stateRect, buttonState );
		}		
	}
}

void ListBoxControl::paintItemImage( GraphicsContext* ctx, Rect& itemRect, ListItem* item )
{
	Rect imageRect;
	imageRect.left_ += leftGutter_;
	imageRect.right_ = imageRect.left_ + imageList_->getImageWidth();

	uint32 index = item->getImageIndex();	

	imageRect.top_ = itemRect.top_ + (itemRect.getHeight()/2.0 - imageList_->getImageHeight()/2.0);
	imageRect.bottom_ = imageRect.top_ + imageList_->getImageHeight();
	
	if ( !imageRect.isEmpty() ) {

		imageRect.inflate( -1, -1 );

		itemRect.left_ = imageRect.right_;

		imageList_->draw( ctx, index, &imageRect );
	}
}

void ListBoxControl::paintItem( GraphicsContext* ctx, Rect& itemRect, 
							   double currentTop, Color* selectedTextColor, 
							   const Rect& bounds, double scrollWidth, double offsetX,
							   ListItem* item )
{
	double y = currentTop + ( (itemRect.getHeight()/2.0) - (ctx->getTextHeight( "EM" )/2.0) );
	if ( item->isSelected() ) {
		paintSelectionRect( ctx, &Rect(bounds.left_-leftGutter_, itemRect.top_, itemRect.right_, itemRect.bottom_), item );
		ctx->getCurrentFont()->setColor( selectedTextColor );
	}
	else {
		ctx->getCurrentFont()->setColor( getFont()->getColor() );
	}
	
	
	Rect tmp = itemRect;

	bool stateNeedsDrawing = false;
	bool imageNeedsDrawing = false;

	if ( NULL != imageList_ ) {
		imageNeedsDrawing = true;
	}

	if ( Item::idsNone != item->getState() ) {
		stateNeedsDrawing = true;
	}

	itemRect.setLeft( bounds.left_ );

	if ( imageNeedsDrawing ) {
		paintItemImage( ctx, itemRect, item );
	}

	if ( stateNeedsDrawing ) {
		paintItemState( ctx, itemRect, item );
	}	

	String itemText = item->getCaption();
	if ( getUseLocaleStrings() ) {
		itemText = System::getCurrentThreadLocale()->translate( itemText );
	}
	
	if( textBounded_ ){
		Rect itemPaintRect;
		itemPaintRect.setRect(itemRect.left_ + leftGutter_, currentTop, offsetX + bounds.getWidth() - scrollWidth - leftGutter_, currentTop + defaultItemHeight_ );
		ctx->textBoundedBy( &itemPaintRect, itemText, false );
	}
	else{
		ctx->textAt( itemRect.left_ + leftGutter_, y, itemText );
	}
	

	if ( item->canPaint() ) {
		item->paint( ctx, &tmp );
	}
}

void ListBoxControl::paint( GraphicsContext* ctx )
{
	CustomControl::paint( ctx );
	ListModel* lm = getListModel();

	Scrollable* scrollable = this->getScrollable();

	Rect bounds = getClientBounds();
	ctx->setClippingRect( &ctx->getViewableBounds() );

	double width = bounds.getWidth();
	double scrollW = 0;
	double offsetx = bounds.left_ + leftGutter_;
	if ( NULL != scrollable ) {
		if ( scrollable->getVirtualViewHeight() > getHeight() ) {
			scrollW = scrollable->getVerticalScrollbarWidth();
		}

		offsetx = maxVal<>( offsetx, offsetx + scrollable->getHorizontalPosition() );
	}

	if ( NULL != lm ) {
		Color* selectedTextColor =
				GraphicsToolkit::getSystemColor( SYSCOLOR_SELECTION_TEXT );
		
		double currentTop = bounds.top_;
		Rect itemRect;
		Rect itemPaintRect;
		double totalHeight = 0;


		Color oldFontColor;
		oldFontColor = *getFont()->getColor();



		Rect viewBounds = ctx->getViewableBounds();
		//Point origin = ctx->getOrigin();
		//viewBounds.offset( -origin.x_, -origin.y_ );
		Array<ListItem*>::iterator it = items_.begin();
		while ( it != items_.end() ) {
			ListItem* item = *it;
			itemRect.setRect( offsetx, currentTop, offsetx + (width-scrollW), currentTop + defaultItemHeight_ );

			if ( ((itemRect.top_ <= viewBounds.bottom_ ) && (itemRect.bottom_ >= viewBounds.top_ )) ||
					(itemRect.containsPt( &viewBounds.getTopLeft()) || (itemRect.containsPt( &viewBounds.getBottomRight() )) ) ) {
				totalHeight += itemRect.getHeight();			

				paintItem( ctx, itemRect, currentTop, selectedTextColor, 
							bounds, scrollW, offsetx, item );
				
			}

			currentTop += itemRect.getHeight();

			++it;
		}
	}
}

void ListBoxControl::paintSelectionRect( GraphicsContext* ctx, Rect* rect, ListItem* item )
{
	Color* selectedColor = GraphicsToolkit::getSystemColor( SYSCOLOR_SELECTION );
	Color* selectedTextColor = GraphicsToolkit::getSystemColor( SYSCOLOR_SELECTION_TEXT );
	Color* greyColor = GraphicsToolkit::getSystemColor( SYSCOLOR_SHADOW );
	ctx->setColor( selectedColor );
	ctx->rectangle( rect );
	ctx->fillPath();
	ctx->setColor( greyColor );
	ctx->strokePath();
	ctx->setColor( selectedTextColor );
}

ListItem* ListBoxControl::findSingleSelectedItem( Point* pt )
{
	ListItem* result = NULL;

	Point tmpPt = *pt;
	tmpPt.x_ = 1;

	ListModel* lm = getListModel();
	if ( NULL != lm ) {
		double currentTop = 0.0;
		Rect itemRect;
		Array<ListItem*>::iterator it = items_.begin();
		while ( it != items_.end() ) {
			ListItem* item = *it;
			itemRect.setRect( 0, currentTop, getWidth(), currentTop + defaultItemHeight_ );
			if ( true == itemRect.containsPt( &tmpPt ) ) {
				result = item;
				break;
			}
			currentTop += itemRect.getHeight();
			++it;
		}
	}

	return result;
}

bool ListBoxControl::stateHitTest( Point* point, ListItem* item )
{
	bool result = false;

	Rect stateRect = getStateRect( item );

	result = stateRect.containsPt( point );

	return result;
}

void ListBoxControl::mouseDown( MouseEvent* event )
{
	CustomControl::mouseDown( event );
	if ( (true == event->hasLeftButton()) && (Component::csNormal == getComponentState()) ) {
		keepMouseEvents();

		ListItem* foundItem = findSingleSelectedItem( event->getPoint() );
		if ( NULL != foundItem ) {
			//check state hit test first!
			if ( foundItem->getState() != Item::idsNone ) {
				if ( stateHitTest( event->getPoint(), foundItem ) ) {
					int32 state = foundItem->getState();
					if ( Item::idsChecked == state ) {
						state = Item::idsUnChecked;
					}
					else if ( Item::idsUnChecked == state ) {
						state = Item::idsChecked;
					}
					else if ( Item::idsRadioUnpressed == state ) {
						state = Item::idsRadioPressed;
					}
					else if ( Item::idsRadioPressed == state ) {
						state = Item::idsRadioUnpressed;
					}
					foundItem->setState( state );

					ItemEvent event( foundItem, ListBoxControl::lbeItemStateChangeRequested );
					ItemStateChangeRequested( &event );

					repaint();

					return;
				}
			}


			if ( true == allowsMultiSelect_ && true == allowsExtendedSelect_ ) {
				if( event->hasShiftKey() ){
					if( foundItem == getSelectedItem() ){
						for( uint32 j=0;j<selectedItems_.size();j++ ){
							selectedItems_[j]->setSelected(false);
						}
						selectedItems_.clear();
						selectedItems_.push_back(foundItem);
						foundItem->setSelected(true);
						selectionChanged( foundItem );
					}
					else if( NULL != getSelectedItem() ){
						ListItem* selected = getSelectedItem();
						uint32 foundItemPos = foundItem->getIndex();
						uint32 singlePos = selected->getIndex();
						for( uint32 j=0;j<selectedItems_.size();j++ ){
							selectedItems_[j]->setSelected(false);
						}
						if(foundItemPos < singlePos){// rangeSelect clears selectedItems_
							rangeSelect(true, foundItem, selected );
						}
						else{
							rangeSelect(true, selected, foundItem );
						}
						selectionChanged( foundItem );
					}
					else{// needed if no items currently selected.
						foundItem->setSelected(true);
						selectedItems_.push_back(foundItem);
						selectionChanged( foundItem );
					}
				}
				else if( event->hasControlKey() ){
					if( foundItem->isSelected() ){
						foundItem->setSelected(false);
						eraseFromSelectedItems( foundItem );
						selectionChanged( foundItem );
					}
					else{
						foundItem->setSelected(true);
						selectedItems_.push_back(foundItem);
						selectionChanged( foundItem );
					}
				}
				else{

					if( !(selectedItems_.empty()) ) {

						for( uint32 j=0;j<selectedItems_.size();j++ ){
							selectedItems_[j]->setSelected(false);
						}

						selectedItems_.clear();
					}					
					
					selectedItems_.push_back(foundItem);
					foundItem->setSelected( true );
					selectionChanged( foundItem );
				}
			}
			else if ( true == allowsMultiSelect_ && false == allowsExtendedSelect_ ){
				if( foundItem->isSelected() ){
					foundItem->setSelected(false);
					eraseFromSelectedItems( foundItem );
					selectionChanged( foundItem );
				}
				else{
					foundItem->setSelected( true );
					selectedItems_.push_back( foundItem );
					selectionChanged( foundItem );
				}
			}
			else {
				//selectedItems_.clear();
				//selectedItems_.push_back( foundItem );
				setSelectedItem( foundItem );
			}
		}
	}
}

void ListBoxControl::mouseMove( MouseEvent* event )
{
	CustomControl::mouseMove( event );
	if ( (true == event->hasLeftButton()) && (Component::csNormal == getComponentState()) ) {
		ListItem* foundItem = findSingleSelectedItem( event->getPoint() );
		if ( NULL != foundItem ) {
			if ( true == allowsMultiSelect_ ) {
				//if ( true == foundItem->isSelected() ){
				//	singleSelectedItem_ = foundItem; 
			//	/}
			//	else {
					//selectedItems_.push_back( foundItem );
					setSelectedItem( foundItem );
			//	}
			}
			else {
				if ( foundItem != getSelectedItem() )  {	   
					//JC - Integrated change by Berkano (Thanks Brian!) - fixes bug [1015368] ListBoxControl Mousemove error 
					//if(!selectedItems_.empty()) {
					// selectedItems_[0] = foundItem;//assumes index 0 exists
					setSelectedItem( foundItem );
					//}
				}
			}
		}
	}
}

void ListBoxControl::mouseUp( MouseEvent* event )
{
	CustomControl::mouseUp( event );
	if ( (true == event->hasLeftButton()) && (Component::csNormal == getComponentState()) ) {
		releaseMouseEvents();
	}
}

void ListBoxControl::mouseClick( MouseEvent* event )
{
	CustomControl::mouseClick( event );
}

void ListBoxControl::mouseDblClick( MouseEvent* event )
{
	CustomControl::mouseDblClick( event );
}

void ListBoxControl::keyDown( KeyboardEvent* event )
{
	CustomControl::keyDown( event );

	if ( Component::csNormal == getComponentState() ) {
		if ( true == this->allowsMultiSelect_ ) {

		}
		else {
			ListItem* item = this->getSelectedItem();
			ListModel* lm = getListModel();
			if ( (NULL != item) && (NULL != lm) ) {
				uint32 index = item->getIndex();

				if ( vkUpArrow == event->getVirtualCode() ) {
					index --;

				}
				else if ( vkDownArrow == event->getVirtualCode() ) {
					index ++;
				}
				Array<ListItem*>::iterator found = 
					items_.begin() + index;
				if ( found != items_.end() ) {
					setSelectedItem( *found );
				}
			}
		}
	}
}

void ListBoxControl::keyUp( KeyboardEvent* event )
{
	CustomControl::keyUp( event );
}

void ListBoxControl::keyPressed( KeyboardEvent* event )
{
	CustomControl::keyPressed( event );
}

void ListBoxControl::setDefaultItemHeight( const double& defaultItemHeight )
{
	defaultItemHeight_ = defaultItemHeight;
	repaint();
}

Enumerator<ListItem*>* ListBoxControl::getSelectedItems()
{
	return selectedItems_.getEnumerator();
}

void ListBoxControl::setAllowsMultiSelect( const bool& allowsMultiSelect )
{
	allowsMultiSelect_ = allowsMultiSelect;
}

void ListBoxControl::selectionChanged( ListItem* item )
{
	ItemEvent event( item, ITEM_EVENT_SELECTED );
	SelectionChanged( &event );
}

void ListBoxControl::setSelectedItem( ListItem* selectedItem )
{
	if ( true != allowsMultiSelect_ ) {

		ListItem* oldSelected = getSelectedItem();

		if( NULL != oldSelected ) {
			oldSelected->setSelected( false );		
		}
		
		if( !selectedItems_.empty() ) {

			for( uint32 j=0;j<selectedItems_.size();j++ ){
				selectedItems_[j]->setSelected(false);
			}

			selectedItems_.clear();
		}
	}

	if ( NULL != selectedItem ) {
		if ( selectedItems_.empty() ) {
			selectedItems_.push_back( selectedItem );		
		}
		else {
			if ( selectedItems_[0] != selectedItem ) {
				Array<ListItem*>::iterator found = 
					std::find( selectedItems_.begin(), selectedItems_.end(), selectedItem );
				if ( found != selectedItems_.end() ) {
					selectedItems_.erase( found );					
				}
				selectedItems_.insert( selectedItems_.begin(), selectedItem );				
			}
		}
	}


	if ( NULL != selectedItem ) {
		if( !(selectedItem->isSelected()) ) { //If already selected, it is already in selectedItems_!!			
			selectedItem->setSelected( true );
			selectionChanged( selectedItem );
		}
	}

	repaint();
}

void ListBoxControl::deselectAllItems()
{
	for( uint32 j=0;j<selectedItems_.size();j++ ){
		selectedItems_[j]->setSelected(false);
	}

	selectedItems_.clear();

	repaint();
}

void ListBoxControl::eraseFromSelectedItems(ListItem* item)
{
	std::vector<ListItem*>::iterator foundItemPos;
	foundItemPos = std::find( selectedItems_.begin(), selectedItems_.end(), item );
	
	if(foundItemPos != selectedItems_.end() ) selectedItems_.erase( foundItemPos );
}

void ListBoxControl::setAllowsExtendedSelect (const bool& allowsExtendedSelect)
{
	if ( true == allowsExtendedSelect ) {
		allowsMultiSelect_ = true;
	}

	allowsExtendedSelect_ = allowsExtendedSelect;
}

void ListBoxControl::setLeftGutter(const double& leftgutter)
{
	leftGutter_ = leftgutter;
	repaint();
}

void ListBoxControl::setRightGutter(const double& rightgutter)
{
	rightGutter_ = rightgutter;
	repaint();
}

void ListBoxControl::setTextBounded( const bool& istextbounded ){
	textBounded_ = istextbounded;
}

void ListBoxControl::setScrollable( Scrollable* scrollable )
{
	Control ::setScrollable( scrollable );

	scrollable_->setVirtualViewVertStep( getDefaultItemHeight() );
	scrollable_->setDiscreteScroll( false, true );
}

void ListBoxControl::setImageList( ImageList* imageList )
{
	imageList_ = imageList;
	repaint();
}

void ListBoxControl::setStateImageList( ImageList* stateImageList )
{
	stateImageList_ = stateImageList;
	repaint();
}

ListItem* ListBoxControl::getSelectedItem()
{	
	ListItem* result = NULL;
	if ( !selectedItems_.empty() ) {
		result = selectedItems_[0];
	}
	return result;
}

/**
$Id$
*/
