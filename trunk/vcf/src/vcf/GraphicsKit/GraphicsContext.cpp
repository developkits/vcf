//GraphicsContext.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/GraphicsKit/GraphicsKit.h"
#include "vcf/GraphicsKit/DrawUIState.h"

#include "thirdparty/common/agg/include/agg_ellipse.h"
#include "thirdparty/common/agg/include/agg_arc.h"


#include "thirdparty/common/agg/include/agg_renderer_scanline.h"
#include "thirdparty/common/agg/include/agg_span_allocator.h"
#include "thirdparty/common/agg/include/agg_span_interpolator_linear.h"
#include "thirdparty/common/agg/include/agg_span_image_filter_rgba.h"
#include "thirdparty/common/agg/include/agg_scanline_u.h"
#include "thirdparty/common/agg/include/agg_bounding_rect.h"
#include "thirdparty/common/agg/include/agg_span_converter.h"

namespace VCF {

/**
* The structure used to fully save and restore the state of a Graphics context.
*@see GraphicsContext::saveState()
*/
class GraphicsState {
public:

	GraphicsState();
	GraphicsState( const GraphicsState& rhs );
	~GraphicsState();

	GraphicsState& operator=( const GraphicsState& rhs );

	void compositeMatrix();

public:
	Fill* fill_;
	Stroke* stroke_;
	Path* clippingPath_;
	GraphicsContext* owningContext_;
	
	double strokeWidth_;
	
	Font font_;
	Matrix2D transformMatrix_;
	Point currentMoveTo_;
	Color color_;

