#include "AnimationSystem.h"
#include "Engine.h"
#include "Graphics/RenderSystem/RenderSystem.h"

using aclip = AnimationClip;

void AnimationSystem::SpriteInit()
{
    size_t totalFrames = 150, row = 15, col = 10;
    _animatorCont.emplace("LoadingLogo60", aclip{ totalFrames, row, col, 10.0f, 0, static_cast<int>(totalFrames - 1 ),"LoadingLogo60"});
    totalFrames = 60, row = 10, col = 6;
    _animatorCont.emplace("Selector", aclip{ totalFrames, row, col, 10.0f, 0,  static_cast<int>(totalFrames - 1),"Selector"});
    _animatorCont.emplace("LoadingLogo", aclip{ totalFrames, row, col, 10.0f, 0,  static_cast<int>(totalFrames - 1),"LoadingLogo"});
    totalFrames = 24, row = 6, col = 4;
    _animatorCont.emplace("SpeedlinesAnim", aclip{ totalFrames, row, col, 10.0f, 0,  static_cast<int>(totalFrames - 1),"SpeedlinesAnim" });
    totalFrames = 50, row = 5, col = 10;
    _animatorCont.emplace("Healing", aclip{ totalFrames, row, col, 10.0f, 0,  static_cast<int>(totalFrames - 1),"Healing" });
    totalFrames = 50, row = 5, col = 10;
    _animatorCont.emplace("Ultimate", aclip{ totalFrames, row, col, 10.0f, 0,  static_cast<int>(totalFrames - 1),"Ultimate" });
}

void AnimationSystem::addAnimatorToEntity()
{
    auto list = Engine::_ecs.getActiveEntities();
    for (int i = 0; i < list.size(); ++i)
    {
        if (Engine::_ecs.getComponent<AnimationCom>(list[i]._entityID))
        {
            MaterialCom& m = *Engine::_ecs.getComponent<MaterialCom>(list[i]._entityID);

            Animator tmp = Engine::_ecs.getSystem<RenderSystem>()->_graphics._animationInfo[m.m_model];
            tmp.texture = m.m_texture;
            allAnimators[list[i]._entityID] = std::make_shared<Animator>(tmp);
            updateAnimationCom(list[i]._entityID);
        }

        if (Engine::_ecs.getComponent<SpriteCom>(list[i]._entityID))
        {
            auto s = Engine::_ecs.getComponent<SpriteCom>(list[i]._entityID);
            if (s->_isAnimation)
            {
                AnimationClip a = _animatorCont[s->SpriteName];
                a._clipSpeed = s->_clipSpeed;
                allSpriteSheets[&(*s)] = std::make_shared<AnimationClip>(a);
            }
        }
    }
}
void AnimationSystem::updateAnimationCom(EntityID eid)
{
    MaterialCom& m = *Engine::_ecs.getComponent<MaterialCom>(eid);
    if (allAnimators[eid] == nullptr)
    {
        Animator tmp = Engine::_ecs.getSystem<RenderSystem>()->_graphics._animationInfo[m.m_model];
        tmp.texture = m.m_texture;
        allAnimators[eid] = std::make_shared<Animator>(tmp);
    }

    if (allAnimators[eid]->model != m.m_model)
    {
        allAnimators[eid]->model = m.m_model;
        *allAnimators[eid] = Engine::_ecs.getSystem<RenderSystem>()->_graphics._animationInfo[m.m_model];
        allAnimators[eid]->texture = m.m_texture;
    }

    allAnimators[eid]->comp = *Engine::_ecs.getComponent<AnimationCom>(eid);

    if (allAnimators[eid]->comp.currentAnim > allAnimators[eid]->totalAnimations)
        allAnimators[eid]->comp.currentAnim = 0;

    if (allAnimators[eid]->displayedAnimation != allAnimators[eid]->comp.currentAnim)
    {
        allAnimators[eid]->displayedAnimation = allAnimators[eid]->comp.currentAnim;
        allAnimators[eid]->animation = allAnimators[eid]->animationContainer[allAnimators[eid]->comp.currentAnim];
        allAnimators[eid]->elapsedTime = 0;
    }

}

