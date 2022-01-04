#include "optionsButton.h"
#include "util.h"

#include <stateManager.h>

using namespace vb01Gui;
using namespace vb01;
using namespace std;

namespace battleship{
	using namespace configData;

    OptionsButton::OkButton::OkButton() : Button(Vector2(50, GameManager::getSingleton()->getHeight() - 150), Vector2(140, 50), "Ok", PATH + "Fonts/batang.ttf", -1, true) {
        this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::GUI_STATE));
    }
    void OptionsButton::OkButton::onClick() {

    }

    OptionsButton::DefaultsButton::DefaultsButton() : Button(Vector2(200, GameManager::getSingleton()->getHeight() - 150), Vector2(140, 50), "Restore defaults", PATH + "Fonts/batang.ttf", -1, true) {
        this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::GUI_STATE));
    }
    void OptionsButton::DefaultsButton::onClick() {
        
    }

    OptionsButton::BackButton::BackButton() : Button(Vector2(350, GameManager::getSingleton()->getHeight() - 150), Vector2(140, 50), "Back", PATH + "Fonts/batang.ttf", -1, true) {
        this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::GUI_STATE));
    }
    void OptionsButton::BackButton::onClick() {
				GameManager *gm = GameManager::getSingleton();
				/*
        gm->detachAllBitmapTexts();
        gm->detachAllImages();
				*/
        state->removeAllListboxes();
        state->removeAllCheckboxes();
        state->removeAllSliders();
        state->removeAllTextboxes();
        state->removeButton("Ok");
        state->removeButton("Restore defaults");
        OptionsButton *optionsButton = new OptionsButton(Vector2(), Vector2(), "Options", true);
        optionsButton->onClick();
        state->removeButton("Back");
    }

    OptionsButton::TabButton::TabButton(Vector2 pos, Vector2 size, string name) :Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, true) {
        this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::GUI_STATE));
    }
    void OptionsButton::TabButton::onClick() {
        state->removeButton("Back");
        OkButton *okButton = new OkButton();
        DefaultsButton *defaultsButton = new DefaultsButton();
        BackButton *returnButton = new BackButton();
        //defaultsButton->moveText(-25, 0);
        state->addButton(okButton);
        state->addButton(defaultsButton);
        state->addButton(returnButton);
    }

    OptionsButton::ControlsTab::ControlsTab() : TabButton(
						Vector2(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10),
					 	Vector2(100, 50),
					 	"Controls"
		) {}

    void OptionsButton::ControlsTab::onClick() {
        TabButton::onClick();
        std::vector<string> lines;
        readFile(PATH + "../options.cfg", lines);

				GameManager *gm = GameManager::getSingleton();
        Listbox *listbox = new Listbox(Vector2(gm->getWidth() / 4, gm->getHeight() / 10), Vector2(360, 20), lines, lines.size() < 5 ? lines.size() : 5, PATH + "Fonts/batang.ttf");

        state->addListbox(listbox);
        state->removeButton("Mouse");
        state->removeButton("Video");
        state->removeButton("Audio");
        state->removeButton("Multiplayer");
        state->removeButton("Controls");
    }

    OptionsButton::MouseTab::MouseTab() : TabButton(
						Vector2(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 60),
					 	Vector2(100, 50),
					 	"Mouse"
		) {}

    void OptionsButton::MouseTab::onClick() {
        TabButton::onClick();

				GameManager *gm = GameManager::getSingleton();
        Vector2 pos = Vector2(gm->getWidth() / 3, gm->getHeight() / 4);

        string font = PATH + "Fonts/batang.ttf";
        Slider *mouseSensitivitySlider = new Slider(Vector2(pos.x, pos.y), Vector2(300, 10), .1, 3.);
        Textbox *mouseSensitivityTextbox = new Textbox(Vector2(pos.x + 320, pos.y - 10), Vector2(100, 20), font);
        Checkbox *reverseMouseCheckbox = new Checkbox(Vector2(pos.x, pos.y + 50), font);

				/*
        gm->attachBitmapText(new BitmapText("MouseSensitivity", vector2d<s32>(gm->getWidth() / 3 - 90, gm->getHeight() / 4 - 5), font));
        gm->attachBitmapText(new BitmapText("ReverseMouse", vector2d<s32>(gm->getWidth() / 3 + 20, gm->getHeight() / 4 + 50), font));
				*/

        state->addTextbox(mouseSensitivityTextbox);
        state->addSlider(mouseSensitivitySlider);
        state->addCheckbox(reverseMouseCheckbox);
        state->removeButton("Controls");
        state->removeButton("Video");
        state->removeButton("Audio");
        state->removeButton("Multiplayer");
        state->removeButton("Mouse");
    }

    OptionsButton::VideoTab::VideoTab() : TabButton(Vector2(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 120), Vector2(100, 50), "Video") {}
    void OptionsButton::VideoTab::onClick() {
        TabButton::onClick();
        std::vector<string> lines;

        for (int i = 0; i < 10; i++) {
            string s;
            s += to_string(i);
            lines.push_back(s);
        }

				GameManager *gm = GameManager::getSingleton();
        string font = PATH + "Fonts/batang.ttf";
        Vector2 pos = Vector2(gm->getWidth() / 3, gm->getHeight() / 8);
        Listbox *resolutionsListbox = new Listbox(Vector2(pos.x, pos.y), Vector2(100, 20), lines, 5, font);

        Checkbox *fullscreenCheckbox = new Checkbox(Vector2(pos.x, pos.y + 30), font);
        Checkbox *vsyncCheckbox = new Checkbox(Vector2(pos.x, pos.y + 50), font);
        Checkbox *normalMapCheckbox = new Checkbox(Vector2(pos.x, pos.y + 70), font);
        Checkbox *parallaxMapCheckbox = new Checkbox(Vector2(pos.x, pos.y + 90), font);
        Checkbox *specularMapCheckbox = new Checkbox(Vector2(pos.x, pos.y + 110), font);

				/*
        gm->attachBitmapText(new BitmapText("Fullscreen", vector2d<s32>(pos.X + 20, pos.Y + 30), font));
        gm->attachBitmapText(new BitmapText("VSync", vector2d<s32>(pos.X + 20, pos.Y + 50), font));
        gm->attachBitmapText(new BitmapText("Normal map", vector2d<s32>(pos.X + 20, pos.Y + 70), font));
        gm->attachBitmapText(new BitmapText("Parallax map", vector2d<s32>(pos.X + 20, pos.Y + 90), font));
        gm->attachBitmapText(new BitmapText("Specular map", vector2d<s32>(pos.X + 20, pos.Y + 110), font));
				*/

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

    OptionsButton::AudioTab::AudioTab() : TabButton(Vector2(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 180), Vector2(100, 50), "Audio") {}
    void OptionsButton::AudioTab::onClick() {
        TabButton::onClick();
				GameManager *gm = GameManager::getSingleton();
        Vector2 pos(gm->getWidth() / 3, gm->getHeight() / 8);
        Slider *volumeSlider = new Slider(Vector2(pos.x, pos.y), Vector2(300, 10), 0., 2.);
        Textbox *volumeTextbox = new Textbox(Vector2(pos.x + 320, pos.y), Vector2(100, 20), PATH + "Fonts/batang.ttf");
				volumeSlider->setTextbox(volumeTextbox);
				volumeTextbox->setSlider(volumeSlider);
        state->addTextbox(volumeTextbox);
        state->addSlider(volumeSlider);
        state->removeButton("Controls");
        state->removeButton("Mouse");
        state->removeButton("Video");
        state->removeButton("Multiplayer");
        state->removeButton("Audio");
    }

    OptionsButton::MultiplayerTab::MultiplayerTab() : TabButton(Vector2(GameManager::getSingleton()->getWidth() / 4, GameManager::getSingleton()->getHeight() / 10 + 240), Vector2(100, 50), "Multiplayer") {}
    void OptionsButton::MultiplayerTab::onClick() {
        TabButton::onClick();
				GameManager *gm = GameManager::getSingleton();
        string font = PATH + "Fonts/batang.ttf";
        Vector2 pos = Vector2(gm->getWidth() / 3, gm->getHeight() / 6);
        Textbox *tcpTextbox = new Textbox(Vector2(pos.x, pos.y), Vector2(100, 20), font);
        Textbox *udpTextbox = new Textbox(Vector2(pos.x, pos.y + 30), Vector2(100, 20), font);
        Textbox *playerNameTextbox = new Textbox(Vector2(pos.x, pos.y + 60), Vector2(100, 20), font);

				/*
        gm->attachBitmapText(new BitmapText("TCP port", vector2d<s32>(pos.X - 50, pos.Y), font));
        gm->attachBitmapText(new BitmapText("UDP port", vector2d<s32>(pos.X - 50, pos.Y + 30), font));
        gm->attachBitmapText(new BitmapText("Player name", vector2d<s32>(pos.X - 65, pos.Y + 60), font));
				*/

        state->addTextbox(tcpTextbox);
        state->addTextbox(udpTextbox);
        state->addTextbox(playerNameTextbox);
        state->removeButton("Controls");
        state->removeButton("Mouse");
        state->removeButton("Video");
        state->removeButton("Audio");
        state->removeButton("Multiplayer");
    }

    OptionsButton::OptionsButton(Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, separate) {
        this->state = ((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::GUI_STATE));
    }
    void OptionsButton::onClick() {
        ControlsTab *controlsTab = new ControlsTab();
        MouseTab *mouseTab = new MouseTab();
        VideoTab *videoTab = new VideoTab();
        AudioTab *audioTab = new AudioTab();
        MultiplayerTab *mupltiplayerTab = new MultiplayerTab();
        //mupltiplayerTab->moveText(-20, 0);
        state->addButton(controlsTab);
        state->addButton(mouseTab);
        state->addButton(videoTab);
        state->addButton(audioTab);
        state->addButton(mupltiplayerTab);
        state->removeButton(this);
    }
}
