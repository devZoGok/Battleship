#pragma once
#ifndef IN_GAME_APP_STATE_H
#define IN_GAME_APP_STATE_H

#include"GameManager.h"
#include"Player.h"
#include"Map.h"
#include"ActiveGameState.h"
#include"GuiAppState.h"
#include"ExitButton.h"
#include"OptionsButton.h"
#include<vector>

namespace game{
    namespace core{
        struct Fx {
            s64 initTime,time;
            irr::scene::IParticleSystemSceneNode *node=nullptr;
            sf::Sound *sfx=nullptr;
        };
        
        class InGameAppState : public AbstractAppState {
        public:
            InGameAppState(GameManager*, std::vector<irr::core::stringw>, std::vector<irr::core::stringw>);
            ~InGameAppState();
            void onAttachment();
            void onDetachment();
            void update();
            void onAction(Bind, bool);
            void onAnalog(Bind, double);
            std::vector<content::Unit*> getSelectedUnits(content::Player*);
            inline std::vector<content::Player*> getPlayers() {return players;}
            inline std::vector<content::Projectile*>& getProjectiles(){return projectiles;}
            inline void addFx(Fx fx){this->fx.push_back(fx);}
            inline void addProjectile(content::Projectile *p){projectiles.push_back(p);}
        private:
            class ResumeButton : public gui::Button {
            public:
                ResumeButton(GuiAppState*, InGameAppState*, GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
                GuiAppState *getGuiState();
            private:
                GuiAppState *guiState;
                InGameAppState *inGameState;
            };

            class ConsoleButton : public gui::Button {
            public:
                ConsoleButton(GuiAppState*, InGameAppState*, GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                GuiAppState *guiState;
                InGameAppState *inGameState;
            };

            class MainMenuButton : public gui::Button {
            public:
                MainMenuButton(GuiAppState*, InGameAppState*, GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                GuiAppState *guiState;
                InGameAppState *inGameState;
            };

            class InGameOptionsButton : public gui::OptionsButton {
            public:
                InGameOptionsButton(GuiAppState*, InGameAppState*, GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            private:
                class ReturnButton : public gui::Button {
                public:
                    ReturnButton(GuiAppState*, InGameAppState*, GameManager*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                    void onClick();
                private:
                    GuiAppState *guiState;
                    InGameAppState *inGameState;
                };
                GuiAppState *guiState;
                InGameAppState *inGameState;
                ReturnButton *returnButton;
            };

            class UnitCreationButton : public gui::Button {
            public:
                UnitCreationButton(GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
                void update();
            private:
                Image *icon;
            };

            class BattleshipCreationButton : public UnitCreationButton {
            public:
                BattleshipCreationButton(GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            };

            class DestroyerCreationButton : public UnitCreationButton {
            public:
                DestroyerCreationButton(GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            };

            class CruiserCreationButton : public UnitCreationButton {
            public:
                CruiserCreationButton(GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            };

            class CarrierCreationButton : public UnitCreationButton {
            public:
                CarrierCreationButton(GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            };

            class SubmarineCreationButton : public UnitCreationButton {
            public:
                SubmarineCreationButton(GameManager*, irr::video::ITexture*, irr::core::vector2d<s32>, irr::core::vector2d<s32>, irr::core::stringw, bool);
                void onClick();
            };
            ResumeButton *resumeButton;
            ConsoleButton *consoleButton;
            InGameOptionsButton *optionsButton;
            MainMenuButton *mainMenuButton;
            gui::ExitButton *exitButton;
            BattleshipCreationButton *bcb;
            DestroyerCreationButton *dcb;
            CruiserCreationButton *crcb;
            CarrierCreationButton *ccb;
            SubmarineCreationButton *scb;
            inline std::vector<content::Player*> getPlayerList(){return players;}
            bool isMainMenuActive = false;
            void toggleMainMenu();
            void attachGui();
            void detachGui();
            std::vector<content::Player*> players;
            std::vector<irr::core::stringw> difficultyLevels, factions;
            std::vector<content::Projectile*> projectiles;
            std::vector<Fx> fx;
            content::Player *mainPlayer;
            GameManager *gameManager;
            content::Map *map;
            int playerId;
            ActiveGameState* activeState;
            GuiAppState* guiState;
        public:
            inline void setResumeButton(ResumeButton *r){this->resumeButton=r;}
            inline void setConsoleButton(ConsoleButton *c){this->consoleButton=c;}
            inline void setOptionsButton(InGameOptionsButton *o){this->optionsButton=o;}
            inline void setMainMenuButton(MainMenuButton *m){this->mainMenuButton=m;}
            inline void setExitButton(gui::ExitButton *e){this->exitButton=e;}
        };
    }
}

#endif
