#include <sstream>
#include <vector.h>
#include <util.h>

#include "util.h"
#include "gameManager.h"
#include "guiAppState.h"

using namespace gameBase;
using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
    GuiAppState::GuiAppState() {
				type = AppStateType::GUI_STATE;
    }

    GuiAppState::~GuiAppState() {}

    void GuiAppState::update() {
				GameManager *gm = GameManager::getSingleton();
        Vector2 mousePos = getCursorPos();

        for (Button *b : buttons) {
            if (b->isSeparate())
                b->update();

            bool withinX = mousePos.x > b->getPos().x && mousePos.x < b->getPos().x + b->getSize().x;
            bool withinY = mousePos.y > b->getPos().y && mousePos.y < b->getPos().y + b->getSize().y;

            if(withinX && withinY)
                b->onMouseOver();
            else
                b->onMouseOff();
        }

        for (Listbox *l : listboxes)
            l->update();

				if(currentListbox && (currentListbox->getScrollingButton()->getPos().x < mousePos.x && mousePos.x < currentListbox->getPos().x + currentListbox->getSize().x))
						currentListbox->scrollToHeight(mousePos.y);

        for (Checkbox *c : checkboxes)
            c->update();

        for (Slider *s : sliders)
            s->update();

				if(currentSlider){
						float percentage = (mousePos.x - currentSlider->getPos().x) / currentSlider->getSize().x;
						currentSlider->setValue(percentage * currentSlider->getMaxValue());
				}

        for (Textbox *t : textboxes){
            t->update();

						if(t->isEnabled() && backspacePressed)
								t->deleteCharacter();
				}

        for (Tooltip *t : tooltips)
            t->update();
    }

    void GuiAppState::onAttached() {
        AbstractAppState::onAttached();

				Mapping *leftClick = new Mapping;
				leftClick->bind = Bind::LEFT_CLICK;
				leftClick->type = Mapping::MOUSE_KEY;
				leftClick->trigger = 0;
				leftClick->action = true;

				Mapping *leftShift = new Mapping;
				leftShift->bind = Bind::SHIFT_CAPS;
				leftShift->type = Mapping::KEYBOARD;
				leftShift->trigger = GLFW_KEY_LEFT_SHIFT;
				leftShift->action = true;

				Mapping *deleteCharacter = new Mapping;
				deleteCharacter->bind = Bind::DELETE_CHAR;
				deleteCharacter->type = Mapping::KEYBOARD;
				deleteCharacter->trigger = GLFW_KEY_BACKSPACE;
				deleteCharacter->action = true;

				Mapping *up = new Mapping;
				up->bind = Bind::SCROLLING_UP;
				up->type = Mapping::KEYBOARD;
				up->trigger = GLFW_KEY_W;
				up->action = true;

				Mapping *down = new Mapping;
				down->bind = Bind::SCROLLING_DOWN;
				down->type = Mapping::KEYBOARD;
				down->trigger = GLFW_KEY_S;
				down->action = true;

				mappings.push_back(leftClick);
				mappings.push_back(leftShift);
				mappings.push_back(deleteCharacter);
				mappings.push_back(up);
				mappings.push_back(down);
    }

    void GuiAppState::onDettached() {
        AbstractAppState::onDettached();
    }

    void GuiAppState::onAction(int bind, bool isPressed) {
        switch((Bind)bind){
						case Bind::LEFT_CLICK:
								currentSlider = nullptr;
								currentListbox = nullptr;

                if(isPressed){
									Textbox *pastTextbox = currentTextbox;
									bool textboxClicked = false;

                  for (int i = 0; i < buttons.size(); i++) {
											Vector2 mousePos = getCursorPos();
                      Button *b = buttons[i];
                      bool withinX = mousePos.x > b->getPos().x && mousePos.x < b->getPos().x + b->getSize().x;
                      bool withinY = mousePos.y > b->getPos().y && mousePos.y < b->getPos().y + b->getSize().y;

                      if (b->isActive() && withinX && withinY){
                          b->onClick();

													for(Listbox *l : listboxes){
															if(b == l->getListboxButton() || b == l->getScrollingButton()){
																	currentListbox = l;
																	break;
															}
													}

													for(Slider *s : sliders)
														if(b == s->getMovableSliderButton()){
																currentSlider = s;
																break;
														}

													for(Textbox *t : textboxes)
															if(b == t->getTextboxButton()){
																	currentTextbox = t;
																	textboxClicked = true;

																	if(pastTextbox && currentTextbox != pastTextbox)
																			pastTextbox->disable();
																	else{
																			currentTextbox = (t->isEnabled() ? t : nullptr);
																	}

																	break;
															}
											}
                  }

									if(!textboxClicked && currentTextbox){
											currentTextbox->disable();
											currentTextbox = nullptr;
									}
								}
                break;
						case Bind::SCROLLING_UP:{
								Listbox *l = getOpenListbox();
								if(l) l->scrollUp();
                break;
						}
						case Bind::SCROLLING_DOWN:{
								Listbox *l = getOpenListbox();
								if(l) l->scrollDown();
                break;
						}
						case Bind::DELETE_CHAR:
								backspacePressed = isPressed;
								break;
        }
    }

    void GuiAppState::updateControlsListbox(int trigger){
        Listbox *controlsListbox=nullptr;

        for(Listbox *l : listboxes)
            if(l->isOpen())
                controlsListbox=l;

        if(controlsListbox){
            int selectedOption=controlsListbox->getSelectedOption(),colonId=-1;
            wstring line = controlsListbox->getContents()[selectedOption];

            for(int i=0;i<line.size()&&colonId==-1;i++)
                if(line.c_str()[i]==':')
                    colonId=i;

            stringstream ss;
            ss<<hex<<trigger;
            line=line.substr(0,colonId+1)+L"0x"/*+ss.str().c_str()*/;
            controlsListbox->changeLine(selectedOption,line);
        }
    }
    
    void GuiAppState::onRawKeyPress(int ch){
				for(Button *b : buttons)
						if(b->getTrigger() == ch){
								b->onClick();
								break;
						}
    }

		void GuiAppState::onRawCharPress(u32 codepoint){
				if(currentTextbox)
					currentTextbox->type(codepoint);
		}
    
    void GuiAppState::onRawMousePress(int trigger){
        //updateControlsListbox(trigger);
    }
    
    Textbox* GuiAppState::getOpenTextbox() {
        for (int i = 0; i < textboxes.size(); i++)
            if (textboxes[i]->isEnabled())
                return textboxes[i];
    }

    Listbox* GuiAppState::getOpenListbox() {
        for (int i = 0; i < listboxes.size(); i++)
            if (listboxes[i]->isOpen())
								return listboxes[i];
    }

    void GuiAppState::onAnalog(int bind, float strength) {}

    void GuiAppState::addButton(Button* b) {
        buttons.push_back(b);
    }

    void GuiAppState::addListbox(Listbox* l) {
        listboxes.push_back(l);
        buttons.push_back(l->getListboxButton());
        buttons.push_back(l->getScrollingButton());
    }

    void GuiAppState::addCheckbox(Checkbox* c) {
        buttons.push_back(c->getCheckboxButton());
        checkboxes.push_back(c);
    }

    void GuiAppState::addSlider(Slider* s) {
        buttons.push_back(s->getMovableSliderButton());
        buttons.push_back(s->getStaticSliderButton());
        sliders.push_back(s);
    }

    void GuiAppState::addTextbox(Textbox* t) {
        buttons.push_back(t->getTextboxButton());
        textboxes.push_back(t);
    }

    void GuiAppState::addTooltip(Tooltip* t) {
        tooltips.push_back(t);
    }

    void GuiAppState::removeButton(Button *b) {
        for (int i = 0; i < buttons.size(); i++) {
            if (b == buttons[i]) {
								/*
                if (b->isImageButton())
                    GameManager::getSingleton()->detachImage(b->getImage());
										*/
                delete b;
                buttons.erase(buttons.begin() + i);
            }
        }
    }

    void GuiAppState::removeButton(string name) {
        for (int i = 0; i < buttons.size(); i++) {
            if (name == buttons[i]->getName() && buttons[i]->isSeparate()) {
								/*
                if (buttons[i]->isImageButton())
                    GameManager::getSingleton()->detachImage(buttons[i]->getImage());
										*/

                delete buttons[i];

                buttons.erase(buttons.begin() + i);
            }
        }
    }

    void GuiAppState::removeSeparateButton(Button *b) {
        for (int i = 0; i < buttons.size(); i++) {
            if (b == buttons[i]) {
                delete b;
                buttons.erase(buttons.begin() + i);
            }
        }
    }

    void GuiAppState::removeListbox(Listbox *l) {
        for (int i = 0; i < listboxes.size(); i++) {
            if (l == listboxes[i]) {
                removeSeparateButton(l->getListboxButton());
                delete l;
                listboxes.erase(listboxes.begin() + i);
            }
        }
    }

    void GuiAppState::removeCheckbox(Checkbox *c) {
        for (int i = 0; i < checkboxes.size(); i++) {
            if (c == checkboxes[i]) {
                removeSeparateButton(c->getCheckboxButton());
                delete c;
                checkboxes.erase(checkboxes.begin() + i);
            }
        }
    }

    void GuiAppState::removeSlider(Slider *s) {
        for (int i = 0; i < sliders.size(); i++) {
            if (s == sliders[i]) {
                removeSeparateButton(s->getMovableSliderButton());
                removeSeparateButton(s->getStaticSliderButton());
                delete s;
                sliders.erase(sliders.begin() + i);
            }
        }
    }

    void GuiAppState::removeTextbox(Textbox* t) {
        for (int i = 0; i < textboxes.size(); i++) {
            if (t == textboxes[i]) {
                removeSeparateButton(t->getTextboxButton());
                delete t;
                textboxes.erase(textboxes.begin() + i);
            }
        }
    }

    void GuiAppState::removeTooltip(Tooltip *t) {
        for (int i = 0; i < tooltips.size(); i++) {
            if (t == tooltips[i]) {
                delete t;
                tooltips.erase(tooltips.begin() + i);
            }
        }
    }

    void GuiAppState::removeAllButtons() {
        while (buttons.size() > 0) {
            delete buttons[buttons.size() - 1];
            buttons.pop_back();
        }
    }

    void GuiAppState::removeAllListboxes() {
        while (listboxes.size() > 0) {
            removeSeparateButton(listboxes[listboxes.size() - 1]->getListboxButton());
            delete listboxes[listboxes.size() - 1];
            listboxes.pop_back();
        }
    }

    void GuiAppState::removeAllCheckboxes() {
        while (checkboxes.size() > 0) {
            removeSeparateButton(checkboxes[checkboxes.size() - 1]->getCheckboxButton());
            delete checkboxes[checkboxes.size() - 1];
            checkboxes.pop_back();
        }
    }

    void GuiAppState::removeAllSliders() {
        while (sliders.size() > 0) {
            removeSeparateButton(sliders[sliders.size() - 1]->getMovableSliderButton());
            removeSeparateButton(sliders[sliders.size() - 1]->getStaticSliderButton());
            delete sliders[sliders.size() - 1];
            sliders.pop_back();
        }
    }

    void GuiAppState::removeAllTextboxes() {
        while (textboxes.size() > 0) {
            removeSeparateButton(textboxes[textboxes.size() - 1]->getTextboxButton());
            delete textboxes[textboxes.size() - 1];
            textboxes.pop_back();
        }
    }
    
    void GuiAppState::removeAllTooltips(){
        while(!tooltips.empty()){
            delete tooltips[tooltips.size()-1];
            tooltips.pop_back();
        }
    }
}
