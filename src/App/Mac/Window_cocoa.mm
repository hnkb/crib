
#include <utils/App>
#import <Cocoa/Cocoa.h>

using namespace crib::App;


namespace
{
	size_t windowCount = 0;
}


@interface WindowDelegate : NSObject <NSWindowDelegate>
@property(nonatomic, assign) crib::App::Window* appWindow;
@end

@interface MainView : NSView
@property(nonatomic, assign) crib::App::Window* appWindow;
@end


Window::Window() : Window({ "crib window", { 100, 100 }, { 800, 600 } })
{}

Window::Window(Options options)
{
	@autoreleasepool
	{
		if (options.size == float2(0))
			options.size = { 800, 600 };
		NSRect frame = NSMakeRect(options.pos.x, options.pos.y, options.size.x, options.size.y);

		NSUInteger style =
			NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
			| NSWindowStyleMaskMiniaturizable;

		native = [[NSWindow alloc]
			initWithContentRect:frame
					  styleMask:style
						backing:NSBackingStoreBuffered
						  defer:NO];

		if (!native)
			throw std::runtime_error("Failed to create window");

		auto delegate = [[WindowDelegate alloc] init];
		delegate.appWindow = this;
		[native setDelegate:delegate];
		// Window does not retain the delegate, so to compensate we won't release it here

		auto view = [[MainView alloc] initWithFrame:frame];
		view.appWindow = this;
		[view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
		[native setContentView:view];
		[view release];

		[native setTitle:[NSString stringWithUTF8String:options.title.c_str()]];
		[native setAcceptsMouseMovedEvents:YES];  // TODO: do we always want this?
		[native makeKeyAndOrderFront:nil];
		[native retain];
	}
	windowCount++;
}

Window::~Window()
{
	@autoreleasepool
	{
		[native close];

		[native setContentView:nil];
		[[native delegate] release];
		[native release];
	}
}

Window::Window(Window&& other) = default;
Window& Window::operator=(Window&& other) = default;


void Window::close()
{
	[native close];
}

Window::Options Window::options() const
{
	Options options;
	@autoreleasepool
	{
		NSRect currentFrame = [native frame];
		options.pos = { (float)currentFrame.origin.x, (float)currentFrame.origin.y };
		options.size = { (float)currentFrame.size.width, (float)currentFrame.size.height };

		NSString* currentTitle = [native title];
		options.title = [currentTitle UTF8String];
	}
	return options;
}

Window& Window::options(const Options& options)
{
	@autoreleasepool
	{
		NSRect frame = NSMakeRect(options.pos.x, options.pos.y, options.size.x, options.size.y);
		[native setFrame:frame display:YES animate:YES];
		[native setTitle:[NSString stringWithUTF8String:options.title.c_str()]];
	}
	return *this;
}


@implementation WindowDelegate

- (void)windowWillClose:(NSNotification*)notification
{
	if (--windowCount <= 0)
		[NSApp stop:nil];
}

- (void)windowDidMove:(NSNotification*)notification
{
	if (self.appWindow)
	{
		NSWindow* window = [notification object];
		NSPoint position = [window frame].origin;
		self.appWindow->onPositionChanged({ (float)position.x, (float)position.y });
	}
}

- (void)windowDidResize:(NSNotification*)notification
{
	if (self.appWindow)
	{
		NSWindow* window = [notification object];
		NSSize size = [window frame].size;
		self.appWindow->onSizeChanged({ (float)size.width, (float)size.height });
	}
}

@end


using Button = MouseEvent::Button;
using Type = MouseEvent::Type;

@implementation MainView

- (BOOL)isFlipped
{
	return YES;  // Invert the coordinate system to top-left origin
}

- (BOOL)acceptsFirstResponder
{
	return YES;  // Allow the view to receive input events, like keyboard and mouse movements
}

- (void)mouseDown:(NSEvent*)event
{
	[self handleMouseEvent:event type:Type::ButtonDown button:Button::Left];
}

- (void)mouseUp:(NSEvent*)event
{
	[self handleMouseEvent:event type:Type::ButtonUp button:Button::Left];
}

- (void)rightMouseDown:(NSEvent*)event
{
	[self handleMouseEvent:event type:Type::ButtonDown button:Button::Right];
}

- (void)rightMouseUp:(NSEvent*)event
{
	[self handleMouseEvent:event type:Type::ButtonUp button:Button::Right];
}

- (void)otherMouseDown:(NSEvent*)event
{
	auto button = [event buttonNumber] == 2 ? Button::Middle : Button::Other;
	[self handleMouseEvent:event type:Type::ButtonDown button:button];
}

- (void)otherMouseUp:(NSEvent*)event
{
	auto button = [event buttonNumber] == 2 ? Button::Middle : Button::Other;
	[self handleMouseEvent:event type:Type::ButtonUp button:button];
}

- (void)mouseMoved:(NSEvent*)event
{
	// NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
	// if (NSPointInRect(pos, [self bounds]))
	[self handleMouseEvent:event type:Type::Move button:Button::Other];
}

- (void)scrollWheel:(NSEvent*)event
{
	if ([event hasPreciseScrollingDeltas])
	{
		if (self.appWindow)
			self.appWindow->onSwipe(
				{ (float)[event scrollingDeltaX], (float)[event scrollingDeltaY] });
	}
	else
		[self handleMouseEvent:event type:Type::Wheel button:Button::Other];
}

- (void)handleMouseEvent:(NSEvent*)event type:(Type)type button:(Button)button
{
	if (self.appWindow)
	{
		NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
		self.appWindow->onMouseEvent(
			{ type,
			  button,
			  { (float)pos.x, (float)pos.y },
			  type == Type::Wheel ?
				  float2([event scrollingDeltaX], [event scrollingDeltaY]) :
				  float2(0) });
	}
}

- (void)magnifyWithEvent:(NSEvent*)event
{
	if (self.appWindow)
		self.appWindow->onPinch([event magnification]);
}

- (void)rotateWithEvent:(NSEvent*)event
{
	if (self.appWindow)
		self.appWindow->onRotate([event rotation]);
}

- (void)keyDown:(NSEvent*)event
{
	if (self.appWindow)
		self.appWindow->onKeyChar([[event charactersIgnoringModifiers] UTF8String]);
}

- (void)keyUp:(NSEvent*)event
{
	// NSLog(@"Key Up: %@", [event charactersIgnoringModifiers]);
}

@end
