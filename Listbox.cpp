#include"Listbox.h"
#include"GuiAppState.h"
#include"DefConfigs.h"

using namespace game::core;
using namespace irr::core;
using namespace irr::gui;
using namespace irr::video;
using namespace std;

namespace game{
    namespace gui{
        Listbox::ListboxButton::ListboxButton(GameManager *gM, Listbox *l, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            listbox = l;
        }

        void Listbox::ListboxButton::onClick() {
            if (!listbox->isOpen())
                listbox->openUp();
            else
                listbox->close();
        }

        Listbox::ScrollingButton::ScrollingButton(GameManager *gM, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
        }

        void Listbox::ScrollingButton::onClick() {

        }

        Listbox::Listbox(GameManager *gM, vector2d<s32> pos, vector2d<s32> size, vector<stringw> lines, int maxDisplay, bool controlsListbox) {
            gameManager = gM;
            this->pos = pos;
            this->size = size;
            this->lines = lines;
            this->maxDisplay = maxDisplay;
            this->controlsListbox=controlsListbox;
            listboxButton = new ListboxButton(gM, this, pos, size, "ListboxButton", false);
            scrollingButton = new ScrollingButton(gM, vector2d<s32>(pos.X + size.X - 20, pos.Y + 20), vector2d<s32>(20, 20.0 * (maxDisplay - 2) / (lines.size() - maxDisplay)), "scrollingButton", false);
        }

        Listbox::~Listbox() {}

        void Listbox::update() {
            IGUIFont *font = gameManager->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/fontcourier.bmp");
            if (open) {
                gameManager->getDevice()->getVideoDriver()->draw2DRectangle(*listboxButton->getColor(), rect<s32>(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y * maxDisplay), nullptr);
                int mousePosX = gameManager->getDevice()->getCursorControl()->getPosition().X, mousePosY = gameManager->getDevice()->getCursorControl()->getPosition().Y;
                for (int i = 0; i < maxDisplay; i++) {
                    if (mousePosY > pos.Y + size.Y * i && mousePosY < pos.Y + size.Y * (i + 1)){
                        selectedOption=scrollOffset+i;
                        gameManager->getDevice()->getVideoDriver()->draw2DRectangle(SColor(255, 200, 125, 0), rect<s32>(pos.X, pos.Y + size.Y * i, pos.X + size.X - 20, pos.Y + size.Y * (i + 1)), nullptr);
                    }
                    else if (mousePosY < pos.Y){
                        selectedOption=scrollOffset;
                        gameManager->getDevice()->getVideoDriver()->draw2DRectangle(SColor(255, 200, 125, 0), rect<s32>(pos.X, pos.Y, pos.X + size.X - 20, pos.Y + size.Y), nullptr);
                    }
                    else if (mousePosY > pos.Y + size.Y * maxDisplay){
                        selectedOption=scrollOffset+maxDisplay-1;
                        gameManager->getDevice()->getVideoDriver()->draw2DRectangle(SColor(255, 200, 125, 0), rect<s32>(pos.X, pos.Y + size.Y * (maxDisplay - 1), pos.X + size.X - 20, pos.Y + size.Y * (maxDisplay + 0)), nullptr);
                    }
                }
                for (int i = 0; i < maxDisplay; i++) {
                    font->draw(lines[i + scrollOffset], rect<s32>(pos.X, pos.Y + size.Y*i, pos.X + size.X, pos.Y + size.Y * (i + 1)), SColor(255, 255, 255, 255));
                }
                if (scrollOffset > 0) {
                    vector2d<s32> scrollingButtonPos = scrollingButton->getPos();
                    scrollingButtonPos.Y = pos.Y + 20 + scrollingButton->getSize().Y * (scrollOffset - 1);
                    scrollingButton->setPos(scrollingButtonPos);
                }
                gameManager->getDevice()->getVideoDriver()->draw2DRectangle(SColor(255, 150, 150, 150), rect<s32>(
                        scrollingButton->getPos().X, scrollingButton->getPos().Y, scrollingButton->getPos().X + scrollingButton->getSize().X,
                        scrollingButton->getPos().Y + scrollingButton->getSize().Y));
            } else {
                gameManager->getDevice()->getVideoDriver()->draw2DRectangle(*listboxButton->getColor(), rect<s32>(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y), nullptr);
                font->draw(lines[selectedOption], rect<s32>(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y), SColor(255, 255, 255, 255));
            }
        }

        void Listbox::openUp() {
            open = true;
            vector2d<s32> size = listboxButton->getSize();
            size.Y *= maxDisplay;
            listboxButton->setSize(size);
        }

        void Listbox::close() {
            if(!controlsListbox){
                open = false;
                vector2d<s32> size = listboxButton->getSize();
                size.Y /= maxDisplay;
                listboxButton->setSize(size);
            }
        }

        void Listbox::scrollUp() {
            if (scrollOffset > 0)
                scrollOffset--;
        }

        void Listbox::scrollDown() {
            if (scrollOffset < lines.size() - maxDisplay)
                scrollOffset++;
        }

        void Listbox::appendLines(vector<stringw>& lines) {
            for (int i = 0; i < this->lines.size(); i++)
                lines.push_back(this->lines[i]);
        }
    }
}
