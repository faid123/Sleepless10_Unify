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
local bRightwardPrime = Vector3:new()
local bRightward      = Vector3:new(1.0, 0.0, 0.0)
local bRightwardOffset = 10.0

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

local hasFired      = false
local spawnTimer      = 5.0
local playAnimOnce  = false
local animTimerPause= -0.5 --5.0
local animTimerPlay = 3.0

-- taret position of where rocket will launch to
local playerTransform

local tagList

local pRigidDynamic
local ent2Pos
local eRigidDynamic
local ent1Pos
local targetRotation = 0.0
local prevTargetRotation = 0.0
local eSetEnemyRot  = Vector3:new(0.0, 0.0, 0.0)

-- global variables

-- Awake is called once and only once when script is loaded.
function Awake()
	pExplosionPrefabID = ECS:getPrefabID("Prefab_Explosion")
    rocketPrefabID = ECS:getPrefabID("Prefab_BossRocket")
end

-- Start is called once and only once after Awake() is called.
function Start()
	-- get the player entity id
	playerID = ECS:getEntityID("Player")
	player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
    -- bossHeadID = ECS:getEntityID("Boss_Head_LVL2")

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
    -- bossHeadID = ECS:getEntityID("Boss_Head_LVL2")
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

    if boss_env:GetEnvironment().bossHealth > 0 then
        if animTimerPause == 0 then
            animTimerPlay = animTimerPlay -dt
        end
        if animTimerPlay == 0 then 
            animTimerPause = animTimerPause - dt
        end
        
        lookTowards(eEntID, playerID)
        if animTimerPause < 0.0 then
            if ECS:hasComponent_AnimationCom(eEntID) then
                local anim = ECS:getComponent_AnimationCom(eEntID)
                --anim.pause = false
                anim.speed = 30
                anim.currentAnim = 0
                animTimerPause = 0
                animTimerPlay = 1.20
                spawnHomingRocket()
            end
        end

        if animTimerPlay < 0.0 then
            if ECS:hasComponent_AnimationCom(eEntID) then
                local anim = ECS:getComponent_AnimationCom(eEntID)
                --anim.pause = true
                anim.currentAnim = 1
                animTimerPlay = 0
                animTimerPause = 3.0
            end
        end
        
        -- spawn rocket every x seconds
        if spawnTimer > 0.0 then
            spawnTimer = spawnTimer - dt
            if not hasFired then
            --  spawnHomingRocket()
                
                hasFired = true
            end
        end

        if spawnTimer <= 0.0 then
            hasFired = false
            spawnTimer = 5.0
        end
        -- Console.PrintFloat(spawnTimer)
    end
end

function initInUpdate()
	-- check and get the luascriptcomponent
    if ECS:hasComponent_LuaScriptCom(bossHeadID) then
        boss_env = ECS:getComponent_LuaScriptCom(bossHeadID)
    end
end


function destory()
	-- destroy the lua entity itself
    ECS:addEntityToDestroyQueue(eEntID)
end

function spawnHomingRocket()
   -- spawn the homing rocket
   -- Console.WriteLine("Spawning Homing Rocket")
   local rigid1 = Vector3:new()
   local rigid2 = Vector3:new()
   local rigid3 = bRigidStatic.pos
   rigid1.x = rigid3.x
   rigid2.x = rigid3.x
   rigid1.y = 18.858
   rigid2.y = 18.858
   rigid3.y = 18.858
   rigid1.z = rigid3.z
   rigid2.z = rigid3.z

   rigid1.z = rigid1.z + 10.0
   rigid2.z = rigid2.z - 10.0

   -- lookTowards(eEntID, playerID)

   rocketBulletID = ECS:instantiatePrefab(rocketPrefabID, "BossRocket", rigid3,eSetEnemyRot,false, false, false)
   rocketBulletID1 = ECS:instantiatePrefab(rocketPrefabID, "BossRocket1", rigid1, eSetEnemyRot,false, false, false)
   rocketBulletID2 = ECS:instantiatePrefab(rocketPrefabID, "BossRocket2", rigid2, eSetEnemyRot,false, false, false)
   AudioSystem:Play3DSFX("PlayerRocketFire_SFX", rocketBulletID)

end

function UpdateWeaponRotation()
    if ECS:hasComponent_Transform(bossHeadID) then
        boss_Transform = ECS:getComponent_Transform(bossHeadID)
    end

    bossRotNew = PhysicsSystem:getGlobalPoseRotation(bossHeadID)

    PhysicsSystem:rotate_vector_by_angle(bRightward, boss_Transform.rotate.y, bRightwardPrime)
    bTransform.pos = boss_Transform.pos + bRightwardPrime * bRightwardOffset
    bTransform.rotate = boss_Transform.rotate

    bRigidStatic.pos = bTransform.pos
    bRigidStatic.rot = bossRotNew
end


-- Logic for atan2
function atan2(y, x)
    if x > 0.0 then
        return Trigo.atan(y/x)
    end

    if x < 0.0 and y >= 0.0 then
        return Trigo.atan(y/x) + Trigo.pi()
    end

    if x < 0.0 and y < 0.0 then
        return Trigo.atan(y/x) - Trigo.pi()
    end

    if x == 0.0 and y > 0.0 then
        return Trigo.pi() / 2
    end

    if x == 0.0 and y < 0.0 then
        return -Trigo.pi() / 2
    end
    
    if x == 0.0 and y == 0.0 then
        return 0.0
    end

    return 0.0
end

-- Convert radian to pi
function radianTopi(x)
    return 180/Trigo.pi()*x
end

function lookTowards(EntID1, EntID2)
    -- Getting player id and their position
    if ECS:hasComponent_RigidDynamic(EntID2) then
        pRigidDynamic = ECS:getComponent_RigidDynamic(EntID2)
        ent2Pos = pRigidDynamic.rigidPos
    end
    
    -- Adding force towards the player direction
    if ECS:hasComponent_RigidStatic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidStatic(EntID1)
        ent1Pos = eRigidDynamic.pos
        
        targetRotation = radianTopi(atan2(ent1Pos.z - ent2Pos.z, ent1Pos.x - ent2Pos.x))
        --Trigo.floatToint(targetRotation)

        if targetRotation ~= prevTargetRotation then
            local rotationDifference = eSetEnemyRot.y + targetRotation -90

            --rotationDifference = rotationDifference % 360.0
            Trigo.floatToint(rotationDifference)
            
            if rotationDifference > 180.0 then
                rotationDifference = rotationDifference - 360.0
            end

            if rotationDifference < -180.0 then
                rotationDifference = rotationDifference + 360.0
            end
            
            if rotationDifference < 0  then
                eSetEnemyRot.y = eSetEnemyRot.y + 2.0
            elseif rotationDifference > 0 then
                eSetEnemyRot.y = eSetEnemyRot.y - 2.0
            end
        end
        prevTargetRotation = targetRotation
    end
end