#pragma once
#include "Argent/Argent.h"
#include <windef.h>

#include "Easing.h"

#include "ChangeMode.h"
#include "FriendCreater.h"

#include "Numbers.h"
#include "Player.h"

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
        ChangeBattle,
    };

    void Initialize()override;
    void Begin()override;
    void Update()override;

    void OpenUpdate();
    void CloseUpdate();

    void DrawDebug()override;

    void OpenShop();
    void CloseShop();

    /*Item& GetItem(ItemType t) 
    {
        for (auto& i : itemArray)
        {
            if (i.type == t)return i;
        }
    }*/

    std::vector<GameObject*> items;

    typedef float (*EasingP)(float, float, float, float);
    EasingP easeFunc;

    ChangeMode* mode;

    float timer;
    float easeMaxTime{1};

    //ÉCÅ[ÉWÉìÉOèIÇÌÇ¡ÇƒÇÈÇ©
    bool easeEnd{true};

    //ÉVÉáÉbÉvêÍópÇ…ï ÇÃÉ}ÉEÉXÉJÅ[É\ÉãÇçÏÇÈ
    DirectX::XMFLOAT2 mousePos;

    GameObject* mouse;

    FriendCreater* friendCreater;

    Number* money;

    Player* player;

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
    Item(const Shop::ItemType type,int price, float priceIncreasePersent,const DirectX::XMFLOAT2 pos,const RECT button = { -32, 32, 32, -32 })
        :BaseActor("Item"), price(price), type(type), priceIncreasePersent(priceIncreasePersent), button(button),initPos(pos)
    {
    }
    ~Item() {}

    void Initialize()override;
    void Finalize()override;
    void Update()override;

    virtual bool BuyConditions();//çwì¸èåè
    virtual void Buy();//çwì¸
    virtual void BuyCommon();//çwì¸ÅAã§í ÇÃèàóù
    virtual bool SaleConditions();//îÑãpèåè
    virtual void Sale();//îÑãp
    virtual void SaleCommon();//îÑãpÅAã§í ÇÃèàóù

    int price;//ã‡äz
    Shop::ItemType type;
    int num;//çwì¸êî
    float priceIncreasePersent;//ÇPçwì¸Ç≤Ç∆ÇÃílè„Ç∞î{ó¶

    RECT button;//çwì¸ÅAîÑãpÉ{É^ÉìÇÃìñÇΩÇËîªíËópãÈå`

    DirectX::XMFLOAT2 initPos;

    DirectX::XMFLOAT3 priceScale{1,1,1};
    DirectX::XMFLOAT3 numScale{1,1,1};

    Shop* shop;

    Number* priceNum;
    Number* _num;

    float timer;
};

class ItemCreature : public Item
{
public:
    ItemCreature(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type,price,priceIncreasePersent,pos,button){}
    ~ItemCreature() {}

    void Initialize()override;
    void Buy()override;
    void Sale()override;

    bool SaleConditions()override;//çwì¸èåè
};

class ItemDrone : public Item
{
public:
    ItemDrone(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemDrone() {}


    void Initialize()override;
    void Buy()override;
    void Sale()override;

    bool SaleConditions()override;//çwì¸èåè
};

class ItemBloodAmo : public Item
{
public:
    ItemBloodAmo(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemBloodAmo() {}


    void Buy()override;
    void Sale()override;

};

class ItemElectricAmo : public Item
{
public:
    ItemElectricAmo(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemElectricAmo() {}


    void Buy()override;
    void Sale()override;

};

class ItemBloodGrenade : public Item
{
public:
    ItemBloodGrenade(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemBloodGrenade() {}

    void Initialize()override;
    void Buy()override;
    void Sale()override;

};

class ItemElectricGrenade : public Item
{
public:
    ItemElectricGrenade(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemElectricGrenade() {}

    void Initialize()override;
    void Buy()override;
    void Sale()override;

};

class ItemChangeEdit : public Item
{
public:
    ItemChangeEdit(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemChangeEdit() {}

    void Initialize()override;
    void Buy()override;

    void BuyCommon()override {}
    void SaleCommon()override {}
};

class ItemChangeBattle : public Item
{
public:
    ItemChangeBattle(const Shop::ItemType type, int price, float priceIncreasePersent, const DirectX::XMFLOAT2 pos, const RECT button = { -32, 32, 32, -32 })
        :Item(type, price, priceIncreasePersent, pos, button) {}
    ~ItemChangeBattle() {}

    void Initialize()override;
    void Buy()override;

    void BuyCommon()override {}
    void SaleCommon()override {}
};