	double rotation_;
	double shearX_;
	double shearY_;
	double translateX_;
	double translateY_;
	double scaleX_;
	double scaleY_;	
	GraphicsContext::CompositingMode compositeMode_;
	double alpha_;
};





GraphicsState::GraphicsState():
	fill_(NULL),
	stroke_(NULL),
	clippingPath_(NULL),
	owningContext_(NULL),
	strokeWidth_(1.0),
	rotation_(0.0),
	shearX_(0.0),
	shearY_(0.0),
	translateX_(0.0),
	translateY_(0.0),
	scaleX_(1.0),
	scaleY_(1.0),
	compositeMode_(GraphicsContext::cmSource),
	alpha_(1.0)
{
	transformMatrix_.identity();
}

GraphicsState::GraphicsState( const GraphicsState& rhs ):
	fill_(NULL),
	stroke_(NULL),
	clippingPath_(NULL),
	owningContext_(NULL),
	strokeWidth_(1.0),
	rotation_(0.0),
	shearX_(0.0),
	shearY_(0.0),
	translateX_(0.0),
	translateY_(0.0),
	scaleX_(1.0),
	scaleY_(1.0),
	compositeMode_(GraphicsContext::cmSource),
	alpha_(1.0)
{
	transformMatrix_.identity();
	*this = rhs;
}

GraphicsState::~GraphicsState()
{

	if ( NULL != clippingPath_ ) {
		Object* pathObj = dynamic_cast<Object*>( clippingPath_ );
		pathObj->release();
	}
	clippingPath_ = NULL;

}

GraphicsState& GraphicsState::operator=( const GraphicsState& rhs )
{
	strokeWidth_ = rhs.strokeWidth_;
	fill_ = rhs.fill_;
	stroke_ = rhs.stroke_;
	font_ = rhs.font_;
	compositeMode_ = rhs.compositeMode_;

	if ( NULL != clippingPath_ ) { //release the underlying object instance
		Object* pathObj = dynamic_cast<Object*>( clippingPath_ );
		pathObj->release();
	}

	clippingPath_ = rhs.clippingPath_;

	if ( NULL != clippingPath_ ) { //take ownership of the underlying object instance
		Object* pathObj = dynamic_cast<Object*>( clippingPath_ );
		pathObj->addRef();
	}


	owningContext_ = rhs.owningContext_;
	transformMatrix_ = rhs.transformMatrix_;
	currentMoveTo_ = rhs.currentMoveTo_;
	color_ = rhs.color_;

	rotation_ = rhs.rotation_;
	shearX_ = rhs.shearX_;
	shearY_ = rhs.shearY_;
	translateX_ = rhs.translateX_;
	translateY_ = rhs.translateY_;
	scaleX_ = rhs.scaleX_;
	scaleY_ = rhs.scaleY_;
	
	alpha_ = rhs.alpha_;

	return *this;
}

void GraphicsState::compositeMatrix()
{
	Matrix2D rotate;
	rotate.rotate( rotation_ );

	Matrix2D scale;
	scale.scale( scaleX_, scaleY_ );

	Matrix2D shear;
	shear.shear( shearX_, shearY_ );

	Matrix2D translation;
	translation.translate( translateX_, translateY_ );

	transformMatrix_.identity();
	transformMatrix_.multiply( scale );
	transformMatrix_.multiply( rotate );
	transformMatrix_.multiply( shear );
	transformMatrix_.multiply( translation );	
}



GraphicsContext::GraphicsContext():
	contextPeer_(NULL),
	currentDrawingState_(GraphicsContext::gsNone),		
	renderArea_(NULL),
	renderBuffer_(NULL),
	renderAreaDirty_(false),
	graphicsStateIndex_(0),
	currentGraphicsState_(NULL)
{
	GraphicsState* newState = new GraphicsState();
	newState->owningContext_ = this;
	stateCollection_.push_back( newState );
	currentGraphicsState_ = stateCollection_[graphicsStateIndex_];

	Font& font = currentGraphicsState_->font_;
	font.setGraphicsContext( this );

	init();
}

GraphicsContext::GraphicsContext( const unsigned long& width, const unsigned long& height ):
	contextPeer_(NULL),
	currentDrawingState_(GraphicsContext::gsNone),		
	renderArea_(NULL),
	renderBuffer_(NULL),
	renderAreaDirty_(false),
	graphicsStateIndex_(0),
	currentGraphicsState_(NULL)
{
	GraphicsState* newState = new GraphicsState();
	newState->owningContext_ = this;
	stateCollection_.push_back( newState );
	currentGraphicsState_ = stateCollection_[graphicsStateIndex_];

	contextPeer_ = GraphicsToolkit::createContextPeer( width, height );
	if ( NULL == contextPeer_ ){
		//throw exception
	}

	contextPeer_->setContext( this );
	init();
	Font& font = currentGraphicsState_->font_;
	font.setGraphicsContext( this );

	font.setPointSize( font.getPointSize() );

}

GraphicsContext::GraphicsContext( OSHandleID contextID ):	
	contextPeer_(NULL),
	currentDrawingState_(GraphicsContext::gsNone),
	renderArea_(NULL),
	renderBuffer_(NULL),
	renderAreaDirty_(false),
	graphicsStateIndex_(0),
	currentGraphicsState_(NULL)
{
	GraphicsState* newState = new GraphicsState();
	newState->owningContext_ = this;
	stateCollection_.push_back( newState );
	currentGraphicsState_ = stateCollection_[graphicsStateIndex_];

	contextPeer_ = GraphicsToolkit::createContextPeer( contextID );
	if ( NULL == contextPeer_ ){
		//throw exception
	}
	contextPeer_->setContext( this );
	init();
	Font& font = currentGraphicsState_->font_;
	font.setGraphicsContext( this );

	font.setPointSize( font.getPointSize() );
}

GraphicsContext::~GraphicsContext()
{
	if ( NULL != contextPeer_ ){
		delete contextPeer_;
	}
	contextPeer_ = NULL;


	GraphicsStateCollection::iterator it = stateCollection_.begin();
	while ( it != stateCollection_.end () ) {
		delete *it;
		it ++;
	}
	stateCollection_.clear();


	if ( NULL != renderArea_ ) {
		renderBuffer_->attach( NULL, 0, 0, 0 );
		delete renderBuffer_;

		delete renderArea_;
	}	
}

void GraphicsContext::init()
{
	//currentFont_ = new Font();
	//currentFont_->setGraphicsContext( this );
	//currentFont_->setPointSize( currentFont_->getPointSize() );

	//transformMatrix_.identity();
}

void GraphicsContext::setAlpha( const double& alpha )
{
	currentGraphicsState_->alpha_ = alpha;
}

double GraphicsContext::getAlpha()
{
	return currentGraphicsState_->alpha_;
}

void GraphicsContext::setCompositingMode( GraphicsContext::CompositingMode compositeMode )
{
	currentGraphicsState_->compositeMode_ = compositeMode;
}
	
GraphicsContext::CompositingMode GraphicsContext::getCompositingMode()
{
	return currentGraphicsState_->compositeMode_;
}

void GraphicsContext::setCurrentFont(Font * font)
{
	Font& currentFont = currentGraphicsState_->font_;
	currentFont = *font;
	currentFont.setGraphicsContext( this );
	currentFont.setPointSize( currentFont.getPointSize() );
}

void GraphicsContext::setCurrentFont( const Font * font )
{
	Font& currentFont = currentGraphicsState_->font_;
	currentFont = *font;
	currentFont.setGraphicsContext( this );
	currentFont.setPointSize( currentFont.getPointSize() );
}

void GraphicsContext::setCurrentFill(Fill * fill)
{
	if ( NULL != fill ){
		fill->setContext( this );
	}
	currentGraphicsState_->fill_ = fill;
}

void GraphicsContext::setCurrentStroke(Stroke * stroke)
{
	if ( NULL != stroke ){
		stroke->setContext( this );
	}
	currentGraphicsState_->stroke_ = stroke;
}

void GraphicsContext::draw(Path * path)
{
	Fill* fill = currentGraphicsState_->fill_;
	Stroke* stroke = currentGraphicsState_->stroke_;
	if ( NULL != fill ){
		fill->render( path );
	}
	if ( NULL != stroke ){
		stroke->render( path );
	}
}

void GraphicsContext::drawImage( const double& x, const double& y, Image * image, const bool& renderImmediately)
{
	Rect bounds(0,0,image->getWidth(), image->getHeight() );
	drawPartialImage( x, y, &bounds, image, renderImmediately );
}

void GraphicsContext::bitBlit( const double& x, const double& y, Image* image )
{
	Rect r(0,0,image->getWidth(),image->getHeight());
	bitBlit( x, y, &r, image );
}

void GraphicsContext::bitBlit( const double& x, const double& y, Rect* imageBounds, Image* image )
{
	if ( contextPeer_->prepareForDrawing( GraphicsContext::doImage ) ) {

		VCF_ASSERT( imageBounds->getWidth() <= image->getWidth() );
		VCF_ASSERT( imageBounds->getHeight() <= image->getHeight() );

		contextPeer_->bitBlit( x, y, imageBounds, image );

		contextPeer_->finishedDrawing( 	GraphicsContext::doImage );
	}
}

void GraphicsContext::drawImageWithState( const double& x, const double& y, Image * image, const bool& enabled, const bool& renderImmediately )
{
	drawImage( x, y, image, renderImmediately );
}

void GraphicsContext::drawImageWithinBounds( Rect* bounds, Image* image, const bool& renderImmediately )
{
	Rect imageBounds( 0,0,bounds->getWidth(), bounds->getHeight() );
	drawPartialImage( bounds->left_, bounds->top_, &imageBounds, image, renderImmediately );
}

void GraphicsContext::drawPartialImage( const double& x, const double& y, Rect* imageBounds, Image* image, const bool& renderImmediately )
{
	//contextPeer_->drawPartialImage( x, y, imageBounds, image );

	if ( renderImmediately ) {
		if ( contextPeer_->prepareForDrawing( GraphicsContext::doImage ) ) {
			
			contextPeer_->drawImage( x, y, imageBounds, image, getCompositingMode() );
			
			contextPeer_->finishedDrawing( 	GraphicsContext::doImage );
		}
	}
	else {
		ImageOperation imgOp;
		imgOp.compositeMode = getCompositingMode();

		Matrix2D* currentXfrm = getCurrentTransform();
		if ( NULL != currentXfrm ) {
			imgOp.matrix = *currentXfrm;
		}
		else {
			imgOp.matrix.identity();
		}

		imgOp.image = image;

		imgOp.imgSrcRect = *imageBounds;
		imgOp.imgXfrmdRect = imgOp.imgSrcRect;
		
		imgOp.pt.x_ = x;
		imgOp.pt.y_ = y;

		imgOp.ctxAlpha = getAlpha();

		imageOperations_.push_back( imgOp );
	}
}

typedef agg::renderer_base<pixfmt> RendererBase;
typedef agg::renderer_scanline_aa_solid<RendererBase> SolidRenderer;
typedef agg::span_allocator<agg::rgba8> SpanAllocator;
typedef agg::span_interpolator_linear<> SpanInterpolator;
typedef agg::span_image_filter_rgba_bilinear_clip<pixfmt,SpanInterpolator> SpanGenerator;
typedef agg::renderer_scanline_aa_solid<RendererBase> RendererType;

typedef agg::rgba8 color;
typedef agg::order_bgra order;

typedef agg::comp_op_adaptor_rgba<color, order> blender_type;
typedef agg::pixfmt_custom_blend_rgba<blender_type, agg::rendering_buffer> pixfmt_type;

typedef agg::blender_rgba<color, order> prim_blender_type; 
typedef agg::pixfmt_alpha_blend_rgba<prim_blender_type, agg::rendering_buffer> prim_pixfmt_type;
typedef agg::renderer_base<pixfmt_type> comp_renderer_type;

struct ImageAlpha {
	typedef agg::rgba8 color_type;
    typedef agg::int8u alpha_type;

