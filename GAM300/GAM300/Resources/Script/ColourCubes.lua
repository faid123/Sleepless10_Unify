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
local flyDirection      = Vector3:new()
local randomForce       = 30.0
local cubeSpeed         = 1000.0

local canpickup = false
local pickuptimer = 0.5
local spawned = false
local despawnTimer

local distance


-- just increase fire rate
-- power bar must press e when full
-- max power: 10

-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("ColorCube script loaded")
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
    if canpickup then
        distance = Vector3.Distance(eTransform.pos, pTransform.pos)
        if distance <= 10 then
            ------Console.WriteLine("Touching player")

            local isLaser = ECS:hasTagInEntity(eEntID, "lAmmo")
            local isRocket = ECS:hasTagInEntity(eEntID, "rAmmo")
            local isPower = ECS:hasTagInEntity(eEntID, "pCube")

            if isLaser then
                --Console.WriteLine("Laser ammo collected")
                ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().lAmmo = 
                    ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().lAmmo + 1
                if ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().lAmmo >= 20 then
                    ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().lAmmo = 20
                end
            elseif isRocket then
                --Console.WriteLine("Rocket ammo collected")
                ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().rAmmo = 
                    ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().rAmmo + 1
                if ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().rAmmo >= 10 then
                    ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().rAmmo = 10
                end
            elseif isPower then
                --Console.WriteLine("Power collected")
                if ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().canCollectCube then
                    ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().currPower = 
                        ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().currPower + 3.755
                    if ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().currPower >= 75.1 then
                        ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().currPower = 75.1
                    end
                end
            else
            end



            AudioSystem:PlaySFX("PlayerCollectCube_SFX", pEntID)
            ECS:addEntityToDestroyQueue(eEntID)
            PhysicsSystem:LuaRemoveActor(eEntID)
        end
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
    if distance <= 25 then
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

function pickup(dt)
    pickuptimer = pickuptimer - dt
    if pickuptimer <= 0 then
        canpickup = true
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
        eMaterial.colour.x = Random.Float() * 30
        eMaterial.colour.y = 100
        eMaterial.colour.z = Random.Float() * 30
    end

end