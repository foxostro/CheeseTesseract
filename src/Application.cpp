#include "stdafx.h"
#include "FileFuncs.h"
#include "World.h"
#include "Actor.h"
#include "ScreenShot.h"
#include "Application.h"
#include "AnimationControllerFactory.h"
#include "devil_wrapper.h"

shared_ptr<AnimationControllerFactory> g_ModelFactory;
shared_ptr<Timer> g_FrameTimer;

Application *g_Application = 0;

/** @brief Application entry-point will immediately call this function.
 *  @return Return an integer exit status to the Operating System
 */
int mymain(void) {
	g_Application = new Application();
	g_Application->start();
	g_Application->run();
	g_Application->destroy();
	//delete g_Application;
	return EXIT_SUCCESS;
}

Application::Application()
: ScopedEventHandler(ScopedEventHandler::genName(), 0)
{
	resetMembers();
	REGISTER_HANDLER(Application::handleInputKeyPress);
	REGISTER_HANDLER(Application::handleActionApplicationQuit);
}

Application::~Application()
{
	destroy();
}

void Application::initializeFonts()
{
	font.open("Data/fonts/ttf-bitstream-vera-1.10/VeraMono.ttf", 16);
}

void Application::initializeAnimationControllerFactory()
{
	AnimationControllerFactory *factory = new AnimationControllerFactory(textureFactory);
	g_ModelFactory = shared_ptr<AnimationControllerFactory>(factory);
}

void Application::initializeFrameTimer()
{
	g_FrameTimer = shared_ptr<Timer>(new Timer);
	TRACE("Created the frame timer");
}

void Application::initializeSoundManager()
{
	soundSystem = shared_ptr<SoundSystem>(new SoundSystem(genName(), this));
	soundSystem->playMusic(FileName("data/music/RachelBerkowitz.mp3"));
	soundSystem->setMute(false);
	soundSystem->setSoundEffectVolume(0.7f);
	soundSystem->setMusicVolume(0.0f);
	registerSubscriber(soundSystem.get());
	TRACE("Sound system initialized");
}

void Application::initializeGameStateMachine()
{
	initializeGameWorld();

	GameStateMachine *s = new GameStateMachine(genName(),
											   this,
	                                           g_FrameTimer,
											   kernel,
											   world);

	gameStateMachine = shared_ptr<GameStateMachine>(s);
	registerSubscriber(gameStateMachine.get());

	TRACE("Game state machine initialized");
}

void Application::start()
{
    TRACE("Starting application...");

	FileName workingDirectory = getApplicationDirectory().append(FileName("../../"));
    setWorkingDirectory(workingDirectory);
	SDL_Init(SDL_INIT_EVERYTHING);
	dInitODE();
	initializeRenderer();
	initializeDevIL();
	
	initializeJoystickDevices();
	initializeAnimationControllerFactory();
	initializeFonts();
	srand(SDL_GetTicks());
	initializeFrameTimer();
	initializeSoundManager();
	initializeInputSubsystem();
	initializeGameStateMachine();

	TRACE("Application start-up completed");
}

void Application::initializeDevIL()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
}

void Application::closeJoysticks()
{
	for(vector<Joystick>::iterator i=joysticks.begin();
		i!=joysticks.end();
		++i)
	{
		SDL_Joystick *joystick = i->handle;

		if(joystick)
		{
			SDL_JoystickClose(joystick);
		}
	}

	joysticks.clear();
}

void Application::initializeJoystickDevices()
{
	closeJoysticks();

	const int numJoysticks = SDL_NumJoysticks();
	for(int which=0; which<numJoysticks; ++which)
	{
		SDL_Joystick *handle = SDL_JoystickOpen(which);

		if(handle)
		{
			TRACE(string("Joystick ") + SDL_JoystickName(which) + " Successfully opened.");
		}

		joysticks.push_back(Joystick(handle, which));
	}
}

void Application::tick( float timeStep )
{
	PROFILE("Total Tick");

	updateScene(timeStep);
	drawScene();

	if(movieMode)
	{
		takeScreenShot("arfox");
	}
}

