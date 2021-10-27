#include "optionsButton.h"
#include "util.h"

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::io;
using namespace irr::video;
using namespace irr::gui;

namespace game{
    namespace gui{
        OptionsButton::OkButton::OkButton(GameManager *gm) :Button(gm, vector2d<s32>(50, gm->getHeight() - 150), vector2d<s32>(140, 50), "Ok", true) {
            this->state = ((GuiAppState*)gm->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::OkButton::onClick() {

        }

        OptionsButton::DefaultsButton::DefaultsButton(GameManager *gm) :Button(gm, vector2d<s32>(200, gm->getHeight() - 150), vector2d<s32>(140, 50), "Restore defaults", true) {
            this->state = ((GuiAppState*)gm->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::DefaultsButton::onClick() {
            
        }

        OptionsButton::BackButton::BackButton(GameManager *gm) :Button(gm, vector2d<s32>(350, gm->getHeight() - 150), vector2d<s32>(140, 50), "Back", true) {
            this->state = ((GuiAppState*)gm->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::BackButton::onClick() {
            gameManager->detachAllBitmapTexts();
            gameManager->detachAllImages();
            state->removeAllListboxes();
            state->removeAllCheckboxes();
            state->removeAllSliders();
            state->removeAllTextboxes();
            state->removeButton("Ok");
            state->removeButton("Restore defaults");
            OptionsButton *optionsButton = new OptionsButton(gameManager, vector2d<s32>(), vector2d<s32>(), "Options", true);
            optionsButton->onClick();
            state->removeButton("Back");
        }

        OptionsButton::TabButton::TabButton(GameManager *gm, vector2d<s32> pos, vector2d<s32> size, stringw name) :Button(gm, pos, size, name, true) {
            this->state = ((GuiAppState*)gm->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::TabButton::onClick() {
            state->removeButton("Back");
            OkButton *okButton = new OkButton(gameManager);
            DefaultsButton *defaultsButton = new DefaultsButton(gameManager);
            BackButton *returnButton = new BackButton(gameManager);
            defaultsButton->moveText(-25, 0);
            state->addButton(okButton);
            state->addButton(defaultsButton);
            state->addButton(returnButton);
        }

        OptionsButton::ControlsTab::ControlsTab(GameManager *gm) :TabButton(gm, vector2d<s32>(gm->getWidth() / 4, gm->getHeight() / 10), vector2d<s32>(100, 50), "Controls") {}
        void OptionsButton::ControlsTab::onClick() {
            TabButton::onClick();
            std::vector<stringw> lines=readFile(std::string((PATH+path("../options.cfg")).c_str()));
            Listbox *listbox = new Listbox(gameManager, vector2d<s32>(gameManager->getWidth() / 4, gameManager->getHeight() / 10), vector2d<s32>(360, 20), lines, lines.size()<5?lines.size():5,true);
            listbox->openUp();
            state->addListbox(listbox);
            state->removeButton("Mouse");
            state->removeButton("Video");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
            state->removeButton("Controls");
        }

        OptionsButton::MouseTab::MouseTab(GameManager *gm) :TabButton(gm, vector2d<s32>(gm->getWidth() / 4, gm->getHeight() / 10 + 60), vector2d<s32>(100, 50), "Mouse") {}
        void OptionsButton::MouseTab::onClick() {
            TabButton::onClick();
            vector2d<s32> pos(gameManager->getWidth() / 3, gameManager->getHeight() / 4);
            IGUIFont *font = gameManager->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
            Slider *mouseSensitivitySlider = new Slider(gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(300, 10), .1, 3.);
            Textbox *mouseSensitivityTextbox = new Textbox(gameManager, vector2d<s32>(pos.X + 320, pos.Y - 10), vector2d<s32>(100, 20));
            Checkbox *reverseMouseCheckbox = new Checkbox(gameManager, vector2d<s32>(pos.X, pos.Y + 50));
            gameManager->attachBitmapText(new BitmapText(gameManager, "MouseSensitivity", vector2d<s32>(gameManager->getWidth() / 3 - 90, gameManager->getHeight() / 4 - 5), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "ReverseMouse", vector2d<s32>(gameManager->getWidth() / 3 + 20, gameManager->getHeight() / 4 + 50), font));
            state->addTextbox(mouseSensitivityTextbox);
            state->addSlider(mouseSensitivitySlider);
            state->addCheckbox(reverseMouseCheckbox);
            state->removeButton("Controls");
            state->removeButton("Video");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
            state->removeButton("Mouse");
        }

        OptionsButton::VideoTab::VideoTab(GameManager *gm) :TabButton(gm, vector2d<s32>(gm->getWidth() / 4, gm->getHeight() / 10 + 120), vector2d<s32>(100, 50), "Video") {}
        void OptionsButton::VideoTab::onClick() {
            TabButton::onClick();
            std::vector<stringw> lines;
            for (int i = 0; i < 10; i++) {
                stringw s;
                s += i;
                lines.push_back(s);
            }
            IGUIFont *font = gameManager->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
            vector2d<s32> pos(gameManager->getWidth() / 3, gameManager->getHeight() / 8);
            Listbox *resolutionsListbox = new Listbox(gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(100, 20), lines, 5);
            Checkbox *fullscreenCheckbox = new Checkbox(gameManager, vector2d<s32>(pos.X, pos.Y + 30));
            Checkbox *vsyncCheckbox = new Checkbox(gameManager, vector2d<s32>(pos.X, pos.Y + 50));
            Checkbox *normalMapCheckbox = new Checkbox(gameManager, vector2d<s32>(pos.X, pos.Y + 70));
            Checkbox *parallaxMapCheckbox = new Checkbox(gameManager, vector2d<s32>(pos.X, pos.Y + 90));
            Checkbox *specularMapCheckbox = new Checkbox(gameManager, vector2d<s32>(pos.X, pos.Y + 110));
            gameManager->attachBitmapText(new BitmapText(gameManager, "Fullscreen", vector2d<s32>(pos.X + 20, pos.Y + 30), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "VSync", vector2d<s32>(pos.X + 20, pos.Y + 50), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "Normal map", vector2d<s32>(pos.X + 20, pos.Y + 70), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "Parallax map", vector2d<s32>(pos.X + 20, pos.Y + 90), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "Specular map", vector2d<s32>(pos.X + 20, pos.Y + 110), font));
            state->addListbox(resolutionsListbox);
            state->addCheckbox(fullscreenCheckbox);
            state->addCheckbox(vsyncCheckbox);
            state->addCheckbox(normalMapCheckbox);
            state->addCheckbox(parallaxMapCheckbox);
            state->addCheckbox(specularMapCheckbox);
            state->removeButton("Controls");
            state->removeButton("Mouse");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
            state->removeButton("Video");
        }

        OptionsButton::AudioTab::AudioTab(GameManager *gm) :TabButton(gm, vector2d<s32>(gm->getWidth() / 4, gm->getHeight() / 10 + 180), vector2d<s32>(100, 50), "Audio") {}
        void OptionsButton::AudioTab::onClick() {
            TabButton::onClick();
            vector2d<s32> pos(gameManager->getWidth() / 3, gameManager->getHeight() / 8);
            Slider *volumeSlider = new Slider(gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(300, 10), 0., 2.);
            Textbox *volumeTextbox = new Textbox(gameManager, vector2d<s32>(pos.X + 320, pos.Y), vector2d<s32>(100, 20));
            state->addTextbox(volumeTextbox);
            state->addSlider(volumeSlider);
            state->removeButton("Controls");
            state->removeButton("Mouse");
            state->removeButton("Video");
            state->removeButton("Multiplayer");
            state->removeButton("Audio");
        }

        OptionsButton::MultiplayerTab::MultiplayerTab(GameManager *gm) :TabButton(gm, vector2d<s32>(gm->getWidth() / 4, gm->getHeight() / 10 + 240), vector2d<s32>(100, 50), "Multiplayer") {}
        void OptionsButton::MultiplayerTab::onClick() {
            TabButton::onClick();
            IGUIFont *font = gameManager->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
            vector2d<s32> pos(gameManager->getWidth() / 3, gameManager->getHeight() / 6);
            Textbox *tcpTextbox = new Textbox(gameManager, vector2d<s32>(pos.X, pos.Y), vector2d<s32>(100, 20));
            Textbox *udpTextbox = new Textbox(gameManager, vector2d<s32>(pos.X, pos.Y + 30), vector2d<s32>(100, 20));
            Textbox *playerNameTextbox = new Textbox(gameManager, vector2d<s32>(pos.X, pos.Y + 60), vector2d<s32>(100, 20));
            gameManager->attachBitmapText(new BitmapText(gameManager, "TCP port", vector2d<s32>(pos.X - 50, pos.Y), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "UDP port", vector2d<s32>(pos.X - 50, pos.Y + 30), font));
            gameManager->attachBitmapText(new BitmapText(gameManager, "Player name", vector2d<s32>(pos.X - 65, pos.Y + 60), font));
            state->addTextbox(tcpTextbox);
            state->addTextbox(udpTextbox);
            state->addTextbox(playerNameTextbox);
            state->removeButton("Controls");
            state->removeButton("Mouse");
            state->removeButton("Video");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
        }

        OptionsButton::OptionsButton(GameManager *gm, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) :Button(gm, pos, size, name, separate) {
            this->state = ((GuiAppState*)gm->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::onClick() {
            ControlsTab *controlsTab = new ControlsTab(this->gameManager);
            MouseTab *mouseTab = new MouseTab(this->gameManager);
            VideoTab *videoTab = new VideoTab(this->gameManager);
            AudioTab *audioTab = new AudioTab(this->gameManager);
            MultiplayerTab *mupltiplayerTab = new MultiplayerTab(this->gameManager);
            mupltiplayerTab->moveText(-20, 0);
            state->addButton(controlsTab);
            state->addButton(mouseTab);
            state->addButton(videoTab);
            state->addButton(audioTab);
            state->addButton(mupltiplayerTab);
            state->removeButton(this);
        }
    }
}
