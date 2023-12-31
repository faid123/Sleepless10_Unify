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


-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Laser Behavior script loaded")
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
	
    --shrinkLaser()
    updateCCdelay(dt)
    randomColor()
end

function shrinkLaser()
    if bTransform.scale.x > 0.0 and bTransform.scale.y > 0.0 then
        bTransform.scale.x = bTransform.scale.x - 0.01
        bTransform.scale.y = bTransform.scale.y - 0.01
        bRigidDynamic.rigidScale.x = bRigidDynamic.rigidScale.x - 0.02
        bRigidDynamic.rigidScale.y = bRigidDynamic.rigidScale.y - 0.02
        if (bRigidDynamic.rigidScale.x < 0.01) then
            bRigidDynamic.rigidScale.x = 0.01
        end
        if (bRigidDynamic.rigidScale.y < 0.01) then
            bRigidDynamic.rigidScale.y = 0.01
        end
    elseif bTransform.scale.x <= 0.0 and bTransform.scale.y <= 0.0 then
        mContactReportCallback.laserHitEnemy = false
        mContactReportCallback.laserHitBuilding = false
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
end

function randomColor()
    if CCdelay <= 0.0 then
        --bMaterial._saturation.x = Random.Float() * 360.0
		bMaterial.colour.x = Random.Float() * 7
		bMaterial.colour.y = Random.Float() * 7
		bMaterial.colour.z = Random.Float() * 7
        CCdelay = 0.05
    end
end

function adjustRigidbody()
    if not adjusted then
        PhysicsSystem:rotate_vector_by_quaternion(bForward, bRotNew, bForwardPrime)
        bForwardPrime = bForwardPrime * 30
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