	ImageAlpha( double a):alpha(a){}
	void prepare() {}
	void generate(color_type* span, int x, int y, unsigned len) const
	{
		if ( fabs(alpha - 1.0) < 1e-14 ) {
			//nothing to do !
			return;
		}
		else if ( fabs(alpha - 0.0) < 1e-14 ) {
			do
			{
				span->a = 0;
				++span;
			}
			while(--len);
		}
		else {			
			do
			{
				span->a = span->a * alpha;
				++span;
			}
			while(--len);
		}
	}

	double alpha;
};

typedef agg::span_converter<SpanGenerator,ImageAlpha> SpanConv;

void GraphicsContext::renderImage( agg::rendering_buffer& destBuffer, Rect& destRect, ImageOperation& imgOp )
{
	//determine if we have a default transform
	
	agg::path_storage imagePath;
	imagePath.move_to( imgOp.imgSrcRect.left_, imgOp.imgSrcRect.top_ );
	imagePath.line_to( imgOp.imgSrcRect.right_, imgOp.imgSrcRect.top_ );
	imagePath.line_to( imgOp.imgSrcRect.right_, imgOp.imgSrcRect.bottom_ );
	imagePath.line_to( imgOp.imgSrcRect.left_, imgOp.imgSrcRect.bottom_ );
	imagePath.close_polygon();
	agg::trans_affine imageMat;
	agg::trans_affine pathMat;

	if ( imgOp.matrix.isIdentity() ) {
		
		imageMat *= agg::trans_affine_translation( -destRect.left_, -destRect.top_ );
		

		imageMat *= agg::trans_affine_translation( imgOp.pt.x_ ,
													imgOp.pt.y_  );		


		
		pathMat *= agg::trans_affine_translation( -destRect.left_, -destRect.top_ );

		pathMat *= agg::trans_affine_translation( imgOp.pt.x_,
													imgOp.pt.y_ );
	}
	else {
		imageMat *= agg::trans_affine_translation( imgOp.pt.x_,
													imgOp.pt.y_ );

		imageMat *= imgOp.matrix;

		imageMat *= agg::trans_affine_translation( -destRect.left_, -destRect.top_ );
		
		
		pathMat *= agg::trans_affine_translation( imgOp.pt.x_,
													imgOp.pt.y_ );

		pathMat *= imgOp.matrix;

		pathMat *= agg::trans_affine_translation( -destRect.left_, -destRect.top_ );
		
	}

	imageMat.invert();
	agg::conv_transform< agg::path_storage > xfrmdImgPath(imagePath,pathMat);

	ImageAlpha imgA( imgOp.ctxAlpha );

	SpanInterpolator interpolator(imageMat);
	ColorPixels imgPix = imgOp.image;
	
	pixfmt imgPixf(imgPix);
	
	SpanGenerator spanGen(imgPixf,
		agg::rgba(1, 0, 1,0),
		interpolator);
	
	SpanConv sc(spanGen, imgA);
	
	SpanAllocator spanAllocator;
	agg::rasterizer_scanline_aa<> rasterizer;
	agg::scanline_u8 scanLine;
	
	rasterizer.add_path(xfrmdImgPath);

	if ( GraphicsContext::cmSource == imgOp.compositeMode ) {		
		pixfmt pixf(destBuffer);
		RendererBase rb(pixf);
		
		agg::render_scanlines_aa(rasterizer, scanLine, rb, spanAllocator, sc );
	}
	else if ( GraphicsContext::cmCustom != imgOp.compositeMode && GraphicsContext::cmNone != imgOp.compositeMode ){		
		pixfmt_type pixf2(destBuffer);
		pixf2.comp_op( imgOp.compositeMode );

		comp_renderer_type crb(pixf2);
		
		agg::render_scanlines_aa(rasterizer, scanLine, crb, spanAllocator, sc );
	}
}

Rect GraphicsContext::getTransformedImageRect( ImageOperation& imgOp )
{
	Rect result = imgOp.imgSrcRect;
		
	uint32 pid[1] = {0};
	agg::trans_affine pathMat;		
	agg::path_storage path;		

	pathMat *= agg::trans_affine_translation( imgOp.pt.x_, imgOp.pt.y_ );
	pathMat *= imgOp.matrix;
	
	
	path.move_to( imgOp.imgSrcRect.left_, imgOp.imgSrcRect.top_ );
	path.line_to( imgOp.imgSrcRect.right_, imgOp.imgSrcRect.top_ );
	path.line_to( imgOp.imgSrcRect.right_, imgOp.imgSrcRect.bottom_ );
	path.line_to( imgOp.imgSrcRect.left_, imgOp.imgSrcRect.bottom_ );
	path.close_polygon();
	
	
	agg::conv_transform< agg::path_storage > xfrmPath(path,pathMat);
	agg::bounding_rect( xfrmPath, pid, 0, 1,
						&result.left_, &result.top_,
						&result.right_, &result.bottom_ );


	return result;
}	

Point GraphicsContext::getTransformedImagePoint( ImageOperation& imgOp )
{
	Point result = imgOp.pt;

	imgOp.matrix.apply( result.x_, result.y_ );

	return result;
}

Rect GraphicsContext::getRenderDestRect()
{
	Rect result;
	
	if ( !imageOperations_.empty() ) {
		ImageOperation& firstImgOp = imageOperations_[0];

		uint32 pid[1] = {0};
		agg::trans_affine pathMat;

		agg::path_storage path;

		Point pt = getTransformedImagePoint( firstImgOp );

		firstImgOp.imgXfrmdRect = getTransformedImageRect( firstImgOp );
		result = firstImgOp.imgXfrmdRect;

		std::vector<ImageOperation>::iterator it = imageOperations_.begin();

		it ++;
		
		//determine the max bounds rect
		//this becomes the rect that we will use to 
		//copy the current gc contents into our "dest"
		//image
		while ( it != imageOperations_.end() ) {
			ImageOperation& imgOp = *it;			
			
			pt = getTransformedImagePoint( imgOp );

			imgOp.imgXfrmdRect = getTransformedImageRect( imgOp );
			
			if ( result.left_ > imgOp.imgXfrmdRect.left_ ) {
				result.left_ = imgOp.imgXfrmdRect.left_ ;
			}
			if ( result.top_ > imgOp.imgXfrmdRect.top_ ) {
				result.top_ = imgOp.imgXfrmdRect.top_ ;
			}
			
			if ( result.right_ < imgOp.imgXfrmdRect.right_ ) {
				result.right_ = imgOp.imgXfrmdRect.right_ ;
			}
			if ( result.bottom_ < imgOp.imgXfrmdRect.bottom_ ) {
				result.bottom_ = imgOp.imgXfrmdRect.bottom_ ;
			}
			
			it ++;
		}

		result.inflate(2,2);
	}

	return result;
}

void GraphicsContext::renderImages( bool freeImages )
{
	if ( imageOperations_.empty() ) {
		return;
	}


	std::vector<Image*> imagesToDelete;
	std::vector<ImageOperation>::iterator it = imageOperations_.begin();
	
	if ( contextPeer_->prepareForDrawing( GraphicsContext::doImage ) ) {
		
		

		Rect gcRect = getRenderDestRect();

		//create the dest image - this is what we'll render into...
		Image* destImg = GraphicsToolkit::createImage( this, &gcRect );
		ColorPixels pix(destImg);
		agg::rendering_buffer& destBuffer = pix;


		it = imageOperations_.begin();

		while ( it != imageOperations_.end() ) {
			ImageOperation& imgOp = *it;
			

			renderImage( destBuffer, gcRect, imgOp );

			if ( freeImages ) {
				//no dups
				if ( std::find( imagesToDelete.begin(), imagesToDelete.end(), imgOp.image ) == imagesToDelete.end() ) {
					imagesToDelete.push_back( imgOp.image );
				}
			}
			
			it ++;
		}


		contextPeer_->bitBlit( gcRect.left_, gcRect.top_, &Rect(0,0,gcRect.getWidth(),gcRect.getHeight()), destImg );

		delete destImg;


		//cleanup...		
		std::vector<Image*>::iterator it2 = imagesToDelete.begin();
		
		while ( it2 != imagesToDelete.end() ) {
			delete *it2;
			it2 ++;
		}
		
		imageOperations_.clear();
		
		contextPeer_->finishedDrawing( 	GraphicsContext::doImage );
	}
}

void GraphicsContext::textWithStateAt( const double& x, const double& y, const String& text, const bool& enabled )
{
	if ( enabled ) {
		textAt( x, y, text );
	}
	else {
		Color* shadow = GraphicsToolkit::getSystemColor( SYSCOLOR_SHADOW );
		Color* hilight = GraphicsToolkit::getSystemColor( SYSCOLOR_HIGHLIGHT );
		Color oldColor = *currentGraphicsState_->font_.getColor();

		currentGraphicsState_->font_.setColor( hilight );

		textAt( x+1, y+1, text );

		currentGraphicsState_->font_.setColor( shadow );

		textAt( x, y, text );

		//reset the original color
		currentGraphicsState_->font_.setColor( &oldColor );
	}
}


void GraphicsContext::textAt(const double & x, const double & y, const String & text)
{
	Rect bounds( x, y, x + getTextWidth(text), y + getTextHeight(text) );
	Rect tmp = bounds;

	double xx = bounds.left_;
	double yy = bounds.top_;

	currentGraphicsState_->transformMatrix_.apply( xx, yy );

	tmp.offset( xx - tmp.left_, yy - tmp.top_ );


	if ( contextPeer_->prepareForDrawing( GraphicsContext::doText ) ) {
		contextPeer_->textAt( tmp, text );

		contextPeer_->finishedDrawing( 	GraphicsContext::doText );
	}
}

void GraphicsContext::textAt( const double & x, const double & y, const String& text, const long drawOptions )
{
	Rect bounds( x, y, x + getTextWidth(text), y + getTextHeight(text) );

	double xx = x;
	double yy = y;

	currentGraphicsState_->transformMatrix_.apply( xx, yy );

	bounds.offset( xx - bounds.left_, yy - bounds.top_ );

	if ( contextPeer_->prepareForDrawing( GraphicsContext::doText ) ) {

		contextPeer_->textAt( bounds, text, drawOptions );

		contextPeer_->finishedDrawing( 	GraphicsContext::doText );
	}
}


void GraphicsContext::rectangle(const double & x1, const double & y1, const double & x2, const double & y2)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(x1,y1,PointOperation::ptRect) );
	pathOperations_.push_back( PointOperation(x2,y2,PointOperation::ptRect) );
}

