//TreeControl.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


/* Generated by Together */
#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/TreeControl.h"
#include "vcf/ApplicationKit/TreePeer.h"
#include "vcf/ApplicationKit/DefaultTreeModel.h"
#include "vcf/ApplicationKit/DefaultTreeItem.h"


using namespace VCF;

TreeControl::TreeControl():
	imageList_(NULL),
	stateImageList_(NULL),
	treeModel_(NULL),
	currentSelectedItem_(NULL)
{
	treePeer_ = UIToolkit::createTreePeer( this );
	peer_ = dynamic_cast<ControlPeer*>( treePeer_ );

	if ( NULL == treePeer_ ){
		throw InvalidPeer(MAKE_ERROR_MSG(NO_PEER), __LINE__);
	};

	peer_->create( this );

	init();
}

TreeControl::~TreeControl()
{
	if ( NULL != treeModel_ ) {
		treeModel_->release();
	}
	treeModel_ = NULL;
}

void TreeControl::init()
{

	TreeModelEventHandler<TreeControl>* tmh =
		new TreeModelEventHandler<TreeControl>( this,
												&TreeControl::onTreeRootNodeChanged,
												"TreeControl::onTreeRootNodeChanged" );



	tmh = new TreeModelEventHandler<TreeControl>( this,
													&TreeControl::onTreeNodeAdded,
													"TreeControl::onTreeNodeAdded" );



	ModelEventHandler<TreeControl>* mh = new ModelEventHandler<TreeControl>( this, &TreeControl::onModelEmptied, "ModelHandler" );


	setTreeModel( new DefaultTreeModel() );

	setBorder( new Basic3DBorder( true ) );
}

void TreeControl::setTreeModel( TreeModel * model )
{
	if ( NULL != treeModel_ ){

		EventHandler* tml = getEventHandler( "TreeControl::onTreeRootNodeChanged" );
		treeModel_->removeTreeRootNodeChangedHandler ( tml );

		tml = getEventHandler( "TreeControl::onTreeNodeAdded" );
		treeModel_->removeTreeNodeAddedHandler( tml );

		EventHandler* ml = getEventHandler( "ModelHandler" );
		treeModel_->removeModelHandler( ml );

		treeModel_->release();
	}

	treeModel_ = model;

	if ( NULL != treeModel_ ) {
		treeModel_->addRef();

		EventHandler* tml = getEventHandler( "TreeControl::onTreeRootNodeChanged" );
		treeModel_->addTreeRootNodeChangedHandler ( tml );

		tml = getEventHandler( "TreeControl::onTreeNodeAdded" );
		treeModel_->addTreeNodeAddedHandler( tml );

		EventHandler* ml = getEventHandler( "ModelHandler" );
		treeModel_->addModelHandler( ml );
	}

	setViewModel( treeModel_ );
}

TreeModel* TreeControl::getTreeModel()
{
	return treeModel_;
}

ImageList* TreeControl::getImageList()
{
	return imageList_;
}

void TreeControl::setImageList( ImageList* imageList )
{
	imageList_ = imageList;
	treePeer_->setImageList( imageList );
}

void TreeControl::paint( GraphicsContext * context )
{
	Rect innerBounds = getClientBounds( false );

	Border* border = getBorder();
	if ( NULL != border ){
		border->paint( this, context );
		innerBounds = border->getClientRect( &innerBounds, this );
	}

	context->setColor( getColor() );
			
	context->rectangle( &innerBounds );
			
	context->fillPath();
}

void TreeControl::onTreeRootNodeChanged( TreeModelEvent* event )
{
	TreeItem* item = event->getTreeItem();
	//treePeer_->addItem( item );
}

void TreeControl::onTreeNodeAdded( TreeModelEvent* event )
{
	TreeItem* item = event->getTreeItem();
	treePeer_->addItem( item );
	EventHandler* il = getEventHandler( "TreeItemListener" );
	if  ( il == NULL ) {
		il = new ItemEventHandler<TreeControl>( this, &TreeControl::onTreeItemPaint, "TreeItemListener" );
	}
	item->addItemPaintHandler( il );
}

void TreeControl::onTreeNodeDeleted( TreeModelEvent* event )
{

}

void TreeControl::onModelEmptied( ModelEvent* event )
{
	treePeer_->clear();
	currentSelectedItem_ = NULL;
}

TreeItem* TreeControl::hitTestForItem( Point* pt, TreeItem* item )
{
	TreeItem* result = NULL;
	if ( true == item->containsPoint( pt ) ) {
		result = item;
	}
	else if ( false == item->isLeaf() ){
		Enumerator<TreeItem*>* children = item->getChildren();
		while ( true == children->hasMoreElements() ) {
			TreeItem* childItem = children->nextElement();
			result = hitTestForItem( pt, childItem );
			if ( result != NULL ) {
				break;
			}
		}
	}

	return result;
}