void AnimationSystem::updateAnimators(float dt)
{
    ComponentID id = Engine::_ecs.getComponentID<AnimationCom>();
    auto animList = std::static_pointer_cast<ComponentContainer<AnimationCom>>(Engine::_ecs.getComContainer(id));
    auto entityList = animList->getEntityTypeIDs();

    EntityID eid = 0;
    for (std::set<EntityID>::iterator idIter = entityList.begin(); idIter != entityList.end(); ++idIter)
    {
        eid = *idIter;
        if (!Engine::_isPlay)
            Engine::_ecs.getComponent<AnimationCom>(eid)->play = false;
        else
            Engine::_ecs.getComponent<AnimationCom>(eid)->play = true;

        updateAnimationCom(eid);
        allAnimators[eid]->update(dt);
    }
}

void AnimationSystem::SpriteUpdate(float dt)
{
    ComponentID id = Engine::_ecs.getComponentID<SpriteCom>();
    auto animList = std::static_pointer_cast<ComponentContainer<SpriteCom>>(Engine::_ecs.getComContainer(id));
    auto entityList = animList->getEntityTypeIDs();

    EntityID eid = 0;
    for (std::set<EntityID>::iterator idIter = entityList.begin(); idIter != entityList.end(); ++idIter)
    {
        eid = *idIter;
        if (Engine::_ecs.getComponent<SpriteCom>(eid)->_isAnimation != true)
            continue;
        /*	if (!Engine::_isPlay)
                Engine::_ecs.getComponent<SpriteCom>(eid)->_isPlaying = false;
            else
                Engine::_ecs.getComponent<SpriteCom>(eid)->_isPlaying= true;*/

        //if (allSpriteSheets[eid] == nullptr)
        //{
        //    SpriteCom& s = *Engine::_ecs.getComponent<SpriteCom>(eid);
        //    AnimationClip a = _animatorCont[s.SpriteName];
        //    a._clipSpeed = s._clipSpeed;
        //    allSpriteSheets[eid] = std::make_shared<AnimationClip>(a);
        //}

        auto s = Engine::_ecs.getComponent<SpriteCom>(eid);
        if (allSpriteSheets[&(*s)] == nullptr)
        {
            AnimationClip a = _animatorCont[s->SpriteName];
            a._clipSpeed = s->_clipSpeed;
            allSpriteSheets[&(*s)] = std::make_shared<AnimationClip>(a);
        }

        if (allSpriteSheets[&(*s)]->spriteName != s->SpriteName
            && (_animatorCont.count(s->SpriteName)>0))
        {
            *allSpriteSheets[&(*s)] = _animatorCont[s->SpriteName];
        }

        if (Engine::_ecs.getComponent<SpriteCom>(eid)->_isPlaying)
        {
            if (allSpriteSheets[&(*s)]->GetClipSpeed() != s->_clipSpeed)
                allSpriteSheets[&(*s)]->_clipSpeed = s->_clipSpeed;

            allSpriteSheets[&(*s)]->_fCurrentFrame += allSpriteSheets[&(*s)]->GetClipSpeed() * dt;
            allSpriteSheets[&(*s)]->_currentFrame = static_cast<int>(floorf(allSpriteSheets[&(*s)]->_fCurrentFrame));

            if (allSpriteSheets[&(*s)]->_currentFrame >= allSpriteSheets[&(*s)]->_numOfFrames ||
                allSpriteSheets[&(*s)]->_currentFrame > allSpriteSheets[&(*s)]->_rangeEnd)
            {
                if (!allSpriteSheets[&(*s)]->_looping)
                {
                    allSpriteSheets[&(*s)]->_currentFrame = allSpriteSheets[&(*s)]->_rangeEnd;
                    s->_isPlaying = false;
                    continue;
                }
                else
                {
                    s->_isPlaying = true;
                }
                allSpriteSheets[&(*s)]->_currentFrame = allSpriteSheets[&(*s)]->_rangeStart;
                allSpriteSheets[&(*s)]->_fCurrentFrame = (float)allSpriteSheets[&(*s)]->_rangeStart;
            }
        }
    }

}


void AnimationSystem::init()
{
    SpriteInit();
}

void AnimationSystem::load()
{
    addAnimatorToEntity();
}

void AnimationSystem::update(float delta_time)
{
    updateAnimators(delta_time);
    SpriteUpdate(delta_time);
}