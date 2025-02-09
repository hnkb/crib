
#include "MetalDevice.h"
#import <QuartzCore/CAMetalLayer.h>
#include <string>

using namespace crib::Graphics::Metal;


Device::Device(NSWindow* window)
{
	@autoreleasepool
	{
		[window.contentView setWantsLayer:YES];

		auto metalLayer = [CAMetalLayer layer];
		[metalLayer setDevice:MTLCreateSystemDefaultDevice()];
		[metalLayer setPixelFormat:MTLPixelFormatBGRA8Unorm];
		[metalLayer setFramebufferOnly:YES];
		[window.contentView setLayer:metalLayer];

		// view = [window.contentView retain];
		view = window.contentView;

		// Save the Metal device and command queue
		device = [metalLayer device];
		commandQueue = [device newCommandQueue];

		{
			// create a description of the device as std::string
			std::string desc;
			desc += [[device name] UTF8String];
			desc += " (";
			desc += [device isLowPower] ? "low-power" : "high-performance";
			desc += [device hasUnifiedMemory] ? ", unified memory" : "";
			desc += ")\n";
			desc += "  Max Threadgroup Memory Length: ";
			double maxThreadgroupMemoryLength =
				floor(10. * [device maxThreadgroupMemoryLength] / 1024.) / 10.;
			desc += std::to_string(maxThreadgroupMemoryLength);
			desc += " KB\n";
			if ([device respondsToSelector:@selector(recommendedMaxWorkingSetSize)])
			{
				desc += "  Recommended Max Working Set Size: ";
				double recommendedMaxWorkingSetSize =
					floor(10. * [device recommendedMaxWorkingSetSize] / 1024. / 1024. / 1024.)
					/ 10.;
				desc += std::to_string(recommendedMaxWorkingSetSize);
				desc += " GB\n";
			}
			printf("%s", desc.c_str());

			// // Display device name
			// NSLog(@"Metal Device Name: %@", [device name]);

			// if ([device isLowPower])
			// 	NSLog(@"Device is a low-power device (e.g., integrated GPU).");
			// else
			// 	NSLog(@"Device is a high-performance GPU.");

			// if ([device hasUnifiedMemory])
			// 	NSLog(@"Device supports unified memory.");
			// else
			// 	NSLog(@"Device does not support unified memory.");

			// NSLog(
			// 	@"Maximum Threadgroup Memory Length: %lu bytes",
			// 	(unsigned long)[device maxThreadgroupMemoryLength]);

			// if ([device respondsToSelector:@selector(recommendedMaxWorkingSetSize)])
			// {
			// 	NSLog(
			// 		@"Recommended Max Working Set Size: %llu bytes",
			// 		[device recommendedMaxWorkingSetSize]);
			// }
		}
	}
}

Device::~Device()
{
	@autoreleasepool
	{
		// [view release];
		[commandQueue release];
	}
}


const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            half3 color;
        };

        v2f vertex vertex_main( uint vertexId [[vertex_id]],
                               device const float3* positions [[buffer(0)]],
                               device const float3* colors [[buffer(1)]] )
        {
            v2f o;
            o.position = float4( positions[ vertexId ], 1.0 );
            o.color = half3 ( colors[ vertexId ] );
            return o;
        }

        half4 fragment fragment_main( v2f in [[stage_in]] )
        {
            return half4( in.color, 1.0 );
        }
    )";