void GraphicsContext::roundRect(const double & x1, const double & y1, const double & x2, const double & y2,
								const double & xc, const double & yc)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(x1,y1,PointOperation::ptRoundRect) );
	pathOperations_.push_back( PointOperation(x2,y2,PointOperation::ptRoundRect) );
	pathOperations_.push_back( PointOperation(xc,yc,PointOperation::ptRoundRect) );
}

void GraphicsContext::circle(const double & x, const double & y, const double & radius)
{
	ellipse( x - radius, y - radius, x + radius, y + radius );
}

void GraphicsContext::ellipse(const double & x1, const double & y1, const double & x2, const double & y2)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(x1,y1,PointOperation::ptEllipse) );
	pathOperations_.push_back( PointOperation(x2,y2,PointOperation::ptEllipse) );
}


void GraphicsContext::arc( const double& centerX,  const double& centerY,
				const double& radiusWidth, const double& radiusHeight,
				const double& startAngle, const double& endAngle)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(centerX,centerY,PointOperation::ptArc) );
	pathOperations_.push_back( PointOperation(radiusHeight,radiusHeight,PointOperation::ptArc) );
	pathOperations_.push_back( PointOperation(startAngle,endAngle,PointOperation::ptArc) );
}


void GraphicsContext::pie(const double & x1, const double & y1, const double & x2, const double & y2,
                          const double & x3, const double & y3, const double & x4, const double & y4)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	//contextPeer_->pie( x1, y1, x2, y2, x3, y3, x4, y4 );
}

void GraphicsContext::chord(const double & x1, const double & y1, const double & x2, const double & y2,
                            const double & x3, const double & y3, const double & x4, const double & y4)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	//contextPeer_->chord( x1, y1, x2, y2, x3, y3, x4, y4 );
}


void GraphicsContext::polyline( const std::vector<Point>& pts )
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	std::vector<Point>::const_iterator it = pts.begin();
	while ( it != pts.end() ) {
		const Point& pt = *it;
		pathOperations_.push_back( PointOperation(pt.x_, pt.y_, PointOperation::ptPolyLine) );
		++it;
	}
}

void GraphicsContext::curve(const double & x1, const double & y1, const double & x2, const double & y2,
                            const double & x3, const double & y3, const double & x4, const double & y4)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(x1,y1,PointOperation::ptBezier) );
	pathOperations_.push_back( PointOperation(x2,y2,PointOperation::ptBezier) );
	pathOperations_.push_back( PointOperation(x3,y3,PointOperation::ptBezier) );
	pathOperations_.push_back( PointOperation(x4,y4,PointOperation::ptBezier) );
}

