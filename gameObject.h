#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector.h>
#include <quaternion.h>
#include <model.h>

namespace sf{
	class SoundBuffer;
	class Sound;
}

namespace battleship{
	class Player;
	class Unit;

	class GameObject{
		public:
			enum class Type{UNIT, PROJECTILE, RESOURCE_DEPOSIT};

			GameObject(Type t, int i, Player *pl, vb01::Vector3 vec, vb01::Quaternion quat) : type(t), id(i), player(pl), pos(vec), rot(quat){}
			~GameObject(){}
			virtual void update();
        	virtual void select(){}
			void placeAt(vb01::Vector3);
			void orientAt(vb01::Quaternion);
			std::string getGameObjTableName();
			vb01::Vector2 calculateSelectionRect();
			void updateGameStats(Unit*);
			static sf::Sound* prepareSfx(sf::SoundBuffer*, std::string);
        	inline vb01::Vector2 getScreenPos(){return screenPos;}
			inline vb01::Vector3 getCorner(int i){return corners[i];}
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
			inline vb01::Node* getHitbox(){return hitbox;}
		private:
			int sortCorners(std::vector<vb01::Vector2>&, bool, bool);
		protected:
			virtual void initProperties();
			virtual void destroyModel();
			virtual void initModel(bool = true);
			virtual void initHitbox();
			virtual void destroyHitbox();
			virtual void destroySound();
			virtual void initSound();

			Type type;
			int id;
			Player *player;
			sf::SoundBuffer *deathSfxBuffer;
			sf::Sound *deathSfx;
			vb01::Model *model = nullptr;
			vb01::Node *hitbox = nullptr;
			vb01::Vector3 pos = vb01::Vector3(0, 0, 0), upVec = vb01::Vector3(0, 1, 0), dirVec = vb01::Vector3(0, 0, 1), leftVec = vb01::Vector3(1, 0, 0), corners[8];
			vb01::Vector2 screenPos;
			vb01::Quaternion rot;
			bool selectable = false, debugging = false;
			float width, height, length;
	};
}

#endif
