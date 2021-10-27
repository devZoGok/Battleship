#include "textbox.h"
#include "defConfigs.h"
#include "util.h"

using namespace game::core;
using namespace game::util;
using namespace irr::core;
using namespace irr::video;

namespace game{
    namespace gui{
        Textbox::TextboxButton::TextboxButton(GameManager *gM, Textbox *t, vector2d<s32> pos, vector2d<s32> size, stringw name, bool separate) : Button(gM, pos, size, name, separate) {
            textbox = t;
        }

        void Textbox::TextboxButton::onClick() {
            if (!textbox->isEnabled())
                textbox->enable();
            else
                textbox->disable();
        }

        Textbox::Textbox(GameManager *gM, vector2d<s32> pos, vector2d<s32> size) {
            gameManager = gM;
            this->pos = pos;
            this->size = size;
            textboxButton = new TextboxButton(gM, this, pos, size, "TextboxButton", false);
            font = gameManager->getDevice()->getGUIEnvironment()->getFont(PATH + "Fonts/bigfont.png");
        }

        Textbox::~Textbox() {}

        void Textbox::update() {
            gameManager->getDevice()->getVideoDriver()->draw2DRectangle(SColor(255, 200, 200, 200), rect<s32>(pos.X, pos.Y, pos.X + size.X, pos.Y + size.Y), nullptr);
            font->draw(entry, rect<s32>(pos.X, pos.Y - 8, pos.X + size.X, pos.Y - 8 + size.Y), SColor(255, 255, 255, 255));
            if (enabled) {
                if (canChangeCursor()) {
                    canShowCursor = !canShowCursor;
                    lastBlinkTime = getTime();
                }
                if (canShowCursor)
                    gameManager->getDevice()->getVideoDriver()->draw2DRectangle(SColor(255, 100, 100, 100), rect<s32>(pos.X + 20 * (entry.size() - cursorPosOffset), pos.Y, pos.X + 4 + 20 * (entry.size() - cursorPosOffset), pos.Y + size.Y), nullptr);
            }
        }

        void Textbox::type(wchar_t ch) {
            if (!capitalLeters)
                ch = tolower(ch);
            else
                ch = toupper(ch);
            std::vector<wchar_t> v;
            for (int i = 0; i < entry.size(); i++)
                v.push_back(entry[i]);
            v.insert(v.end() - cursorPosOffset, ch);
            entry += 0;
            for (int i = 0; i < v.size(); i++)
                entry[i] = v[i];
        }

        void Textbox::moveCursor(bool left) {
            if (left && entry.size() - cursorPosOffset > 0)
                cursorPosOffset++;
            else if (!left && cursorPosOffset > 0)
                cursorPosOffset--;
        }

        void Textbox::deleteCharacter() {
            if (cursorPosOffset != entry.size()) {
                std::vector<wchar_t> v;
                for (int i = 0; i < entry.size(); i++)
                    v.push_back(entry[i]);
                v.erase(v.end() - 1 - cursorPosOffset);
                entry.erase(entry.size() - 1);
                for (int i = 0; i < v.size(); i++)
                    entry[i] = v[i];
            }
        }

        void Textbox::enable() {
            enabled = true;
            lastBlinkTime = getTime();
        }

        void Textbox::disable() {
            enabled = false;
        }
    }
}