void GraphicsContext::lineTo(const double & x, const double & y)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(x,y,PointOperation::ptLineTo) );
}

void GraphicsContext::moveTo(const double & x, const double & y)
{
	checkPathOperations();
	currentDrawingState_ = GraphicsContext::gsAddingGraphicsOps;

	pathOperations_.push_back( PointOperation(x,y,PointOperation::ptMoveTo) );
}

void GraphicsContext::fillPath()
{
	if ( contextPeer_->prepareForDrawing( GraphicsContext::doFill ) ) {

		execPathOperations();

		contextPeer_->finishedDrawing( 	GraphicsContext::doFill );
	}
}

void GraphicsContext::strokePath()
{
	if ( contextPeer_->prepareForDrawing( GraphicsContext::doStroke ) ) {

		execPathOperations();

		contextPeer_->finishedDrawing( 	GraphicsContext::doStroke );
	}
}

ContextPeer* GraphicsContext::getPeer()
{
	return contextPeer_;
}

void GraphicsContext::setOrigin( const double& x, const double& y )
{
	contextPeer_->setOrigin( x, y );
}

Point GraphicsContext::getOrigin()
{
	return contextPeer_->getOrigin();
}


void GraphicsContext::copyContext( const Rect& sourceRect,
						const Rect& destRect,
						GraphicsContext* context )
{
	contextPeer_->copyContext( sourceRect, destRect, context->getPeer() );
}

void GraphicsContext::setCurrentTransform( const Matrix2D& transform )
{
	currentGraphicsState_->transformMatrix_ = transform;
}

Matrix2D* GraphicsContext::getCurrentTransform()
{
	return &currentGraphicsState_->transformMatrix_;
}

void GraphicsContext::setRotation( const double& theta )
{
	currentGraphicsState_->rotation_  = theta;
	currentGraphicsState_->compositeMatrix();
}

void GraphicsContext::setTranslation( const double transX, const double& transY )
{
	currentGraphicsState_->translateX_  = transX;
	currentGraphicsState_->translateY_  = transY;
	currentGraphicsState_->compositeMatrix();
}

void GraphicsContext::setShear( const double& shearX, const double& shearY )
{
	currentGraphicsState_->shearX_  = shearX;
	currentGraphicsState_->shearY_  = shearY;
	currentGraphicsState_->compositeMatrix();
}

void GraphicsContext::setScale( const double& scaleX, const double& scaleY )
{
	currentGraphicsState_->scaleX_  = scaleX;
	currentGraphicsState_->scaleY_  = scaleY;
	currentGraphicsState_->compositeMatrix();
}

void GraphicsContext::concatRotation( const double& theta )
{
	currentGraphicsState_->rotation_  += theta;
	Matrix2D rot;
	rot.rotate( currentGraphicsState_->rotation_ );
	currentGraphicsState_->transformMatrix_.multiply( rot );
}

void GraphicsContext::concatTranslation( const double transX, const double& transY )
{
	currentGraphicsState_->translateX_  += transX;
	currentGraphicsState_->translateY_  += transY;
	Matrix2D translation;
	translation.translate( currentGraphicsState_->translateX_, currentGraphicsState_->translateY_);
	
	currentGraphicsState_->transformMatrix_.multiply( translation );
}

void GraphicsContext::concatShear( const double& shearX, const double& shearY )
{
	currentGraphicsState_->shearX_  += shearX;
	currentGraphicsState_->shearY_  += shearY;
	Matrix2D shear;
	shear.shear( currentGraphicsState_->shearX_, currentGraphicsState_->shearY_ );
	currentGraphicsState_->transformMatrix_.multiply( shear );
}

void GraphicsContext::concatScale( const double& scaleX, const double& scaleY )
{
	currentGraphicsState_->scaleX_  += scaleX;
	currentGraphicsState_->scaleY_  += scaleY;
	Matrix2D scale;
	scale.scale( currentGraphicsState_->scaleX_, currentGraphicsState_->scaleY_ );
	
	currentGraphicsState_->transformMatrix_.multiply( scale );
}

Font* GraphicsContext::getCurrentFont()
{
	return &currentGraphicsState_->font_;
}

void GraphicsContext::setColor( Color* color )
{
	if ( NULL != color ) {
		currentGraphicsState_->color_ = *color;
	}
}

void GraphicsContext::setColor( const Color* color )
{
	if ( NULL != color ) {
		currentGraphicsState_->color_ = *color;
	}
}

Color* GraphicsContext::getColor()
{
	return &currentGraphicsState_->color_;
}


void GraphicsContext::textBoundedBy( Rect* bounds, const String& text, const bool& wordWrap )
{
	long drawOptions = wordWrap ? GraphicsContext::tdoWordWrap : GraphicsContext::tdoNone;

	Rect tmp = *bounds;
	double xx = tmp.left_;
	double yy = tmp.top_;

	currentGraphicsState_->transformMatrix_.apply( xx, yy );
	tmp.offset( xx - tmp.left_, yy - tmp.top_ );

	if ( contextPeer_->prepareForDrawing( GraphicsContext::doText ) ) {
		contextPeer_->textAt( tmp, text, drawOptions );

		contextPeer_->finishedDrawing( 	GraphicsContext::doText );
	}
}

void GraphicsContext::textBoundedBy( Rect* bounds, const String& text, const long drawOptions )
{
	Rect tmp = *bounds;
	double xx = tmp.left_;
	double yy = tmp.top_;

	currentGraphicsState_->transformMatrix_.apply( xx, yy );

	tmp.offset( xx - tmp.left_, yy - tmp.top_ );

	if ( contextPeer_->prepareForDrawing( GraphicsContext::doText ) ) {
		contextPeer_->textAt( tmp, text, drawOptions );

		contextPeer_->finishedDrawing( 	GraphicsContext::doText );
	}
}

bool GraphicsContext::isXORModeOn()
{
	return contextPeer_->isXORModeOn();
}

void GraphicsContext::setXORModeOn( const bool& XORModeOn )
{
	contextPeer_->setXORModeOn( XORModeOn );
}

double GraphicsContext::getTextWidth( const String& text )
{
	return contextPeer_->getTextWidth( text );
}

double GraphicsContext::getTextHeight( const String& text )
{
	return contextPeer_->getTextHeight( text );
}

