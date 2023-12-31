--stuff goes here

-- Component Variables


--Player
local pTransform
local pEntID

--Cubes
local eTransform
local eRigidDynamic
local eEntID

local cubeRot   = Vector3:new()
local newPos    = Vector3:new()
local newRot    = Quat:new()

local randomDirection   = Vector3:new()
local luckyNumber       = 2
local flyDirection      = Vector3:new()
local randomForce       = 1.0
local cubeSpeed         = 1000.0

local spawned = false
local despawnTimer
local canpickup = false
local pickuptimer = 0.5
local distance


-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    
    -- ----Console.WriteLine("Ammo script loaded")
    ammo = 1

    
    
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    pEntID = ECS:getEntityID("Player")

    initInUpdate()

    newPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    newRot = PhysicsSystem:getGlobalPoseRotation(eEntID)

    checkDistance()
    --cubeMoveToPlayer()
    cubeTouchPlayer()
    --updateGravity()

    updateRotation()

    if spawned == false then
        despawnTimer = 10.0
        randomDirectionSpawn()
        randomColourSpawn()
        spawned = true
    end

    if spawned == true then
        despawn(dt)
        pickup(dt)
    end

end

-- If there is collision detected, this physics component runs this function
function onTriggerEnter2D(gameObject, dt, manifold)
 
end

-- If there is collision detected, this physics component runs this function
function onTriggerStay2D(gameObject, dt, manifold)

end

function initInUpdate()

    if ECS:hasComponent_RigidDynamic(eEntID) and ECS:hasComponent_Transform(eEntID) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
        eTransform = ECS:getComponent_Transform(eEntID)
    end

    if ECS:hasComponent_Transform(pEntID) then
        pTransform = ECS:getComponent_Transform(pEntID)
    end

    

end


function updateGravity()
    if mContactReportCallback.cubeOnFloor then
        ------Console.WriteLine("cube on floor")
        PhysicsSystem:setGravityFlag(eEntID, true)
    else
        PhysicsSystem:setGravityFlag(eEntID, false)
    end
end


function updateRotation()

    cubeRot = PhysicsSystem:convertQuatToEuler(newRot)

    eTransform.rotate.x = cubeRot.x
    eTransform.rotate.y = cubeRot.y
    eTransform.rotate.z = cubeRot.z
    

end

function cubeTouchPlayer()
    distance = Vector3.Distance(eTransform.pos, pTransform.pos)
    if eRigidDynamic.getHit  or distance <= 3 then
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end

end

function pickup(dt)
    pickuptimer = pickuptimer - dt
    if pickuptimer <= 0 then
        canpickup = true
    end

end

function cubeMoveToPlayer()
    if canpickup then
        flyDirection = pTransform.pos - eTransform.pos
        PhysicsSystem:addForceToBody(eEntID, flyDirection, cubeSpeed, 0)
        PhysicsSystem:setGravityFlag(eEntID, true)
    end
end

function checkDistance()

    distance = Vector3.Distance(eTransform.pos, pTransform.pos)
    ----Console.PrintFloat(distance)
    if distance <= 14 then
        cubeMoveToPlayer()
    end
end


function despawn(dt)
    
    despawnTimer = despawnTimer - dt
    ----Console.PrintFloat(despawnTimer)
    if despawnTimer <= 0 then
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
    
end


function randomDirectionSpawn()

     if ECS:hasComponent_RigidDynamic(eEntID) then
        randomDirection.x = Random.randInRange(-100.0, 100.0)

        randomDirection.y = Random.randInRange(300.0, 500.0)
        randomDirection.z = Random.randInRange(-100.0, 100.0)

        ----Console.PrintFloat(randomDirection.x)
        ----Console.PrintFloat(randomDirection.y)
        ----Console.PrintFloat(randomDirection.z)

        PhysicsSystem:addForceToBody(eEntID, randomDirection, randomForce, 0)
    end
end

function randomColourSpawn()

    if ECS:hasComponent_Material(eEntID) then
        eMaterial = ECS:getComponent_Material(eEntID)
       -- eMaterial.colour.x = Random.Float() * 30
       
       eMaterial.colour.y = 30
       -- eMaterial.colour.z = Random.Float() * 30
    end

end