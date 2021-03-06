#pragma once

#include "types.h"
#include <string>
#include <memory>
#include <functional>
#include "windowhandle.h"


class Window;

class WindowConfig {

public:

	enum class OpenGLProfile {
		Core,
		Compatibility,
		Any
	};

	constexpr WindowConfig() : 
		openglMajor(3), openglMinor(3), profile(OpenGLProfile::Core), forwardContext(true), debugContext(false), srgbRendering(true),
		samples(1), redBits(8), greenBits(8), blueBits(8), alphaBits(8), depthBits(24), stencilBits(8),
		resizable(false), maximized(false), alwaysOnTop(false) {}

	WindowConfig& setOpenGLVersion(u32 major, u32 minor);
	WindowConfig& setOpenGLProfile(OpenGLProfile profile);
	WindowConfig& setOpenGLContextMode(bool forward, bool debug);
	WindowConfig& setSRGBMode(bool enable);
	WindowConfig& setSamples(u8 samples);
	WindowConfig& setFramebuffer(u8 redBits, u8 greenBits, u8 blueBits, u8 alphaBits, u8 depthBits, u8 stencilBits);
	WindowConfig& setResizable(bool enable);
	WindowConfig& setMaximized(bool enable);
	WindowConfig& setAlwaysOnTop(bool enable);

private:
	friend class Window;

	u32 openglMajor;
	u32 openglMinor;
	OpenGLProfile profile;
	bool forwardContext;
	bool debugContext;
	bool srgbRendering;
	u32 samples;
	u8 redBits;
	u8 greenBits;
	u8 blueBits;
	u8 alphaBits;
	u8 depthBits;
	u8 stencilBits;
	bool resizable;
	bool maximized;
	bool alwaysOnTop;

};



struct Monitor {

	u32 monitorID;
	u32 virtualX;
	u32 virtualY;
	u32 workX;
	u32 workY;
	u32 workW;
	u32 workH;
	u32 physicalW;
	u32 physicalH;
	u32 width;
	u32 height;
	u32 refreshRate;
	std::string name;

};


enum class WindowState {
	CloseRequest,
	Minimized,
	Restored,
	Focused,
	Unfocused,
	Maximized
};

struct WindowHandle;
struct GLFWmonitor;

typedef std::function<void(u32, u32)> WindowMoveFunction;
typedef std::function<void(u32, u32)> WindowResizeFunction;
typedef std::function<void(WindowState)> WindowStateChangeFunction;
typedef std::function<void(u32, u32)> FramebufferResizeFunction;

class Window final {

public:

	Window();
	~Window();

	Window(const Window& window) = delete;
	Window& operator=(const Window& window) = delete;

	void setWindowConfig(const WindowConfig& config);
	bool create(u32 w, u32 h, const std::string& title);
	bool createFullscreen(const std::string& title, u32 monitorID = 0);
	void close();

	void setWindowed();
	void setFullscreen(u32 monitorID = 0);

	void setSize(u32 w, u32 h);
	void setTitle(const std::string& title);
	void setX(u32 x);
	void setY(u32 y);
	void setPosition(u32 x, u32 y);
	void setLimits(u32 minW, u32 minH, u32 maxW, u32 maxH);
	void setMinLimits(u32 minW, u32 minH);
	void setMaxLimits(u32 maxW, u32 maxH);
	void setAspectRatio(double aspect);
	void setOpacity(double opacity);
	//void setIcon(); //Define when textures are implemented

	u32 getWidth() const;
	u32 getHeight() const;
	u32 getFramebufferWidth() const;
	u32 getFramebufferHeight() const;
	u32 getX() const;
	u32 getY() const;

	//void getIcon();

	void minimize();
	void restore();
	void maximize();
	void show();
	void hide();
	void focus();
	void requestAttention();
	void disableConstraints();

	static void pollEvents();
	void swapBuffers();

	bool isOpen() const;
	bool isFullscreen() const;
	bool closeRequested() const;
	bool focused() const;
	bool minimized() const;
	bool maximized() const;
	bool hovered() const;
	bool visible() const;
	bool resizable() const;
	bool decorated() const;
	bool alwaysOnTop() const;
	

	void enableContext();
	void disableContext();
	void enableVSync();
	void disableVSync();

	void requestClose();
	void dismissCloseRequest();

	static u32 getMonitorCount();
	static Monitor getMonitor(u32 id);
	static bool monitorConfigurationChanged();

	void setWindowMoveFunction(WindowMoveFunction function);
	void setWindowResizeFunction(WindowResizeFunction function);
	void setWindowStateChangeFunction(WindowStateChangeFunction function);
	void setFramebufferResizeFunction(FramebufferResizeFunction function);
	void resetWindowFunctions();

	std::weak_ptr<WindowHandle> getInternalHandle() const;

	static bool initialize();
	static void shutdown();

private:

	static void initMonitorCallback();
	static void queryMonitors();
	static bool setupGLContext();

	static GLFWmonitor** connectedMonitors;
	static bool monitorsChanged;
	static u32 monitorCount;

	WindowConfig contextConfig;

	std::shared_ptr<WindowHandle> windowHandle;

	u32 backupWidth;
	u32 backupHeight;

	WindowMoveFunction moveFunction;
	WindowResizeFunction resizeFunction;
	WindowStateChangeFunction stateChangeFunction;
	FramebufferResizeFunction fbResizeFunction;

};