void GraphicsContext::setClippingRect( Rect* rect )
{
	BezierCurve* rectClipPath = NULL;
	if ( NULL != rect ) {
		rectClipPath = new BezierCurve();
		rectClipPath->moveTo( rect->left_, rect->top_ );
		rectClipPath->lineTo( rect->right_, rect->top_ );
		rectClipPath->lineTo( rect->right_, rect->bottom_ );
		rectClipPath->lineTo( rect->left_, rect->bottom_ );
		rectClipPath->close();
	}

	Path* clipPath = currentGraphicsState_->clippingPath_;

	if ( NULL != clipPath ) { //release the underlying object instance
		Object* pathObj = dynamic_cast<Object*>( clipPath );
		pathObj->release();
	}

	clipPath = rectClipPath;

	if ( NULL != clipPath ) { //take ownership of the underlying object instance
		Object* pathObj = dynamic_cast<Object*>( clipPath );
		pathObj->addRef();
	}

	currentGraphicsState_->clippingPath_ = clipPath;

	contextPeer_->setClippingRect( rect );
}

void GraphicsContext::setClippingPath( Path* clippingPath )
{
	Path* clipPath = currentGraphicsState_->clippingPath_;

	if ( NULL != clipPath ) { //release the underlying object instance
		Object* pathObj = dynamic_cast<Object*>( clipPath );
		pathObj->release();
	}

	clipPath = clippingPath;

	if ( NULL != clipPath ) { //take ownership of the underlying object instance
		Object* pathObj = dynamic_cast<Object*>( clipPath );
		pathObj->addRef();
	}

	currentGraphicsState_->clippingPath_ = clipPath;

	contextPeer_->setClippingPath( clipPath );
}

Rect GraphicsContext::getClippingRect()
{
	Rect result;

	Path* clipPath = currentGraphicsState_->clippingPath_;

	if ( NULL != clipPath ) {
		result = clipPath->getBounds();
	}

	return result;
}

Path* GraphicsContext::getClippingPath()
{
	return currentGraphicsState_->clippingPath_;
}

void GraphicsContext::drawThemeSelectionRect( Rect* rect, DrawUIState& state )
{
	contextPeer_->drawThemeSelectionRect( rect, state );
}

void GraphicsContext::drawThemeFocusRect( Rect* rect, DrawUIState& state )
{
	contextPeer_->drawThemeFocusRect( rect, state );
}

void GraphicsContext::drawThemeButtonRect( Rect* rect, ButtonState& state, Rect* captionRect )
{
	contextPeer_->drawThemeButtonRect( rect, state, captionRect );
}

void GraphicsContext::drawThemeButtonFocusRect( Rect* rect )
{
	contextPeer_->drawThemeButtonFocusRect( rect );
}

void GraphicsContext::drawThemeCheckboxRect( Rect* rect, ButtonState& state )
{
	contextPeer_->drawThemeCheckboxRect( rect, state );
}

void GraphicsContext::drawThemeRadioButtonRect( Rect* rect, ButtonState& state )
{
	contextPeer_->drawThemeRadioButtonRect( rect, state );
}

void GraphicsContext::drawThemeComboboxRect( Rect* rect, ButtonState& state )
{
	contextPeer_->drawThemeComboboxRect( rect, state );
}


void GraphicsContext::drawThemeScrollButtonRect( Rect* rect, ScrollBarState& state )
{
	contextPeer_->drawThemeScrollButtonRect( rect, state );
}

void GraphicsContext::drawThemeDisclosureButton( Rect* rect, DisclosureButtonState& state )
{
	contextPeer_->drawThemeDisclosureButton( rect, state );
}

void GraphicsContext::drawThemeTab( Rect* rect, TabState& state )
{
	contextPeer_->drawThemeTab( rect, state );
}

void GraphicsContext::drawThemeTabPage( Rect* rect, DrawUIState& state )
{
	contextPeer_->drawThemeTabPage( rect, state );
}

void GraphicsContext::drawThemeTabContent( Rect* rect, DrawUIState& state )
{
	contextPeer_->drawThemeTabContent( rect, state );
}

void GraphicsContext::drawThemeTabs( Rect* rect, DrawUIState& paneState, TabState& selectedTabState, TabState& otherTabs, const std::vector<String>& tabNames, int selectedTabIndex )
{
	contextPeer_->drawThemeTabs( rect, paneState, selectedTabState, otherTabs, tabNames, selectedTabIndex );
}

void GraphicsContext::drawThemeTickMarks( Rect* rect, SliderState& state )
{
	contextPeer_->drawThemeTickMarks( rect, state );
}

void GraphicsContext::drawThemeSlider( Rect* rect, SliderState& state )
{
	contextPeer_->drawThemeSlider( rect, state );
}

void GraphicsContext::drawThemeProgress( Rect* rect, ProgressState& state )
{
	contextPeer_->drawThemeProgress( rect, state );
}

void GraphicsContext::drawThemeImage( Rect* rect, Image* image, DrawUIState& state )
{
	contextPeer_->drawThemeImage( rect, image, state );
}

void GraphicsContext::drawThemeHeader( Rect* rect, ButtonState& state )
{
	contextPeer_->drawThemeHeader( rect, state );
}

void GraphicsContext::drawThemeEdge( Rect* rect, DrawUIState& state, const long& edgeSides, const long& edgeStyle )
{
	contextPeer_->drawThemeEdge( rect, state, edgeSides, edgeStyle );
}

void GraphicsContext::drawThemeSizeGripper( Rect* rect, DrawUIState& state )
{
	contextPeer_->drawThemeSizeGripper( rect, state );
}

void GraphicsContext::drawThemeBackground( Rect* rect, BackgroundState& state )
{
	contextPeer_->drawThemeBackground( rect, state );
}

void GraphicsContext::drawThemeMenuItem( Rect* rect, MenuState& state )
{
	contextPeer_->drawThemeMenuItem( rect, state );
}

void GraphicsContext::drawThemeMenuItemText( Rect* rect, MenuState& state )
{
	contextPeer_->drawThemeMenuItemText( rect, state );
}

void GraphicsContext::drawThemeText( Rect* rect, TextState& state )
{
	contextPeer_->drawThemeText( rect, state );
}

void GraphicsContext::setStrokeWidth( const double& width )
{
	currentGraphicsState_->strokeWidth_ = width;
}

double GraphicsContext::getStrokeWidth()
{
	return currentGraphicsState_->strokeWidth_;
}

