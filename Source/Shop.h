#pragma once
#include "Argent/Argent.h"
#include <windef.h>

#include "Easing.h"

#include "ChangeMode.h"

class Item;
class Shop : public Argent::Component::BaseComponent
{
//private:
//    Shop() : BaseComponent("Shop") {}
//    ~Shop() {}

public:
    Shop() : BaseComponent("Shop") {}
    ~Shop() {}

    enum class ItemType
    {
        Creature,
        Drone,
        BloodAmo,
        ElectricAmo,
        BloodGrenade,
        ElectricGrenade,
        ChangeEdit,
        ChangeFight,
    };

    void Initialize()override;
    void Begin()override;
    void Update()override;

    void OpenUpdate();
    void CloseUpdate();

    void DrawDebug()override;

    /*Item& GetItem(ItemType t) 
    {
        for (auto& i : itemArray)
        {
            if (i.type == t)return i;
        }
    }*/

    int money;
    std::vector<GameObject*> items;

    typedef float (*EasingP)(float, float, float, float);
    EasingP easeFunc;

    ChangeMode* mode;

    float timer;
    float easeMaxTime{1};

    //イージング終わってるか
    bool easeEnd{true};

    //ショップ専用に別のマウスカーソルを作る
    DirectX::XMFLOAT2 mousePos;

    GameObject* mouse;
private:
    void SetItem();

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);


#ifdef _DEBUG
    std::string currentEase;
    std::vector<std::string> easeFunctoins;

    std::vector<EasingP> easeFuncs;

    bool ImGuiComboUI(const std::string& caption, std::string& current_item,
        const std::vector<std::string>& items) {
        bool changed = false;

        if (ImGui::BeginCombo(caption.c_str(), current_item.c_str())) {
            for (int n = 0; n < items.size(); n++) {
                bool is_selected = (current_item == items[n]);
                if (ImGui::Selectable(items[n].c_str(), is_selected)) {
                    current_item = items[n];
                    easeFunc = easeFuncs.at(n);
                    changed = true;
                }
                if (is_selected) {
                    // Set the initial focus when opening the combo (scrolling + for
                    // keyboard navigation support in the upcoming navigation branch)
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        return changed;
    }
#endif // _DEBUG

    
};


class Item : public Argent::Component::BaseActor
{
public:
    Item(const Shop::ItemType type,int price, float priceIncreasePersent,const DirectX::XMFLOAT2 pos,const RECT button = { -65, 30, 65, -30 })
        :BaseActor("Item"), price(price), type(type), priceIncreasePersent(priceIncreasePersent), button(button),initPos(pos)
    {
    }
    ~Item() {}

    void Initialize()override;
    void Update()override;

    void Buy();//購入
    void Sale();//売却

    int price;//金額
    Shop::ItemType type;
    int num;//購入数
    float priceIncreasePersent;//１購入ごとの値上げ倍率

    RECT button;//購入、売却ボタンの当たり判定用矩形

    DirectX::XMFLOAT2 initPos;

    Shop* shop;
};
