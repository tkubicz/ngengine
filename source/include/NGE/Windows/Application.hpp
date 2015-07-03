#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "NGE/Events/MouseEvent.hpp"
#include "NGE/Events/KeyboardEvent.hpp"
#include "NGE/Windows/AbstractWindow.hpp"

namespace NGE {
	namespace Windows {

		class Application {
		  public:
			Application();
			virtual ~Application();

			virtual bool Init();
			virtual void Prepare(float dt);
			virtual void Render();
			virtual void Shutdown();

			virtual void OnKeyPressed(Events::KeyboardEvent& event);
			virtual void OnMouse(Events::MouseEvent& event);
			virtual void OnMouseDrag(int x, int y);

			virtual void OnResize(int width, int height);

			void SetWindow(AbstractWindow* window);
			AbstractWindow* GetWindow();

		  protected:
			AbstractWindow* window;
		};
	}
}
#endif