void GraphicsContext::execPathOperations()
{
	currentDrawingState_ = GraphicsContext::gsExecutingGraphicsOps;

	std::vector<PointOperation>::iterator it = pathOperations_.begin();

	double tmpX;
	double tmpY;


	Matrix2D& transform = currentGraphicsState_->transformMatrix_;

	while ( it != pathOperations_.end() ) {
		PointOperation& pointOp = *it;

		switch ( pointOp.primitive ){
			case PointOperation::ptMoveTo : {
				currentGraphicsState_->currentMoveTo_.x_ = pointOp.x;
				currentGraphicsState_->currentMoveTo_.y_ = pointOp.y;

				tmpX = pointOp.x;
				tmpY = pointOp.y;
				transform.apply( tmpX, tmpY );

				contextPeer_->moveTo( tmpX, tmpY );
				++it;
			}
			break;

			case PointOperation::ptLineTo :{
				tmpX = pointOp.x;
				tmpY = pointOp.y;
				transform.apply( tmpX, tmpY );

				contextPeer_->lineTo( tmpX, tmpY );
				++it;
			}
			break;

			case PointOperation::ptPolyLine :{
				std::vector<Point> tmpPts;
				PointOperation::PrimitiveType type = pointOp.primitive;

				while ( (it != pathOperations_.end()) && (type == PointOperation::ptPolyLine) ) {
					PointOperation& pt = *it;
					type = pt.primitive;
					tmpX = pt.x;
					tmpY = pt.y;
					transform.apply( tmpX, tmpY );					

					tmpPts.push_back( Point(tmpX,tmpY) );
					++it;
				}

				contextPeer_->polyline( tmpPts );
			}
			break;

			case PointOperation::ptBezier :{
				/*
				if ( (remaingOps - 3) < 0 ){
					//throw exception !!
				}
				*/
				if ( (pathOperations_.end() - it) < 4 ) {
					throw RuntimeException( MAKE_ERROR_MSG_2("Not enough path operations left to complete the bezier curve") );
				}

				Point p1;
				Point p2;
				Point p3;
				Point p4;

				PointOperation& pt1 = *it;


				p1.x_ = pt1.x;
				p1.y_ = pt1.y;

				transform.apply( p1.x_, p1.y_ );

				++it;

				PointOperation& pt2 = *it;
				p2.x_ = pt2.x;
				p2.y_ = pt2.y;

				transform.apply( p2.x_, p2.y_ );

				++it;

				PointOperation& pt3 = *it;
				p3.x_ = pt3.x;
				p3.y_ = pt3.y;

				transform.apply( p3.x_, p3.y_ );

				++it;

				PointOperation& pt4 = *it;
				p4.x_ = pt4.x;
				p4.y_ = pt4.y;

				transform.apply( p4.x_, p4.y_ );

				++it;

				contextPeer_->curve( p1.x_, p1.y_, p2.x_, p2.y_,
										p3.x_, p3.y_, p4.x_, p4.y_ );


			}
			break;

			case PointOperation::ptRect:{

				PointOperation& pt1 = *it;
				++it;

				PointOperation& pt2 = *it;
				++it;


				std::vector<Point> tmpPts(5);
				tmpPts[0].x_ = pt1.x;
				tmpPts[0].y_ = pt1.y;
				tmpPts[1].x_ = pt2.x;
				tmpPts[1].y_ = pt1.y;
				tmpPts[2].x_ = pt2.x;
				tmpPts[2].y_ = pt2.y;
				tmpPts[3].x_ = pt1.x;
				tmpPts[3].y_ = pt2.y;
				tmpPts[4] = tmpPts[0];

				std::vector<Point>::iterator ptIt = tmpPts.begin();

				while ( ptIt != tmpPts.end() ) {
					Point& p = *ptIt;

					transform.apply( p.x_, p.y_ );

					++ptIt;
				}

				contextPeer_->polyline( tmpPts );



				//contextPeer_->rectangle( pt1.x, pt1.y, pt2.x, pt2.y );
			}
			break;

			case PointOperation::ptRoundRect :{



				PointOperation& pt1 = *it;
				++it;

				PointOperation& pt2 = *it;
				++it;

				PointOperation& pt3 = *it;
				++it;

				std::vector<Point> tmpPts;
				buildRoundRect( pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, tmpPts, transform );

				contextPeer_->polyline( tmpPts );

			}
			break;

			case PointOperation::ptEllipse : {
				PointOperation& pt1 = *it;
				++it;

				PointOperation& pt2 = *it;
				++it;

				//contextPeer_->ellipse( pt1.x, pt1.y, pt2.x, pt2.y );

				std::vector<Point> tmpPts;
				buildEllipse( pt1.x, pt1.y, pt2.x, pt2.y, tmpPts, transform );

				contextPeer_->polyline( tmpPts );
			}
			break;

			case PointOperation::ptArc :{
				if ( (pathOperations_.end() - it) < 3 ) {
					throw RuntimeException( MAKE_ERROR_MSG_2("Not enough path operations left to complete the bezier curve") );
				}

				PointOperation& pt1 = *it;
				++it;

				PointOperation& pt2 = *it;
				++it;

				PointOperation& pt3 = *it;
				++it;


				std::vector<Point> tmpPts;
				buildArc( pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, tmpPts, transform );

				contextPeer_->polyline( tmpPts );
			}
			break;

			case PointOperation::ptClose : {
				PointOperation& pt = *it;
				contextPeer_->lineTo( pt.x, pt.y );
				++it;
			}
			break;

			default : {
				it = pathOperations_.end();
				break; //quit the loop
			}
			break;
		}
	}

	currentDrawingState_ = GraphicsContext::gsNone;
}


void GraphicsContext::checkPathOperations()
{
	if ( GraphicsContext::gsAddingGraphicsOps != currentDrawingState_ ) {
		pathOperations_.clear();
	}
}


void GraphicsContext::setRenderArea( Rect bounds )
{
	drawingAreaTopLeft_.x_ = bounds.left_;
	drawingAreaTopLeft_.y_ = bounds.top_;

	if ( NULL == renderArea_ ) {
		renderArea_ = GraphicsToolkit::createImage( (uint32)bounds.getWidth(), (uint32)bounds.getHeight() );
		renderBuffer_ = new agg::rendering_buffer();
	}
	else {
		renderArea_->setSize( (uint32)bounds.getWidth(), (uint32)bounds.getHeight() );
	}

	renderBuffer_->attach( (unsigned char*)renderArea_->getData(), 
							renderArea_->getWidth(), 
							renderArea_->getHeight(),
							renderArea_->getWidth() * renderArea_->getType() );

	renderArea_->getImageContext()->setRenderingBuffer( renderBuffer_ );

	renderArea_->getImageContext()->setOrigin( -bounds.left_, -bounds.top_ );

	renderAreaDirty_ = true;
}

void GraphicsContext::deleteRenderArea()
{
	renderBuffer_->attach( NULL, 0, 0, 0 );
	delete renderBuffer_;

	delete renderArea_;
	
	renderBuffer_ = NULL;
	renderArea_ = NULL;
}

void GraphicsContext::flushRenderArea()
{
	if ( viewableBounds_.isNull() ) {
		drawImage( drawingAreaTopLeft_, renderArea_ );
	}
	else {

		//FIXME!!!!!!!
		//renderArea_->getImageBits()->renderBuffer_->clip_box( agg::rect( viewableBounds_.left_,
		//																	viewableBounds_.top_,
		//																	viewableBounds_.right_,
		//																	viewableBounds_.bottom_ ) );

		drawPartialImage(  viewableBounds_.getTopLeft(), &viewableBounds_, renderArea_ );
	}

	renderAreaDirty_ = false;
}

