#ifndef FX_MANAGER_H
#define FX_MANAGER_H

#include <util.h>

namespace sf{
	class Sound;
}

namespace vb01{
	class Node;
}

namespace battleship{
	class FxManager{
		public:
			struct Fx {
				struct Component{
					vb01::s64 duration, offsetTime = 0;
					bool vfx;
					void *comp = nullptr;

					Component(void *c, bool v, vb01::s64 dur, vb01::s64 ot = 0) : comp(c), vfx(v), duration(dur), offsetTime(ot) {}
				};

				vb01::s64 initTime;
				std::vector<Component> components;
			
				Fx(std::vector<Component> comp) : initTime(vb01::getTime()), components(comp){}
			};

			static FxManager* getSingleton();
			void update();
			Fx& addFx(Fx);
			void removeFx(int);
		private:
			FxManager(){}
			void destroyFxComponent(int, int);
			std::vector<Fx> fxs;
	};
}

#endif
