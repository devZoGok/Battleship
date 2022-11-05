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
        InGameAppState(std::vector<std::string>, std::vector<std::string>, std::string);
        ~InGameAppState();
        void onAttached();
        void onDettached();
        void update();
        void onAction(int, bool);
        void onAnalog(int, float);
        std::vector<Unit*> getSelectedUnits(Player*);
        inline std::vector<Player*> getPlayers() {return players;}
        inline std::vector<Projectile*>& getProjectiles(){return projectiles;}
        inline void addFx(Fx fx){this->fx.push_back(fx);}
        inline void addProjectile(Projectile *p){projectiles.push_back(p);}
    private:
        class ResumeButton : public vb01Gui::Button {
        public:
            ResumeButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2);
            void onClick();
            GuiAppState *getGuiState();
        private:
            GuiAppState *guiState;
            InGameAppState *inGameState;
        };

        class ConsoleButton : public vb01Gui::Button {
        public:
			class ConsoleCommandEntryButton : public Button {
			public:
			    ConsoleCommandEntryButton(InGameAppState*, vb01Gui::Textbox*, vb01Gui::Listbox*, vb01::Vector2, vb01::Vector2, std::string, bool);
			    void onClick();
			private:
			    InGameAppState *inGameState;
			    vb01Gui::Textbox *textbox;
			    vb01Gui::Listbox *listbox;
			};

            ConsoleButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2);
            void onClick();
			inline ConsoleCommandEntryButton* getEntryButton(){return entryButton;} 
        private:
            GuiAppState *guiState;
            InGameAppState *inGameState;
			ConsoleCommandEntryButton *entryButton = nullptr;
        };

        class MainMenuButton : public vb01Gui::Button {
        public:
            MainMenuButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2);
            void onClick();
        private:
            GuiAppState *guiState;
            InGameAppState *inGameState;
        };

        class InGameOptionsButton : public OptionsButton {
        public:
            InGameOptionsButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2);
            void onClick();
        private:
            class ReturnButton : public vb01Gui::Button {
            public:
                ReturnButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2, std::string, bool);
                void onClick();
            private:
                GuiAppState *guiState;
                InGameAppState *inGameState;
            };

            GuiAppState *guiState;
            InGameAppState *inGameState;
            ReturnButton *returnButton;
        };

        ResumeButton *resumeButton;
        ConsoleButton *consoleButton;
        InGameOptionsButton *optionsButton;
        MainMenuButton *mainMenuButton;
        ExitButton *exitButton;
        bool isMainMenuActive = false;
        std::vector<Player*> players;
        std::vector<std::string> difficultyLevels, factions;
        std::vector<Projectile*> projectiles;
        std::vector<Fx> fx;
		std::string mapName;
        Player *mainPlayer;
        int playerId;
        ActiveGameState* activeState;
        GuiAppState* guiState;

		void loadModels();
        void toggleMainMenu();
		void updateUnits(Player*);
		void updateProjectiles();
    public:
        inline void setResumeButton(ResumeButton *r){this->resumeButton = r;}
        inline void setConsoleButton(ConsoleButton *c){this->consoleButton = c;}
        inline void setOptionsButton(InGameOptionsButton *o){this->optionsButton = o;}
        inline void setMainMenuButton(MainMenuButton *m){this->mainMenuButton = m;}
        inline void setExitButton(ExitButton *e){this->exitButton = e;}
        inline ResumeButton* getResumeButton(){return resumeButton;}
        inline ConsoleButton* getConsoleButton(){return consoleButton;}
        inline InGameOptionsButton* getOptionsButton(){return optionsButton;}
        inline MainMenuButton* getMainMenuButton(){return mainMenuButton;}
        inline ExitButton* getExitButton(){return exitButton;}
    };
}

#endif
