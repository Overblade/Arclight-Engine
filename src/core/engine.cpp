#include "core/engine.h"
#include "util/file.h"
#include "util/log.h"
#include "thread/concurrentqueue.h"


Engine::Engine() : profiler(Time::Unit::Seconds, 3) {}



bool Engine::initialize() {

	Uri::setApplicationUriRoot(Config::getUriRootPath());

	//Open up log file
	Log::openLogFile();
	Log::info("Core", "Setting up engine");

	//Initialize backend
	if (!initializeBackend()) {
		Log::error("Core", "Library backend initialization failed");
		return false;
	}

	//Start engine profiler
	profiler.start();

	//Create the main window
	if (!createWindow()) {
		Log::error("Core", "Failed to create window");
		return false;
	}

	window.setFramebufferResizeFunction([this](u32 w, u32 h) {onFBResize(w, h); });

	Log::info("Core", "Window successfully created");

	//Setup input system and input handler
	setupInputSystem();
	Log::info("Core", "Input system successfully set up");

	//Initialize audio engine
	if (!audioEngine.initialize()) {
		Log::error("Core", "Audio engine failed to initialize");
		return false;
	}

	Log::info("Core", "Audio engine initialized");

	//Create render test
	renderTest.create(window.getFramebufferWidth(), window.getFramebufferHeight());

	//Disable V-Sync
	//window.disableVSync();

	//We're successfully running
	Log::info("Core", "Starting engine");

	//Start FPS tracker
	tracker.start();

	ConcurrentQueue<int> queue;
	queue.push(3);
	queue.push(2);
	queue.push(1);
	int a = queue.pop();
	int b = queue.pop();
	int c = queue.pop();
	int d = queue.pop();
	Log::info("", "%d %d %d %d", a, b, c, d);

	return true;

}



void Engine::run() {

	//Create the tick timer
	Timer timer;
	timer.start();

	//Variables to store current tick state
	u64 lastTime = timer.getElapsedTime();
	u64 accum = 0;

	//Play sounds
	//audioEngine.playSound(":/sounds/rickroll.mp3");

	//Loop until window close event is requested
	while (!window.closeRequested()) {

		//Calculate delta time and add to accumulator
		u64 delta = timer.getElapsedTime() - lastTime;
		lastTime = timer.getElapsedTime();
		accum += delta;
		u32 ticks = 0;

		//Get tick count on accumulator overflow
		if (accum > 16667) {
			ticks = accum / 16667;
			accum %= 16667;
		}

		//Update window and input system
		window.pollEvents();
		inputSystem.updateContinuous(ticks);

		//Update audio engine
		audioEngine.update();

		//Run render test
		renderTest.run();

		//Swap render buffers
		window.swapBuffers();

		//Debug FPS
		window.setTitle(Config::getBaseWindowTitle() + " | FPS: " + std::to_string(tracker.getFPS()));

	}

}



void Engine::shutdown() {

	Log::info("Core", "Shutting down engine");

	//Close instances
	renderTest.destroy();
	audioEngine.shutdown();
	inputSystem.disconnect();
	window.close();

	//Stop the profiler and get engine runtime
	profiler.stop();

	Log::info("Core", "Shutting down backend");

	//Shutdown engine backend libraries
	shutdownBackend();

	Log::info("Core", "Bye");

	//Finally close log file
	Log::closeLogFile();

}



bool Engine::initializeBackend() {
	return Window::initialize();
}



void Engine::shutdownBackend() {
	Window::shutdown();
}



bool Engine::createWindow() {

	window.setWindowConfig(WindowConfig().setResizable(true).setOpenGLVersion(3, 3));
	return window.create(Config::getDefaultWindowWidth(), Config::getDefaultWindowHeight(), Config::getBaseWindowTitle());

}



