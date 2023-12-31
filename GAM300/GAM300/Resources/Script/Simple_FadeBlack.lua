-- This is a script for simple controlling a fade black screen



local fade_timer = 0.0  --for counting down
local fadeTime = 1.0    --Time for each fade
local holdTime = 1.0    --Time for holding Panels

local currState = 0     --either for 0 for fade out or 1 for fade in or 2 for hold or 3 for Ending Cutscene

local progressStage = 0 --sets the state for progress stage on the cutscene
local maxProgress = 2
-- Component Variables
local eEntID
local eSprite
local initBool = false
local changingScene = false
local playingVideo = false

-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("fade Black screen script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --Console.WriteLine("fade Black screen script runnning")
    initBool = false
    changingScene = false
    playingVideo = false
end

-- Updates the Game Object per game loop
function Update(dt)
  
    eEntID = ScriptSystem:getLuaEntityID()
    eSprite  = ECS:getComponent_SpriteCom(eEntID)

    if(initBool == false) then
        eSprite._drawSprite = true
        eSprite._alphaVal = 1.0
        initBool = true
        changingScene = UISystem:getChangingSceneState()
        playingVideo = RenderSystem:getFadetoBlack()
    end 
    if (fade_timer <= 0) then
        
        if(currState == 3) then        
            if(changingScene == true) then
                UISystem:setChangingSceneState(false)
                local val1 = UISystem:getNextState()
                UISystem:setGameState(val1) --change scene to next stage
            else
                if(playingVideo == true) then
                    RenderSystem:setPlayVideo(true)
                end
                eSprite._drawSprite = false
                eSprite._alphaVal = 0.0
                ECS:addEntityToDestroyQueue(eEntID)
            end
        else
            if progressStage < maxProgress then
                progressStage = progressStage + 1
            else
                SetStateEnd()
            end

            if progressStage == 1 then
                SetStateFadeOut()
            end
        end

    else
        fade_timer = fade_timer - dt
        updateScene(dt / (fadeTime))
    end

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

    if(currState == 0) then
        eSprite._alphaVal = eSprite._alphaVal - dt
    elseif(currState == 1) then
        eSprite._alphaVal = eSprite._alphaVal + dt
    end

    --checking within bounds
    if(eSprite._alphaVal < 0.0) then
        eSprite._alphaVal = 0.0
    elseif (eSprite._alphaVal > 1.0) then
        eSprite._alphaVal = 1.0
    end
end

function SetStateFadeOut()
    currState = 0
    eSprite._alphaVal = 1.0
    fade_timer = fadeTime
end
function SetStateFadeIn()
    currState = 1
    eSprite._alphaVal = 0.0
    fade_timer = fadeTime
end
function SetStateHold()
    currState = 2
    fade_timer = holdTime
end
function SetStateEnd()
    currState = 3
end