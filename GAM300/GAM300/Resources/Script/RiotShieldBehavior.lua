-- RiotShield
local rsEntID
local rsTransform
local rsRigidStatic
local rsMaterial
local rsRotNew          = Quat:new()
local forward			= Vector3:new(0.0, 0.0, -1.0)
local forwardPrime      = Vector3:new()
local forwardOffset		= 2.25

-- Enemy
local sEnemyID           = {}
local sEnemyScript
local sEnemyTransform

function Awake()

end

function Start()
end

function Update(dt)
    rsEntID = ScriptSystem:getLuaEntityID()
    rsTransform = ECS:getComponent_Transform(rsEntID)
    rsRigidStatic = ECS:getComponent_RigidStatic(rsEntID)
    rsMaterial = ECS:getComponent_Material(rsEntID)

    sEnemyID = ECS:getEntityTagged("Shield")
    for i = 1, #sEnemyID do
        sEnemyScript = ECS:getComponent_LuaScriptCom(sEnemyID[i])
        if sEnemyScript:GetEnvironment().RiotShieldID == rsEntID then
            sEnemyTransform = ECS:getComponent_Transform(sEnemyID[i])
            rsRotNew = PhysicsSystem:getGlobalPoseRotation(sEnemyID[i])

            PhysicsSystem:rotate_vector_by_quaternion(forward, rsRotNew, forwardPrime)
            rsTransform.pos = sEnemyTransform.pos + forwardPrime * forwardOffset
            rsTransform.rotate = sEnemyTransform.rotate

            rsRigidStatic.pos = rsTransform.pos
            rsRigidStatic.rot = rsRotNew
            
            PhysicsSystem:setGlobalPose(rsEntID, rsRigidStatic.pos, rsRigidStatic.rot)

            break
        end
    end
end
