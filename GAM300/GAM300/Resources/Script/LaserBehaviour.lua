--stuff goes here

-- Component Variables
local bTransform
local bRigidDynamic
local bMaterial
local eEntID
local adjusted      = false
local CCdelay       = 0.0
local bulletForce   = 2000.0
local didOnce       = false
local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()

-- Player environment
local player_env
local playerID

-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Laser Behavior script loaded")
    playerID = ECS:getEntityID("Player")
    player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
end
                                                                                                                                                                                           
-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bMaterial = ECS:getComponent_Material(eEntID)

    if not didOnce then
        bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
        bPosNew = PhysicsSystem:getGlobalPosePosition(eEntID)
        bTransform.pos.x = bPosNew.x
        bTransform.pos.y = bPosNew.y
        bTransform.pos.z = bPosNew.z
        adjustRigidbody()
        local bRotTemp = bRotNew
        bRotTemp.x = bRotTemp.x * -1.0
        bRotTemp.z = bRotTemp.z * -1.0
        bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)
        bRigidDynamic.rigidRot = bRotTemp
        PhysicsSystem:setGravityFlag(eEntID, true)
        didOnce = true
    end

	onCollide()
    shrinkLaser(dt)
    updateCCdelay(dt)
    randomColor()
end

function shrinkLaser(dt)
    if bTransform.scale.x > 0.0 and bTransform.scale.y > 0.0 then
        if not player_env.laserUlt then
            bTransform.scale.x = bTransform.scale.x - dt * 1.2
            bTransform.scale.y = bTransform.scale.y - dt * 1.2
        else
            bTransform.scale.x = bTransform.scale.x - dt * 2.0
            bTransform.scale.y = bTransform.scale.y - dt * 2.0
        end
        bRigidDynamic.rigidScale.x = bRigidDynamic.rigidScale.x - dt * 3.0
        bRigidDynamic.rigidScale.y = bRigidDynamic.rigidScale.y - dt * 3.0
        if (bRigidDynamic.rigidScale.x < 0.01) then
            bRigidDynamic.rigidScale.x = 0.01
        end
        if (bRigidDynamic.rigidScale.y < 0.01) then
            bRigidDynamic.rigidScale.y = 0.01
        end
    elseif bTransform.scale.x <= 0.0 and bTransform.scale.y <= 0.0 then
        mContactReportCallback.laserHitEnemy = false
        mContactReportCallback.laserHitBuilding = false
        mContactReportCallback.laserHitBarrier = false
        mContactReportCallback.laserHitCage = false
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
end

function randomColor()
    if CCdelay <= 0.0 then
        bMaterial._saturation.x = Random.Float() * 360.0
        CCdelay = 0.05
    end
end

function adjustRigidbody()
    if not adjusted then
        PhysicsSystem:rotate_vector_by_quaternion(bForward, bRotNew, bForwardPrime)
        bForwardPrime = bForwardPrime * bTransform.scale.z
        bRigidDynamic.rigidPos = bRigidDynamic.rigidPos + bForwardPrime
        PhysicsSystem:setGlobalPose(eEntID, bRigidDynamic.rigidPos, bRigidDynamic.rigidRot)
        adjusted = true
    end
end

function updateCCdelay(dt)
    if CCdelay > 0.0 then
        CCdelay = CCdelay - dt
    end
end

function onCollide()
    if bRigidDynamic.getHit and (mContactReportCallback.laserHitBarrier or 
        mContactReportCallback.laserHitCage) then
        local distance = Vector3.Length(bTransform.pos - player_env.TargetPoint)

	    if distance < 0.0 then 
		    distance = distance * -1.0
	    end

        bTransform.scale.z = distance / 2.0

        bRigidDynamic.rigidScale.z = bTransform.scale.z * 2.0

        PhysicsSystem:rotate_vector_by_quaternion(bForward, bRotNew, bForwardPrime)
        bForwardPrime = bForwardPrime * bTransform.scale.z
        bRigidDynamic.rigidPos = bTransform.pos + bForwardPrime

        PhysicsSystem:setGlobalPose(eEntID, bRigidDynamic.rigidPos, bRigidDynamic.rigidRot)
    end
end