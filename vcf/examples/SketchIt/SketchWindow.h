#ifndef _VCF_SKETCHWINDOW_H__
#define _VCF_SKETCHWINDOW_H__
//SketchWindow.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


#define SKETCHWINDOW_CLASSID		"e035e1bd-f375-4b97-9736-710c17e672c8"

/**
class SketchWindow documentation
*/
class SketchWindow : public VCF::Window {
public:
	BEGIN_CLASSINFO( SketchWindow, "SketchWindow", "VCF::Window", SKETCHWINDOW_CLASSID )
	END_CLASSINFO( SketchWindow )


	SketchWindow();
	virtual ~SketchWindow();


protected:

	void onDocInitialized( VCF::Event* e ) ;
};


/**
*CVS Log info
*$Log$
*Revision 1.2.6.3  2004/04/29 03:40:56  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.2  2003/08/09 02:56:42  ddiego
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
*Revision 1.1.2.1  2003/07/17 03:02:46  ddiego
*added sketch example
*
*/


#endif // _VCF_SKETCHWINDOW_H__


