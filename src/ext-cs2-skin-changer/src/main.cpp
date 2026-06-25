#include <openssl/evp.h>
#include <openssl/ssl.h>



#include "menu.h"
#include "SDK/entity/CWeaponServices.h"
#include "SDK/weapon/C_EconEntity.h"
#include "SDK/CEconItemAttributeManager.h"
#include "update_offsets.h"

void Radar()
{
    for (uint8_t i = 1; i < 64; i++)
    {
        const uintptr_t Controller = GetEntityByHandle(i);
        if(!Controller)
			continue;
        const uintptr_t Pawn = GetEntityByHandle(mem.Read<uint16_t>(Controller + 0x6B4));
		if (!Pawn)
            continue;

		const uintptr_t entitySpotted = Pawn + 0x2700;
        mem.Write<bool>(entitySpotted + 0x8, true);
    }
}

class CBaseEntity
{
public:
	char pad0[0x330];//0x000
	uintptr_t sceneNode; //0x330
};

void su()
{
    INPUT input = { 0 };
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_SPACE;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void sd()
{
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_SPACE;
    SendInput(1, &input, sizeof(INPUT));
}

int main()
{
    SetConsoleOutputCP(CP_UTF8); // Fix UTF-8 skin names (☆ etc.)
    SetConsoleCP(CP_UTF8);

    mem.Write<uint16_t>(Sigs::RegenerateWeaponSkins + 0x52, Offsets::m_AttributeManager + Offsets::m_Item + Offsets::m_AttributeList + Offsets::m_Attributes);

    skindb->Dump();
    Updater::UpdateOffsets();
    
    InitMenu();
    
    configManager->Setup();
    configManager->AutoLoad();

    std::cout << "Core Skin Changer Started\n";

    while (true)
    {
        Sleep(5);

        const uintptr_t localController = GetLocalController();
        const uintptr_t localPlayer = GetLocalPlayer();
        const uintptr_t pWeaponServices = mem.Read<uintptr_t>(localPlayer + Offsets::m_pWeaponServices);

        // Skip everything if not in a live match
        if (!localPlayer) {
            static int notInGameLog = 0;
            if (++notInGameLog % 2000 == 0) // ~every 10 seconds
                std::cout << "[Info] Waiting for a live match (localPlayer = 0)...\n";
            UpdateActiveMenuDef(localPlayer);
            OnFrame();
            continue;
        }

        const uintptr_t inventoryServices = mem.Read<uintptr_t>(localController + Offsets::m_pInventoryServices);

        mem.Write<uint16_t>(inventoryServices + Offsets::m_unMusicID, skinManager->MusicKit.id);

        UpdateActiveMenuDef(localPlayer);
        OnFrame();

        bool ShouldUpdate = false;

        const std::vector<uintptr_t> weapons = GetWeapons(localPlayer);
    /*
    //clean up & hud update

    for (const uintptr_t& weapon : weapons)
    {
        const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
        
        if (mem.Read<uint32_t>(weapon + Offsets::m_nFallbackPaintKit) == -1)
            continue;

        mem.Write<uint32_t>(weapon + Offsets::m_nFallbackPaintKit, -1);

        //UpdateHud(weapon);

        econItemAttributeManager.Remove(item);
        continue;
    }
    */
        // --- F1 Live Debug Dump ---
        static bool prevF1 = false;
        bool f1 = (GetAsyncKeyState(VK_F1) & 0x8000) != 0;
        if (f1 && !prevF1) {
            std::cout << "\n========== [DEBUG DUMP] ==========\n";
            std::cout << std::hex << std::uppercase;
            std::cout << "  localController : 0x" << localController << "\n";
            std::cout << "  localPlayer     : 0x" << localPlayer << "\n";
            std::cout << "  pWeaponServices : 0x" << pWeaponServices << "\n";
            std::cout << "  weapons found   : " << std::dec << weapons.size() << "\n";
            for (size_t wi = 0; wi < weapons.size(); wi++) {
                const uintptr_t witem = weapons[wi] + Offsets::m_AttributeManager + Offsets::m_Item;
                uint16_t defIdx = mem.Read<uint16_t>(witem + Offsets::m_iItemDefinitionIndex);
                uint32_t itemIDHigh = mem.Read<uint32_t>(witem + Offsets::m_iItemIDHigh);
                SkinInfo_t found = skinManager->GetSkin(static_cast<WeaponsEnum>(defIdx));
                std::cout << "  [" << wi << "] addr=0x" << std::hex << weapons[wi]
                          << " defIdx=" << std::dec << defIdx
                          << " itemIDHigh=0x" << std::hex << itemIDHigh
                          << " skinMatch=" << (found.Paint ? "YES paint=" + std::to_string(found.Paint) : "NO") << "\n";
            }
            std::cout << "  ForceUpdate     : " << (ForceUpdate ? "true" : "false") << "\n";
            std::cout << "  skinManager has " << std::dec << skinManager->Skins.size() << " stored skins:\n";
            for (auto& s : skinManager->Skins)
                std::cout << "    weaponType=" << (int)s.weaponType << " paint=" << s.Paint << " name=" << s.name << "\n";
            std::cout << "==================================\n\n";
        }
        prevF1 = f1;

        for (const uintptr_t& weapon : weapons)
        {
            const uintptr_t item = weapon + Offsets::m_AttributeManager + Offsets::m_Item;
            
            if(ForceUpdate)
                mem.Write<uint32_t>(item + Offsets::m_iItemIDHigh, NULL);

            if (mem.Read<uint32_t>(item + Offsets::m_iItemIDHigh) == -1)
                continue;

            mem.Write<uint32_t>(item + Offsets::m_iItemIDHigh, -1);

            SkinInfo_t skin = GetSkin(item);
            if (!skin.Paint)
                continue;

            mem.Write<uint32_t>(weapon + Offsets::m_nFallbackPaintKit, skin.Paint);

            // If this is a knife and we have a target knife model selected,
            // write the defindex to change the knife model (e.g. Tknife 59 → Bayonet 500)
            bool isKnife = (mem.Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex) == 42 ||
                            mem.Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex) == 59 ||
                            mem.Read<uint16_t>(item + Offsets::m_iItemDefinitionIndex) >= 500);
            if (isKnife && skinManager->Knife.defIndex != 0) {
                mem.Write<uint16_t>(item + Offsets::m_iItemDefinitionIndex, skinManager->Knife.defIndex);
                mem.Write<uint32_t>(weapon + Offsets::m_nSubclassID, skinManager->Knife.defIndex);
            }

            const uint64_t mask = isKnife ? 2 : (skin.bUsesOldModel + 1);

            const uintptr_t hudWeapon = GetHudWeapon(weapon);
            SetMeshMask(weapon, mask);
            SetMeshMask(hudWeapon, mask);

            econItemAttributeManager.Create(item, skin);

            ShouldUpdate = true;
        }

        // --- Glove Changer Logic ---
        static uint16_t lastGloveDef = 0;
        static int lastGlovePaint = 0;
        
        bool userChangedSettings = (skinManager->Gloves.defIndex != lastGloveDef || 
                                    skinManager->Gloves.Paint != lastGlovePaint);

        if (skinManager->Gloves.defIndex != 0)
        {
             const uintptr_t localPlayerPawn = GetLocalPlayer(); 
             if (localPlayerPawn) {
                 const uintptr_t econGloves = localPlayerPawn + Offsets::m_EconGloves;
                 
                 // Read current (before we overwrite)
                 uint16_t currentDef = mem.Read<uint16_t>(econGloves + Offsets::m_iItemDefinitionIndex);
                 int currentPaint = mem.Read<int>(econGloves + Offsets::m_nFallbackPaintKit);
                 // int currentIDHigh = mem.Read<int>(econGloves + Offsets::m_iItemIDHigh);

                 // Get Valid XUID from Active Weapon (if existing) or fallback to AccountID
                 int xuidLow = -1;
                 int xuidHigh = -1;
                 if (weapons.size() > 0) {
                     uintptr_t firstWep = weapons[0]; 
                     if (firstWep) {
                         xuidLow = mem.Read<int>(firstWep + Offsets::m_OriginalOwnerXuidLow);
                         xuidHigh = mem.Read<int>(firstWep + Offsets::m_OriginalOwnerXuidLow + 4);
                     }
                 }

                 // Debug Output (Throttle to every 60 frames ~ 1 sec)
                 static int frameCount = 0;
                 if (frameCount++ % 60 == 0) {
                     std::cout << "[GloveDebug] CurDef: " << currentDef << " | CurPaint: " << currentPaint 
                               << " | TargetDef: " << skinManager->Gloves.defIndex 
                               << " | TargetPaint: " << skinManager->Gloves.Paint 
                               << " | XUID_Low: " << xuidLow << std::endl;
                 }
                 
                 // Attributes logic
                 bool gameReverted = (currentDef != skinManager->Gloves.defIndex);
                 
                 if (userChangedSettings || ForceUpdate || gameReverted) 
                 {
                     // Disable Initialization during write
                     mem.Write<bool>(econGloves + Offsets::m_bInitialized, false);
                     
                     mem.Write<uint16_t>(econGloves + Offsets::m_iItemDefinitionIndex, skinManager->Gloves.defIndex);
                     
                     // Set ItemID to -1 (both Low and High)
                     mem.Write<int>(econGloves + Offsets::m_iItemIDHigh, -1);
                     mem.Write<int>(econGloves + Offsets::m_iItemIDHigh + 4, -1);
                     
                     mem.Write<int>(econGloves + Offsets::m_iEntityQuality, 3);
                     
                     // Apply XUID if found
                     if (xuidLow != -1) {
                         mem.Write<int>(econGloves + Offsets::m_OriginalOwnerXuidLow, xuidLow);
                         mem.Write<int>(econGloves + Offsets::m_OriginalOwnerXuidLow + 4, xuidHigh);
                         mem.Write<int>(econGloves + Offsets::m_iAccountID, xuidLow); 
                     } else {
                         mem.Write<int>(econGloves + Offsets::m_iAccountID, 12345);
                     }

                     // Create Attribute List
                     SkinInfo_t gloveSkin;
                     gloveSkin.Paint = skinManager->Gloves.Paint;
                     gloveSkin.weaponType = WeaponsEnum::none; 
                     econItemAttributeManager.Create(econGloves, gloveSkin);


                     mem.Write<bool>(econGloves + Offsets::m_bInitialized, true);
                     
                     mem.Write<bool>(localPlayerPawn + Offsets::m_bNeedToReApplyGloves, true);
                     
                     lastGloveDef = skinManager->Gloves.defIndex;
                     lastGlovePaint = skinManager->Gloves.Paint;
                 }
             }
        }

        if (ShouldUpdate || ForceUpdate)
            UpdateWeapons(weapons);

        // Keep ForceUpdate alive if skins are stored but none were applied this frame
        // (e.g. all weapons already had itemIDHigh=-1). This forces a re-process next frame.
        if (!ShouldUpdate && !skinManager->Skins.empty()) {
            bool anySkinHasPaint = false;
            for (auto& s : skinManager->Skins) if (s.Paint) { anySkinHasPaint = true; break; }
            if (anySkinHasPaint) ForceUpdate = true;
        } else {
            ForceUpdate = false;
        }
        
        if (ShouldUpdate) {
            configManager->AutoSave();
        }
    }
    
    return 0;
}