TreeItem* TreeControl::findItem( Point* pt )
{
	TreeItem* result = NULL;

	Enumerator<TreeItem*>* rootItems = treeModel_->getRootItems();
	while ( true == rootItems->hasMoreElements() ) {
		TreeItem* item = rootItems->nextElement();
		result = hitTestForItem( pt, item );
		if ( NULL != result ) {
			break;
		}
	}

	return result;
}

double TreeControl::getItemIndent()
{
	return treePeer_->getItemIndent();
}

void TreeControl::setItemIndent( const double& indent )
{
	treePeer_->setItemIndent( indent );
}

void TreeControl::onTreeItemPaint( ItemEvent* event )
{
	return ;

	TreeItem* item = (TreeItem*)event->getSource();
	GraphicsContext* context = event->getContext();
	if ( NULL != context ) {
		ImageList* il = getImageList();
		if ( NULL != il ) {
			Rect imgRect = *(item->getBounds());
			imgRect.left_ += (item->getLevel()+1) * getItemIndent();
			imgRect.right_ = imgRect.left_ + il->getImageWidth();
			context->setColor( GraphicsToolkit::getSystemColor( SYSCOLOR_WINDOW ) );
			context->rectangle( &imgRect );
			context->fillPath();

			if ( stateImageList_ == NULL ) {
				il->draw( context, item->getImageIndex(), &imgRect );
			}
			else {
				if ( true == item->isSelected() ) {
					if ( true == item->isExpanded() ) {
						if ( item->getExpandedImageIndex() >= 0 ) {
							stateImageList_->draw( context, item->getExpandedImageIndex(), &imgRect );
						}
						else if ( item->getSelectedImageIndex() >= 0 ) {
							stateImageList_->draw( context, item->getSelectedImageIndex(), &imgRect );
						}
						else {
							il->draw( context, item->getImageIndex(), &imgRect );
						}
					}
					else {
						if ( item->getSelectedImageIndex() >= 0 ) {
							stateImageList_->draw( context, item->getSelectedImageIndex(), &imgRect );
						}
						else {
							il->draw( context, item->getImageIndex(), &imgRect );
						}
					}
				}
				else {
					if ( true == item->isExpanded() ) {
						if ( item->getExpandedImageIndex() >= 0 ) {
							stateImageList_->draw( context, item->getExpandedImageIndex(), &imgRect );
						}
						else {
							il->draw( context, item->getImageIndex(), &imgRect );
						}
					}
					else {
						il->draw( context, item->getImageIndex(), &imgRect );
					}
				}
			}
		}
	}
}

void TreeControl::setStateImageList( ImageList* imageList )
{
	stateImageList_ = imageList;
	treePeer_->setStateImageList( imageList );
}

ImageList* TreeControl::getStateImageList()
{
	return stateImageList_;
}

void TreeControl::addItem( TreeItem* parent, TreeItem* item )
{
	treeModel_->addNodeItem( item, parent );
}

TreeItem* TreeControl::addItem( TreeItem* parent, const String& caption, const ulong32 imageIndex )
{
	TreeItem* result = NULL;
	result = new DefaultTreeItem( caption, this, treeModel_ );
	result->setImageIndex( imageIndex );
	treeModel_->addNodeItem( result, parent );
	return result;
}

void TreeControl::removeItem( TreeItem* item )
{
	treeModel_->deleteNodeItem( item );
}

TreeItem* TreeControl::getSelectedItem()
{
	return currentSelectedItem_;
}

Rect TreeControl::getItemImageRect( TreeItem* item )
{
	return treePeer_->getItemImageRect( item );
}

void TreeControl::handleEvent( Event* event )
{
	Control::handleEvent( event );
	switch ( event->getType() ) {
		case TREEITEM_SELECTED : {
			currentSelectedItem_ = (TreeItem*)event->getUserData();

			ItemSelected.fireEvent( (ItemEvent*)event );
		}
		break;

		case TREEITEM_EXPANDED : {
			ItemExpanded.fireEvent( (ItemEvent*)event );
		}
		break;

		case TreeControl::ITEM_STATECHANGE_REQUESTED : {
			ItemStateChangeRequested.fireEvent( (ItemEvent*)event );
		}
		break;

	}
}


