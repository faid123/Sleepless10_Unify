--stuff goes here



local fade_timer = 0.0  --for counting down
local fadeTime = 0.5    --Time for each fade
local holdTime = 2.42    --Time for holding Panels

local skipTimer = 0.5

local currState = 0     --either for 0 for fade out or 1 for fade in or 2 for hold or 3 for Ending Cutscene

local progressStage = 0 --sets the state for progress stage on the cutscene
local maxProgress = 4
-- Component Variables
local eEntID
local eSprite
local initBool = false

-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("fade Black screen script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --Console.WriteLine("fade Black screen script runnning")
    initBool = false
end

-- Updates the Game Object per game loop
function Update(dt)
  
    eEntID = ScriptSystem:getLuaEntityID()
    eSprite  = ECS:getComponent_SpriteCom(eEntID)

    if(initBool == false) then
        eSprite._drawSprite = true
        eSprite._alphaVal = 1.0
        initBool = true

        if(UISystem:getGameState() == 14) then --the very first loading screen
            holdTime = 5.0
            skipTimer = 1000 --some large number so that it never triggers
        end
    end    

    if skipTimer < 0 then
        if Inputs.get_Key(InputKey.Z_KEY) or Inputs.get_Controller(InputControllerKey.B_BUTTON) then
            if(currState ~= 3) then
                SetStateEnd()
            end
        end
    end
            
    if (fade_timer <= 0) then
        
        if(currState == 3) then

            
            UISystem:setPrevState() -- set previous game state
            if(UISystem:getGameState() == 14) then -- starting loading scene
                UISystem:setGameState(15) -- digipen logo scene
            elseif(UISystem:getGameState() == 15) then -- digipen logo scene
                UISystem:setGameState(16) -- fmod logo scene
            elseif(UISystem:getGameState() == 16) then -- fmod logo scene
                UISystem:setGameState(17) -- team logo scene
            else -- team logo scene
                UISystem:setGameState(0) --main menu scene
            end
        else
            if progressStage < maxProgress then
                progressStage = progressStage + 1
            else
                SetStateEnd()
            end

            if progressStage == 1 then
                SetStateFadeOut()
            elseif progressStage == 2 then
                SetStateHold()
            elseif progressStage == 3 then
                SetStateFadeIn()
            end
        end

    else
        fade_timer = fade_timer - dt
        updateScene(dt / (fadeTime))
    end

    skipTimer = skipTimer - dt

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
    fade_timer = fadeTime
end
function SetStateFadeIn()
    currState = 1
    fade_timer = fadeTime
end
function SetStateHold()
    currState = 2
    fade_timer = holdTime
end
function SetStateEnd()
    currState = 3
    fade_timer = 0.0
end