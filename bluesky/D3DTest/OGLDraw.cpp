//OGLDraw.cpp




#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/FoundationKit/Dictionary.h"



#include "GL/glew.h"




#include "vcf/OpenGLKit/OpenGLKit.h"
#include "vcf/OpenGLKit/VCFOpenGL.h"


#ifdef  _DEBUG
#pragma comment (lib,"glew32sd.lib")
#else
#pragma comment (lib,"glew32s.lib")
#endif




#define FRAG_CODE	"void main()\n"\
	"{\n"\
		"gl_FragColor = vec4(0.4,0.4,0.8,1.0);\n"\
	"}\n"





namespace VCF { 


class GL2DView : public View {
public:
	virtual void paintView( GraphicsContext* ctx ) {
		Rect r = getViewControl()->getClientBounds();

		Color c = *getViewControl()->getColor();

		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, r.getWidth(), r.getHeight(), 0, -1, 1);
		glViewport( 0, 0, r.getWidth(), r.getHeight() );
		
		glDisable(GL_DEPTH_TEST);		

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor( c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha() );
		glClear(GL_COLOR_BUFFER_BIT);
	}
};



class GLMat {
public:

	GLMat(){
		ident();
	}

	GLMat& operator=( const Matrix2D& rhs ) {
		ident();

		mat[0] = rhs[Matrix2D::mei00];
		mat[1] = rhs[Matrix2D::mei01];
		mat[2] = 0.0;//z ignored
		mat[3] = rhs[Matrix2D::mei02];

		mat[4] = rhs[Matrix2D::mei10];
		mat[5] = rhs[Matrix2D::mei11];
		mat[6] = 0.0;//z ignored
		mat[7] = rhs[Matrix2D::mei12];

		mat[8] = 0.0;
		mat[9] = 0.0;
		mat[10] = 1.0;//z ignored
		mat[11] = 0.0;

		mat[12] = rhs[Matrix2D::mei20];
		mat[13] = rhs[Matrix2D::mei21];
		mat[14] = 0.0;//z ignored
		mat[15] = rhs[Matrix2D::mei22];

		return *this;
	}

	operator double* () {
		return &mat[0];
	}

	operator const double* () const {
		return &mat[0];
	}

	double mat[16];

	void ident() {
		mat[0] = 1.0;
		mat[1] = 0.0;
		mat[2] = 0.0;
		mat[3] = 0.0;
		mat[4] = 0.0;
		mat[5] = 1.0;
		mat[6] = 0.0;
		mat[7] = 0.0;
		mat[8] = 1.0;
		mat[9] = 0.0;
		mat[10] = 0.0;
		mat[11] = 1.0;
		mat[12] = 0.0;
		mat[13] = 0.0;
		mat[14] = 0.0;
		mat[15] = 1.0;
	}
};



class ImageKit {
public:
	static void init( int argc, char** argv );
	static void terminate();

	static void attachControl( Control* c );
	static void detachControl( Control* c );
protected:
	static ImageKit* instance;
	
private:
	ImageKit();
	~ImageKit();
};


class IKImage;


class IKImageContext {
public:

	IKImageContext();

	void initView( const double width, const double& height );

	void draw( const double& x, const double& y, IKImage* image );

	void setTransformMatrix( const Matrix2D& m ) {
		xfrm_ = m;
	}

	void multiTransformMatrix( const Matrix2D& m ) {
		xfrm_ *= m;
	}

	Matrix2D getTransformMatrix() const {
		return xfrm_;
	}

	void setOpacity( const double& val ) {
		opacity_ = val;
	}

	double getOpacity() const {
		return opacity_;
	}
protected:
	Matrix2D xfrm_;
	double opacity_;
};



class IKImage : public Object {
public:

	enum {
		NullHandle = 0
	};

	IKImage();
	IKImage( Image* image );
	IKImage( const uchar* data, const size_t& size, const MIMEType& type  );
	IKImage( const uchar* data, const size_t& size );
	IKImage( const uchar* data, const Size& dimensions );
	IKImage( const String& fileName );

	virtual ~IKImage();

	void initFromImage( Image* image );
	void initFromData( const uchar* data, const size_t& size, const MIMEType& type );
	void initFromData( const uchar* data, const size_t& size );
	void initFromBits( const uchar* data, const Size& dimensions );
	void initFromFile( const String& fileName );

	Size getSize() const {
		return size_;
	}
protected:

	friend class IKImageContext;

	uint32 imageHandle_;
	Size size_;

	void bind();


	void destroyTexture();
};





class IKFilter : public Object {
public:
	IKFilter();
	virtual ~IKFilter();

	std::vector<String> getInputNames() {
		return inputNames_;
	}

	std::vector<String> getOutputNames() {
		return outputNames_;
	}
	
