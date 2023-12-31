--stuff goes here

-- Editable Variable
-- local testString            = "dofileInEnemyChase"
-- local eWalkForce            = 50
-- local eSpeedCap             = 25.0
-- local eHealth               = 15
-- local eFlashTimer           = 1
-- local eUnified              = false
-- local spawnedCubes          = false
-- local canApplyLaserDamage   = true
-- local offsetPos             = 10.0
-- local lifeTime              = 5.0

local fade_timer = 0.0  --for counting down
local fadeTime = 2.0    --Time for each fade
local holdTime = 2.0    --Time for holding Panels

local currState = 0     --either for 0 for fade or 1 for stay or 2 for holdFade or 3 for Ending Cutscene

local progressStage = 0 --sets the state for progress stage on the cutscene
local maxProgress = 7
-- Component Variables
local eEntID
local eTransform
local eSprite

--for the actual changing of the texture
local pEntID
local pSprite

local creditsID

local timer = 0.0

player_env = {}
-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("CutsceneBehavior script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --Console.WriteLine("CutsceneBehavior script runnning")

    local playerID = ECS:getEntityID("Player")
    player_env["Player_Env"] = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
    -- player_env["Player_Env"]["playVideoOnce"] = true
    -- player_env["Player_Env"]["currentVideo"] = 9
    creditsID = ECS:getPrefabID("Prefab_AAACredits")
    -- RenderSystem:setVideoStart(true)
    -- RenderSystem:setPlayVideo(true)
    -- UISystem:toggleCutscene(true)
    -- player_env["Player_Env"]["CutsceneBool"] = true
    UISystem:toggleCutscene(true)

end

-- Updates the Game Object per game loop
function Update(dt)
--[[     eEntID = ScriptSystem:getLuaEntityID()
    eSprite  = ECS:getComponent_SpriteCom(eEntID)
    pSprite = ECS:getComponent_SpriteCom(ECS:getEntityID("AAFrontBGCutscene")) ]]
    
    -- if(eSprite._drawSprite ~= true) then
    --     eSprite._drawSprite = true
    -- end
    -- if(pSprite._drawSprite ~= true) then
    --     pSprite._drawSprite = true
    -- end
    timer = timer + dt

    if timer >= 3.0 then
        if Inputs.get_Key(InputKey.Z_KEY) or Inputs.get_Controller(InputControllerKey.B_BUTTON) or (RenderSystem:getPlayVideo() == false) then
            if(currState ~= 3) then
                SetStateEnd()
                ECS:instantiatePrefab(creditsID, "Credits", Vector3:new(0,0,1),Quat:new(0,0,0,1),false, false, false)
                
            end
        end
    end
    --if(currState ~= 3) then
    --    eSprite._alphaVal = 1
    --end
    ----Console.WriteLine("CUTSCENE BOSS DEFEAT")
    --if (fade_timer <= 0) then
    --    
    --    if(currState == 3) then
    --        ECS:addEntityToDestroyQueue(eEntID)
    --        ECS:addEntityToDestroyQueue(ECS:getEntityID("AAFrontBGCutscene"))
    --    elseif (currState ~= 2) then
    --        if progressStage < maxProgress then
    --            progressStage = progressStage + 1
    --            pSprite.SpriteName = "bossdefeat_scene_" .. tostring(progressStage)
    --        end
    --
    --        if progressStage == 1 then
    --            SetStateHold()
    --        elseif progressStage == 2 then
    --            SetStateHold()
    --        elseif progressStage == 3 then
    --            SetStateHoldFade()
    --        elseif progressStage == 4 then
    --            SetStateHold()
    --        elseif progressStage == 5 then
    --            SetStateHold()
    --        elseif progressStage == 6 then
    --            SetStateHoldFade()
    --        elseif progressStage == 7 then
    --            SetStateEnd()
    --        end
    --
    --
    --    elseif(currState ==2) then
    --        SetStateFade()
    --    end
    --
    --else
    --    fade_timer = fade_timer - dt
    --    updateScene(dt)
    --end

    --Console.PrintFloat(fade_timer)


end

-- If there is collision detected, this physics component runs this function
function onTriggerEnter2D(gameObject, dt, manifold)
 
end

-- If there is collision detected, this physics component runs this function
function onTriggerStay2D(gameObject, dt, manifold)

end

function changeState(nextState,dt)
    
    
end 

function updateScene(dt)

--[[     if(currState == 0) then
        pSprite._alphaVal = pSprite._alphaVal - dt       
    else
        pSprite._alphaVal = 1
    end
 ]]
end

function SetStateFade()
    currState = 0
    fade_timer = fadeTime
end
function SetStateHold()
    currState = 1
    fade_timer = holdTime
end
function SetStateHoldFade()
    currState = 2
    fade_timer = holdTime
end
function SetStateEnd()
    currState = 3    
    UISystem:toggleCutscene(false)
    --UISystem:setGameState(8)
--[[     fade_timer = fadeTime
    pSprite._alphaVal = 0

    UISystem:toggleCutscene(false)
    player_env["Player_Env"]["CutsceneBool"] = false
    UISystem:setGameState(8) ]]
end