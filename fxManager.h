#ifndef FX_MANAGER_H
#define FX_MANAGER_H

#include <util.h>

namespace battleship{
	class FxManager{
		public:
			struct Fx {
				struct Component{
					vb01::s64 duration, offsetTime = 0;
					bool vfx, active = false;
					void *comp = nullptr;

					Component(void*, bool, vb01::s64, vb01::s64 = 0);
				};

				bool reuse;
				vb01::s64 initTime;
				std::vector<Component> components;
			
				Fx(std::vector<Component> comp, bool re = false) : initTime(vb01::getTime()), components(comp), reuse(re){}
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
