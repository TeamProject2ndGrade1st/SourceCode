#include "Tutorial.h"

void Tutorial::Initialize()
{
    BaseActor::Initialize();

    state = State::tutorial_0;
}

void Tutorial::Update()
{
    if (Argent::Input::GetKeyDown(KeyCode::Enter))
    {
        state++;
    }

    StateUpdate();
}

void Tutorial::StateUpdate()
{
    switch (state)
    {
    case Tutorial::tutorial_0:
        if (!init[state])
        {
            explanation[state] = GameObject::Instantiate("exp1", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_1.png"));
            init[state] = true;
        }

        break;


    case Tutorial::tutorial_1:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp2", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_2.png"));
            init[state] = true;
        }

        break;


    case Tutorial::tutorial_2:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp3", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_3.png"));
            init[state] = true;
        }
        break;


    case Tutorial::tutorial_3:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp4", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_4.png"));
            init[state] = true;
        }
        break;


    case Tutorial::tutorial_4:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp5", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_5.png"));
            init[state] = true;
        }
        break;


    case Tutorial::tutorial_5:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            explanation[state] = GameObject::Instantiate("exp6", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_6.png"));
            init[state] = true;
        }
        break;


    case Tutorial::tutorial_6:
        if (!init[state])
        {
            GameObject::Destroy(explanation[state - 1]);
            //explanation[state] = GameObject::Instantiate("exp1", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Explanation/UI_E_7"));
            init[state] = true;
        }
        break;


    case Tutorial::end:
        GameObject::Destroy(GetOwner());
        break;
    }
}
