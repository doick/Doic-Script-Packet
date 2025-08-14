// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

#if ENGINE >= Engine_G2A
	HOOK Hook_oCNpc_EV_DropVob PATCH(&oCNpc::EV_DropVob, &oCNpc::EV_DropVob_Union);

	int oCNpc::EV_DropVob_Union(oCMsgManipulate* t_csg)
	{
		zCModel* model = GetModel();
		SetBodyState(BS_INVENTORY);

		if (anictrl->IsStateAniActive(anictrl->_s_walk))
		{
			if (t_csg->IsInUse()) return 1;

			oCItem* itm = dynamic_cast<oCItem*>(t_csg->targetVob);

			if (itm && (itm->flags & ITM_FLAG_MI))
			{
				cmd << "[DSP_Hooks::Hook_oCNpc_EV_DropVob] Cannot drop quest related item: " << itm->GetInstanceName() << endl;
				GetEM()->OnMessage(zNEW(oCMsgManipulate)(
					oCMsgManipulate::EV_CALLSCRIPT,
					"PLAYER_CANNOT_DROP_MISSIONITEM",
					-1), this);
				return 1;
			}

			model->StartAni("T_STAND_2_IDROP", NULL);
		}
		else if (anictrl->IsStateAniActive(anictrl->s_idrop))
		{
			if (t_csg->targetVob) {
				DoDropVob(t_csg->targetVob);
			}
			else if (!slot.IsEmpty()) {
				RemoveFromSlot(t_csg->npcSlot, 1, 1);
			}
			else if (!t_csg->name.IsEmpty())
			{
				oCItem* item = RemoveFromInv(t_csg->name, 1);
				if (item) DoDropVob(item);
			}

			if (anictrl->t_idrop_2_stand)
			{
				anictrl->SetNextAni(anictrl->t_idrop_2_stand, anictrl->_s_walk);
				model->StartAni(anictrl->t_idrop_2_stand, NULL);
			}
			else {
				model->StartAni(anictrl->_s_walk, NULL);
			}

			t_csg->SetInUse(1);
		}

		return 0;
	}
#endif
}