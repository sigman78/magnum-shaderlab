#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Renderer.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/MeshTools/FullScreenTriangle.h>
#include <Magnum/Shaders/Generic.h>
#include <Magnum/Context.h>
#include <Magnum/Shader.h>
#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Version.h>
#include <Magnum/Timeline.h>
#include <Magnum/Color.h>

namespace Magnum { namespace Examples {

using namespace Magnum::Math::Literals;

class TheShader : public AbstractShaderProgram {
public:
	typedef typename Shaders::Generic3D::Position Position;

	TheShader() {
		Utility::Resource rs("MagnumShaders");

#ifndef MAGNUM_TARGET_GLES
		const Version version = Context::current().supportedVersion({ Version::GL320, Version::GL310, Version::GL300, Version::GL210 });
#else
		const Version version = Context::current().supportedVersion({ Version::GLES300, Version::GLES200 });
#endif
		Shader vert { version, Shader::Type::Vertex };
		Shader frag { version, Shader::Type::Fragment };
		
		vert.addSource(rs.get("shader.vert"));
		frag.addSource(rs.get("shader.frag"));

		CORRADE_INTERNAL_ASSERT(Shader::compile({ vert, frag }));
		attachShaders({ vert, frag });
		CORRADE_INTERNAL_ASSERT(link());

		_iResolution = uniformLocation("iResolution");
		_iGlobalTime = uniformLocation("iGlobalTime");
		_iTimeDelta = uniformLocation("iTimeDelta");
		_iFrame = uniformLocation("iTime");
		_iMouse = uniformLocation("iMouse");
	}

	TheShader& setResolution(const Vector2i& size) {
		Vector3 tmp{ (Float)size.x(), (Float)size.y(), 0 };
		setUniform(_iResolution, tmp);
		return *this;
	}

	TheShader& setTimeFrame(Float global, Float dt, Int frame) {
		setUniform(_iGlobalTime, global);
		setUniform(_iTimeDelta, dt);
		setUniform(_iFrame, frame);
		return *this;
	}

	TheShader& setMouse(const Vector2i& mp) {
		Vector3 tmp{ (Float)mp.x(), (Float)mp.y(), 0 };
		setUniform(_iMouse, tmp);
		return *this;
	}

private:
	Int _iResolution{ 0 }; // vec3
	Int _iGlobalTime{ 1 }; // float
	Int _iTimeDelta{ 2 }; // float
	Int _iFrame{ 3 }; // int
	Int _iMouse{ 4 }; // vec4
};

class ShaderLab: public Platform::Application {
    public:
        explicit ShaderLab(const Arguments& arguments);

    private:
        void drawEvent() override;
        void mousePressEvent(MouseEvent& event) override;
        void mouseReleaseEvent(MouseEvent& event) override;
        void mouseMoveEvent(MouseMoveEvent& event) override;
		void viewportEvent(const Vector2i& size) override;
		void keyPressEvent(KeyEvent& event) override;

        std::unique_ptr<Buffer> _vertexBuffer;
        Mesh _mesh;
        TheShader _shader;

        Vector2i _previousMousePosition;
		Timeline _timeline;
		int _frameNo;
		bool _running;
};

ShaderLab::ShaderLab(const Arguments& arguments): Platform::Application{arguments, Configuration{}.setTitle("Magnum Primitives Example").setWindowFlags(Configuration::WindowFlag::Resizable) } {
    Renderer::disable(Renderer::Feature::DepthTest); 
    Renderer::disable(Renderer::Feature::FaceCulling);

	std::tie(_vertexBuffer, _mesh) = MeshTools::fullScreenTriangle();

	_running = true;
	_frameNo = 0;
	_timeline.start();

	//setSwapInterval(16);
	//setMinimalLoopPeriod(40);
}
 
void ShaderLab::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);

	_shader
		.setResolution(defaultFramebuffer.viewport().size())
		.setTimeFrame(_timeline.previousFrameTime(), _timeline.previousFrameDuration(), _frameNo)
		.setMouse(Vector2i(0, 0));

    _mesh.draw(_shader);

    swapBuffers();

	if (_running) {
		_frameNo++;
		_timeline.nextFrame();
	}

	redraw();
}

void ShaderLab::mousePressEvent(MouseEvent& event) {
    if(event.button() != MouseEvent::Button::Left) return;

    _previousMousePosition = event.position();
    event.setAccepted();
}

void ShaderLab::mouseReleaseEvent(MouseEvent& event) {

    event.setAccepted();
    redraw();
}

void ShaderLab::mouseMoveEvent(MouseMoveEvent& event) {
    if(!(event.buttons() & MouseMoveEvent::Button::Left)) return;

    const Vector2 delta = 3.0f*
        Vector2{event.position() - _previousMousePosition}/
        Vector2{defaultFramebuffer.viewport().size()};

    _previousMousePosition = event.position();
    event.setAccepted();
    redraw();
}

void ShaderLab::viewportEvent(const Vector2i& size)  {
	defaultFramebuffer.setViewport({ {}, size });
}

void ShaderLab::keyPressEvent(KeyEvent& key) {
	if (key.key() == KeyEvent::Key::Space) {
		_running = !_running;
		key.setAccepted();
	}
	else if (key.key() == KeyEvent::Key::Esc) {
		exit();
	}
}


}}

MAGNUM_APPLICATION_MAIN(Magnum::Examples::ShaderLab)