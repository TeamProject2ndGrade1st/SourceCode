#include "Tutorial.h"

void Tutorial::Initialize()
{
    BaseActor::Initialize();

    state = State::Move;
}

void Tutorial::Update()
{
    StateUpdate();
}

void Tutorial::StateUpdate()
{
    switch (state)
    {
    case Tutorial::Move:
        if (!init[state])
        {
            explanation[state] = GameObject::Instantiate("exp1", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_1.png"));
            explanation[state]->AddChild(new GameObject("Enter", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/Enter.png")));
            init[state] = true;
        }

        if (Argent::Input::GetKeyDown(KeyCode::Enter))
        {
            state++;
        }

        if (Argent::Input::GetKeyDown(KeyCode::B))
        {
            GameObject::Destroy(explanation[state]);
            state = Shop1;
        }

        break;


    case Tutorial::OpenShop:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp2", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_2.png"));
            init[state] = true;
        }

        if (Argent::Input::GetKeyDown(KeyCode::B))
        {
            state++;
        }

        break;


    case Tutorial::Shop1:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp3", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_3.png"));
            explanation[state]->AddChild(new GameObject("Enter", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/Enter.png")));
            init[state] = true;
        }

        if (Argent::Input::GetKeyDown(KeyCode::Enter))
        {
            state++;
        }
        break;


    case Tutorial::Shop2:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp4", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_4.png"));
            explanation[state]->AddChild(new GameObject("Enter", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/Enter.png")));
            init[state] = true;
        }

        if (Argent::Input::GetKeyDown(KeyCode::Enter))
        {
            state++;
        }
        break;


    case Tutorial::ChangeEdit:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp5", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_5.png"));
            init[state] = true;
        }

        if (!mode)
        {
            std::vector<GameObject*> m;
            GameObject::FindByTag(GameObject::Tag::ChangeMode, m);
            mode = m.at(0)->GetComponent<ChangeMode>();
        }
        
        if (mode->currentMode == ChangeMode::Mode::Edit)
        {
            state++;
        }
        break;


    case Tutorial::Edit:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp6", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_6.png"));
            explanation[state]->AddChild(new GameObject("Enter", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/Enter.png")));
            init[state] = true;
        }

        if (Argent::Input::GetKeyDown(KeyCode::Enter))
        {
            state++;
        }
        break;


    case Tutorial::tutorial_6:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp7", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_7.png"));
            init[state] = true;
        }

        if (Argent::Input::GetKeyDown(KeyCode::B))
        {
            state++;
        }
        if (Argent::Input::GetKeyDown(KeyCode::Enter))
        {
            state++;
        }
        break;

    case Tutorial::tutorial_7:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp8", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_8.png"));
            init[state] = true;
        }

        if (mode->currentMode == ChangeMode::Mode::Battle || Argent::Input::GetKeyDown(KeyCode::Enter))
        {
            GameObject::Destroy(explanation[state]);
            state = end;
        }
        

        break;


    case Tutorial::end:
        GameObject::Destroy(GetOwner());
        break;
    }
}
