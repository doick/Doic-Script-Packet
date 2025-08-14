// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

#if ENGINE >= Engine_G2A
    HOOK Hook_oCViewDialogTrade_OnTransferLeft PATCH(&oCViewDialogTrade::OnTransferLeft, &oCViewDialogTrade::OnTransferLeft_Union);

    int __fastcall oCViewDialogTrade::OnTransferLeft_Union(short t_amount) {
		oCItem* pItemChosen = NULL;

		switch (this->SectionTrade)
		{
			case TRADE_SECTION_RIGHT_INVENTORY:
			{
				IncTransferCount(t_amount);
				oCItem* item = this->DlgInventoryPlayer->GetSelectedItem();

				if (!item)
				{
					cmd << "[DSP_Hooks::Hook_oCViewDialogTrade_OnTransferLeft] No item selected in the player's inventory." << endl;
					return 1;
				}

				if (item->amount <= t_amount)
				{
					t_amount = item->amount;
					SetTransferCount(0);
				}

				if (item->GetInstanceName() == oCItemContainer::GetCurrencyInstanceName())
				{
					cmd << "[DSP_Hooks::Hook_oCViewDialogTrade_OnTransferLeft] oCItemContainer::GetCurrencyInstanceName()" << oCItemContainer::GetCurrencyInstanceName() << endl;
					return 1;
				}

				int conditionFunc = parser->GetIndex("C_PLAYERCANSELLITEM");
				int canPlayerSellItem = 1;

				if (conditionFunc <= 0) {
					cmd << "[DSP_Hooks::Hook_oCViewDialogTrade_OnTransferLeft] `C_PlayerCanSellItem` function not found." << endl;
				}
				else
				{
					parser->SetInstance("ITEM", item);
					canPlayerSellItem = *(int*)parser->CallFunc(conditionFunc);
				}

				if (!canPlayerSellItem)
				{
					cmd << "[DSP_Hooks::Hook_oCViewDialogTrade_OnTransferLeft] Player cannot sell item: " << item->GetInstanceName() << endl;
					return 1;
				}

				short i;
				for (i = 0; i < t_amount; i++)
				{
					pItemChosen = this->DlgInventoryPlayer->RemoveSelectedItem();

					if (pItemChosen)
					{
						int value = pItemChosen->GetValue();

						if (value > 0)
						{
							value = zINT(zREAL(value) * this->DlgInventoryNpc->ValueMultiplier);

							if (value <= 0) value = 1;
						}

						if (value <= 0)
						{
							this->DlgInventoryNpc->InsertItem(pItemChosen);
						}
						else
						{
							oCItem* coin = oCItemContainer::CreateCurrencyItem(value);

							if (!coin)
							{
								this->DlgInventoryPlayer->InsertItem(pItemChosen);
							}
							else
							{
								this->DlgInventoryNpc->InsertItem(pItemChosen);
								this->DlgInventoryPlayer->InsertItem(coin);
								coin->Release();
							}
						}
					}
				}
			}

			break;
		}

		return 1;
    }
#endif
}