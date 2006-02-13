//Browser.cpp


#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/ControlsKit.h"


namespace VCF {


	class URLCombo : public ComboBoxControl {
	public:
		DELEGATE(URLChanged);

		URLCombo(){
			edit_->KeyPressed += 
				new KeyboardEventHandler<URLCombo>( this, &URLCombo::onCBEditReturnKeyPressed, "URLCombo::onCBEditReturnKeyPressed" );
		}

		void onCBEditReturnKeyPressed( KeyboardEvent* event ) {
			if ( vkReturn == event->getVirtualCode() ) {
				Event e(this);
				URLChanged.fireEvent( &e );
			}
		}
	};



	class ActionManager : public Component {
	public:
		ActionManager( Component* owner ) : Component(owner){}
		ActionManager(){}
		virtual ~ActionManager(){}

		Action* addAction( const String& name ) {
			Action* action = new Action(this);
			action->setName( name );			
			return action;
		}

		void addTarget( const String& actionName, Component* component ) {
			Action* action = (Action*) findComponent( actionName );
			if ( NULL == action ) {
				action = addAction( actionName );
			}
			action->addTarget(component) ;
		}

		Action* getAction( const String& action ) {
			return (Action*) findComponent( action );
		}

		void addPerformed( const String& actionName, EventHandler* ev ) {
			Action* action = (Action*) findComponent( actionName );
			if ( NULL == action ) {
				action = addAction( actionName );
			}

			action->Performed += ev;
		}

		void addUpdate( const String& actionName, EventHandler* ev ) {
			Action* action = (Action*) findComponent( actionName );
			if ( NULL == action ) {
				action = addAction( actionName );
			}

			action->Update += ev;
		}
	};

};

using namespace VCF;



class CustomHTMLUI : public Window {
public:
	CustomHTMLUI() {
		browser = new HTMLBrowserControl();

		browser->URLLoaded += 
			new GenericEventHandler<CustomHTMLUI>(this,&CustomHTMLUI::onURLLoaded, "CustomHTMLUI::onURLLoaded" );

		add( browser, AlignClient );

		browser->setAllowsScrollbars(false);
		browser->setAllowsTextSelection(false);
		browser->setAllowDefaultContextMenu(false);
		
		

		String resHTML = System::findResourceDirectory() + "customUI.html";

		browser->setCurrentURL( resHTML );
	}

	HTMLBrowserControl* browser;
protected:
	void onURLLoaded( Event* e ) {
		//add callbacks for various UI elements here

		browser->setElementClickedEventHandler( "ClickMe", 
			new GenericEventHandler<CustomHTMLUI>(this,&CustomHTMLUI::onElementClicked, "CustomHTMLUI::onElementClicked" ) ); 

	}

	void onElementClicked( Event* e ) {
		HTMLElementEvent* htmElementEv = (HTMLElementEvent*)e;

		Dialog::showMessage( "Element \"" + htmElementEv->elementID + "\" clicked!" );
	}
};

class BrowserApp : public Application {
public:

	BrowserApp( int argc, char** argv ) : Application(argc, argv) {

	}
	