	void apply();
protected:
	GLuint fragment_;
	GLuint program_;
	String fragmentFilename_;

	std::vector<String> inputNames_;
	std::vector<String> outputNames_;
	
	void initProgram();

};







ImageKit* ImageKit::instance = NULL;

ImageKit::ImageKit()
{
	
}

ImageKit::~ImageKit()
{
	
}

void ImageKit::init( int argc, char** argv )
{
	ImageKit::instance = new ImageKit();

#ifdef WIN32
	//init glew
	WNDCLASSEXW wcex;
	
	wcex.cbSize = sizeof(wcex);
	
	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)DefWindowProcW;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= ::GetModuleHandleW(NULL);
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= 0;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"ImageKit";
	wcex.hIconSm		= NULL;
	
	RegisterClassExW(&wcex);
	
	HWND hwnd = ::CreateWindowExW( 0,
		wcex.lpszClassName,
		NULL,
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0,
		0,
		1,
		1,
		NULL,
		NULL, wcex.hInstance, NULL );
	
	int e = GetLastError();
	
	HDC dc = ::GetDC( hwnd );
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,                              // version number
			PFD_DRAW_TO_WINDOW |            // support window
			PFD_SUPPORT_OPENGL |          // support OpenGL
			PFD_DOUBLEBUFFER,             // double buffered
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			32,                             // 32-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
	};
	
	int pixelformat = ChoosePixelFormat( dc, &pfd );
	::SetPixelFormat( dc, pixelformat, &pfd );
	HGLRC hrc = wglCreateContext( dc );
	wglMakeCurrent( dc, hrc );
	
	
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		Dialog::showMessage( String("Error: ") + (const char*)glewGetString(err) ); 
	}
	
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hrc );

	ReleaseDC( hwnd, dc );
	DestroyWindow(hwnd);
	UnregisterClassW(wcex.lpszClassName, wcex.hInstance);
#endif
}

void ImageKit::terminate()
{
	delete ImageKit::instance;
}













IKImage::IKImage():Object(),imageHandle_(IKImage::NullHandle)
{

}

IKImage::IKImage( Image* image )
	:Object(),imageHandle_(IKImage::NullHandle)
{

}

IKImage::IKImage( const uchar* data, const size_t& size, const MIMEType& type  )
	:Object(),imageHandle_(IKImage::NullHandle)
{

}

IKImage::IKImage( const uchar* data, const size_t& size )
:Object(),imageHandle_(IKImage::NullHandle)
{

}

IKImage::IKImage( const uchar* data, const Size& dimensions )
:Object(),imageHandle_(IKImage::NullHandle)
{

}

IKImage::IKImage( const String& fileName )
:Object(),imageHandle_(IKImage::NullHandle)
{

}

IKImage::~IKImage()
{
	destroyTexture();
}

void IKImage::initFromImage( Image* image )
{
	if ( imageHandle_ != IKImage::NullHandle ) {
		glDeleteTextures( 1, &imageHandle_ );
	}

	glGenTextures( 1, &imageHandle_ );

	glBindTexture(GL_TEXTURE_2D, imageHandle_);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use nice (linear) scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Use nice (linear) scaling
	glTexImage2D(GL_TEXTURE_2D, 0, 
				image->getType() , 
				image->getWidth(), 
				image->getHeight(), 
				0, 
				image->getType() == Image::itColor ? GL_BGRA_EXT : GL_LUMINANCE, 
				GL_UNSIGNED_BYTE, 
				image->getData() );

	size_.width_ = image->getWidth();
	size_.height_ = image->getHeight();

}

void IKImage::initFromData( const uchar* data, const size_t& size, const MIMEType& type )
{

}

void IKImage::initFromData( const uchar* data, const size_t& size )
{

}

void IKImage::initFromBits( const uchar* data, const Size& dimensions )
{

}

void IKImage::initFromFile( const String& fileName )
{
	Image* img = GraphicsToolkit::createImage( fileName );
	if ( NULL != img ) {
		initFromImage( img );
		delete img;
	}
}

void IKImage::bind() 
{	
	glBindTexture(GL_TEXTURE_2D, imageHandle_);
}

void IKImage::destroyTexture()
{
	if ( imageHandle_ != IKImage::NullHandle ) {
		glDeleteTextures( 1, &imageHandle_ );

		imageHandle_ = IKImage::NullHandle;
	}
}



IKImageContext::IKImageContext():
	opacity_(1.0)
{

}

void IKImageContext::initView( const double width, const double& height )
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, width, height, 0, -1, 1);
	glViewport( 0, 0, width, height );
	
	glDisable(GL_DEPTH_TEST);		

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
}


