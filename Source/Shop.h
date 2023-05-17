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
    };

    //�V���b�v�̓Q�[���I���܂Ń��Z�b�g����Ȃ��悤�ɂ�����
    //�C���X�^���V���O�ŃV�[���J�ڌ���p�������悤�ɂ���
    //�܂�����ĂȂ����ǖ��������w
    /*static Shop& Instance()
    {
        static Shop instance;
        return instance;
    }*/

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
    std::vector<Item> itemArray;

    typedef float (*EasingP)(float, float, float, float);
    EasingP easeFunc;

    ChangeMode* mode;

    float timer;
    float easeMaxTime{1};

    //�C�[�W���O�I����Ă邩
    bool easeEnd;

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


class Item
{
public:
    Item() {}
    ~Item() {}

    void Update(float x, float y);

    void Buy();//�w��
    void Sale();//���p

    int amount;//���z
    Shop::ItemType type;
    int num;//�w����
    float priceIncreasePersent;//�P�w�����Ƃ̒l�グ�{��

    DirectX::XMFLOAT2 pos;//���S
    RECT button;//�w���A���p�{�^���̓����蔻��p��`
};
