--pretty much only happens in main menu so most of the code is focused in main menu



local fade_timer = 0.0  --for counting down
local fadeTime = 2    --Time for each fade
local holdTime = 3.0    --Time for holding Panels

local currState = 0     --either for 0 for fade out or 1 for fade in or 2 for hold or 3 for Ending Cutscene

local progressStage = 0 --sets the state for progress stage on the cutscene
local maxProgress = 2
-- Component Variables
local eEntID
local eSprite
local initBool = false

-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("fade white screen script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --Console.WriteLine("fade white screen script runnning")
    initBool = false
end

-- Updates the Game Object per game loop
function Update(dt)
  
    --Console.WriteLine("fade white screen script updating")

    
    eEntID = ScriptSystem:getLuaEntityID()
    eSprite  = ECS:getComponent_SpriteCom(eEntID)

    if(initBool == false) then
    
        local val1 = UISystem:getPrevState()
        local val2 = UISystem:getGameState()

        if(val1 == 17) then -- previous scene was team logo scene
            eSprite._drawSprite = true
            eSprite._alphaVal = 1.0
        elseif(val1 == 1 or val1 == 2 or val1 == 3) then --if prev scene was the other levels and returning from mainmenu
            eSprite._colour = Vector3:new(0.0,0.0,0.0) --black
            eSprite._drawSprite = true
            eSprite._alphaVal = 1.0
         elseif(val1 ~=  15 and val2 == 0) then -- if prevscene not team logo and currState is main menu
            eSprite._drawSprite = false
            eSprite._alphaVal = 0.0
            fade_timer = 0.0
            SetStateEnd()
        elseif(val2 ~= 0) then --if not main menu (basically anywhere else)
            eSprite._drawSprite = true
            eSprite._alphaVal = 1.0
        end
        initBool = true
        UISystem:setPrevState()
    end    
    --if Inputs.get_Key(InputKey.Z_KEY) or Inputs.get_Controller(InputControllerKey.B_BUTTON) then
    --    if(currState ~= 3) then
    --        SetStateEnd()
    --    end
    --end


    --Console.WriteLine("fade white screen script ENTER")
    if (fade_timer <= 0) then
        
        if(currState == 3) then
            eSprite._alphaVal = 0.0
            ECS:addEntityToDestroyQueue(eEntID)
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
    fade_timer = 0
end