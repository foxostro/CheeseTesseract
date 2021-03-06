#ifndef APP_H
#define APP_H

#include "myassert.h"
#include "FrameTimer.h"
#include "Task.h"
#include "SoundSystem.h"
#include "TextureFactory.h"
#include "Camera.h"
#include "Kernel.h"
#include "SDLinput.h"
#include "ScopedEventHandler.h"
#include "GameStateMachine.h"
#include "Renderer.h"
#include "ActionApplicationQuit.h"
#include "ProfileScope.h"

#include "GLFT_Font.hpp"

/**
The Application class may be considered the entry point of the application.
While the main or WinMain functions (depending on the OS we are compiling for)
are used to enter the application from the Operating System kernel, the only
purpose they serve is to immediately instantiate an Application type object.
*/
class Application : public ScopedEventHandler {
public:
	Application();
	
	~Application();
	
	/** Run the game loop */
	void run();
	
	/** Initialization code to run before the game loop */
	void start();
	
	/** Shutdown code to run after the game loop exits */
	void destroy();
	
	/** Tag -> Total Time */
	map<string, double> profile_entries;
	
	void record_profile_entry(const string &tag, double elapsed);
	
private:
	/**
	Tick the scene: Update and Draw
	@param timeStep Time since the last update
	*/
	void tick(float timeStep);
	
	/** Renders the scene and swaps buffers to display it on-screen */
	void drawScene();
	
	/**
	Updates the scene
	@param timeStep Time since the last update
	*/
	void updateScene(float timeStep);
	
	/** Reset the state of the application to a "just-constructed" state */
	void resetMembers();
	
	/** Start DevIL */
	void initializeDevIL();
	
	/** Start OpenGL */
	void initializeRenderer();
	
	/** Closes all joysticks */
	void closeJoysticks();
	
	void initializeJoystickDevices();
	void initializeFonts();
	void initializeAnimationControllerFactory();
	void initializeFrameTimer();
	void initializeSoundManager();
	void initializeInputSubsystem();
	void initializeGameStateMachine();
	void initializeGameWorld();
	
	void handleInputKeyPress(const InputKeyPress *input);
	void handleActionApplicationQuit(const ActionApplicationQuit *action);
	
	/** Encapsulates text render parameters */
	struct string_to_draw {
		vec2 position;
		string text;
	};
	
	/** Queue of text to render at the render step */
	queue<string_to_draw> text_to_draw;
	
	/**
	Adds text to the queue to be drawn at the render step of the frame
	@param position Position of the text on-screen
	@param text String
	*/
	void queue_text_for_draw(const vec2 &position, const string &text);
	
	/** Render queued text to the screen in one shot */
	void render_text_queue();
	
	/** Generates text from profiler entries */
	void generate_profiler_text();
	
private:
	Camera camera;
	shared_ptr<GameStateMachine> gameStateMachine;
	shared_ptr<SoundSystem> soundSystem;
	shared_ptr<SDLinput> input;
	bool quit;
	Kernel kernel;
	vector<Joystick> joysticks;
	bool movieMode;
	GLFT_Font font;
	TextureFactory textureFactory;
	shared_ptr<Renderer> renderer;
	shared_ptr<World> world;
};

#endif