void GraphicsContext::buildArc( double centerX,  double centerY,
            double radiusWidth, double radiusHeight,
            double startAngle, double endAngle, std::vector<Point>& pts, const Matrix2D& transform )
{
	agg::path_storage path;

	agg::arc arcPath(centerX,
						centerY,
						radiusWidth, radiusHeight,
						Math::degreesToRadians(startAngle), Math::degreesToRadians(endAngle));

	path.concat_path( arcPath );

	for ( size_t i=0;i<path.total_vertices();i++ ) {
		Point pt;
		path.vertex(i,&pt.x_, &pt.y_);	
		transform.apply( pt.x_, pt.y_ );
		pts.push_back( pt );
	}


	path.remove_all();
}


void GraphicsContext::buildRoundRect( double x1, double y1, double x2, double y2,
							double cornerArcWidth, double cornerArcHeight,
							std::vector<Point>& pts, const Matrix2D& transform )
{
	agg::path_storage path;

	path.move_to( x1 + cornerArcWidth/2.0, y1 );
	path.line_to( x2 - cornerArcWidth, y1 );

	agg::arc arc1(x2 - cornerArcWidth/2.0,
		y1 + cornerArcHeight/2.0,
		cornerArcWidth/2.0, cornerArcHeight/2.0,
		Math::degreesToRadians(270), Math::degreesToRadians(0));

	path.concat_path( arc1 );

	path.line_to( x2, y2 - cornerArcHeight/2.0 );

	agg::arc arc2(x2 - cornerArcWidth/2.0,
		y2 - cornerArcHeight/2.0,
		cornerArcWidth/2.0, cornerArcHeight/2.0,
		Math::degreesToRadians(0), Math::degreesToRadians(90));

	path.concat_path( arc2 );

	path.line_to( x1 + cornerArcWidth/2.0, y2 );

	agg::arc arc3(x1 + cornerArcWidth/2.0,
		y2 - cornerArcHeight/2.0,
		cornerArcWidth/2.0, cornerArcHeight/2.0,
		Math::degreesToRadians(90), Math::degreesToRadians(180));

	path.concat_path( arc3 );

	path.line_to( x1, y1 + cornerArcHeight/2.0 );

	agg::arc arc4(x1 + cornerArcWidth/2.0,
		y1 + cornerArcHeight/2.0,
		cornerArcWidth/2.0, cornerArcHeight/2.0,
		Math::degreesToRadians(180), Math::degreesToRadians(270));

	path.concat_path( arc4 );


	//agg::path_storage::const_iterator it = path.begin();
	for (size_t i=0;i<path.total_vertices();i++ ) {	
		Point pt;

		path.vertex( i, &pt.x_, &pt.y_ );

		transform.apply( pt.x_, pt.y_ );

		pts.push_back( pt );
	}
}

void GraphicsContext::buildEllipse( double x1, double y1, double x2, double y2,
							std::vector<Point>& pts, const Matrix2D& transform )
{
	agg::path_storage path;
	agg::ellipse ellipseShape( x1 + ((x2-x1)/2.0), y1 + ((y2-y1)/2.0),
								abs(static_cast<long>(x2-x1)), abs(static_cast<long>(y2-y1)), 100 );
	path.concat_path( ellipseShape );

	for (size_t i=0;i<path.total_vertices();i++ ) {
		Point pt;
		path.vertex( i, &pt.x_, &pt.y_ );

		transform.apply( pt.x_, pt.y_ );


		pts.push_back( pt );
	}

	pts.back() = pts[0];
}

int GraphicsContext::saveState()
{
	GraphicsState* newState = new GraphicsState(*stateCollection_.back());

	stateCollection_.push_back( newState );
	graphicsStateIndex_ = stateCollection_.size() - 1;

	currentGraphicsState_ = stateCollection_[graphicsStateIndex_];

	return graphicsStateIndex_;
}

void GraphicsContext::restoreState( int state )
{
	VCF_ASSERT( state < (int)stateCollection_.size() );
	VCF_ASSERT( state >= 0 );
	if ( (stateCollection_.size() - state) > 0 ) {
		for ( size_t i=state;i<stateCollection_.size();i++ ) {
			delete stateCollection_[i];
		}

		stateCollection_.erase( stateCollection_.begin() + state,
								stateCollection_.end() );
	}

	graphicsStateIndex_ = maxVal<>(0,(state - 1));
	currentGraphicsState_ = stateCollection_[graphicsStateIndex_];
}

bool GraphicsContext::isDefaultTransform()
{
	return (currentGraphicsState_->rotation_ == 0.0) &&
			(currentGraphicsState_->translateX_ == 0.0) &&
			(currentGraphicsState_->translateY_ == 0.0) &&
			(currentGraphicsState_->shearX_ == 0.0) &&
			(currentGraphicsState_->shearY_ == 0.0) &&
			(currentGraphicsState_->scaleX_ == 1.0) &&
			(currentGraphicsState_->scaleY_ == 1.0);
}

void GraphicsContext::makeDefaultTransform()
{
	currentGraphicsState_->rotation_ = 0.0;
	currentGraphicsState_->translateX_ = 0.0;
	currentGraphicsState_->translateY_ = 0.0;
	currentGraphicsState_->shearX_ = 0.0;
	currentGraphicsState_->shearY_ = 0.0;
	currentGraphicsState_->scaleX_ = 1.0;
	currentGraphicsState_->scaleY_ = 1.0;

	currentGraphicsState_->compositeMatrix();
}

double GraphicsContext::getRotation()
{
	return currentGraphicsState_->rotation_;
}

double GraphicsContext::getTranslationX()
{
	return currentGraphicsState_->translateX_;
}

double GraphicsContext::getTranslationY()
{
	return currentGraphicsState_->translateY_;
}

double GraphicsContext::getShearX()
{
	return currentGraphicsState_->shearX_;
}

double GraphicsContext::getShearY()
{
	return currentGraphicsState_->shearY_;
}

double GraphicsContext::getScaleX()
{
	return currentGraphicsState_->scaleX_;
}

double GraphicsContext::getScaleY()
{
	return currentGraphicsState_->scaleY_;
}

bool GraphicsContext::isAntiAliasingOn()
{
	return contextPeer_->isAntiAliasingOn();
}

void GraphicsContext::setAntiAliasingOn( bool antiAliasingOn )
{
	contextPeer_->setAntiAliasingOn( antiAliasingOn );
}


};	// namespace VCF


/**
$Id$
*/