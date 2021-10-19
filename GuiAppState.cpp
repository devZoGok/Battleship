#include"GuiAppState.h"
#include<sstream>

using namespace game::gui;
using namespace irr;
using namespace irr::core;
using namespace std;

namespace game{
    namespace core{
        irr::u16 mousePos[2]{};

        GuiAppState::GuiAppState(GameManager* gM) {
            gameManager = gM;
            type = AppStateTypes::GUI_STATE;
        }

        GuiAppState::~GuiAppState() {}

        void GuiAppState::update() {
            mousePos[0] = gameManager->getDevice()->getCursorControl()->getPosition().X;
            mousePos[1] = gameManager->getDevice()->getCursorControl()->getPosition().Y;
            for (Button *b : buttons) {
                if (b->isSeparate())
                    b->update();
                bool withinX=mousePos[0] > b->getPos().X && mousePos[0] < b->getPos().X + b->getSize().X;
                bool withinY=mousePos[1] > b->getPos().Y && mousePos[1] < b->getPos().Y + b->getSize().Y;
                if(withinX&&withinY)
                    b->onMouseOver();
                else
                    b->onMouseAway();
                b->setMouseOverDone(withinX&&withinY);
                b->setMouseAwayDone(!(withinX&&withinY));
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

        void GuiAppState::onAttachment() {
            AbstractAppState::onAttachment();
            gameManager->getDevice()->getCursorControl()->setVisible(true);
            attachKeyboardKeys();
        }

        void GuiAppState::onDetachment() {
            AbstractAppState::onDetachment();
            AbstractAppState::detachAllKeys();
            gameManager->getDevice()->getCursorControl()->setVisible(false);
        }
        
        void GuiAppState::attachKeyboardKeys() {
            int firstId=Bind::LAST_BIND+1,lastId=firstId+numKeys;
            for (int i = firstId; i <= lastId; i++){
                int offset=i-firstId;
                int trigger=offset<10?int(irr::KEY_KEY_0)+offset:int(irr::KEY_KEY_A)+offset-10;
                AbstractAppState::attachKey(new Key((Bind)i, trigger, true, false));
            } 
        }

        void GuiAppState::onAction(Bind bind, bool isPressed) {
            switch(bind){
                case LEFT_CLICK:
                    if(isPressed)
                        for (int i=0;i<buttons.size();i++) {
                            Button *b=buttons[i];
                            bool withinX=mousePos[0] > b->getPos().X && mousePos[0] < b->getPos().X + b->getSize().X;
                            bool withinY=mousePos[1] > b->getPos().Y && mousePos[1] < b->getPos().Y + b->getSize().Y;
                            if (withinX&&withinY) 
                                b->onClick();
                        }
                    break;
                case SCROLLING_UP:
                    for (Listbox *l : listboxes) 
                        if (l->isOpen())
                            l->scrollUp();
                    break;
                case SCROLLING_DOWN: 
                    for (Listbox *l : listboxes) 
                        if (l->isOpen())
                            l->scrollDown();
                    break;
            }
            Textbox *t = getOpenTextbox();
                if (t) 
                    switch(bind){
                        case SHIFT_CAPS:
                            shiftPressed = !shiftPressed;
                            t->setIsCapitalLeters(isPressed);
                            break;
                        case LEFT:
                            if(isPressed)t->moveCursor(true);
                            break;
                        case RIGHT:
                            if(isPressed)t->moveCursor(false);
                            break;
                        case CAPS_LOCK: 
                            if(isPressed)t->setIsCapitalLeters(!t->isCapitalLeters());
                            break;
                        case SPACE:
                            if(isPressed)t->type(' ');
                            break;
                        case DELETE_CHAR:
                            if(isPressed)t->deleteCharacter();
                            break;
                        case PLUS:
                            if(isPressed) t->type(shiftPressed?'+':'=');
                            break;
                        case MINUS:
                            if(isPressed) t->type(shiftPressed?'_':'-');
                            break;
                        case DEVSTERISK:
                            if(isPressed) t->type(shiftPressed?'~':'`');
                            break;
                        default:
                            if(bind>LAST_BIND)checkKeyboard(t, bind, isPressed);
                    }
        }

        void GuiAppState::checkKeyboard(Textbox *t, Bind bind, bool isPressed) {
            int firstId=Bind::LAST_BIND+1,lastId=firstId+numKeys;
            for (int i = firstId; i <= lastId; i++) {
                int offset=i-firstId;
                char c = keyChars[offset];
                if (bind==i&&isPressed) {
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
                if(l->isOpen()&&l->isControlsListbox())
                    controlsListbox=l;
            if(controlsListbox){
                int selectedOption=controlsListbox->getSelectedOption(),colonId=-1;
                stringw line=controlsListbox->getLine(selectedOption);
                for(int i=0;i<line.size()&&colonId==-1;i++)
                    if(line.c_str()[i]==':')
                        colonId=i;
                stringstream ss;
                ss<<hex<<trigger;
                line=line.subString(0,colonId+1)+stringw("0x")+stringw(ss.str().c_str());
                controlsListbox->changeLine(selectedOption,line);
            }
        }
        
        void GuiAppState::onRawKeyPress(SEvent::SKeyInput event){
            updateControlsListbox(event.Key);
        }
        
        void GuiAppState::onRawMousePress(SEvent::SMouseInput event){
            int trigger=0;
            if(event.isMiddlePressed())
                trigger=1;
            else if(event.isRightPressed())
                trigger=2;
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

        void GuiAppState::onAnalog(Bind bind, double strength) {}

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
                    if (b->isImageButton())
                        gameManager->detachImage(b->getImage());
                    delete b;
                    buttons.erase(buttons.begin() + i);
                }
            }
        }

        void GuiAppState::removeButton(stringw name) {
            for (int i = 0; i < buttons.size(); i++) {
                if (name == buttons[i]->getName() && buttons[i]->isSeparate()) {
                    if (buttons[i]->isImageButton())
                        gameManager->detachImage(buttons[i]->getImage());
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
}
