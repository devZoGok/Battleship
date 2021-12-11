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

        for (Checkbox *c : checkboxes)
            c->update();

        for (Slider *s : sliders)
            s->update();

        for (Textbox *t : textboxes)
            t->update();

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

				mappings.push_back(leftClick);
				mappings.push_back(leftShift);

        attachKeyboardKeys();
    }

    void GuiAppState::onDettached() {
        AbstractAppState::onDettached();
    }
    
    void GuiAppState::attachKeyboardKeys() {
				/*
        int firstId = Bind::LAST_BIND + 1, lastId = firstId + numKeys;

        for (int i = firstId; i <= lastId; i++){
            int offset = i - firstId;
            int trigger = (offset < 10 ? int(irr::KEY_KEY_0) + offset : int(irr::KEY_KEY_A) + offset - 10);

						Mapping *mapping = new Mapping;
						mapping->bind = i;
						mapping->type = Mapping::KEYBOARD;
						mapping->trigger = trigger;
						mapping->action = true;

            mappings.push_back(mapping);
        } 
				*/
    }

    void GuiAppState::onAction(int bind, bool isPressed) {
        switch((Bind)bind){
						case Bind::LEFT_CLICK:
                if(isPressed)
                    for (int i = 0; i < buttons.size(); i++) {
												Vector2 mousePos = getCursorPos();
                        Button *b = buttons[i];
                        bool withinX = mousePos.x > b->getPos().x && mousePos.x < b->getPos().x + b->getSize().x;
                        bool withinY = mousePos.y > b->getPos().y && mousePos.y < b->getPos().y + b->getSize().y;

                        if (withinX && withinY) 
                            b->onClick();
                    }
                break;
						case Bind::SCROLLING_UP:
                for (Listbox *l : listboxes) 
                    if (l->isOpen())
                        l->scrollUp();
                break;
						case Bind::SCROLLING_DOWN: 
                for (Listbox *l : listboxes) 
                    if (l->isOpen())
                        l->scrollDown();
                break;
        }

        Textbox *t = getOpenTextbox();
            if (t) 
                switch((Bind)bind){
										case Bind::SHIFT_CAPS:
                        shiftPressed = !shiftPressed;
                        //t->setIsCapitalLeters(isPressed);
                        break;
										case Bind::LEFT:
                        //if(isPressed)t->moveCursor(true);
                        break;
										case Bind::RIGHT:
                        //if(isPressed)t->moveCursor(false);
                        break;
										case Bind::CAPS_LOCK: 
                        //if(isPressed)t->setIsCapitalLeters(!t->isCapitalLeters());
                        break;
										case Bind::SPACE:
                        if(isPressed)t->type(' ');
                        break;
										case Bind::DELETE_CHAR:
                        if(isPressed)t->deleteCharacter();
                        break;
										case Bind::PLUS:
                        if(isPressed) t->type(shiftPressed ? '+' : '=');
                        break;
										case Bind::MINUS:
                        if(isPressed) t->type(shiftPressed ? '_' : '-');
                        break;
										case Bind::DEVSTERISK:
                        if(isPressed) t->type(shiftPressed ? '~' : '`');
                        break;
                    default:
                        if(bind > Bind::LAST_BIND)checkKeyboard(t, (Bind)bind, isPressed);
                }
    }

    void GuiAppState::checkKeyboard(Textbox *t, Bind bind, bool isPressed) {
        int firstId = Bind::LAST_BIND + 1, lastId = firstId + numKeys;

        for (int i = firstId; i <= lastId; i++) {
            int offset = i - firstId;
            char c = keyChars[offset];

            if (bind == i && isPressed) {
                if (shiftPressed) {
                    switch(offset){
                        case 1:
                            c = '!';
                            break;
                        case 2:
                            c = '@';
                            break;
                        case 3:
                            c = '#';
                            break;
                        case 4:
                            c = '$';
                            break;
                        case 5:
                            c = '%';
                            break;
                        case 6:
                            c = '^';
                            break;
                        case 7:
                            c = '&';
                            break;
                        case 8:
                            c = '*';
                            break;
                        case 9:
                            c = '(';
                            break;
                        case 0:
                            c = ')';
                            break;
                    }
                }

                t->type(c);
            }
        }
    }

    void GuiAppState::updateControlsListbox(int trigger){
        Listbox *controlsListbox=nullptr;

        for(Listbox *l : listboxes)
            if(l->isOpen() /*&& l->isControlsListbox()*/)
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
				Button *button = nullptr;
				
				for(Button *b : buttons)
						if(b->getTrigger() == ch){
								button = b;
								break;
						}
				
				if(button)
						button->onClick();
				
				Textbox *t = nullptr;
				
				for(int i = 0; i < textboxes.size() && !t; i++)
					if(textboxes[i]->isEnabled())
						t = textboxes[i];
				
				if(t && canType()){
					switch(ch){
						case '/':
							if(shiftPressed)ch = '?';
							break;
						case '.':
							if(shiftPressed)ch = '>';
							break;
						case ',':
							if(shiftPressed)ch = '<';
							break;
						case ';':
							if(shiftPressed)ch = ':';
							break;
						case 39:
							if(shiftPressed)ch = '"';
							break;
						case '-':
							if(shiftPressed)ch = '_';
							break;
						case '=':
							if(shiftPressed)ch = '+';
							break;
						case 92:
							if(shiftPressed)ch = '|';
							break;
						case '`':
							if(shiftPressed)ch = '~';
							break;
						case '[':
							if(shiftPressed)ch = '{';
							break;
						case ']':
							if(shiftPressed)ch = '}';
							break;
						case '1':
							if(shiftPressed)ch = '!';
							break;
						case '2':
							if(shiftPressed)ch = '@';
							break;
						case '3':
							if(shiftPressed)ch = '#';
							break;
						case '4':
							if(shiftPressed)ch = '$';
							break;
						case '5':
							if(shiftPressed)ch = '%';
							break;
						case '6':
							if(shiftPressed)ch = '^';
							break;
						case '7':
							if(shiftPressed)ch = '&';
							break;
						case '8':
							if(shiftPressed)ch = '*';
							break;
						case '9':
							if(shiftPressed)ch = '(';
							break;
						case '0':
							if(shiftPressed)ch = ')';
							break;
					}
				
					if(ch != GLFW_KEY_BACKSPACE)
						t->type(ch, shiftPressed);
					else 
						t->deleteCharacter();
				
					lastTypeTime = getTime();
				}
    }
    
    void GuiAppState::onRawMousePress(int trigger){
        updateControlsListbox(trigger);
    }
    
    Textbox* GuiAppState::getOpenTextbox() {
        Textbox* t = nullptr;
        for (int i = 0; i < textboxes.size(); i++)
            if (textboxes[i]->isEnabled())
                t = textboxes[i];
        if (t)
            return t;
        else
            return nullptr;
    }

    Listbox* GuiAppState::getOpenListbox() {
        Listbox *l = nullptr;
        for (int i = 0; i < listboxes.size(); i++)
            if (listboxes[i]->isOpen())
                l = listboxes[i];
        if (l)
            return l;
        else
            return nullptr;
    }

    void GuiAppState::onAnalog(int bind, float strength) {}

    void GuiAppState::addButton(Button* b) {
        buttons.push_back(b);
    }

    void GuiAppState::addListbox(Listbox* l) {
        listboxes.push_back(l);
        buttons.push_back(l->getListboxButton());
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
