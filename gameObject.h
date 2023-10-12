#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector.h>
#include <quaternion.h>
#include <model.h>

namespace battleship{
	class Player;

	class GameObject{
		public:
			enum class Type{UNIT, PROJECTILE, RESOURCE_DEPOSIT};

			GameObject(Type t, int i, Player *pl, vb01::Vector3 vec, vb01::Quaternion quat) : type(t), id(i), player(pl), pos(vec), rot(quat){}
			~GameObject(){}
			virtual void update();
        	virtual void toggleSelection(bool sel){selected = sel;}
			void placeAt(vb01::Vector3);
			void orientAt(vb01::Quaternion);
			std::string getGameObjTableName();
			inline vb01::Vector3 getCorner(int i){return corners[i];}
			inline bool isSelected(){return selected;}
			inline bool isSelectable(){return selectable;}
			inline bool isDebuggable(){return debugging;}
			inline vb01::Vector3 getPos() {return pos;}
			inline vb01::Quaternion getRot(){return rot;}
			inline float getWidth() {return width;}
			inline float getHeight() {return height;}
			inline float getLength() {return length;}
			inline vb01::Model* getModel() {return model;}
			inline Player* getPlayer(){return player;}
			inline void toggleDebugging(bool d){this->debugging=d;}
			inline vb01::Vector3 getDirVec() {return dirVec;}
			inline vb01::Vector3 getLeftVec() {return leftVec;}
			inline vb01::Vector3 getUpVec() {return upVec;}
        	inline int getId() {return id;}
			inline Type getType(){return type;}
		private:
		protected:
			virtual void initProperties();
			virtual void destroyModel();
			virtual void initModel(bool = true);
			virtual void destroySound();
			virtual void initSound();

			Type type;
			int id;
			Player *player;
			vb01::Model *model = nullptr;
			vb01::Vector3 pos = vb01::Vector3(0, 0, 0), upVec = vb01::Vector3(0, 1, 0), dirVec = vb01::Vector3(0, 0, 1), leftVec = vb01::Vector3(1, 0, 0), corners[8];
			vb01::Quaternion rot;
			bool selected = false, selectable, debugging = false;
			float width, height, length;
	};
}

#endif
