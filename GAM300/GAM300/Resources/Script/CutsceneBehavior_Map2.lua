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
    player_env["Player_Env"]["playVideoOnce"] = true
    player_env["Player_Env"]["currentVideo"] = 7
    RenderSystem:setVideoStart(true)
    AudioSystem:StopUISound("OtherDefeatCutScene");
end

-- Updates the Game Object per game loop
function Update(dt)
    if (RenderSystem:getPlayVideo() == false and RenderSystem:getStartVideo() == false) then
            UISystem:toggleCutscene(false)
            UISystem:setGameState(9)
    end
end