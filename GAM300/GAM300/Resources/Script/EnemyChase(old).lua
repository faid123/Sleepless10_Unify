--stuff goes here

-- Component Variables
local eRigidbody
local eTransform
local eEnemyEntID
local eEnemyPos
local eEnemyRot
local ePlayerEntID
local ePlayerPos
local targetRotation
local rotationDifference
local e1Pos
local e2Pos
local eMaterial
local eHealth
local Unified

local minDis = 0.0
local moveSpeed = 0.005
local lifeTime = 1.0


-- Awake is called once and only once when script is loaded.
function Awake()
    eHealth = 15
    eFlashTimer = 1
    Unified = false

end

-- Start is called once and only once after Awake() is called.
function Start()
    -- ----Console.WriteLine("EnemyChase script loaded")
   
end

-- Updates the Game Object per game loop
function Update(dt)
    eEnemyEntID = ScriptSystem:getLuaEntityID()
    ------Console.WriteLine("EnemyID: ")
    ----Console.PrintEntityID(eEnemyEntID)

    ePlayerEntID = ECS:getEntityID("Player")
    ------Console.WriteLine("PlayerID: ")
    ----Console.PrintEntityID(ePlayerEntID)
    

    -- Move the obj by incrementing transform component
    if ECS:hasComponent_Transform(ePlayerEntID) then
        eTransform = ECS:getComponent_Transform(ePlayerEntID)
        ePlayerPos = eTransform.pos
    end

    -- Move the obj by incrementing transform component
    if ECS:hasComponent_Transform(eEnemyEntID) then
        eTransform = ECS:getComponent_Transform(eEnemyEntID)
        eEnemyPos = eTransform.pos
        eEnemyRot = eTransform.rotate
        if positiveOnly(Vector3.Dot(ePlayerPos, eEnemyPos)) > minDis then
            eTransform.pos.x = eEnemyPos.x + (ePlayerPos.x - eEnemyPos.x) * moveSpeed
            eTransform.pos.z = eEnemyPos.z + (ePlayerPos.z - eEnemyPos.z) * moveSpeed
        end
    end
    
    -- Enemy looking at Player
    lookTowards(eEnemyEntID, ePlayerEntID)

    ----Console.PrintFloat(positiveOnly(Vector3.Dot(ePlayerPos, eEnemyPos)))
    ----Console.PrintFloat(eEnemyPos.x)
    ----Console.PrintFloat(eEnemyPos.y)
    ----Console.PrintFloat(eEnemyRot.y)
    ----Console.PrintFloat(radianTopi(2))

    getHit()
    if eHealth <= 0 then
        Unify(dt)
    end

    if Inputs.get_Key_Triggered(InputKey.Q_KEY) then
        -- ----Console.WriteLine("Enemy Destroyed")
        ECS:addEntityToDestroyQueue(eEnemyEntID)
    end
end

-- If there is collision detected, this physics component runs this function
function onTriggerEnter2D(gameObject, dt, manifold)
 
end

-- If there is collision detected, this physics component runs this function
function onTriggerStay2D(gameObject, dt, manifold)

end

function positiveOnly(x)
    if x < 0.0 then
        x = -x
    end
    return x
end

function lookTowards(EntID1, EntID2)
    if ECS:hasComponent_Transform(EntID1) then
        e1Transform = ECS:getComponent_Transform(EntID1)
        e1Pos = e1Transform.pos
    end

    if ECS:hasComponent_Transform(EntID2) then
        e2Transform = ECS:getComponent_Transform(EntID2)
        e2Pos = e2Transform.pos
    end
    
    targetRotation = radianTopi(atan2(e1Pos.z - e2Pos.z, e1Pos.x - e2Pos.x))

    ----Console.PrintFloat(e2Pos.x - e1Pos.x)
    ----Console.PrintFloat(atan2(e2Pos.z - e1Pos.z, e2Pos.x - e1Pos.x))
    ----Console.PrintFloat(targetRotation)

    if ECS:hasComponent_Transform(EntID1) then
        e1Transform = ECS:getComponent_Transform(EntID1)
        ----Console.PrintFloat(e1Transform.rotate.y)
        ----Console.PrintFloat(targetRotation)
        Trigo.floatToint(targetRotation)
        rotationDifference = e1Transform.rotate.y - targetRotation - 270
        rotationDifference = rotationDifference % 360
        Trigo.floatToint(rotationDifference)
        ----Console.PrintFloat(rotationDifference)

        if rotationDifference > 180.0 then
            rotationDifference = rotationDifference - 360.0
        end

        if rotationDifference < -180.0 then
            rotationDifference = rotationDifference + 360.0
        end

        if rotationDifference < 0  then
            --e1Transform.rotate.y = e1Transform.rotate.y + 1.0
        elseif rotationDifference > 0 then
            --e1Transform.rotate.y = e1Transform.rotate.y - 1.0
        end
    end
end

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

function radianTopi(x)
    return 180/Trigo.pi()*x
end

function getHit()

    if mContactReportCallback.bulletHitBuilding then
        if Unified == false then
            -- ----Console.WriteLine("HIT")
            eHealth = eHealth - 5
        end
    end
end


function Unify(dt)
    Unified = true
    handleEnemyLifeTime(dt)
end

function handleEnemyLifeTime(dt)
    if lifeTime > 0.0 then
        lifeTime = lifeTime - dt        
    end

    if lifeTime <= 0.0 then
        -- ----Console.WriteLine("Enemy Destroyed")
        ECS:addEntityToDestroyQueue(eEnemyEntID)
    end
end