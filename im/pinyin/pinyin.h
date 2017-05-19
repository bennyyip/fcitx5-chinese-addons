/*
 * Copyright (C) 2017~2017 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _PINYIN_PINYIN_H_
#define _PINYIN_PINYIN_H_

#include <fcitx-config/configuration.h>
#include <fcitx/addonfactory.h>
#include <fcitx/addonmanager.h>
#include <fcitx/inputcontextproperty.h>
#include <fcitx/inputmethodengine.h>
#include <fcitx/instance.h>
#include <libime/pinyinime.h>
#include <memory>

namespace fcitx {

FCITX_CONFIGURATION(
    PinyinEngineConfig,
    fcitx::Option<int, IntConstrain> pageSize{this, "PageSize", "Page size", 5,
                                              IntConstrain(3, 10)};
    fcitx::Option<KeyList> prevPage{this,
                                    "Prev Page",
                                    "Prev Page",
                                    {Key(FcitxKey_minus), Key(FcitxKey_Up)}};
    fcitx::Option<KeyList> nextPage{this,
                                    "Next Page",
                                    "Next Page",
                                    {Key(FcitxKey_equal), Key(FcitxKey_Down)}};
    fcitx::Option<int, IntConstrain> nbest{this, "Number of sentence",
                                           "Number of Sentence", 2,
                                           IntConstrain(1, 3)};);

class PinyinState;

class PinyinEngine : public InputMethodEngine {
public:
    PinyinEngine(Instance *instance);
    ~PinyinEngine();
    Instance *instance() { return instance_; }
    void keyEvent(const InputMethodEntry &entry, KeyEvent &keyEvent) override;
    std::vector<InputMethodEntry> listInputMethods() override;
    void reloadConfig() override;
    void reset(const InputMethodEntry &entry,
               InputContextEvent &event) override;
    void save() override;

    libime::PinyinIME *ime() { return ime_.get(); }

    void updateUI(InputContext *inputContext);
    auto state() { return &factory_; }

private:
    Instance *instance_;
    PinyinEngineConfig config_;
    std::unique_ptr<libime::PinyinIME> ime_;
    KeyList selectionKeys_;
    FactoryFor<PinyinState> factory_;
    AddonInstance *quickphrase_;
};

class PinyinEngineFactory : public AddonFactory {
public:
    AddonInstance *create(AddonManager *manager) override {
        return new PinyinEngine(manager->instance());
    }
};
}

#endif // _PINYIN_PINYIN_H_
