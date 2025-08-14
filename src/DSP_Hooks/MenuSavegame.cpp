// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

    HOOK Ivk_oCMenuSavegame_HandleSlotChange PATCH(&oCMenuSavegame::HandleSlotChange, &oCMenuSavegame::HandleSlotChange_Union);

    void oCMenuSavegame::HandleSlotChange_Union(int key) {
        int index = parserMenu->GetIndex("MENU_SAVEGAME_API");
        THISCALL(Ivk_oCMenuSavegame_HandleSlotChange)(key);
        if (index > 0) parserMenu->CallFunc(index);
    }
}