	virtual bool initRunningApplication(){
		bool result = Application::initRunningApplication();
		
		Window* mainWindow = new Window();
		setMainWindow(mainWindow);
		mainWindow->setBounds( 100.0, 100.0, 500.0, 500.0 );

		Toolbar* tb = new Toolbar();
		tb->setName( "Toolbar1" );
		tb->setHeight( 25 );
		mainWindow->add( tb, AlignTop );


		ImageList* il = new ImageList(mainWindow);
		il->setImageHeight( 17 );
		il->setImageWidth( 17 );
		il->setTransparentColor( &Color( (uchar)212, (uchar)208, (uchar)200 ) );		

		tb->setImageList( il );

		GraphicsResourceBundle* resBundle = getResourceBundle();
		Image* img = resBundle->getImage( "back.bmp" );
		il->addImage( img );
		delete img;

		img = resBundle->getImage( "forward.bmp" );
		il->addImage( img );
		delete img;

		img = resBundle->getImage( "refresh.bmp" );
		il->addImage( img );
		delete img;

		img = resBundle->getImage( "home.bmp" );
		il->addImage( img );
		delete img;

		img = resBundle->getImage( "go.bmp" );
		il->addImage( img );
		delete img;


		ToolbarItem* item = tb->addToolBarButton( "back" );
		item->setImageIndex(0);
		item->setTooltip( "Go back one page" );

		item = tb->addToolBarButton( "forward" );
		item->setImageIndex(1);
		item->setTooltip( "Go forward one page" );

		

		item = tb->addToolBarButton( "refresh" );
		item->setImageIndex(2);
		item->setTooltip( "Reload current page" );

		item = tb->addToolBarButton( "home" );
		item->setImageIndex(3);
		item->setTooltip( "Load home page" );


		item = tb->addToolBarButton( "url" );
		item->setEnabled( false );

		urlBox = new URLCombo();
		urlBox->setWidth( 175 );
		urlBox->setComboBoxStyle( cbsDropDownWithEdit );
		urlBox->URLChanged += 
			new GenericEventHandler<BrowserApp>(this,&BrowserApp::urlChanged,"BrowserApp::urlChanged");

		item->setItemControl( urlBox );

		

		item = tb->addToolBarButton( "go" );
		item->setImageIndex(4);
		item->setTooltip( "Load URL" );


		browser = new HTMLBrowserControl();
		browser->setDoubleBuffered( false );
		mainWindow->add( browser, AlignClient );

		browser->StatusChanged += 
			new GenericEventHandler<BrowserApp>(this,&BrowserApp::onHTMLStatusChanged, "BrowserApp::onHTMLStatusChanged" );



		statusLabel = new Label();
		statusLabel->setCaption( "Enter URL" );

		Panel* statPane = new Panel();
		statPane->setBorder( NULL );
		statPane->setHeight( statusLabel->getPreferredHeight() + 5 );
		statPane->add( statusLabel, AlignClient );
		
		mainWindow->add( statPane, AlignBottom );





		mgr = new ActionManager(mainWindow);




		MenuBar* menuBar = new MenuBar(mainWindow);
		mainWindow->setMenuBar( menuBar );
		MenuItem* root = menuBar->getRootMenuItem();
		DefaultMenuItem* app = new DefaultMenuItem( "Browser App", root, menuBar );

		DefaultMenuItem* appEVDemo = new DefaultMenuItem( "Event Handler Demo...", app, menuBar );
		

		mgr->addTarget( "appEVDemo", appEVDemo );
		mgr->addPerformed( "appEVDemo", new GenericEventHandler<BrowserApp>(this, &BrowserApp::doEventHandlerDemo, "BrowserApp::doEventHandlerDemo" ) );




		
		


		//hook up actions
		ToolbarModel* tbModel = tb->getToolbarModel();

		mgr->addTarget( "back", tbModel->getItemAtIndex(0) );
		mgr->addPerformed( "back", new GenericEventHandler<BrowserApp>(this, &BrowserApp::back, "BrowserApp::back" ) );

		
		mgr->addTarget( "forward", tbModel->getItemAtIndex(1) );
		mgr->addPerformed( "forward", 
			new GenericEventHandler<BrowserApp>(this, &BrowserApp::forward, "BrowserApp::forward" ) );

		mgr->addTarget( "refresh", tbModel->getItemAtIndex(2) );
		mgr->addPerformed( "refresh", 
			new GenericEventHandler<BrowserApp>(this, &BrowserApp::refresh, "BrowserApp::refresh" ) );

		mgr->addTarget( "home", tbModel->getItemAtIndex(3) );
		mgr->addPerformed( "home", 
			new GenericEventHandler<BrowserApp>(this, &BrowserApp::home, "BrowserApp::home" ) );

		mgr->addTarget( "go", tbModel->getItemAtIndex(5) );
		mgr->addPerformed( "go", 
			new GenericEventHandler<BrowserApp>(this, &BrowserApp::go, "BrowserApp::go" ) );



		browser->setCurrentURL( "http://vcf-online.org" );

		mainWindow->setCaption( "Browser" );

		mainWindow->show();
		
		return result;
	}

protected:
	HTMLBrowserControl* browser;
	URLCombo* urlBox;
	ActionManager* mgr;
	Label* statusLabel;

	void urlChanged( Event* ) {
		browser->setCurrentURL( urlBox->getCurrentText() );	
	}

	void go( Event* ) {
		browser->setCurrentURL( urlBox->getCurrentText() );
	}

	void back( Event* ) {
		browser->goBack();
	}

	void forward( Event* ) {
		browser->goForward();
	}

	void home( Event* ) {
		browser->goHome();
	}

	void refresh( Event* ) {
		browser->refresh();
	}

	void onHTMLStatusChanged( Event* e ) {
		HTMLEvent* htmlEvent = (HTMLEvent*)e;

		statusLabel->setCaption( htmlEvent->status );
	}

	void doEventHandlerDemo( Event* ) {
		CustomHTMLUI* window = new CustomHTMLUI();
		window->setBounds( 100, 100, 500, 500 );
		window->show();
	}
};


int main(int argc, char *argv[])
{
	Application* app = new BrowserApp( argc, argv );

	Application::main();
	
	return 0;
}


