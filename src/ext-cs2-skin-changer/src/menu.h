#pragma once
#include "SDK/weapon/C_EconEntity.h"
#include "../ext/skindb.h"
#include "window/window.hpp"
#include "ui_engine.hpp"
#include "config.h"

static WeaponsEnum CurrentWeaponDef;
static int selectedSkinIndex = 0;
static int selectedKnifeIndex = 0;
static int selectedKnifeSkinIndex = 0;
static int selectedMusicKitIndex = 0;
static char searchBuffer[128] = "";

void UpdateSearchInput() {
}

inline void CoreStage5Panel(float x, float y, float w, float h, float radius)
{
    SC_GUI::DrawFilledRoundedRect(x, y, w, h, radius, Color(255, 6, 13, 9));
    SC_GUI::DrawStrokeRoundedRect(x, y, w, h, radius, Color(255, 25, 64, 39), 1.0f);

    // subtle inner top highlight
    SC_GUI::DrawFilledRoundedRect(x + 1, y + 1, w - 2, 2.0f, radius, Color(28, 86, 255, 119));
}

inline void CoreStage5Header(const std::string& title, const std::string& subtitle, float x, float y, float w)
{
    SC_GUI::DrawFilledRoundedRect(x, y, w, 82, 14.0f, Color(255, 6, 14, 10));
    SC_GUI::DrawStrokeRoundedRect(x, y, w, 82, 14.0f, Color(255, 26, 68, 42), 1.0f);

    SC_GUI::DrawFilledRoundedRect(x + 18, y + 20, 36, 36, 8.0f, Color(255, 9, 42, 21));
    SC_GUI::DrawStrokeRoundedRect(x + 18, y + 20, 36, 36, 8.0f, SC_GUI::currentTheme.accent, 1.0f);
    SC_GUI::DrawStringA("+", x + 31, y + 26, SC_GUI::currentTheme.accent, SC_GUI::largeFont, false);

    SC_GUI::DrawStringA(title, x + 72, y + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(subtitle, x + 72, y + 48, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
}

inline void CoreStage5Pill(const std::string& text, float x, float y, float w, bool active)
{
    Color bg = active ? Color(255, 13, 58, 27) : Color(255, 7, 16, 11);
    Color br = active ? SC_GUI::currentTheme.accent : Color(255, 28, 62, 40);
    Color tx = active ? SC_GUI::currentTheme.accent : SC_GUI::currentTheme.textDim;

    SC_GUI::DrawFilledRoundedRect(x, y, w, 30, 8.0f, bg);
    SC_GUI::DrawStrokeRoundedRect(x, y, w, 30, 8.0f, br, 1.0f);
    SC_GUI::DrawStringA(text, x + 14, y + 8, tx, SC_GUI::smallFont, false);
}

inline void CoreStage5EmptyState(const std::string& title, const std::string& subtitle, float x, float y, float w, float h)
{
    SC_GUI::DrawFilledRoundedRect(x, y, w, h, 14.0f, Color(255, 6, 13, 9));
    SC_GUI::DrawStrokeRoundedRect(x, y, w, h, 14.0f, Color(255, 26, 68, 42), 1.0f);

    SC_GUI::DrawFilledRoundedRect(x + 24, y + 24, 42, 42, 10.0f, Color(255, 9, 42, 21));
    SC_GUI::DrawStrokeRoundedRect(x + 24, y + 24, 42, 42, 10.0f, SC_GUI::currentTheme.accent, 1.0f);
    SC_GUI::DrawStringA("!", x + 42, y + 33, SC_GUI::currentTheme.accent, SC_GUI::largeFont, false);

    SC_GUI::DrawStringA(title, x + 84, y + 28, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(subtitle, x + 84, y + 62, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);
}
void RenderWeaponTab(float x, float y, float w, float h)
{
    float pad = 24.0f;
    float headerX = x + pad;
    float headerY = y + 18.0f;
    float headerW = w - pad * 2;

    std::vector<SkinInfo_t> availableSkins = skindb->GetWeaponSkins(CurrentWeaponDef);
    std::sort(availableSkins.begin(), availableSkins.end(), [](const SkinInfo_t& a, const SkinInfo_t& b) {
        return a.rarity > b.rarity;
    });

    std::string query = searchBuffer;
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);

    std::vector<int> filteredIndices;
    for (int i = 0; i < (int)availableSkins.size(); i++) {
        if (!query.empty()) {
            std::string name = availableSkins[i].name;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (name.find(query) == std::string::npos) continue;
        }
        filteredIndices.push_back(i);
    }

    CoreStage5Header("Weapons", "Browse, search, and apply premium weapon finishes.", headerX, headerY, headerW);
    SC_GUI::TextInput("search_wep", searchBuffer, 128, headerX + headerW - 360, headerY + 20, 330, 36, "Search weapon skins...");

    float pillY = headerY + 92.0f;
    CoreStage5Pill("Loaded: " + std::to_string((int)availableSkins.size()), headerX, pillY, 120, true);
    CoreStage5Pill("Filtered: " + std::to_string((int)filteredIndices.size()), headerX + 130, pillY, 130, false);
    CoreStage5Pill("Sorted by rarity", headerX + 270, pillY, 140, false);
    CoreStage5Pill("Functional grid", headerX + 420, pillY, 135, false);

    static float scrollY = 0.0f;
    float panelX = x + pad;
    float panelY = headerY + 144.0f;
    float panelW = w - pad * 2;
    float panelH = h - (panelY - y) - 22.0f;

    if (filteredIndices.empty()) {
        CoreStage5EmptyState("No weapon skins found", "Try a different search term or clear the search box.", panelX, panelY, panelW, 150);
        return;
    }

    CoreStage5Panel(panelX, panelY, panelW, panelH, 16.0f);
    SC_GUI::DrawStringA("Weapon Skin Library", panelX + 20, panelY + 16, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Click any card to apply it to the selected weapon.", panelX + 20, panelY + 44, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    float viewX = panelX + 12.0f;
    float viewY = panelY + 76.0f;
    float viewW = panelW - 24.0f;
    float viewH = panelH - 88.0f;

    float itemW = 162.0f;
    float itemH = 188.0f;
    float gap = 18.0f;
    int cols = (int)((viewW - 8.0f) / (itemW + gap));
    if (cols < 1) cols = 1;

    int rows = ((int)filteredIndices.size() + cols - 1) / cols;
    float contentHeight = rows * (itemH + gap);

    if (SC_GUI::Input.mousePos.x >= viewX && SC_GUI::Input.mousePos.x <= viewX + viewW &&
        SC_GUI::Input.mousePos.y >= viewY && SC_GUI::Input.mousePos.y <= viewY + viewH) {
        scrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }

    float minScroll = -(contentHeight - viewH + gap);
    if (minScroll > 0) minScroll = 0;
    if (scrollY > 0) scrollY = 0;
    if (scrollY < minScroll) scrollY = minScroll;

    int selectedLocalSkinIndex = 0;
    SkinInfo_t currentSkin = skinManager->GetSkin(CurrentWeaponDef);
    if (currentSkin.weaponType != WeaponsEnum::none) {
        for (int i = 0; i < (int)availableSkins.size(); i++) {
            if (availableSkins[i].Paint == currentSkin.Paint) {
                selectedLocalSkinIndex = i;
                break;
            }
        }
    }

    SC_GUI::SetClip(viewX, viewY, viewW, viewH);
    int displayIdx = 0;
    for (int idx : filteredIndices) {
        float cX = viewX + 4 + (displayIdx % cols) * (itemW + gap);
        float cY = viewY + scrollY + (displayIdx / cols) * (itemH + gap);

        if (cY + itemH < viewY || cY > viewY + viewH) {
            displayIdx++;
            continue;
        }

        bool selected = (selectedLocalSkinIndex == idx);
        if (SC_GUI::SkinCard("wskin_" + availableSkins[idx].name, availableSkins[idx].name, availableSkins[idx].image_url, availableSkins[idx].rarity, cX, cY, itemW, itemH, selected, configManager->diskCacheEnabled)) {
            skinManager->AddSkin(availableSkins[idx]);
        }
        displayIdx++;
    }
    SC_GUI::ResetClip();

    if (contentHeight > viewH) {
        float ratio = viewH / contentHeight;
        float barH = viewH * ratio;
        float barY = viewY + (-scrollY / contentHeight) * viewH;
        SC_GUI::DrawFilledRoundedRect(viewX + viewW - 6, barY, 4, barH, 2.0f, SC_GUI::currentTheme.accent);
    }
}
void RenderKnifeTab(float x, float y, float w, float h)
{
    float pad = 24.0f;
    float headerX = x + pad;
    float headerY = y + 18.0f;
    float headerW = w - pad * 2;

    CoreStage5Header("Knives", "Choose a knife model, then select a finish.", headerX, headerY, headerW);

    if (Knifes.empty()) {
        CoreStage5EmptyState("No knife models found", "Knife data did not load yet.", headerX, headerY + 96, headerW, 150);
        return;
    }

    bool isDefaultKnife = true;
    int autoDetectedIndex = -1;
    auto it = KnifeNames.find(static_cast<uint16_t>(CurrentWeaponDef));
    if (it != KnifeNames.end()) {
        std::string knifeName = it->second;
        for (int i = 0; i < (int)Knifes.size(); i++) {
            if (Knifes[i].name.find(knifeName) != std::string::npos) {
                autoDetectedIndex = i;
                break;
            }
        }
    }

    if (autoDetectedIndex != -1) {
        if (selectedKnifeIndex != autoDetectedIndex) {
            selectedKnifeIndex = autoDetectedIndex;
            selectedKnifeSkinIndex = 0;
        }
        isDefaultKnife = false;
    }

    if (selectedKnifeIndex < 0) selectedKnifeIndex = 0;
    if (selectedKnifeIndex >= (int)Knifes.size()) selectedKnifeIndex = 0;

    std::string filterName = Knifes[selectedKnifeIndex].name;
    const std::vector<SkinInfo_t>& allKnifeSkins = skindb->GetKnifeSkins();
    std::vector<SkinInfo_t> filteredSkins;
    filteredSkins.push_back(SkinInfo_t{ 0, false, "Vanilla", WeaponsEnum::none, 1 });

    for (const auto& skin : allKnifeSkins) {
        if (skin.name.find(filterName) == std::string::npos) continue;
        filteredSkins.push_back(skin);
    }

    std::sort(filteredSkins.begin(), filteredSkins.end(), [](const SkinInfo_t& a, const SkinInfo_t& b) {
        return a.rarity > b.rarity;
    });

    float contentY = headerY + 96.0f;
    float leftW = 210.0f;
    float leftX = headerX;
    float rightX = leftX + leftW + 18.0f;
    float rightW = headerW - leftW - 18.0f;
    float panelH = h - (contentY - y) - 22.0f;

    CoreStage5Panel(leftX, contentY, leftW, panelH, 14.0f);
    SC_GUI::DrawStringA("Knife Model", leftX + 18, contentY + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(isDefaultKnife ? "Manual selection" : "Auto-detected", leftX + 18, contentY + 45, isDefaultKnife ? SC_GUI::currentTheme.textDim : SC_GUI::currentTheme.accent, SC_GUI::smallFont, false);

    static float modelScrollY = 0.0f;
    float modelListY = contentY + 78.0f;
    float modelListH = panelH - 94.0f;
    float modelContentH = (float)Knifes.size() * 42.0f;

    if (SC_GUI::Input.mousePos.x >= leftX && SC_GUI::Input.mousePos.x <= leftX + leftW &&
        SC_GUI::Input.mousePos.y >= modelListY && SC_GUI::Input.mousePos.y <= modelListY + modelListH) {
        modelScrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }
    if (modelScrollY > 0) modelScrollY = 0;
    float modelMin = -(modelContentH - modelListH);
    if (modelMin > 0) modelMin = 0;
    if (modelScrollY < modelMin) modelScrollY = modelMin;

    SC_GUI::SetClip(leftX, modelListY, leftW, modelListH);
    for (int k = 0; k < (int)Knifes.size(); k++) {
        float itemY = modelListY + modelScrollY + k * 42.0f;
        if (itemY + 36 < modelListY || itemY > modelListY + modelListH) continue;
        if (SC_GUI::TabButton("k_model_" + std::to_string(k), Knifes[k].name, leftX + 12, itemY, leftW - 24, 36, selectedKnifeIndex == k)) {
            selectedKnifeIndex = k;
            selectedKnifeSkinIndex = 0;
        }
    }
    SC_GUI::ResetClip();

    CoreStage5Panel(rightX, contentY, rightW, panelH, 14.0f);
    SC_GUI::DrawStringA(filterName, rightX + 20, contentY + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Finishes: " + std::to_string((int)filteredSkins.size()), rightX + 20, contentY + 45, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    static float kSkinScrollY = 0.0f;
    float viewX = rightX + 18.0f;
    float viewY = contentY + 78.0f;
    float viewW = rightW - 36.0f;
    float viewH = panelH - 96.0f;
    float itemW = 154.0f;
    float itemH = 180.0f;
    float gap = 16.0f;
    int cols = (int)((viewW - 4.0f) / (itemW + gap));
    if (cols < 1) cols = 1;

    int rows = ((int)filteredSkins.size() + cols - 1) / cols;
    float contentH = rows * (itemH + gap);

    if (SC_GUI::Input.mousePos.x >= viewX && SC_GUI::Input.mousePos.x <= viewX + viewW &&
        SC_GUI::Input.mousePos.y >= viewY && SC_GUI::Input.mousePos.y <= viewY + viewH) {
        kSkinScrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }
    if (kSkinScrollY > 0) kSkinScrollY = 0;
    float skinMin = -(contentH - viewH + gap);
    if (skinMin > 0) skinMin = 0;
    if (kSkinScrollY < skinMin) kSkinScrollY = skinMin;

    SC_GUI::SetClip(viewX, viewY, viewW, viewH);
    for (int i = 0; i < (int)filteredSkins.size(); i++) {
        float cX = viewX + (i % cols) * (itemW + gap);
        float cY = viewY + kSkinScrollY + (i / cols) * (itemH + gap);
        if (cY + itemH < viewY || cY > viewY + viewH) continue;

        bool selected = (selectedKnifeSkinIndex == i);
        if (SC_GUI::SkinCard("kskin_" + std::to_string(i) + filteredSkins[i].name, filteredSkins[i].name, filteredSkins[i].image_url, filteredSkins[i].rarity, cX, cY, itemW, itemH, selected, configManager->diskCacheEnabled)) {
            selectedKnifeSkinIndex = i;
            if (i != 0) {
                SkinInfo_t s = filteredSkins[i];
                // Track which knife model to load
                skinManager->Knife = Knifes[selectedKnifeIndex];
                // Store under BOTH default knife defindexes so GetSkin() matches
                // the weapon the player actually holds (59=Tknife, 42=CtKnife)
                s.weaponType = WeaponsEnum::Tknife;
                skinManager->AddSkin(s);
                s.weaponType = WeaponsEnum::CtKnife;
                skinManager->AddSkin(s);
                // ALSO store under the target knife defIndex so it matches AFTER we change it
                s.weaponType = static_cast<WeaponsEnum>(Knifes[selectedKnifeIndex].defIndex);
                skinManager->AddSkin(s);
            } else {
                // Vanilla — clear knife skin and model
                skinManager->Knife = Knife_t();
                SkinInfo_t vanilla;
                vanilla.Paint = 0;
                vanilla.weaponType = WeaponsEnum::Tknife;
                skinManager->AddSkin(vanilla);
                vanilla.weaponType = WeaponsEnum::CtKnife;
                skinManager->AddSkin(vanilla);
                vanilla.weaponType = static_cast<WeaponsEnum>(Knifes[selectedKnifeIndex].defIndex);
                skinManager->AddSkin(vanilla);
            }
        }
    }
    SC_GUI::ResetClip();
}
void RenderMusicTab(float x, float y, float w, float h)
{
    float pad = 24.0f;
    float headerX = x + pad;
    float headerY = y + 18.0f;
    float headerW = w - pad * 2;

    std::string query = searchBuffer;
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);

    std::vector<int> filteredIndices;
    for (int i = 0; i < (int)musicKits.size(); i++) {
        if (!query.empty()) {
            std::string name = musicKits[i].name;
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (name.find(query) == std::string::npos) continue;
        }
        filteredIndices.push_back(i);
    }

    CoreStage5Header("Music Kits", "Browse and select your active music kit.", headerX, headerY, headerW);
    SC_GUI::TextInput("search_music", searchBuffer, 128, headerX + headerW - 330, headerY + 20, 300, 36, "Search music kits...");

    float pillY = headerY + 92.0f;
    CoreStage5Pill("Kits: " + std::to_string((int)musicKits.size()), headerX, pillY, 105, true);
    CoreStage5Pill("Filtered: " + std::to_string((int)filteredIndices.size()), headerX + 115, pillY, 125, false);
    CoreStage5Pill("Selected: " + std::to_string(selectedMusicKitIndex), headerX + 250, pillY, 120, false);

    static float mScrollY = 0.0f;
    float viewX = x + pad;
    float viewY = headerY + 132.0f;
    float viewW = w - pad * 2;
    float viewH = h - (viewY - y) - 20.0f;
    float itemW = 158.0f;
    float itemH = 132.0f;
    float gap = 16.0f;
    int cols = (int)((viewW - 8.0f) / (itemW + gap));
    if (cols < 1) cols = 1;

    int rows = ((int)filteredIndices.size() + cols - 1) / cols;
    float contentH = rows * (itemH + gap);

    if (SC_GUI::Input.mousePos.x >= viewX && SC_GUI::Input.mousePos.x <= viewX + viewW &&
        SC_GUI::Input.mousePos.y >= viewY && SC_GUI::Input.mousePos.y <= viewY + viewH) {
        mScrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }
    if (mScrollY > 0) mScrollY = 0;
    float minScroll = -(contentH - viewH + gap);
    if (minScroll > 0) minScroll = 0;
    if (mScrollY < minScroll) mScrollY = minScroll;

    if (filteredIndices.empty()) {
        CoreStage5EmptyState("No music kits found", "Try a different search term or clear the search box.", viewX, viewY, viewW, 150);
        return;
    }

    SC_GUI::SetClip(viewX, viewY, viewW, viewH);
    int displayIdx = 0;
    for (int idx : filteredIndices) {
        float cX = viewX + 4 + (displayIdx % cols) * (itemW + gap);
        float cY = viewY + mScrollY + (displayIdx / cols) * (itemH + gap);
        if (cY + itemH < viewY || cY > viewY + viewH) {
            displayIdx++;
            continue;
        }

        bool selected = (selectedMusicKitIndex == idx);
        if (SC_GUI::SkinCard("music_" + std::to_string(idx), musicKits[idx].name, musicKits[idx].image_url, 3, cX, cY, itemW, itemH, selected, configManager->diskCacheEnabled)) {
            selectedMusicKitIndex = idx;
            skinManager->MusicKit = musicKits[idx];
            ForceUpdate = true;
        }
        displayIdx++;
    }
    SC_GUI::ResetClip();
}
void RenderConfigTab(float x, float y, float w, float h)
{
    float pad = 24.0f;
    float headerX = x + pad;
    float headerY = y + 18.0f;
    float headerW = w - pad * 2;

    CoreStage5Header("Configs", "Save, load, and manage your local profiles.", headerX, headerY, headerW);

    static char cfgNameBuf[64] = "";
    float createY = headerY + 96.0f;
    CoreStage5Panel(headerX, createY, headerW, 78, 14.0f);
    SC_GUI::TextInput("cfg_name", cfgNameBuf, 64, headerX + 18, createY + 21, 260, 36, "New config name");

    if (SC_GUI::Button("cfg_create_save", "Create / Save", headerX + 292, createY + 21, 136, 36)) {
        std::string name = cfgNameBuf;
        if (!name.empty()) {
            configManager->Save(name);
            memset(cfgNameBuf, 0, 64);
        }
    }

    CoreStage5Pill("Stored locally", headerX + headerW - 270, createY + 25, 120, true);
    CoreStage5Pill("APPDATA path", headerX + headerW - 140, createY + 25, 115, false);

    std::vector<std::string> configs = configManager->GetConfigs();

    float listY = createY + 98.0f;
    float listH = h - (listY - y) - 22.0f;
    CoreStage5Panel(headerX, listY, headerW, listH, 14.0f);
    SC_GUI::DrawStringA("Saved Configs", headerX + 20, listY + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(std::to_string((int)configs.size()) + " profiles found", headerX + 20, listY + 45, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    if (configs.empty()) {
        SC_GUI::DrawStringA("No configs yet. Type a name above and press Create / Save.", headerX + 20, listY + 88, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);
        return;
    }

    static float scrollY = 0.0f;
    float rowY = listY + 78.0f;
    float rowH = listH - 94.0f;
    float contentH = configs.size() * 56.0f;

    if (SC_GUI::Input.mousePos.x >= headerX && SC_GUI::Input.mousePos.x <= headerX + headerW &&
        SC_GUI::Input.mousePos.y >= rowY && SC_GUI::Input.mousePos.y <= rowY + rowH) {
        scrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }
    if (scrollY > 0) scrollY = 0;
    float minScroll = -(contentH - rowH);
    if (minScroll > 0) minScroll = 0;
    if (scrollY < minScroll) scrollY = minScroll;

    SC_GUI::SetClip(headerX, rowY, headerW, rowH);
    for (int i = 0; i < (int)configs.size(); i++) {
        float itemY = rowY + scrollY + i * 56.0f;
        if (itemY + 46 < rowY || itemY > rowY + rowH) continue;

        SC_GUI::DrawFilledRoundedRect(headerX + 18, itemY, headerW - 36, 46, 10.0f, Color(255, 12, 22, 16));
        SC_GUI::DrawStrokeRoundedRect(headerX + 18, itemY, headerW - 36, 46, 10.0f, SC_GUI::currentTheme.border, 1.0f);
        SC_GUI::DrawStringA(configs[i], headerX + 36, itemY + 13, SC_GUI::currentTheme.text, SC_GUI::mainFont, false);

        if (SC_GUI::Button("cfg_load_" + std::to_string(i), "Load", headerX + headerW - 210, itemY + 6, 82, 34)) {
            configManager->Load(configs[i]);
        }

        if (SC_GUI::Button("cfg_save_" + std::to_string(i), "Save", headerX + headerW - 118, itemY + 6, 82, 34)) {
            configManager->Save(configs[i]);
        }
    }
    SC_GUI::ResetClip();
}
void RenderSettingsTab(float x, float y, float w, float h)
{
    float pad = 24.0f;
    float headerX = x + pad;
    float headerY = y + 18.0f;
    float headerW = w - pad * 2;

    CoreStage5Header("Settings", "Adjust visuals, caching, transparency, and configs.", headerX, headerY, headerW);

    float contentY = headerY + 96.0f;
    float leftW = (headerW - 18.0f) * 0.45f;
    float rightW = headerW - leftW - 18.0f;
    float panelH = h - (contentY - y) - 22.0f;

    CoreStage5Panel(headerX, contentY, leftW, panelH, 14.0f);
    SC_GUI::DrawStringA("Interface", headerX + 20, contentY + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Core green theme controls", headerX + 20, contentY + 45, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    float curY = contentY + 86.0f;
    SC_GUI::DrawStringA("Menu Transparency", headerX + 20, curY, SC_GUI::currentTheme.text, SC_GUI::mainFont, false);
    curY += 28.0f;

    static float alphaVal = 255.0f;
    if (SC_GUI::Slider("alpha_slider", &alphaVal, 50.0f, 255.0f, headerX + 20, curY, leftW - 40, 24)) {
        overlay::GlobalAlpha = (BYTE)alphaVal;
    }

    curY += 54.0f;
    if (SC_GUI::Checkbox("disk_cache_check", "Enable image disk cache", &configManager->diskCacheEnabled, headerX + 20, curY)) {
        if (!configManager->diskCacheEnabled) {
            SC_GUI::TextureCache.ClearDisk();
        }
    }

    curY += 60.0f;
    SC_GUI::DrawStringA("Theme Presets", headerX + 20, curY, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Keep Core Green for the premium look.", headerX + 20, curY + 26, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
    curY += 64.0f;

    if (SC_GUI::Button("theme_core_green", "Core Green", headerX + 20, curY, leftW - 40, 36, true)) {
        SC_GUI::currentTheme = {
            Color(255, 8, 12, 8),
            Color(255, 13, 18, 14),
            Color(255, 18, 24, 20),
            Color(255, 34, 50, 38),
            Color(255, 27, 40, 31),
            Color(255, 92, 255, 122),
            Color(255, 230, 234, 231),
            Color(255, 160, 166, 163)
        };
    }

    curY += 46.0f;
    if (SC_GUI::Button("theme_midnight", "Midnight Blue", headerX + 20, curY, leftW - 40, 36)) {
        SC_GUI::currentTheme = {
            Color(255, 15, 20, 30),
            Color(255, 20, 25, 40),
            Color(255, 40, 50, 70),
            Color(255, 30, 40, 60),
            Color(255, 30, 45, 65),
            Color(255, 0, 120, 215),
            Color(255, 255, 255, 255),
            Color(255, 170, 180, 200)
        };
    }

    CoreStage5Panel(headerX + leftW + 18.0f, contentY, rightW, panelH, 14.0f);
    RenderConfigTab(headerX + leftW + 18.0f, contentY - 18.0f, rightW, panelH + 18.0f);
}
// --- Glove Tab ---
void RenderGloveTab(float x, float y, float w, float h)
{
    float pad = 24.0f;
    float headerX = x + pad;
    float headerY = y + 18.0f;
    float headerW = w - pad * 2;

    CoreStage5Header("Gloves", "Select glove model and finish combinations.", headerX, headerY, headerW);

    if (GloveTypes.empty()) {
        CoreStage5EmptyState("No glove models found", "Glove data did not load yet.", headerX, headerY + 96, headerW, 150);
        return;
    }

    float contentY = headerY + 96.0f;
    float typeW = 210.0f;
    float typeX = headerX;
    float skinX = typeX + typeW + 18.0f;
    float skinW = headerW - typeW - 18.0f;
    float panelH = h - (contentY - y) - 22.0f;

    CoreStage5Panel(typeX, contentY, typeW, panelH, 14.0f);
    SC_GUI::DrawStringA("Glove Model", typeX + 18, contentY + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Choose base type", typeX + 18, contentY + 45, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    static int selectedGloveIdx = 0;
    if (selectedGloveIdx < 0) selectedGloveIdx = 0;
    if (selectedGloveIdx >= (int)GloveTypes.size()) selectedGloveIdx = 0;

    static float typeScrollY = 0.0f;
    float typeListY = contentY + 78.0f;
    float typeListH = panelH - 94.0f;
    float typeContentH = GloveTypes.size() * 44.0f;

    if (SC_GUI::Input.mousePos.x >= typeX && SC_GUI::Input.mousePos.x <= typeX + typeW &&
        SC_GUI::Input.mousePos.y >= typeListY && SC_GUI::Input.mousePos.y <= typeListY + typeListH) {
        typeScrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }
    if (typeScrollY > 0) typeScrollY = 0;
    float typeMin = -(typeContentH - typeListH);
    if (typeMin > 0) typeMin = 0;
    if (typeScrollY < typeMin) typeScrollY = typeMin;

    SC_GUI::SetClip(typeX, typeListY, typeW, typeListH);
    for (int i = 0; i < (int)GloveTypes.size(); i++) {
        float itemY = typeListY + typeScrollY + i * 44.0f;
        if (itemY + 38 < typeListY || itemY > typeListY + typeListH) continue;
        bool active = (selectedGloveIdx == i);
        if (SC_GUI::TabButton("glove_type_" + std::to_string(i), GloveTypes[i].name, typeX + 12, itemY, typeW - 24, 38, active)) {
            selectedGloveIdx = i;
        }
    }
    SC_GUI::ResetClip();

    std::string currentGloveName = GloveTypes[selectedGloveIdx].name;
    std::vector<SkinInfo_t> gloves = skindb->GetGloveSkins(currentGloveName);

    CoreStage5Panel(skinX, contentY, skinW, panelH, 14.0f);
    SC_GUI::DrawStringA(currentGloveName, skinX + 20, contentY + 18, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Finishes: " + std::to_string((int)gloves.size()), skinX + 20, contentY + 45, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    if (gloves.empty()) {
        SC_GUI::DrawStringA("No glove finishes found for this model.", skinX + 20, contentY + 92, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);
        return;
    }

    static float scrollY = 0.0f;
    float viewX = skinX + 18.0f;
    float viewY = contentY + 78.0f;
    float viewW = skinW - 36.0f;
    float viewH = panelH - 96.0f;
    float cardW = 170.0f;
    float cardH = 198.0f;
    float gap = 16.0f;
    int cols = (int)((viewW - 4.0f) / (cardW + gap));
    if (cols < 1) cols = 1;

    float totalH = ((gloves.size() + cols - 1) / cols) * (cardH + gap);

    if (SC_GUI::Input.mousePos.x >= viewX && SC_GUI::Input.mousePos.x <= viewX + viewW &&
        SC_GUI::Input.mousePos.y >= viewY && SC_GUI::Input.mousePos.y <= viewY + viewH) {
        scrollY += SC_GUI::Input.scrollDelta * 0.5f;
    }
    if (scrollY > 0) scrollY = 0;
    float minScroll = -(totalH - viewH + gap);
    if (minScroll > 0) minScroll = 0;
    if (scrollY < minScroll) scrollY = minScroll;

    int currentPaint = skinManager->Gloves.Paint;
    uint16_t currentDef = skinManager->Gloves.defIndex;

    SC_GUI::SetClip(viewX, viewY, viewW, viewH);
    for (int i = 0; i < (int)gloves.size(); i++) {
        float drawX = viewX + (i % cols) * (cardW + gap);
        float drawY = viewY + scrollY + (i / cols) * (cardH + gap);
        if (drawY + cardH < viewY || drawY > viewY + viewH) continue;

        bool isSelected = (currentPaint == gloves[i].Paint && currentDef == GloveTypes[selectedGloveIdx].defIndex);
        if (SC_GUI::SkinCard("glove_" + std::to_string(i), gloves[i].name, gloves[i].image_url, gloves[i].rarity, drawX, drawY, cardW, cardH, isSelected, configManager->diskCacheEnabled)) {
            skinManager->Gloves.defIndex = GloveTypes[selectedGloveIdx].defIndex;
            skinManager->Gloves.Paint = gloves[i].Paint;
            ForceUpdate = true;
        }
    }
    SC_GUI::ResetClip();
}
static int active_tab = 0;
bool MenuOpen = true;

void RenderCoreMetricCard(const std::string& title, const std::string& value, const std::string& sub, float x, float y, float w, float h)
{
    SC_GUI::DrawFilledRoundedRect(x, y, w, h, 12.0f, Color(255, 12, 21, 15));
    SC_GUI::DrawStrokeRoundedRect(x, y, w, h, 12.0f, SC_GUI::currentTheme.border, 1.0f);
    SC_GUI::DrawFilledRoundedRect(x + 14, y + 14, 34, 34, 8.0f, Color(255, 18, 55, 29));
    SC_GUI::DrawStrokeRoundedRect(x + 14, y + 14, 34, 34, 8.0f, SC_GUI::currentTheme.accent, 1.0f);
    SC_GUI::DrawStringA("+", x + 26, y + 18, SC_GUI::currentTheme.accent, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(title, x + 60, y + 15, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
    SC_GUI::DrawStringA(value, x + 60, y + 40, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(sub, x + 18, y + h - 28, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
}

void RenderCoreSectionLabel(const std::string& title, const std::string& sub, float x, float y)
{
    SC_GUI::DrawStringA(title, x, y, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(sub, x, y + 24, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
}

void CoreStage7GlowPanel(float x, float y, float w, float h, float radius, Color fill, bool strong = false)
{
    SC_GUI::DrawFilledRoundedRect(x - 7, y - 7, w + 14, h + 14, radius + 4.0f, Color(strong ? 38 : 20, 82, 255, 130));
    SC_GUI::DrawFilledRoundedRect(x - 3, y - 3, w + 6, h + 6, radius + 2.0f, Color(strong ? 30 : 16, 82, 255, 130));
    SC_GUI::DrawFilledRoundedRect(x, y, w, h, radius, fill);
    SC_GUI::DrawStrokeRoundedRect(x, y, w, h, radius, SC_GUI::currentTheme.border, 1.0f);
}

void CoreStage7TinyPill(const std::string& text, float x, float y, float w, bool active = false)
{
    Color bg = active ? Color(255, 17, 58, 30) : Color(255, 8, 18, 12);
    Color border = active ? SC_GUI::currentTheme.accent : SC_GUI::currentTheme.border;
    Color txt = active ? SC_GUI::currentTheme.accent : SC_GUI::currentTheme.textDim;
    SC_GUI::DrawFilledRoundedRect(x, y, w, 28, 9.0f, bg);
    SC_GUI::DrawStrokeRoundedRect(x, y, w, 28, 9.0f, border, active ? 1.3f : 1.0f);
    SC_GUI::DrawStringA(text, x + 13, y + 7, txt, SC_GUI::smallFont, false);
}

void CoreStage7BigMetric(const std::string& label, const std::string& value, const std::string& sub, float x, float y, float w, float h)
{
    CoreStage7GlowPanel(x, y, w, h, 15.0f, Color(255, 8, 18, 12), false);
    SC_GUI::DrawFilledRoundedRect(x + 18, y + 18, 46, 46, 13.0f, Color(255, 14, 52, 28));
    SC_GUI::DrawStrokeRoundedRect(x + 18, y + 18, 46, 46, 13.0f, SC_GUI::currentTheme.accent, 1.2f);
    SC_GUI::DrawStringA("+", x + 35, y + 25, SC_GUI::currentTheme.accent, SC_GUI::titleFont, false);
    SC_GUI::DrawStringA(label, x + 80, y + 18, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
    SC_GUI::DrawStringA(value, x + 80, y + 43, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(sub, x + 22, y + h - 30, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
}

void CoreStage7DrawGrid(float x, float y, float w, float h)
{
    for (int i = 0; i < 18; ++i) {
        float gx = x + 22.0f + i * 52.0f;
        SC_GUI::DrawRect(gx, y + 12, 1, h - 24, Color(16, 82, 255, 130));
    }
    for (int i = 0; i < 12; ++i) {
        float gy = y + 22.0f + i * 48.0f;
        SC_GUI::DrawRect(x + 12, gy, w - 24, 1, Color(12, 82, 255, 130));
    }
}

void RenderCoreStage8Chip(const std::string& text, float x, float y, float w, bool highlighted = false)
{
    Color bg = highlighted ? Color(255, 18, 62, 30) : Color(255, 10, 18, 14);
    Color br = highlighted ? SC_GUI::currentTheme.accent : SC_GUI::currentTheme.border;

    SC_GUI::DrawFilledRoundedRect(x, y, w, 30, 8.0f, bg);
    SC_GUI::DrawStrokeRoundedRect(x, y, w, 30, 8.0f, br, 1.0f);
    SC_GUI::DrawStringA(text, x + 14, y + 8, highlighted ? SC_GUI::currentTheme.accent : SC_GUI::currentTheme.text, SC_GUI::smallFont, false);
}
void RenderDashboardTab(float x, float y, float w, float h)
{
    float curY = y + 10.0f;

    // Main premium hero only. The outer shell already has the topbar.
    SC_GUI::DrawFilledRoundedRect(x + 8, curY, w - 16, 190, 20.0f, Color(255, 7, 15, 11));
    SC_GUI::DrawStrokeRoundedRect(x + 8, curY, w - 16, 190, 20.0f, SC_GUI::currentTheme.border, 1.0f);

    // Soft grid lines
    for (int i = 0; i < 9; ++i) {
        float gx = x + 42 + i * 100.0f;
        SC_GUI::DrawFilledRoundedRect(gx, curY + 18, 1.0f, 154.0f, 0.0f, Color(22, 92, 255, 122));
    }
    for (int j = 0; j < 4; ++j) {
        float gy = curY + 30 + j * 40.0f;
        SC_GUI::DrawFilledRoundedRect(x + 28, gy, w - 56.0f, 1.0f, 0.0f, Color(18, 92, 255, 122));
    }

    SC_GUI::DrawFilledRoundedRect(x + 46, curY + 38, 74, 74, 16.0f, Color(255, 10, 55, 24));
    SC_GUI::DrawStrokeRoundedRect(x + 46, curY + 38, 74, 74, 16.0f, SC_GUI::currentTheme.accent, 2.0f);
    SC_GUI::DrawStringA("C", x + 73, curY + 56, SC_GUI::currentTheme.accent, SC_GUI::titleFont, false);

    SC_GUI::DrawStringA("Core Skin Changer", x + 150, curY + 38, SC_GUI::currentTheme.text, SC_GUI::titleFont, false);
    SC_GUI::DrawStringA("Premium black/green loadout control center", x + 150, curY + 82, SC_GUI::currentTheme.accent, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("A stronger Core Keys inspired visual layer with glow panels, cleaner cards, and premium spacing.", x + 150, curY + 122, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);

    RenderCoreStage8Chip("CORE UI v3", x + w - 365, curY + 42, 110, true);
    RenderCoreStage8Chip("Release x64", x + w - 242, curY + 42, 120, false);
    RenderCoreStage8Chip("Ready", x + w - 108, curY + 42, 80, false);

    curY += 220.0f;

    // Metrics row
    float rowW = (w - 56.0f) / 3.0f;
    RenderCoreMetricCard("INVENTORY", std::to_string((int)skinManager->Skins.size()) + " items", "Loaded into current session", x + 8, curY, rowW, 132);
    RenderCoreMetricCard("PROFILE", "Main", "Default workspace active", x + 28 + rowW, curY, rowW, 132);
    RenderCoreMetricCard("THEME", "Neon Core", "High contrast premium shell", x + 48 + rowW * 2, curY, rowW, 132);

    curY += 168.0f;

    // Quick navigation
    SC_GUI::DrawStringA("Fast Navigation", x + 14, curY, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Jump between the main customization pages.", x + 14, curY + 30, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);

    curY += 62.0f;

    float navW = (w - 60.0f) / 4.0f;
    if (SC_GUI::Button("st8_wep", "Weapons", x + 10, curY, navW, 48)) active_tab = 1;
    if (SC_GUI::Button("st8_knife", "Knives", x + 22 + navW, curY, navW, 48)) active_tab = 2;
    if (SC_GUI::Button("st8_gloves", "Gloves", x + 34 + navW * 2, curY, navW, 48)) active_tab = 3;
    if (SC_GUI::Button("st8_settings", "Settings", x + 46 + navW * 3, curY, navW, 48)) active_tab = 5;

    curY += 86.0f;

    float lowerW = (w - 34.0f) / 2.0f - 8.0f;

    SC_GUI::DrawFilledRoundedRect(x + 8, curY, lowerW, 138, 16.0f, Color(255, 8, 16, 12));
    SC_GUI::DrawStrokeRoundedRect(x + 8, curY, lowerW, 138, 16.0f, SC_GUI::currentTheme.border, 1.0f);
    SC_GUI::DrawStringA("Design Upgrade", x + 34, curY + 30, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Bigger frame, stronger glow, richer contrast, and cleaner navigation.", x + 34, curY + 68, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);
    SC_GUI::DrawStringA("This is now much closer to a premium Core Keys dashboard.", x + 34, curY + 100, SC_GUI::currentTheme.accent, SC_GUI::mainFont, false);

    SC_GUI::DrawFilledRoundedRect(x + 24 + lowerW, curY, lowerW, 138, 16.0f, Color(255, 8, 16, 12));
    SC_GUI::DrawStrokeRoundedRect(x + 24 + lowerW, curY, lowerW, 138, 16.0f, SC_GUI::currentTheme.border, 1.0f);
    SC_GUI::DrawStringA("Core Keys Feel", x + 50 + lowerW, curY + 30, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Matte black panels, neon green accents, and dashboard-style cards.", x + 50 + lowerW, curY + 68, SC_GUI::currentTheme.textDim, SC_GUI::mainFont, false);
    SC_GUI::DrawStringA("Next: polish weapons, knives, gloves, and config screens.", x + 50 + lowerW, curY + 100, SC_GUI::currentTheme.accent, SC_GUI::mainFont, false);
}void RenderMenu()
{
    float w = 1210;
    float h = 790;

    static float x = -1;
    static float y = -1;
    if (x == -1) {
        x = (overlay::G_Width - w) / 2;
        y = (overlay::G_Height - h) / 2;
    }

    float sidebarW = 260.0f;
    float topbarH = 82.0f;

    if (SC_GUI::Input.leftClick) {
        bool overTop = SC_GUI::Input.mousePos.x >= x && SC_GUI::Input.mousePos.x <= x + w &&
                       SC_GUI::Input.mousePos.y >= y && SC_GUI::Input.mousePos.y <= y + topbarH;
        if (overTop) {
            x += SC_GUI::Input.mouseDelta.x;
            y += SC_GUI::Input.mouseDelta.y;
        }
    }

    // Outer glow + main frame
    SC_GUI::DrawFilledRoundedRect(x - 10, y - 10, w + 20, h + 20, 24.0f, Color(24, 82, 255, 130));
    SC_GUI::DrawFilledRoundedRect(x - 5, y - 5, w + 10, h + 10, 21.0f, Color(18, 82, 255, 130));
    SC_GUI::DrawFilledRoundedRect(x, y, w, h, 20.0f, SC_GUI::currentTheme.mainBg);
    SC_GUI::DrawStrokeRoundedRect(x, y, w, h, 20.0f, Color(255, 44, 110, 58), 1.2f);

    // Sidebar shell
    SC_GUI::DrawFilledRoundedRect(x + 14, y + 14, sidebarW - 28, h - 28, 18.0f, SC_GUI::currentTheme.sidebarBg);
    SC_GUI::DrawStrokeRoundedRect(x + 14, y + 14, sidebarW - 28, h - 28, 18.0f, SC_GUI::currentTheme.border, 1.0f);

    // Brand block
    SC_GUI::DrawFilledRoundedRect(x + 30, y + 30, sidebarW - 60, 92, 18.0f, Color(255, 7, 17, 11));
    SC_GUI::DrawStrokeRoundedRect(x + 30, y + 30, sidebarW - 60, 92, 18.0f, Color(255, 35, 80, 47), 1.0f);
    SC_GUI::DrawFilledRoundedRect(x + 46, y + 50, 50, 50, 14.0f, Color(255, 13, 50, 27));
    SC_GUI::DrawStrokeRoundedRect(x + 46, y + 50, 50, 50, 14.0f, SC_GUI::currentTheme.accent, 1.5f);
    SC_GUI::DrawStringA("C", x + 64, y + 59, SC_GUI::currentTheme.accent, SC_GUI::titleFont, false);
    SC_GUI::DrawStringA("Core Skin", x + 112, y + 47, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Changer", x + 112, y + 71, SC_GUI::currentTheme.accent, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Premium Suite", x + 112, y + 96, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    // Sidebar tabs
    float tabY = y + 150;
    float tabH = 50;
    float tabW = sidebarW - 50;
    float tabX = x + 25;

    if (SC_GUI::TabButton("tab_dashboard", "Dashboard", tabX, tabY, tabW, tabH, active_tab == 0)) active_tab = 0;
    if (SC_GUI::TabButton("tab_wep", "Weapons", tabX, tabY + 57, tabW, tabH, active_tab == 1)) active_tab = 1;
    if (SC_GUI::TabButton("tab_knife", "Knives", tabX, tabY + 114, tabW, tabH, active_tab == 2)) active_tab = 2;
    if (SC_GUI::TabButton("tab_gloves", "Gloves", tabX, tabY + 171, tabW, tabH, active_tab == 3)) active_tab = 3;
    if (SC_GUI::TabButton("tab_music", "Music Kits", tabX, tabY + 228, tabW, tabH, active_tab == 4)) active_tab = 4;
    if (SC_GUI::TabButton("tab_settings", "Settings", tabX, tabY + 285, tabW, tabH, active_tab == 5)) active_tab = 5;

    // Sidebar bottom status
    SC_GUI::DrawFilledRoundedRect(x + 30, y + h - 116, sidebarW - 60, 82, 17.0f, Color(255, 7, 17, 11));
    SC_GUI::DrawStrokeRoundedRect(x + 30, y + h - 116, sidebarW - 60, 82, 17.0f, SC_GUI::currentTheme.border, 1.0f);
    SC_GUI::DrawStringA("SYSTEM STATUS", x + 48, y + h - 98, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);
    SC_GUI::DrawStringA("Ready", x + 48, y + h - 71, SC_GUI::currentTheme.accent, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA("Home toggles menu", x + 48, y + h - 47, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    // Page title metadata
    float contentX = x + sidebarW;
    float contentW = w - sidebarW;
    std::string pageTitle = "Dashboard";
    std::string pageSub = "Premium loadout customization suite";

    switch (active_tab) {
        case 1: pageTitle = "Weapons"; pageSub = "Browse and customize weapon finishes"; break;
        case 2: pageTitle = "Knives"; pageSub = "Select knife models and finishes"; break;
        case 3: pageTitle = "Gloves"; pageSub = "Manage glove combinations"; break;
        case 4: pageTitle = "Music Kits"; pageSub = "Choose music kit presets"; break;
        case 5: pageTitle = "Settings"; pageSub = "Configure visuals, cache, and configs"; break;
    }

    // Topbar
    SC_GUI::DrawFilledRoundedRect(contentX + 22, y + 18, contentW - 44, topbarH - 28, 17.0f, Color(255, 7, 16, 11));
    SC_GUI::DrawStrokeRoundedRect(contentX + 22, y + 18, contentW - 44, topbarH - 28, 17.0f, SC_GUI::currentTheme.border, 1.0f);
    SC_GUI::DrawStringA(pageTitle, contentX + 50, y + 27, SC_GUI::currentTheme.text, SC_GUI::largeFont, false);
    SC_GUI::DrawStringA(pageSub, contentX + 50, y + 52, SC_GUI::currentTheme.textDim, SC_GUI::smallFont, false);

    CoreStage7TinyPill("Core UI v2", x + w - 330, y + 31, 104, true);
    CoreStage7TinyPill("x64 Release", x + w - 214, y + 31, 116, false);
    if (SC_GUI::Button("core_close_menu", "X", x + w - 70, y + 28, 36, 36)) {
        MenuOpen = false;
        overlay::SetInput(false);
    }

    // Content area
    float cX = contentX;
    float cY = y + topbarH;
    float cW = contentW;
    float cH = h - topbarH;

    SC_GUI::SetClip(cX, cY, cW, cH);
    switch (active_tab) {
        case 0: RenderDashboardTab(cX, cY, cW, cH); break;
        case 1: RenderWeaponTab(cX, cY, cW, cH); break;
        case 2: RenderKnifeTab(cX, cY, cW, cH); break;
        case 3: RenderGloveTab(cX, cY, cW, cH); break;
        case 4: RenderMusicTab(cX, cY, cW, cH); break;
        case 5: RenderSettingsTab(cX, cY, cW, cH); break;
    }
    SC_GUI::ResetClip();
}void OnFrame()
{
     // Toggle Logic
    static bool prevHome = false;
    bool home = (GetAsyncKeyState(VK_HOME) & 0x8000) != 0;
    if (home && !prevHome) {
        MenuOpen = !MenuOpen;
        overlay::SetInput(MenuOpen);
    }
    prevHome = home;

    // Pass our RenderMenu function to the overlay loop
    overlay::Render(RenderMenu, MenuOpen); 
}

void MenuThread()
{
	while (!overlay::ShouldQuit)
	{
		OnFrame(); 
	}
}

void UpdateActiveMenuDef(const uintptr_t localPlayer)
{
    // m_pClippingWeapon removed in build 14165 — read active weapon via weapon services instead
    const uintptr_t weaponServices = mem.Read<uintptr_t>(localPlayer + Offsets::m_pWeaponServices);
    if (!weaponServices) return;
    const uintptr_t activeWeapon = GetEntityByHandle(mem.Read<uint32_t>(weaponServices + Offsets::m_hActiveWeapon));
    if (!activeWeapon) return;
    const uintptr_t activeItem = activeWeapon + Offsets::m_AttributeManager + Offsets::m_Item;
    CurrentWeaponDef = mem.Read<WeaponsEnum>(activeItem + Offsets::m_iItemDefinitionIndex);
}

void InitMenu(const bool autoThread = false)
{
	overlay::Setup();
    if (autoThread) {
        std::thread menuthread(MenuThread);
        menuthread.detach();
    }
	std::cout << "GDI+ Menu Setup\n";
}