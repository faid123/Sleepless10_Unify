-- local variables

local playerID
local player_env

local boss_env
local bossHeadID
local bossRotNew
local boss_Transform

local rocketBulletID
local rocketBulletID1
local rocketBulletID2
local rocketPrefabID
local pExplosionPrefabID
local eEntID
-- Component Variables
local bTransform
local bRigidStatic
local bEmitter
local bMaterial
local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)
local bLeftwardPrime = Vector3:new()
local bLeftward      = Vector3:new(-1.0, 0.0, 0.0)
local bLeftwardOffset = 10.0


local bRotNew       = Quat:new()
local bPos          = Vector3:new()

local hasFired      = false
local spawnTimer    = 0.6
local spawnTimerControl = 4.0
local spawnCounter = 0
local animPause = false

-- for boss head id and name mismatch
local tagList

-- taret position of where rocket will launch to
local playerTransform

-- global variables

-- Awake is called once and only once when script is loaded.
function Awake()
	pExplosionPrefabID = ECS:getPrefabID("Prefab_Explosion")
    rocketPrefabID = ECS:getPrefabID("Prefab_BossRocket1")
end

-- Start is called once and only once after Awake() is called.
function Start()
	-- get the player entity id
	playerID    = ECS:getEntityID("Player")
	player_env  = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
    tagList     = ECS:getEntityTagged("Boss") 
    -- bossHeadID = ECS:getEntityID("3_Boss_Head")
end

-- Updates the Game Object per game loop
function Update(dt)
    tagList = ECS:getEntityTagged("Boss") 
    for i=0, #tagList, 1 do
        local id = tagList[i]
        if id ~= nil then
            bossHeadID = tagList[i]
        end
    end
    --bossHeadID = ECS:getEntityID("3_Boss_Head")
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidStatic = ECS:getComponent_RigidStatic(eEntID)
    bEmitter = ECS:getComponent_Emitter(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    bMaterial = ECS:getComponent_Material(eEntID)
    local bRotTemp = bRotNew
    --bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)  

    initInUpdate()
    UpdateWeaponRotation()
    -- spawn rocket every x seconds
    -- Console.WriteLine("Boss Health")
    -- Console.WriteLine(boss_env:GetEnvironment().bossHealth)
    if boss_env:GetEnvironment().bossHealth > 0 then
        if boss_env:GetEnvironment().bossHealth < 75.0 then
            spawnTimerControl = spawnTimerControl - dt
            -- Console.WriteLine(spawnTimerControl)
            local barrierCount = ECS:getEntityTagged("Barrier")
            if #barrierCount == 0 then
                if spawnTimerControl <= 0.0 then
                    if spawnTimer > 0.0 then
                        spawnTimer = spawnTimer - dt
                        if not hasFired then
                            if ECS:hasComponent_AnimationCom(eEntID) == true then
                                local animation = ECS:getComponent_AnimationCom(eEntID)
                                animation.speed = 13
                                animation.currentAnim = 0
                            end
                            spawnShowerRocket()
                            spawnCounter = spawnCounter + 1
                            hasFired = true
                        end
                    end

                    if spawnTimer <= 0.0 then
                        hasFired = false
                        spawnTimer = 0.6
                    end

                    if spawnCounter == 3 then
                        spawnTimerControl = 4.0
                        hasFired = false
                        spawnTimer = 0.6
                        spawnCounter = 0
                        if ECS:hasComponent_AnimationCom(eEntID) == true then
                            local animation = ECS:getComponent_AnimationCom(eEntID)
                            animation.currentAnim = 1
                        end
                    end
                    -- Console.PrintFloat(spawnTimer)
                end
            end
        end
    end
end

function initInUpdate()
	-- check and get the luascriptcomponent
    if ECS:hasComponent_LuaScriptCom(bossHeadID) then
        boss_env = ECS:getComponent_LuaScriptCom(bossHeadID)
    end

    if boss_env:GetEnvironment().bossHealth >= 75.0 then
        if ECS:hasComponent_AnimationCom(eEntID) == true then
            local animation = ECS:getComponent_AnimationCom(eEntID)
            animation.currentAnim = 1
        end
    end

    --if animPause == false then
    --    if ECS:hasComponent_AnimationCom(eEntID) == true then
    --        local animation = ECS:getComponent_AnimationCom(eEntID)
    --        animation.pause = true
    --        animPause = true
    --    end
    --end
end


function destory()
	-- destroy the lua entity itself
    ECS:addEntityToDestroyQueue(eEntID)
end

function spawnShowerRocket()
   -- spawn the homing rocket
   -- Console.WriteLine("Spawning Homing Rocket")
   local rigid1 = Vector3:new()
   local rigid2 = Vector3:new()
   rigid1.x = bRigidStatic.pos.x
   rigid2.x = bRigidStatic.pos.x
   rigid1.y = bRigidStatic.pos.y
   rigid2.y = bRigidStatic.pos.y
   rigid1.z = bRigidStatic.pos.z
   rigid2.z = bRigidStatic.pos.z

   rigid1.z = rigid1.z + 5.0
   rigid2.z = rigid2.z - 5.0

   bRigidStatic.pos.y = 18.0

   rocketBulletID = ECS:instantiatePrefab(rocketPrefabID, "BossRocket_Shower", bRigidStatic.pos,bRigidStatic.rot,false, false, false)
   -- rocketBulletID1 = ECS:instantiatePrefab(rocketPrefabID, "BossRocket_Shower1", rigid1, bRigidStatic.rot,false, false, false)
   -- rocketBulletID2 = ECS:instantiatePrefab(rocketPrefabID, "BossRocket_Shower2", rigid2, bRigidStatic.rot,false, false, false)

   ECS:addEntityTag(rocketBulletID, "Center")
   -- ECS:addEntityTag(rocketBulletID1, "Left")
   -- ECS:addEntityTag(rocketBulletID2, "Right")
   AudioSystem:Play3DSFX("PlayerRocketFire_SFX", rocketBulletID)

end

function UpdateWeaponRotation()
    if ECS:hasComponent_Transform(bossHeadID) then
        boss_Transform = ECS:getComponent_Transform(bossHeadID)
    end

    bossRotNew = PhysicsSystem:getGlobalPoseRotation(bossHeadID)

    PhysicsSystem:rotate_vector_by_angle(bLeftward, boss_Transform.rotate.y, bLeftwardPrime)
    bTransform.pos = boss_Transform.pos + bLeftwardPrime * bLeftwardOffset
    bTransform.rotate = boss_Transform.rotate

    bRigidStatic.pos = bTransform.pos
    bRigidStatic.rot = bossRotNew
end