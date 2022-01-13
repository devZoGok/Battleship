#pragma once
#ifndef OPTIONS_BUTTON_H
#define OPTIONS_BUTTON_H

#include <button.h>

#include "guiAppState.h"
#include "gameManager.h"

namespace battleship {
    class OptionsButton : public vb01Gui::Button {
    public:
        OptionsButton(vb01::Vector2, vb01::Vector2, std::string, bool);
        class OkButton : public Button {
        public:
            OkButton();
            void onClick();
        private:
            GuiAppState *state;
        };
        class DefaultsButton : public Button {
        public:
            DefaultsButton();
            void onClick();
        private:
            GuiAppState *state;
        };
        class BackButton : public Button {
        public:
            BackButton();
            void onClick();
        private:
            GuiAppState *state;
        };
        class TabButton : public Button {
        public:
            TabButton(vb01::Vector2, vb01::Vector2, std::string);
            void onClick();
        protected:
            GuiAppState *state;
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
        GuiAppState *state;
    };
}

#endif
