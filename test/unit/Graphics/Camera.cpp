
#include <crib/Graphics>
#include <doctest.h>

using namespace crib;
using Graphics::Camera;


// utilites for easier Math usage with doctest

namespace crib
{
	doctest::String toString(const float2& val)
	{
		char buffer[100];
		sprintf(buffer, "(%f, %f)", val.x, val.y);
		return buffer;
	}

	class approx
	{
	public:
		approx(double x, double y) : x(x), y(y), z(0), w(0) {}
		//approx(float x, float y) : x(x), y(y), z(0), w(0) {}
		double x, y, z, w;

		//static constexpr double epsilon = .000001;
		static constexpr double epsilon = .0001;
	};

	bool operator==(const float2& vec, const approx& a)
	{
		return abs(vec.x - a.x) < approx::epsilon && abs(vec.y - a.y) < approx::epsilon;
	}

}

TEST_CASE("Camera TBD")
{
	Camera cam;

	cam.setViewport({ 500, 600 });

	CHECK(cam.pixelSize == approx(.004, -.00333333));  // or -1./300.
	CHECK(cam.aspectRatio == doctest::Approx(5. / 6.));
	CHECK(cam.scaleWithAR() == approx(1.2, 1));

	cam.view.scale = .25;
	CHECK(cam.scaleWithAR() == approx(.3, .25));

	CHECK(cam.pixelToClip({ 0, 0 }) == approx(-1, 1));
	CHECK(cam.pixelToClip({ 250, 300 }) == approx(0, 0));
	CHECK(cam.pixelToClip({ 500, 600 }) == approx(1, -1));
	CHECK(cam.pixelToClip({ 50, 500 }) == approx(-.8, -2. / 3.));
	CHECK(cam.pixelToClip({ 350, 150 }) == approx(.4, .5));

	CHECK(cam.clipToPixel({ -1, 1 }) == approx(0, 0));
	CHECK(cam.clipToPixel({ 0, 0 }) == approx(250, 300));
	CHECK(cam.clipToPixel({ 1, -1 }) == approx(500, 600));
	CHECK(cam.clipToPixel({ -.8f, -2.f / 3.f }) == approx(50, 500));
	CHECK(cam.clipToPixel({ .4f, .5f }) == approx(350, 150));

}
