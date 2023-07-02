#pragma once
#ifndef IN_GAME_APP_STATE_H
#define IN_GAME_APP_STATE_H

#include "gameManager.h"
#include "player.h"
#include "map.h"
#include "activeGameState.h"
#include "guiAppState.h"
#include "exitButton.h"
#include "optionsButton.h"

#include <vector>

namespace battleship{
    struct Fx {
        s64 initTime, time;
        sf::Sound *sfx = nullptr;
    };
    
    class InGameAppState : public gameBase::AbstractAppState {
    public:
        class ResumeButton : public vb01Gui::Button {
        public:
            ResumeButton(vb01::Vector2, vb01::Vector2);
            void onClick();
        private:
        };

        class ConsoleButton : public vb01Gui::Button {
        public:
			class ConsoleCommandEntryButton : public Button {
			public:
			    ConsoleCommandEntryButton(vb01Gui::Textbox*, vb01Gui::Listbox*, vb01::Vector2, vb01::Vector2, std::string);
			    void onClick();
			private:
			    vb01Gui::Textbox *textbox;
			    vb01Gui::Listbox *listbox;
			};

            ConsoleButton(vb01::Vector2, vb01::Vector2);
            void onClick();
        private:
        };

        InGameAppState(std::vector<std::string>, std::vector<std::string>, std::string);
        ~InGameAppState();
        void onAttached();
        void onDettached();
        void update();
        void onAction(int, bool);
        void onAnalog(int, float);
        std::vector<Unit*> getSelectedUnits(Player*);
        inline std::vector<Projectile*>& getProjectiles(){return projectiles;}
        inline void addFx(Fx fx){this->fx.push_back(fx);}
        inline void addProjectile(Projectile *p){projectiles.push_back(p);}
    private:
        bool isMainMenuActive = false;
        std::vector<std::string> difficultyLevels, factions, modelPaths;
        std::vector<Projectile*> projectiles;
        std::vector<Fx> fx;
		std::string mapName;
        Player *mainPlayer;
        int playerId;
        ActiveGameState* activeState;

        void toggleMainMenu();
    };
}

#endif
