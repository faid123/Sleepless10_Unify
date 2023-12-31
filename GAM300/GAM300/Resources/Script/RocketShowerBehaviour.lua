-- local variables

local playerID
local player_env

local prefabRoadTileID
local pExplosionPrefabID
local redSquareID
local eEntID
-- Component Variables
local bTransform
local bRigidDynamic
local bEmitter
local bMaterial
local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

-- taret position of where rocket will launch to
local playerTransform
local playerPos
-- the speed to change the angle
local angleChangeSpeed = 0.5
-- movement speed of the rocket towards the player
local rocketSpeed = 1.0

local hasFired      = false
local lifeTime      = 4.0

local initialPos    = Vector3:new()
local distance      = Vector3:new()
local distanceReached = false
local changeDir     = false
local isLeft        = false
local isRight       = false
local colorTime     = 0.3
-- global variables

-- Awake is called once and only once when script is loaded.
function Awake()
	pExplosionPrefabID = ECS:getPrefabID("Prefab_EnemyExplosion")
    prefabRoadTileID = ECS:getPrefabID("Prefab_RedSquare")
end

-- Start is called once and only once after Awake() is called.
function Start()
	-- get the player entity id
	playerID = ECS:getEntityID("Player")
    playerTransform = ECS:getComponent_Transform(playerID)
	player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()

    local entID = ScriptSystem:getLuaEntityID()
    initialPos = PhysicsSystem:getGlobalPosePosition(entID)
end

-- Updates the Game Object per game loop
function Update(dt)
	eEntID = ScriptSystem:getLuaEntityID()
	bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bEmitter = ECS:getComponent_Emitter(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    bMaterial = ECS:getComponent_Material(eEntID)
    playerPos = PhysicsSystem:getGlobalPosePosition(playerID)
	-- local bRotTemp = bRotNew
    -- bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)
    -- Console.WriteLine(bPos.y)

    if ECS:hasTagInEntity(eEntID, "Left") == true then
        isLeft = true
    elseif ECS:hasTagInEntity(eEntID, "Right") == true then
        isRight = true
    end

	fireRocket(dt)
	onCollide()
	-- handleBulletLifeTime(dt)
	updateParticleColor()
end


function destory()
	-- destroy the lua entity itself
    ECS:addEntityToDestroyQueue(eEntID)
end


function fireRocket(dt)
    PhysicsSystem:setGravityFlag(eEntID , true)
    distance = Vector3.Length(bPos - initialPos)
    local targetDir = Vector3.new()
    if distance <= 100.0 then 
        targetDir.y = 1.0
        PhysicsSystem:addForceToBody(eEntID, targetDir, 150.0, 0)
    elseif distance >= 100.0 and distance <= 110.0 and not distanceReached then
        distanceReached = true
        -- Console.WriteLine("DISTANCE REACHED TRUE")
    end
    
    if distanceReached == true and changeDir == false then
        local newPos = playerPos
        newPos.y = 50.0
        if isLeft then
            newPos.x = newPos.x + 5.0
            newPos.z = newPos.z - 5.0
        elseif isRight then
            newPos.x = newPos.x - 5.0
            newPos.z = newPos.z - 5.0
        end
        PhysicsSystem:setGlobalPose(eEntID, newPos, bRotNew)
        if ECS:hasComponent_Transform(eEntID) == true then
            local transform = ECS:getComponent_Transform(eEntID)
            transform.rotate.x = 180
        end
        -- need to instantiate the marker on the ground at playerPos
        newPos.y = 1.5
        redSquareID = ECS:instantiatePrefab(prefabRoadTileID, "RedSquare", newPos, bRotNew,false, false, false)
        -- PhysicsSystem:setGlobalPose(redSquareID, newPos, bRotNew)
        -- Console.WriteLine("SPAWNED RED SQUARE")
        local trans = ECS:getComponent_Transform(redSquareID)
        trans.pos = newPos
        changeDir = true
    end

    if changeDir then
        targetDir.y = -1.0
        PhysicsSystem:addForceToBody(eEntID, targetDir, 200.0, 0)
        local sqMaterial = ECS:getComponent_Material(redSquareID)
        if colorTime  > 0.0 then
            colorTime = colorTime - dt
        end
        
        if colorTime <= 0.0 then
            sqMaterial.colour.x = 1.0
            sqMaterial.colour.y = Random.Float()
            sqMaterial.colour.z = Random.Float()
            colorTime = 0.3
        end

        distanceReached = false
    end

end

function onCollide()
    if bRigidDynamic.getHit then
        local explosionID = ECS:instantiatePrefab(pExplosionPrefabID, "Boss_Explosion", bRigidDynamic.rigidPos,bRigidDynamic.rigidRot,false, false, false)
        -- ECS:addEntityTag(explosionID, "Boss_Explosion")
        AudioSystem:Play3DSFX("PlayerRocketHit_SFX", eEntID)
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
        ECS:addEntityToDestroyQueue(redSquareID)
        -- Console.WriteLine("ROCKET SHOWER DESTROYED")
        changeDir = false
    end
end

function handleBulletLifeTime(dt)
    if lifeTime > 0.0 then
        lifeTime = lifeTime - dt
    end

    if lifeTime <= 0.0 then
        -- Console.WriteLine("Rocket Destroyed")
        hasFired = false
        lifeTime = 4.0
        -- ECS:addEntityToDestroyQueue(eEntID)
        -- PhysicsSystem:LuaRemoveActor(eEntID)
    end
end

function updateParticleColor()
    bEmitter.colorBegin.x = 1.0
    bEmitter.colorBegin.y = Random.Float()
    bEmitter.colorBegin.z = Random.Float()

    bEmitter.sizeBegin = 0.7
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = 10.0
	bEmitter.velocityVariation.y = 10.0
	bEmitter.velocityVariation.z = 10.0
end