void Application::updateScene( float timeStep )
{
	PROFILE("Update Scene");
	renderer->setupScene();
	input->poll();
	gameStateMachine->update(timeStep);
	renderer->tick(timeStep);
	kernel.update(timeStep);
}

void Application::drawScene()
{
	PROFILE("Render Scene");

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	renderer->changeCamera(camera);
	renderer->drawScene();
	world->draw();

	//renderTreesDirectly();

	render_text_queue();

	glFlush();
	SDL_GL_SwapBuffers();
}

void Application::run()
{
	ASSERT(g_FrameTimer,     "Frame timer was null");
	ASSERT(gameStateMachine, "Member \"gameStateMachine\" was null");

	// Lock game update rate at 30 fps
	const double rate = 1.0 / 30.0 * 1000.0;

	while(!quit)
	{
		tick((float)rate); // fixed time step for physics

		// Generate text output of the in-game profiler
		generate_profiler_text();
		
		// Finish processing and possibly stall to maintain constant rate
		while(g_FrameTimer->getElapsedTimeMS() < rate);
		
		g_FrameTimer->update();
	}
}

void Application::destroy()
{
	TRACE("Shutting down application...");

	kernel.destroy();
	TRACE("Kernel has been shutdown");

	soundSystem.reset();
	TRACE("Sound subsystem has been shutdown");

	input.reset();
	TRACE("Input subsystem has been shutdown");

	renderer.reset();
	TRACE("Renderer has been shutdown");

	dCloseODE();
	TRACE("Physics subsystem has been shutdown");

	SDL_Quit();
	TRACE("SDL libraries have been shutdown");

	resetMembers();
	TRACE("...shutdown completed");
}

void Application::resetMembers()
{
	quit = false;
	movieMode = false;
	soundSystem.reset();
	input.reset();
	g_FrameTimer.reset();
}

void Application::initializeRenderer()
{
	renderer = shared_ptr<Renderer>(new Renderer(genName(), this));
	registerSubscriber(renderer.get());
	TRACE("Renderer initialized");
}

void Application::handleInputKeyPress(const InputKeyPress *input)
{
	switch(input->key)
	{
	case SDLK_q:
		{
			ActionApplicationQuit action;
			sendGlobalAction(&action);
		} break;

	case SDLK_F11: takeScreenShot("screen"); break;
	}
}

void Application::initializeInputSubsystem()
{
	input = shared_ptr<SDLinput>(new SDLinput(genName(),
	                                          this,
	                                          joysticks));
	
	registerSubscriber(input.get());
}

void Application::handleActionApplicationQuit(const ActionApplicationQuit *)
{
	quit = true;
}

void Application::initializeGameWorld()
{
	// Create the game world
	world = shared_ptr<World>(new World(genName(),
	                                    this,
										renderer,
	                                    textureFactory,
	                                    &camera));

	registerSubscriber(world.get());
	TRACE("Created the game world");
}

void Application::render_text_queue()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Save model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Save projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Set up ortho rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Write text and empty the queue
	while(!text_to_draw.empty())
	{
		font.drawText(text_to_draw.front().position.x,
		              text_to_draw.front().position.y,
					  text_to_draw.front().text.c_str());
		text_to_draw.pop();
	}

	// Restore model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Restore settings
	glPopAttrib();
}

void Application::queue_text_for_draw(const vec2 &position,
									  const string &text)
{
	string_to_draw s;
	s.position = position;
	s.text = text;
	text_to_draw.push(s);
}

void Application::record_profile_entry(const string &tag, double elapsed)
{
	profile_entries[tag] += elapsed;
}

void Application::generate_profiler_text()
{
	vec2 position = vec2(100, 100);
	vec2 delta = vec2(0, -20);

	for(map<string, double>::const_iterator i = profile_entries.begin();
		i != profile_entries.end(); ++i, position = position + delta)
	{
		const string &tag = i->first;
		const double &elapsed = i->second;

		string text = fitToFieldSize(tag + ":", ' ', 16, JUSTIFY_LEFT)
		            + dtos(elapsed)
					+ "ms";

		queue_text_for_draw(position, text);
	}

	profile_entries.clear();
}