/**
*CVS Log info
*$Log$
*Revision 1.1.2.3  2004/07/16 04:01:46  ddiego
*fixed the last of border redraw issues, I hope.
*
*Revision 1.1.2.2  2004/04/29 03:43:15  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:19  ddiego
*migration towards new directory structure
*
*Revision 1.25  2003/12/18 05:16:00  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.24.2.2  2003/09/22 01:48:04  ddiego
*some minor additions ot teh DropTarget to allow it to have multiple
*control targets
*also a few other misc fixes
*
*Revision 1.24.2.1  2003/09/15 02:52:36  ddiego
*miscellaneous changes
*
*Revision 1.24  2003/08/09 02:56:45  ddiego
*merge over from the devmain-0-6-1 branch
*Changes
*Features:
*-Added additional implementation to better support the MVC architecture in
*the VCF
*
*-Added a Document/View architecure that is similar to MFC's or NextSteps's
*Doc/View architectures
*
*-Integrated the Anti Grain Graphics library into the GraphicsKit. There is
*now basic support for it in terms of drawing vector shapes
*(fills and strokes). Image support will come in the next release
*
*-Added several documented graphics tutorials
*
*Bugfixes:
*
*[ 775744 ] wrong buttons on a dialog
*[ 585239 ] Painting weirdness in a modal dialog ?
*[ 585238 ] Modal dialog which makes a modal Dialog
*[ 509004 ] Opening a modal Dialog causes flicker
*[ 524878 ] onDropped not called for MLTcontrol
*
*Plus an issue with some focus and getting the right popup window to activate
*has also been fixed
*
*Revision 1.23.2.3  2003/08/01 00:49:26  ddiego
*added a fix for popup windows that also fixed a problem with the
*ComboBoxControl drop down list disapearing.
*
*Revision 1.23.2.2  2003/07/24 04:10:44  ddiego
*added fixes for the following tasks:
*Task #82279 ApplicationKit: add static methods to singleton objects
*Task #82277 FoundationKit: add static methods to singleton objects
*this required a bunch of changes in terms of getting rid of older style code
*
*Revision 1.23.2.1  2003/05/27 04:45:40  ddiego
*doing some code cleanup to fix some bugs in the way dialogs and popups
*work in Win32 port. Cleaned up the ControlPeer and DialogPeer interfaces
*a bit.
*
*Revision 1.23  2003/05/17 20:37:50  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.22.2.2  2003/04/17 04:29:53  ddiego
*updated scintilla, added gtk support for the application kit, with stubs
*for the basic peers.
*
*Revision 1.22.2.1  2003/03/12 03:13:26  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.22  2003/02/26 04:30:57  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.21.2.5  2003/02/24 05:42:19  ddiego
*moved the code for the VariantData calss into it's own header
*migrated to the new event style using Delegates instead of relying on
*the ugly macros that we were using before - same functionality though
*made sure everything still works and compiles, including neccessary
*changes in the VCF Builder so that it creates code in the new style
*This changes to teh new style of using Delegates completes task 58837
*
*Revision 1.21.2.4  2003/01/08 00:20:01  marcelloptr
*mispellings and newlines at the end of all source files
*
*Revision 1.21.2.3  2002/12/28 01:56:22  marcelloptr
*Improved macros for automatic import/export of libraries. - Warning fixes. - Other Minor Changes.
*
*Revision 1.21.2.2  2002/12/27 23:05:10  marcelloptr
*Improved macros for automatic import/export of libraries. - Warning fixes. - Other Minor Changes.
*
*Revision 1.21.2.1  2002/12/25 22:06:36  ddiego
*whole bunch of little changes to the header files used by the ApplicationKit
*to get rid of no newline warnings by gcc.
*fixes to various event handlers in the ApplicationKit to compile with gcc
*since gcc does not like a member function pointer without the "&"
*addressof operator.
*Added initial file for the X11 UIToolkit implementation
*
*Revision 1.21  2002/11/18 00:46:10  ddiego
*this is the merge over of all the bug fixes and features (mostly
*VCF Builder stuff) from the devmain-0-5-8 branch
*
*Revision 1.20.4.2  2002/11/17 04:48:03  ddiego
**** empty log message ***
*
*Revision 1.20.4.1  2002/11/16 20:31:35  ddiego
*this fixes the following bugs:
*632802 TextControl's setViewModel not called
*639416 ListViewControl's setViewModel not called
*639417 TreeControl's setViewModel not called
*639418 ListBoxControl's setViewModel not called
*639419 ComboBoxControl's setViewModel not called
*639420 TreeListControl's setViewModel not called
*basically just required that setViewModel() get called. Also made changes to
*the TableControl and teh TabbedPages control
*
*Revision 1.20  2002/09/12 03:26:06  ddiego
*merged over the changes from the devmain-0-5-5b branch
*
*Revision 1.19.6.1  2002/09/05 04:31:46  ddiego
*fixed hte bug in hte VCFInputStream, nested class instances will now
*read in correctly.
*Also added code to properly update the name manager whenever a new form is
*loaded up to prevent name collision within ina a form class. Fixed
*a bit of selection code in the TreeControl class as well.
*
*Revision 1.19  2002/05/09 03:10:46  ddiego
*merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
*Revision 1.18.4.2  2002/04/27 15:52:53  ddiego
*Changed a bunch of files include and made it a bit faster and added better include
*guards in the common headers
*
*Revision 1.18.4.1  2002/04/08 20:55:31  zzack
*changed include style
*
*Revision 1.18  2002/02/20 04:08:00  ddiego
*consolidated event handling code a bit
*introduced a virtual handleEvent() method to Component
*clean up some of the event constants, so they are now localized in the
*appropriate control classes.
*
*Revision 1.17  2002/01/24 01:46:50  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