void Device::draw()
{
	printf("Drawing here\n");
	//// prepare resources

//>>>>>>>>>>> generic buffer

	// Define the vertex data for a simple triangle
	static const float triangleVertices[] = {
		0.0f,  0.5f,  0.0f, 1.0f,  // Top vertex
		-0.5f, -0.5f, 0.0f, 1.0f,  // Bottom left vertex
		0.5f,  -0.5f, 0.0f, 1.0f   // Bottom right vertex
	};

// >>>>>>>>>>> metal buffer
	// Store these in a vertex buffer once, outside the draw function
	id<MTLBuffer> vertexBuffer = [device
		newBufferWithBytes:triangleVertices
					length:sizeof(triangleVertices)
				   options:MTLResourceStorageModeShared];

// >>>>>>>>>>> metal shader
	// Set up shaders (define them in your `.metal` file)
	NSError* error = nil;
	NSString* source = [NSString stringWithUTF8String:shaderSrc];
	id<MTLLibrary> library = [device newLibraryWithSource:source options:nil error:&error];

	// id<MTLLibrary> library = [device newDefaultLibrary];
	id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertex_main"];
	id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragment_main"];



	/// temporary to make things work
	CAMetalLayer* metalLayer = (CAMetalLayer*)[view layer];

	// Create a render pipeline descriptor
	MTLRenderPipelineDescriptor* pipelineDescriptor =
		[[MTLRenderPipelineDescriptor alloc] init];
	pipelineDescriptor.vertexFunction = vertexFunction;
	pipelineDescriptor.fragmentFunction = fragmentFunction;
	pipelineDescriptor.colorAttachments[0].pixelFormat = metalLayer.pixelFormat;

	// Create the pipeline state (once, outside the draw function)
	id<MTLRenderPipelineState> pipelineState =
		[device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:nil];


// >>>>>>>>>>> frame begin -> check swap chain size and get next drawable
	constexpr auto superSamplingScale = 2.;
	auto frame = [view frame];
	// auto frame2 = [metalLayer frame];
	auto drawableSize = [metalLayer drawableSize];
	// auto factor = [metalLayer contentsScale];
	// auto factor2 = [view backingScaleFactor];
	// auto factor3 = [view window].backingScaleFactor;
	auto physicalSize = [view convertSizeToBacking:frame.size];
	physicalSize.width = physicalSize.width * superSamplingScale;
	physicalSize.height = physicalSize.height * superSamplingScale;
	if (drawableSize.width != physicalSize.width || drawableSize.height != physicalSize.height)
	{
		printf("Resizing frame buffer to %f %f\n", physicalSize.width, physicalSize.height);
		[metalLayer setDrawableSize:physicalSize];
	}
	// printf("Frame: %f %f %f %f\n", frame.origin.x, frame.origin.y, frame.size.width,
	// frame.size.height); printf("Frame2: %f %f %f %f\n", frame2.origin.x, frame2.origin.y,
	// frame2.size.width, frame2.size.height); printf("Drawable size: %f %f\n",
	// drawableSize.width, drawableSize.height); printf("Factor 1,2,3: %f %f %f\n", factor,
	// factor2, factor3); printf("Physical size: %f %f\n", physicalSize.width,
	// physicalSize.height);

	// Get the Metal layer and current drawable
	// CAMetalLayer* metalLayer = (CAMetalLayer*)[view layer];
	id<CAMetalDrawable> drawable = [metalLayer nextDrawable];
	if (!drawable)
		return;

	// Create a render pass descriptor
	MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
	passDescriptor.colorAttachments[0].texture = drawable.texture;
	passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
	passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.8, 0.3, 0.22, 1.0);
	passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

	// Create a command buffer
	id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];

	// Create a render command encoder
	id<MTLRenderCommandEncoder> encoder =
		[commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];



	//// actual drawing
	{
		[encoder setRenderPipelineState:pipelineState];
		[encoder setVertexBuffer:vertexBuffer offset:0 atIndex:0];

		// Draw the triangle
		[encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
	}



	// Finish encoding
	[encoder endEncoding];

	// Present the drawable and commit the command buffer
	[commandBuffer presentDrawable:drawable];
	[commandBuffer commit];
}

// void Surface::resize(float2 dims)
// {
// 	NSRect frame = NSMakeRect(0, 0, dims.x, dims.y);
// 	[metalView setFrame:frame];

// 	// Update Metal layer properties (if necessary)
// 	CAMetalLayer* metalLayer = (CAMetalLayer*)[metalView layer];
// 	metalLayer.drawableSize = CGSizeMake(dims.x, dims.y);

// 	draw();
// }
