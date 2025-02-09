
#include <crib/App>
#import <Cocoa/Cocoa.h>


int crib::App::run()
{
	@autoreleasepool
	{
		[NSApplication sharedApplication];


		// Create a menu bar
		NSMenu* menuBar = [[NSMenu alloc] init];
		NSMenuItem* appMenuItem = [[NSMenuItem alloc] init];
		[menuBar addItem:appMenuItem];
		[NSApp setMainMenu:menuBar];

		// Create the application menu
		NSMenu* appMenu = [[NSMenu alloc] init];
		NSMenuItem* quitMenuItem = [[NSMenuItem
			alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
		[appMenu addItem:quitMenuItem];
		[appMenuItem setSubmenu:appMenu];


		[NSApp run];

		// Ensure that any pending events, timers, or autoreleased objects tied to the current
		// run loop iteration are processed and cleared out immediately.
		[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
	}
	return 0;
}
