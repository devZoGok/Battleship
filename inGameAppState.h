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
        //irr::scene::IParticleSystemSceneNode *node=nullptr;
        sf::Sound *sfx = nullptr;
    };
    
    class InGameAppState : public AbstractAppState {
    public:
        InGameAppState(std::vector<std::string>, std::vector<std::string>);
        ~InGameAppState();
        void onAttachment();
        void onDetachment();
        void update();
        void onAction(Mapping::Bind, bool);
        void onAnalog(Mapping::Bind, double);
        std::vector<Unit*> getSelectedUnits(Player*);
        inline std::vector<Player*> getPlayers() {return players;}
        inline std::vector<Projectile*>& getProjectiles(){return projectiles;}
        inline void addFx(Fx fx){this->fx.push_back(fx);}
        inline void addProjectile(Projectile *p){projectiles.push_back(p);}
    private:
        class ResumeButton : public vb01Gui::Button {
        public:
            ResumeButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
            GuiAppState *getGuiState();
        private:
            GuiAppState *guiState;
            InGameAppState *inGameState;
        };

        class ConsoleButton : public vb01Gui::Button {
        public:
            ConsoleButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        private:
            GuiAppState *guiState;
            InGameAppState *inGameState;
        };

        class MainMenuButton : public vb01Gui::Button {
        public:
            MainMenuButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        private:
            GuiAppState *guiState;
            InGameAppState *inGameState;
        };

        class InGameOptionsButton : public OptionsButton {
        public:
            InGameOptionsButton(GuiAppState*, InGameAppState*, vb01::Vector2, vb01::Vector2, std::string, bool);
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

        class UnitCreationButton : public vb01Gui::Button {
        public:
            UnitCreationButton(std::string, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
            void update();
        private:
        };

        class BattleshipCreationButton : public UnitCreationButton {
        public:
            BattleshipCreationButton(std::string, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        };

        class DestroyerCreationButton : public UnitCreationButton {
        public:
            DestroyerCreationButton(std::string, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        };

        class CruiserCreationButton : public UnitCreationButton {
        public:
            CruiserCreationButton(std::string, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        };

        class CarrierCreationButton : public UnitCreationButton {
        public:
            CarrierCreationButton(std::string, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        };

        class SubmarineCreationButton : public UnitCreationButton {
        public:
            SubmarineCreationButton(std::string, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        };

        ResumeButton *resumeButton;
        ConsoleButton *consoleButton;
        InGameOptionsButton *optionsButton;
        MainMenuButton *mainMenuButton;
        ExitButton *exitButton;
        BattleshipCreationButton *bcb;
        DestroyerCreationButton *dcb;
        CruiserCreationButton *crcb;
        CarrierCreationButton *ccb;
        SubmarineCreationButton *scb;
        inline std::vector<Player*> getPlayerList(){return players;}
        bool isMainMenuActive = false;
        void toggleMainMenu();
        void attachGui();
        void detachGui();
        std::vector<Player*> players;
        std::vector<std::string> difficultyLevels, factions;
        std::vector<Projectile*> projectiles;
        std::vector<Fx> fx;
        Player *mainPlayer;
        Map *map;
        int playerId;
        ActiveGameState* activeState;
        GuiAppState* guiState;
    public:
        inline void setResumeButton(ResumeButton *r){this->resumeButton=r;}
        inline void setConsoleButton(ConsoleButton *c){this->consoleButton=c;}
        inline void setOptionsButton(InGameOptionsButton *o){this->optionsButton=o;}
        inline void setMainMenuButton(MainMenuButton *m){this->mainMenuButton=m;}
        inline void setExitButton(ExitButton *e){this->exitButton=e;}
    };
}

#endif