void Engine::setupInputSystem() {

	//Connect the input system to the window in order to receive events
	inputSystem.connect(window);
	inputSystem.disableCursor();

	//Create the root context and add actions to it
	InputContext& rootContext = inputSystem.createContext(0);
	rootContext.addState(0);

	rootContext.addAction(RenderTest::ActionID::CameraRotRight,		KeyTrigger({ KeyCode::Right }), true);
	rootContext.addAction(RenderTest::ActionID::CameraRotLeft,		KeyTrigger({ KeyCode::Left }), true);
	rootContext.addAction(RenderTest::ActionID::CameraRotDown,		KeyTrigger({ KeyCode::Down }), true);
	rootContext.addAction(RenderTest::ActionID::CameraRotUp,		KeyTrigger({ KeyCode::Up }), true);
	rootContext.addAction(RenderTest::ActionID::CameraMoveLeft,		KeyTrigger({ KeyCode::A }), true);
	rootContext.addAction(RenderTest::ActionID::CameraMoveRight,	KeyTrigger({ KeyCode::D }), true);
	rootContext.addAction(RenderTest::ActionID::CameraMoveBackward,	KeyTrigger({ KeyCode::S }), true);
	rootContext.addAction(RenderTest::ActionID::CameraMoveForward,	KeyTrigger({ KeyCode::W }), true);
	rootContext.addAction(RenderTest::ActionID::CameraMoveDown,		KeyTrigger({ KeyCode::Q }), true);
	rootContext.addAction(RenderTest::ActionID::CameraMoveUp,		KeyTrigger({ KeyCode::E }), true);
	rootContext.addAction(RenderTest::ActionID::CameraSpeedUp,		KeyTrigger({ KeyCode::LeftShift }, KeyState::Pressed), false);
	rootContext.addAction(RenderTest::ActionID::CameraSlowDown,		KeyTrigger({ KeyCode::LeftShift }, KeyState::Released), false);
	rootContext.addAction(RenderTest::ActionID::CameraGrab,			KeyTrigger({ Mouse::Right }), false);
	rootContext.addAction(RenderTest::ActionID::CameraGrabScroll,	KeyTrigger({ Mouse::Right }), true);
	rootContext.addAction(RenderTest::ActionID::FovIn,				KeyTrigger({ KeyCode::NumPad0 }, KeyState::Pressed), false);
	rootContext.addAction(RenderTest::ActionID::FovOut,				KeyTrigger({ KeyCode::NumPad0 }, KeyState::Released), false);

	rootContext.addAction(RenderTest::ActionID::QuickScreenshot,	KeyTrigger({ KeyCode::F2 }), false);
	rootContext.addAction(RenderTest::ActionID::ReloadShaders,		KeyTrigger({ KeyCode::F3 }, KeyState::Released), false);
	rootContext.addAction(RenderTest::ActionID::ReloadResources,	KeyTrigger({ KeyCode::F4 }, KeyState::Released), false);
	rootContext.addAction(RenderTest::ActionID::ToggleDebug,		KeyTrigger({ KeyCode::F5 }), false);

	rootContext.registerAction(0, RenderTest::ActionID::CameraRotLeft);
	rootContext.registerAction(0, RenderTest::ActionID::CameraRotRight);
	rootContext.registerAction(0, RenderTest::ActionID::CameraRotDown);
	rootContext.registerAction(0, RenderTest::ActionID::CameraRotUp);
	rootContext.registerAction(0, RenderTest::ActionID::CameraMoveLeft);
	rootContext.registerAction(0, RenderTest::ActionID::CameraMoveRight);
	rootContext.registerAction(0, RenderTest::ActionID::CameraMoveBackward);
	rootContext.registerAction(0, RenderTest::ActionID::CameraMoveForward);
	rootContext.registerAction(0, RenderTest::ActionID::CameraMoveDown);
	rootContext.registerAction(0, RenderTest::ActionID::CameraMoveUp);
	rootContext.registerAction(0, RenderTest::ActionID::CameraSpeedUp);
	rootContext.registerAction(0, RenderTest::ActionID::CameraSlowDown);
	rootContext.registerAction(0, RenderTest::ActionID::CameraGrab);
	rootContext.registerAction(0, RenderTest::ActionID::CameraGrabScroll);
	rootContext.registerAction(0, RenderTest::ActionID::FovIn);
	rootContext.registerAction(0, RenderTest::ActionID::FovOut);

	rootContext.registerAction(0, RenderTest::ActionID::QuickScreenshot);
	rootContext.registerAction(0, RenderTest::ActionID::ReloadShaders);
	rootContext.registerAction(0, RenderTest::ActionID::ReloadResources);
	rootContext.registerAction(0, RenderTest::ActionID::ToggleDebug);

	//Define input handler callbacks
	inputHandler.setCoActionListener([this](KeyAction action, double scale) {
		//Log::info("Input Context", "Action triggered: %d", action);
		renderTest.onKeyAction(action);
		return true;
	});

	inputHandler.setActionListener([this](KeyAction action) {
		//Log::info("Input Context", "Action triggered: %d", action);
		renderTest.onKeyAction(action);
		return true;
	});

	inputHandler.setKeyListener([this](Key key, KeyState state) {
		//Log::info("Input Context", "Key triggered: %d, %d", key, state);
		return true;
	});

	inputHandler.setCharListener([this](KeyChar character) {
		//Log::info("Input Context", "Char obtained: %d", character);
		return true;
	});

	inputHandler.setCursorListener([this](double x, double y) {
		//Log::info("Input Context", "Cursor at: %f, %f", x, y);
		renderTest.onMouseScroll(x, y);
		return true;
	});

	inputHandler.setScrollListener([this](double x, double y) {
		//Log::info("Input Context", "Scrolled: %f, %f", x, y);
		renderTest.onScroll(y);
		return true;
	});

	//Link handler to the root context
	rootContext.linkHandler(inputHandler);

}



void Engine::onFBResize(u32 w, u32 h) {
	renderTest.resizeWindowFB(w, h);
}