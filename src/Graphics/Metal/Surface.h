
#pragma once

// #include <Graphics>
// #include <App>
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
// #import <QuartzCore/CAMetalLayer.h>


namespace crib::Graphics::Metal
{

	class Device  //: public Graphics::Surface
	{
	public:
		Device(NSWindow* window);
		~Device();
		// virtual ~Surface();

		void init();
		void draw();
		// virtual void draw() override;
		// virtual void resize(float2 dims) override;

	private:
		NSView* view;
		id<MTLDevice> device;
		id<MTLCommandQueue> commandQueue;
	};

}
