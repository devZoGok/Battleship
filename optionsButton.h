#pragma once
#ifndef OPTIONS_BUTTON_H
#define OPTIONS_BUTTON_H

#include <button.h>

#include "guiAppState.h"
#include "gameManager.h"

namespace game{
    namespace gui {
        class OptionsButton : public vb01Gui::Button {
        public:
            OptionsButton(vb01::Vector2, vb01::Vector2, std::string, bool);
            class OkButton : public Button {
            public:
                OkButton();
                void onClick();
            private:
                core::GuiAppState *state;
            };
            class DefaultsButton : public Button {
            public:
                DefaultsButton();
                void onClick();
            private:
                core::GuiAppState *state;
            };
            class BackButton : public Button {
            public:
                BackButton();
                void onClick();
            private:
                core::GuiAppState *state;
            };
            class TabButton : public Button {
            public:
                TabButton(vb01::Vector2, vb01::Vector2, std::string);
                void onClick();
            protected:
                core::GuiAppState *state;
            };
            class ControlsTab : public TabButton {
            public:
                ControlsTab();
                void onClick();
            };
            class MouseTab : public TabButton {
            public:
                MouseTab();
                void onClick();
            private:
            };
            class VideoTab : public TabButton {
            public:
                VideoTab();
                void onClick();
            };
            class AudioTab : public TabButton {
            public:
                AudioTab();
                void onClick();
            };
            class MultiplayerTab : public TabButton {
            public:
                MultiplayerTab();
                void onClick();
            };
            virtual void onClick();
            /*
            class ReturnButton :public Button {
            public:
                    ReturnButton(GuiAppState *state, GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) :Button(gM, pos, size, name, separate) {
                            this->state = state;
                    }
                    void onClick() {
                            state->removeAllListboxes();
                            state->removeAllCheckboxes();
                            state->removeAllTextboxes();
                            state->removeAllSliders();
                            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
                            SpButton *spButton = new SpButton(state, gameManager, vector2d<s32>(driver->getScreenSize().Width / 16, driver->getScreenSize().Height / 12), vector2d<s32>(150, 40), "Singleplayer", true);
                            OptionsButton *optionsButton = new OptionsButton(state, gameManager, vector2d<s32>(driver->getScreenSize().Width / 16, driver->getScreenSize().Height / 12 * 2), vector2d<s32>(150, 40), "Options", true);
                            ExitButton *exitButton = new ExitButton(gameManager, vector2d<s32>(driver->getScreenSize().Width / 16, driver->getScreenSize().Height / 12 * 3), vector2d<s32>(150, 40), "Exit", true);
                            state->addButton(spButton);
                            state->addButton(optionsButton);
                            state->addButton(exitButton);
                            state->removeButton("Controls");
                            state->removeButton("Mouse");
                            state->removeButton("Video");
                            state->removeButton("Audio");
                            state->removeButton("Multiplayer");
                            state->removeAllListboxes();
                            state->removeAllCheckboxes();
                            state->removeAllSliders();
                            state->removeAllTextboxes();
                            state->removeButton("Back");
                    }
            private:
                    GuiAppState *state;
            };
            */
            /*ControlsTab *controlsTab = new ControlsTab(state, this->gameManager, vector2d<s32>(gameManager->getWidth() / 4, gameManager->getHeight() / 10), vector2d<s32>(100, 50), "Controls", true);
            MouseTab *mouseTab = new MouseTab(state, this->gameManager, vector2d<s32>(gameManager->getWidth() / 4, gameManager->getHeight() / 10 + 60), vector2d<s32>(100, 50), "Mouse", true);
            VideoTab *videoTab = new VideoTab(state, this->gameManager, vector2d<s32>(gameManager->getWidth() / 4, gameManager->getHeight() / 10 + 120), vector2d<s32>(100, 50), "Video", true);
            AudioTab *audioTab = new AudioTab(state, this->gameManager, vector2d<s32>(gameManager->getWidth() / 4, gameManager->getHeight() / 10 + 180), vector2d<s32>(100, 50), "Audio", true);
            MultiplayerTab *mupltiplayerTab = new MultiplayerTab(state, this->gameManager, vector2d<s32>(gameManager->getWidth() / 4, gameManager->getHeight() / 10 + 240), vector2d<s32>(100, 50), "Multiplayer", true);
            //ReturnButton *returnButton = new ReturnButton(state, this->gameManager, vector2d<s32>(50, gameManager->getHeight() - 150), vector2d<s32>(140, 50), "Back", true);
            //state->addButton(returnButton);
            mupltiplayerTab->moveText(-20, 0);
            state->addButton(controlsTab);
            state->addButton(mouseTab);
            state->addButton(videoTab);
            state->addButton(audioTab);
            state->addButton(mupltiplayerTab);
            state->removeButton("Exit");
            state->removeButton("Singleplayer");
            state->removeButton("Options");*/
            core::GuiAppState *state;
        };
    }
}

#endif