void IKImageContext::draw( const double& x, const double& y, IKImage* image )
{
	/*
	glEnable (GL_LINE_SMOOTH);
	glEnable (GL_POLYGON_SMOOTH );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

//	glPolygonMode(GL_FRONT,GL_FILL);
 //glPolygonMode(GL_BACK,GL_FILL);
 glShadeModel(GL_SMOOTH);
*/

	

	glActiveTexture(GL_TEXTURE0 + 0);
	image->bind();

	double u=1.0;
	double v=1.0;

	Size sz = image->getSize();
	glPushMatrix();

	GLMat m;
	m = xfrm_;
	glMultMatrixd( m );

	glColor4f(1.0, 1.0, 1.0, opacity_);

	glBegin(GL_POLYGON);


		glTexCoord2f(0.0, 0.0);
		glVertex2i(x, y);

		glTexCoord2f(u, 0.0);
		glVertex2i(x+sz.width_, y);

		glTexCoord2f(u, v);
		glVertex2i(x+sz.width_, y+sz.height_);

		glTexCoord2f(0.0, v);
		glVertex2i(x, y+sz.height_);

		
	glEnd();

	glPopMatrix();

/*
	glDisable (GL_LINE_SMOOTH);
	glDisable (GL_POLYGON_SMOOTH );
	*/
}




IKFilter::IKFilter()
{

}

IKFilter::~IKFilter()
{

}

void IKFilter::apply()
{
	glUseProgramObjectARB( program_ );
}

void IKFilter::initProgram()
{
	program_ = fragment_ = 0;
	program_ = glCreateProgramObjectARB();

	
	String data;
	{
		FileInputStream fis(fragmentFilename_);
		fis >> data;
	}

	if( !data.empty() ) {

		fragment_ = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		AnsiString src = data;
		int len = src.size();
		const char* srcStr = src.c_str();
		glShaderSourceARB(fragment_, 1, (const GLcharARB**)&srcStr, &len);
		
		glCompileShaderARB(fragment_);
		
		int status = 0;
		glGetObjectParameterivARB(fragment_,GL_OBJECT_COMPILE_STATUS_ARB,&status);
		
		char* errString = (char*)malloc(32768);
		glGetInfoLogARB(fragment_,32768,NULL,errString);
		//s = String("Compile Log:\n") +  s2;
		::free(errString);
		//Dialog::showMessage( s );


		glAttachObjectARB(program_, fragment_);
		glLinkProgramARB(program_);
	}
}

};//namespace vcf




using namespace VCF;



class OGLView : public OpenGLControl {
public:

	GLuint frag;
	GLuint prog;

	OGLView() {

		

	}

	//GLuint texture;
	//GLuint tex2;

	//uint32 imWidth;
	//uint32 imHeight;

	IKImage img;

	virtual void sizeChange( ControlEvent* e ) {
		OpenGLControl::sizeChange(e);
		//makeCurrent();

		//doGL();

		//swapBuffers();
	}


