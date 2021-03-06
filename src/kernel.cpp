#include <zmq.hpp>
#include "kernel.hpp"

Kernel::Kernel(): m_window(nullptr), m_screenSurface(nullptr), m_GLContext(nullptr), m_renderer(nullptr), m_Gui(nullptr) {}

Kernel* Kernel:: getInstance( )
{
	if ( !instance )	
		instance = new Kernel();

	return instance;
}

void Kernel:: openWindow( std::string title, int width, int height, bool openGL, Uint32 flags )
{
	if ( m_window )
		throw( "Window already created" );

	//Initialize SDL Video subsystem
    if( SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0 )
		throw( std::runtime_error( std::string( "SDL failed to initialize video! SDL_Error: " ) + SDL_GetError() ) );
    else {
        //Create window
		m_width	 = width;
		m_height = height;

		if ( openGL )
			flags = flags | SDL_WINDOW_OPENGL;

        m_window = SDL_CreateWindow( "SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, flags );
        if ( m_window == NULL )
			throw( std::runtime_error( std::string( "SDL_CreateWindow failed! SDL_Error: " ) + SDL_GetError() ) );
        else
        {
            // Get window surface
            m_screenSurface = SDL_GetWindowSurface( m_window );

			// Clear window
			if ( openGL )
			{
				openGLContext();
//				glClearColor(0,0,0,1);
//				glClear(GL_COLOR_BUFFER_BIT);
			} else
			{
				m_renderer = SDL_CreateRenderer(m_window, -1, 0);
				SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
				SDL_RenderClear(m_renderer);
			}
            
            //Update the surface
            swapBuffers();
        }
	}
}

void Kernel :: startGui()
{
	if ( m_window && m_GLContext )
	{
		m_Gui = new Gui(m_window, m_width, m_height);
		m_Gui->addWindow("this is a test");
	} else
		throw(std::runtime_error(std::string("Did not have windows / GL context when attempting to create GUI! SDL_Error: ") + SDL_GetError()));
}

void Kernel :: openGLContext()
{
	m_GLContext = SDL_GL_CreateContext(m_window);

	// Load the GL functions we need
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		m_GLContext = nullptr;
		throw(std::runtime_error(std::string("Failure importing GL routines via GLAD! SDL_Error: ") + SDL_GetError()));
	}

	if ( !m_GLContext )
		throw( std::runtime_error( std::string( "Failed to create GL context! SDL_Error: " ) + SDL_GetError() ) );
}

void Kernel:: dropGLContext()
{
	if (m_GLContext)
		SDL_GL_DeleteContext(m_GLContext); 
}

void Kernel::eventLoop()
{
	// Setup zeromq context and socket and turn it in a zmq_pollitem_t
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:5555");
	zmq::pollitem_t connections [] = {
		{socket, 0, ZMQ_POLLIN, 0}
	};

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
			{
				SDL_Keycode key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE)
					done = true;
				break;
			}

			case SDL_QUIT:
				done = true;
				break;
			}

			if ( m_Gui )
				m_Gui->passEvents(event);

			drawGui();
			swapBuffers();

		}


		// Poll zmq connections
		// zmq::poll waits 40msec for events and thus sets the frame rate
		try {
			zmq::poll(&connections[0], 1, 40);
		} catch(std::exception &ex) {
			std::cout << ex.what() << std::endl << std::flush;
		}

		zmq::message_t message;
		if ( connections[0].revents & ZMQ_POLLIN ) {
			socket.recv( &message );
			std::cout << message.size() << std::endl;

			// Fire off threads

			// Mandatory reply, otherwise client stays blocked.
			zmq::message_t reply(4);
			memcpy(reply.data(), "Done", 4);
			socket.send(reply);
		}
	}
}

void Kernel :: drawGui()
{
	if ( m_Gui )
		m_Gui->draw();
}

void Kernel :: swapBuffers()
{
	if ( m_GLContext )
		SDL_GL_SwapWindow(m_window);
	else if ( m_renderer )
		SDL_RenderPresent(m_renderer);
	else
		throw( std::runtime_error( "Attempted to swap buffers without renderer" ) );
}

void Kernel :: closeWindow( )
{
	if ( m_window )
	{
		dropGLContext();
		SDL_DestroyWindow( m_window );
	}
}

Kernel ::  ~Kernel()
{
	if ( m_Gui )
	{
		delete m_Gui;
		m_Gui = nullptr;
	}

	closeWindow( );
}

Kernel* Kernel::instance = nullptr;
