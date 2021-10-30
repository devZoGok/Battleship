#include "optionsButton.h"
#include "stateManager.h"
#include "util.h"

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::io;
using namespace irr::video;
using namespace irr::gui;

namespace game{
    namespace gui{
        OptionsButton::OkButton::OkButton() :Button(vector2d<s32>(50, GameManager::getSingleton()->getHeight() - 150), vector2d<s32>(140, 50), "Ok", true) {
            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::OkButton::onClick() {

        }

        OptionsButton::DefaultsButton::DefaultsButton() :Button(vector2d<s32>(200, GameManager::getSingleton()->getHeight() - 150), vector2d<s32>(140, 50), "Restore defaults", true) {
            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::DefaultsButton::onClick() {
            
        }

        OptionsButton::BackButton::BackButton() :Button(vector2d<s32>(350, GameManager::getSingleton()->getHeight() - 150), vector2d<s32>(140, 50), "Back", true) {
            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::BackButton::onClick() {
						GameManager *gm = GameManager::getSingleton();
            gm->detachAllBitmapTexts();
            gm->detachAllImages();
            state->removeAllListboxes();
            state->removeAllCheckboxes();
            state->removeAllSliders();
            state->removeAllTextboxes();
            state->removeButton("Ok");
            state->removeButton("Restore defaults");
            OptionsButton *optionsButton = new OptionsButton(vector2d<s32>(), vector2d<s32>(), "Options", true);
            optionsButton->onClick();
            state->removeButton("Back");
        }

        OptionsButton::TabButton::TabButton(vector2d<s32> pos, vector2d<s32> size, stringw name) :Button(pos, size, name, true) {
            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::TabButton::onClick() {
            state->removeButton("Back");
            OkButton *okButton = new OkButton();
            DefaultsButton *defaultsButton = new DefaultsButton();
            BackButton *returnButton = new BackButton();
            defaultsButton->moveText(-25, 0);
            state->addButton(okButton);
            state->addButton(defaultsButton);
            state->addButton(returnButton);
        }

        OptionsButton::ControlsTab::ControlsTab() : TabButton(
								vector2d<s32>(GameManager::getSingleton()->getWidth() / 4,
								GameManager::getSingleton()->getHeight() / 10),
							 	vector2d<s32>(100, 50),
							 	"Controls"
				) {}

        void OptionsButton::ControlsTab::onClick() {
            TabButton::onClick();
            std::vector<stringw> lines=readFile(std::string((PATH+path("../options.cfg")).c_str()));
						GameManager *gm = GameManager::getSingleton();
            Listbox *listbox = new Listbox(vector2d<s32>(gm->getWidth() / 4, gm->getHeight() / 10), vector2d<s32>(360, 20), lines, lines.size()<5?lines.size():5,true);
            listbox->openUp();
            state->addListbox(listbox);
            state->removeButton("Mouse");
            state->removeButton("Video");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
            state->removeButton("Controls");
        }

        OptionsButton::MouseTab::MouseTab() : TabButton(
								vector2d<s32>(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 60),
							 	vector2d<s32>(100, 50),
							 	"Mouse"
				) {}

        void OptionsButton::MouseTab::onClick() {
            TabButton::onClick();
						GameManager *gm = GameManager::getSingleton();
            vector2d<s32> pos(gm->getWidth() / 3, gm->getHeight() / 4);
            IGUIFont *font = gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
            Slider *mouseSensitivitySlider = new Slider(vector2d<s32>(pos.X, pos.Y), vector2d<s32>(300, 10), .1, 3.);
            Textbox *mouseSensitivityTextbox = new Textbox(vector2d<s32>(pos.X + 320, pos.Y - 10), vector2d<s32>(100, 20));
            Checkbox *reverseMouseCheckbox = new Checkbox(vector2d<s32>(pos.X, pos.Y + 50));
            gm->attachBitmapText(new BitmapText("MouseSensitivity", vector2d<s32>(gm->getWidth() / 3 - 90, gm->getHeight() / 4 - 5), font));
            gm->attachBitmapText(new BitmapText("ReverseMouse", vector2d<s32>(gm->getWidth() / 3 + 20, gm->getHeight() / 4 + 50), font));
            state->addTextbox(mouseSensitivityTextbox);
            state->addSlider(mouseSensitivitySlider);
            state->addCheckbox(reverseMouseCheckbox);
            state->removeButton("Controls");
            state->removeButton("Video");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
            state->removeButton("Mouse");
        }

        OptionsButton::VideoTab::VideoTab() :TabButton(vector2d<s32>(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 120), vector2d<s32>(100, 50), "Video") {}
        void OptionsButton::VideoTab::onClick() {
            TabButton::onClick();
            std::vector<stringw> lines;
            for (int i = 0; i < 10; i++) {
                stringw s;
                s += i;
                lines.push_back(s);
            }
						GameManager *gm = GameManager::getSingleton();
            IGUIFont *font = gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
            vector2d<s32> pos(gm->getWidth() / 3, gm->getHeight() / 8);
            Listbox *resolutionsListbox = new Listbox(vector2d<s32>(pos.X, pos.Y), vector2d<s32>(100, 20), lines, 5);

            Checkbox *fullscreenCheckbox = new Checkbox(vector2d<s32>(pos.X, pos.Y + 30));
            Checkbox *vsyncCheckbox = new Checkbox(vector2d<s32>(pos.X, pos.Y + 50));
            Checkbox *normalMapCheckbox = new Checkbox(vector2d<s32>(pos.X, pos.Y + 70));
            Checkbox *parallaxMapCheckbox = new Checkbox(vector2d<s32>(pos.X, pos.Y + 90));
            Checkbox *specularMapCheckbox = new Checkbox(vector2d<s32>(pos.X, pos.Y + 110));

            gm->attachBitmapText(new BitmapText("Fullscreen", vector2d<s32>(pos.X + 20, pos.Y + 30), font));
            gm->attachBitmapText(new BitmapText("VSync", vector2d<s32>(pos.X + 20, pos.Y + 50), font));
            gm->attachBitmapText(new BitmapText("Normal map", vector2d<s32>(pos.X + 20, pos.Y + 70), font));
            gm->attachBitmapText(new BitmapText("Parallax map", vector2d<s32>(pos.X + 20, pos.Y + 90), font));
            gm->attachBitmapText(new BitmapText("Specular map", vector2d<s32>(pos.X + 20, pos.Y + 110), font));

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

        OptionsButton::AudioTab::AudioTab() :TabButton(vector2d<s32>(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 180), vector2d<s32>(100, 50), "Audio") {}
        void OptionsButton::AudioTab::onClick() {
            TabButton::onClick();
						GameManager *gm = GameManager::getSingleton();
            vector2d<s32> pos(gm->getWidth() / 3, gm->getHeight() / 8);
            Slider *volumeSlider = new Slider(vector2d<s32>(pos.X, pos.Y), vector2d<s32>(300, 10), 0., 2.);
            Textbox *volumeTextbox = new Textbox(vector2d<s32>(pos.X + 320, pos.Y), vector2d<s32>(100, 20));
            state->addTextbox(volumeTextbox);
            state->addSlider(volumeSlider);
            state->removeButton("Controls");
            state->removeButton("Mouse");
            state->removeButton("Video");
            state->removeButton("Multiplayer");
            state->removeButton("Audio");
        }

        OptionsButton::MultiplayerTab::MultiplayerTab() : TabButton(vector2d<s32>(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 240), vector2d<s32>(100, 50), "Multiplayer") {}
        void OptionsButton::MultiplayerTab::onClick() {
            TabButton::onClick();
						GameManager *gm = GameManager::getSingleton();
            IGUIFont *font = gm->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fonthaettenschweiler.bmp");
            vector2d<s32> pos(gm->getWidth() / 3, gm->getHeight() / 6);
            Textbox *tcpTextbox = new Textbox(vector2d<s32>(pos.X, pos.Y), vector2d<s32>(100, 20));
            Textbox *udpTextbox = new Textbox(vector2d<s32>(pos.X, pos.Y + 30), vector2d<s32>(100, 20));
            Textbox *playerNameTextbox = new Textbox(vector2d<s32>(pos.X, pos.Y + 60), vector2d<s32>(100, 20));
            gm->attachBitmapText(new BitmapText("TCP port", vector2d<s32>(pos.X - 50, pos.Y), font));
            gm->attachBitmapText(new BitmapText("UDP port", vector2d<s32>(pos.X - 50, pos.Y + 30), font));
            gm->attachBitmapText(new BitmapText("Player name", vector2d<s32>(pos.X - 65, pos.Y + 60), font));
            state->addTextbox(tcpTextbox);
            state->addTextbox(udpTextbox);
            state->addTextbox(playerNameTextbox);
            state->removeButton("Controls");
            state->removeButton("Mouse");
            state->removeButton("Video");
            state->removeButton("Audio");
            state->removeButton("Multiplayer");
        }

        OptionsButton::OptionsButton(vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(pos, size, name, separate) {
            this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
        }
        void OptionsButton::onClick() {
            ControlsTab *controlsTab = new ControlsTab();
            MouseTab *mouseTab = new MouseTab();
            VideoTab *videoTab = new VideoTab();
            AudioTab *audioTab = new AudioTab();
            MultiplayerTab *mupltiplayerTab = new MultiplayerTab();
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