	void doGL()
	{
		static initialized = false;

		if ( !initialized ) {


			initialized = true;
			//texture = -1;
			//tex2 = -1;

			//glewInit takes over 110 ms!!!??? Why so slow???
			//GLenum err = glewInit();
/*
			prog = glCreateProgramObjectARB();


			String s;
			{
				FileInputStream fis("frag.shader");
				fis >> s;
			}

			frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

			AnsiString src = s;
			int len = src.size();
			const char* srcStr = src.c_str();
			glShaderSourceARB(frag, 1, (const GLcharARB**)&srcStr, &len);

			glCompileShaderARB(frag);


			//int i;
			//glGetObjectParameterivARB(frag,GL_OBJECT_COMPILE_STATUS_ARB,&i);

			//char* s2 = (char*)malloc(32768);
			//glGetInfoLogARB(frag,32768,NULL,s2);
			//s = String("Compile Log:\n") +  s2;
			//::free(s2);
			//Dialog::showMessage( s );



			glAttachObjectARB(prog, frag);
			glLinkProgramARB(prog);

*/


/*
			
			Image* img = GraphicsToolkit::createImage( "res:logo.png" );
			if ( img ) {
				imWidth = img->getWidth();
				imHeight = img->getHeight();

				glGenTextures(1, &texture);
				glGenTextures(1, &tex2);
				
				
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use nice (linear) scaling
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Use nice (linear) scaling
				glTexImage2D(GL_TEXTURE_2D, 0, 4, img->getWidth(), img->getHeight(), 
					0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->getData() );

				delete img;
			}
			*/

			img.initFromFile( "res:logo.png" );
		}

		initialized = true;



		Rect r = getClientBounds();

		IKImageContext ic;

		/*
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, r.getWidth(), r.getHeight(), 0, -1, 1);
		glViewport( 0, 0, r.getWidth(), r.getHeight() );
		
		glDisable(GL_DEPTH_TEST);		

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		*/

		ic.initView( r.getWidth(), r.getHeight() );

		Color c = *getColor();
		glClearColor( c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha() );
		glClear(GL_COLOR_BUFFER_BIT);		
		
		static double a = 0;	
		
		ic.multiTransformMatrix( Matrix2D::rotation(a) * Matrix2D::scaling(1.3,1.3) );
		
		ic.draw( 10, 10, &img );


		ic.setTransformMatrix( Matrix2D::translation( 50, 100 ) );
		ic.setOpacity( sin( 0.1250 * (a/6.28) ) );
		ic.draw( 10, 10, &img );

		ic.setTransformMatrix( Matrix2D() );
		ic.draw( 30, 210, &img );
		ic.draw( 130, 310, &img );

		ic.multiTransformMatrix( Matrix2D::rotation(a) );
		ic.draw( 10, 210, &img );
		ic.draw( 231, 410, &img );

		a += 1.0;
		/*

		if ( texture != -1 ) {


			glBindTexture(GL_TEXTURE_2D, texture);
			float alpha = 1.0;
			glColor4f(1.0, 1.0, 1.0, alpha);

			uint32 h,w;
			w = imWidth;
			h = imHeight;

			int x,y;
			x = 10;
			y = 10;

			double u,v;
			u=0.5;
			v=0.5;

			glUseProgramObjectARB( prog );
			int u1 = glGetUniformLocationARB(prog, "myTexture");
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1iARB(u1, 0);







			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glVertex2i(x, y);

			glTexCoord2f(u, 0.0);
			glVertex2i(x+w, y);

			glTexCoord2f(u, v);
			glVertex2i(x+w, y+h);

			glTexCoord2f(0.0, v);
			glVertex2i(x, y+h);

			glEnd();



			x = 10;
			y = 100;
			u=1.0;
			v=1.0;

			w = imWidth/2;
			h = imHeight/2;

			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glVertex2i(x, y);

			glTexCoord2f(u, 0.0);
			glVertex2i(x+w, y);

			glTexCoord2f(u, v);
			glVertex2i(x+w, y+h);

			glTexCoord2f(0.0, v);
			glVertex2i(x, y+h);

			glEnd();



			x = 10;
			y = 200;
			u=1.0;
			v=1.0;

			w = imWidth;
			h = imHeight;


			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glVertex2i(x, y);

			glTexCoord2f(u, 0.0);
			glVertex2i(x+w, y);

			glTexCoord2f(u, v);
			glVertex2i(x+w, y+h);

			glTexCoord2f(0.0, v);
			glVertex2i(x, y+h);

			glEnd();


			x = 10;
			y = 300;
			u=1.0;
			v=1.0;

			w = imWidth;
			h = imHeight;

			glUseProgramObjectARB(0);

			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glVertex2i(x, y);

			glTexCoord2f(u, 0.0);
			glVertex2i(x+w, y);

			glTexCoord2f(u, v);
			glVertex2i(x+w, y+h);

			glTexCoord2f(0.0, v);
			glVertex2i(x, y+h);

			glEnd();


			glUseProgramObjectARB(prog);

			x = 10;
			y = 320;
			u=1.0;
			v=1.0;

			w = imWidth;
			h = imHeight;


			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0);
			glVertex2i(x, y);

			glTexCoord2f(u, 0.0);
			glVertex2i(x+w, y);

			glTexCoord2f(u, v);
			glVertex2i(x+w, y+h);

			glTexCoord2f(0.0, v);
			glVertex2i(x, y+h);

			glEnd();			
		}
*/
	}


	virtual void paint(GraphicsContext * context) {
		OpenGLControl::paint( context );

		doGL();

		swapBuffers();
	}
};

class OGLDrawWindow : public Window {
public:
	OGLDrawWindow() {
		setCaption( "OGLDraw" );		

		OGLView* view = new OGLView();
		add( view, AlignClient );
	}

	virtual ~OGLDrawWindow(){};

};




class OGLDrawApplication : public Application {
public:

	OGLDrawApplication( int argc, char** argv ) : Application(argc, argv) {
		
		OpenGLKit::init(argc,argv);
		ImageKit::init(argc,argv);
	}

	virtual bool initRunningApplication(){
		bool result = Application::initRunningApplication();
		
		

		Window* mainWindow = new OGLDrawWindow();
		setMainWindow(mainWindow);
		mainWindow->setBounds( 100.0, 100.0, 700.0, 500.0 );
		mainWindow->show();
		
		return result;
	}

	virtual void terminateRunningApplication() {
		ImageKit::terminate();
		Application::terminateRunningApplication();
	}
};


int main(int argc, char *argv[])
{
	return ApplicationKitMain<OGLDrawApplication>(argc,argv);
}


