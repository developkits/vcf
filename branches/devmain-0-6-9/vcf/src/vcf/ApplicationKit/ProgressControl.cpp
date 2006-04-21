//ProgressControl.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/ProgressControl.h"
#include "vcf/ApplicationKit/EtchedBorder.h"
#include "vcf/GraphicsKit/DrawUIState.h"

using namespace VCF;


ProgressControl::ProgressControl():
	CustomControl(false),
	displayAlignment_(paHorizontal),
	minVal_(0.0),
	maxVal_(100.0),
	position_(0.0),
	displayProgressText_(false),
	useProgressFormatString_(false),
	stepItIncrement_(1.0)
{
	progressBarColor_ = new Color();

	*progressBarColor_ = *GraphicsToolkit::getSystemColor(SYSCOLOR_SELECTION);

	progressTextColor_ = new Color();

	*progressTextColor_ = *GraphicsToolkit::getSystemColor(SYSCOLOR_SELECTION_TEXT);
}

ProgressControl::~ProgressControl()
{
	progressBarColor_->free();
	progressTextColor_->free();
}


void ProgressControl::setDisplayAlignment( const ProgressAlignment& val )
{
	displayAlignment_ = val;
	repaint();
}

void ProgressControl::setMinValue( const double& val )
{
	minVal_ = val;
	repaint();
}

void ProgressControl::setMaxValue( const double& val )
{
	maxVal_ = val;
	repaint();
}

void ProgressControl::setPosition( const double& val )
{
	position_ = minVal<>( maxVal_, maxVal<>( minVal_, val ) );
	repaint();
}

void ProgressControl::setName( const String& name )
{
	CustomControl::setName( name );
	if ( isDesigning() ) {
		if ( progressFormatString_.empty() ) {
			setProgressFormatString( name + " %0.3f" );
		}
	}
}

void ProgressControl::setDisplayProgressText( const bool& val )
{
	displayProgressText_ = val;
	repaint();
}

void ProgressControl::setUseProgressFormatString( const bool& val )
{
	useProgressFormatString_ = val;
	repaint();
}

void ProgressControl::setProgressFormatString( const String& val )
{
	progressFormatString_ = val;
	repaint();
}

void ProgressControl::setStepItIncrement( const double& val )
{
	stepItIncrement_ = val;
	repaint();
}

void ProgressControl::setProgressBarColor( Color* val )
{
	if ( NULL == val ) {
		return;
	}

	*progressBarColor_ = *val;
	repaint();
}

void ProgressControl::setProgressTextColor( Color* val )
{
	if ( NULL == val ) {
		return;
	}

	*progressTextColor_ = *val;
	repaint();
}

void ProgressControl::stepIt()
{
	setPosition( position_ + stepItIncrement_ );
}

double ProgressControl::getPreferredWidth()
{
	if ( paVertical == displayAlignment_ ) {
		return UIToolkit::getUIMetricValue( UIMetricsManager::mtVerticalProgressWidth );
	}
	else {
		return 100;
	}

	return 0.0;
}

double ProgressControl::getPreferredHeight()
{
	if ( paVertical == displayAlignment_ ) {
		return 100;
	}
	else {
		return UIToolkit::getUIMetricValue( UIMetricsManager::mtHorizontalProgressHeight );
	}

	return 0.0;
}

void ProgressControl::paint( GraphicsContext* ctx )
{
	CustomControl::paint( ctx );

	Rect clientBounds = getClientBounds();

	ProgressState state;
	state.min_ = minVal_;
	state.max_ = maxVal_;
	state.position_ = position_;
	state.setActive( isActive() );
	state.setEnabled( isEnabled() );
	state.setFocused( isFocused() && state.isActive() );
	if ( displayProgressText_ ) {
		String text;
		if ( useProgressFormatString_ && (!progressFormatString_.empty()) ) {
			text = StringUtils::format( Format(progressFormatString_) % position_ );
		}
		else {
			text = StringUtils::format( Format("%0.1f %%") % position_ );
		}
		state.progressCaption_ = text; 
	}
	state.setVertical( (paVertical == displayAlignment_) ? true : false );
	ctx->drawThemeProgress( &clientBounds, 	state );
}


/**
*CVS Log info
*$Log$
*Revision 1.4.2.3  2006/03/26 15:07:48  ddiego
*removed unneccessary border from constructor of progress control.
*
*Revision 1.4.2.2  2006/03/01 04:34:56  ddiego
*fixed tab display to use themes api.
*
*Revision 1.4.2.1  2006/02/21 04:32:51  ddiego
*comitting moer changes to theme code, progress bars, sliders and tab pages.
*
*Revision 1.4  2005/07/09 23:14:55  ddiego
*merging in changes from devmain-0-6-7 branch.
*
*Revision 1.3.2.4  2005/04/17 06:09:07  iamfraggle
*Fixed old-style format call
*
*Revision 1.3.2.3  2005/03/06 22:50:59  ddiego
*overhaul of RTTI macros. this includes changes to various examples to accommadate the new changes.
*
*Revision 1.3.2.2  2005/02/28 04:51:56  ddiego
*fixed issue in handling componenent state and events when in design mode
*
*Revision 1.3.2.1  2005/02/27 01:45:33  ddiego
*fixed bug in testing whether a path should be loaded as a bundle.
*added some additional rtti info for certain classes in app kit.
*
*Revision 1.3  2004/12/01 04:31:38  ddiego
*merged over devmain-0-6-6 code. Marcello did a kick ass job
*of fixing a nasty bug (1074768VCF application slows down modal dialogs.)
*that he found. Many, many thanks for this Marcello.
*
*Revision 1.2.2.1  2004/11/15 05:41:28  ddiego
*finished almost all the osx menu code except for custom drawing. This completes this releases osx effort.
*
*Revision 1.2  2004/08/07 02:49:09  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.3  2004/06/30 19:19:29  ddiego
*fixed some font issues. got rid of methods that are not implementable on other platforms
*
*Revision 1.1.2.2  2004/04/29 03:43:14  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:19  ddiego
*migration towards new directory structure
*
*Revision 1.2  2003/12/18 05:16:00  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.1.2.1  2003/10/25 04:30:20  ddiego
*added two more examples one for progress bars and one for slider controls.
*also added two more control classes to the ApplicationKit, a SliderControl
*and a ProgressControl.
*
*/

