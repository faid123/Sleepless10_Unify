-- Target
local targetPrefabID

local grp1_TargetID					= {}
local grp2_TargetID					= {}
local grp3_TargetID					= {}
local grp4_TargetID					= {}

local grp1_destroyTargetsCountdown		= 0.0
local grp2_destroyTargetsCountdown		= 0.0
local grp3_destroyTargetsCountdown		= 0.0
local grp4_destroyTargetsCountdown		= 0.0

local grp1_deactivateTargetsCountdown	= 0.0
local grp2_deactivateTargetsCountdown	= 0.0
local grp3_deactivateTargetsCountdown	= 0.0
local grp4_deactivateTargetsCountdown	= 0.0

local grp1_deactivateTargets			= false
local grp2_deactivateTargets			= false
local grp3_deactivateTargets			= false
local grp4_deactivateTargets			= false

local grp1_destroyTargets				= false
local grp2_destroyTargets				= false
local grp3_destroyTargets				= false
local grp4_destroyTargets				= false

local grp1_destroyed					= false
local grp2_destroyed					= false
local grp3_destroyed					= false
local grp4_destroyed					= false

local targetsSpawned					= false


-- Barrier
local barrierID
local barrier_Transform
local barrier_RigidStatic
local powerOffCountdown = 0.0
local BarrierDown		= false
local nameSet			= false

-- General
local sRot				= Quat:new(0,0,0,1)
local sRot2				= Quat:new(0,0.707,0,0.707)
local forward			= Vector3:new(0.0, 0.0, -1.0)
local backward			= Vector3:new(0.0, 0.0, 1.0)
local leftward			= Vector3:new(-1.0, 0.0, 0.0)
local rightward			= Vector3:new(1.0, 0.0, 0.0)
local forwardPrime		= Vector3:new()
local backwardPrime		= Vector3:new()
local leftwardPrime		= Vector3:new()
local rightwardPrime	= Vector3:new()

function Awake()
end

function Start()
	targetPrefabID = ECS:getPrefabID("Prefab_RocketTarget")
end

function Update(dt)
	barrierID = ScriptSystem:getLuaEntityID()
    barrierRotateVectorByQuaternion()
	initInUpdate()
	setName()
	spawnAllTargets()

	if not BarrierDown then
		checkTargetsActivated()

		if grp1_deactivateTargets or grp2_deactivateTargets
			or grp3_deactivateTargets or grp4_deactivateTargets then
			handleDeactivateTargetsCountdown(dt)
			DeactivatingTargets()
		end

		if grp1_destroyTargets or grp2_destroyTargets
			or grp3_destroyTargets or grp4_destroyTargets then
			handleDestroyTargetsCountdown(dt)
			DestroyTargets()
		end
	elseif BarrierDown then
		handlePowerOffCountdown(dt)
		AudioSystem:UnmuteSound("SFX_EnemyLaser")
		if powerOffCountdown <= 0.0 then
			powerOffBarrier()
		end
	end
end

function barrierRotateVectorByQuaternion()
    globalRot = PhysicsSystem:getGlobalPoseRotation(barrierID)

    PhysicsSystem:rotate_vector_by_quaternion(forward, globalRot, forwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(backward, globalRot, backwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(leftward, globalRot, leftwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(rightward, globalRot, rightwardPrime)
end

function initInUpdate()
	if ECS:hasComponent_Transform(barrierID) then
		barrier_Transform = ECS:getComponent_Transform(barrierID)
	end

	if ECS:hasComponent_RigidStatic(barrierID) then
		barrier_RigidStatic = ECS:getComponent_RigidStatic(barrierID)
	end
end

function setName()
	if not nameSet then
		barrier_RigidStatic:setActorName("Barrier")
		nameSet = true
	end
end

function spawnTarget(foreOffset, backOffset, leftOffset, rightOffset, upOffset, downOffset, quatRot, GroupNumTag)
	local targetSpawnPos = barrier_Transform.pos
	targetSpawnPos = targetSpawnPos + forwardPrime * foreOffset
	targetSpawnPos = targetSpawnPos + backwardPrime * backOffset
	targetSpawnPos = targetSpawnPos + leftwardPrime * leftOffset
	targetSpawnPos = targetSpawnPos + rightwardPrime * rightOffset
	targetSpawnPos.y = targetSpawnPos.y + upOffset
	targetSpawnPos.y = targetSpawnPos.y - downOffset
	local _targetID = ECS:instantiatePrefab(targetPrefabID, "Target", targetSpawnPos, quatRot, false, false, false)
	ECS:addEntityTag(_targetID,"BarrierTarget")
	ECS:addEntityTag(_targetID, GroupNumTag)
	if ECS:hasComponent_Transform(_targetID) and ECS:hasComponent_RigidStatic(_targetID) then
		local targetTransform = ECS:getComponent_Transform(_targetID)
		targetTransform.pos = targetSpawnPos
		targetTransform.rotate = PhysicsSystem:convertQuatToEuler(quatRot)
		local rigid = ECS:getComponent_RigidStatic(_targetID)
		rigid.pos = targetSpawnPos
		rigid.rot = quatRot
	end
end

function spawnAllTargets()
	if not targetsSpawned then
		-- GROUP 1
		spawnTarget(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, sRot2, "Group1B_Target")
		spawnTarget(3.0, 0.0, 20.0, 0.0, 0.0, 0.0, sRot2, "Group1B_Target")
		spawnTarget(0.0, 3.0, 20.0, 0.0, 0.0, 0.0, sRot2, "Group1B_Target")
		spawnTarget(0.0, 0.0, 20.0, 0.0, 3.0, 0.0, sRot2, "Group1B_Target")
		spawnTarget(1.5, 0.0, 20.0, 0.0, 0.0, 3.0, sRot2, "Group1B_Target")
		spawnTarget(0.0, 1.5, 20.0, 0.0, 0.0, 3.0, sRot2, "Group1B_Target")

		-- GROUP 2
		spawnTarget(0.0, 0.0, 0.0, 20.0, 0.0, 0.0, sRot2, "Group2B_Target")
		spawnTarget(3.0, 0.0, 0.0, 20.0, 0.0, 0.0, sRot2, "Group2B_Target")
		spawnTarget(0.0, 3.0, 0.0, 20.0, 0.0, 0.0, sRot2, "Group2B_Target")
		spawnTarget(0.0, 0.0, 0.0, 20.0, 3.0, 0.0, sRot2, "Group2B_Target")
		spawnTarget(1.5, 0.0, 0.0, 20.0, 0.0, 3.0, sRot2, "Group2B_Target")
		spawnTarget(0.0, 1.5, 0.0, 20.0, 0.0, 3.0, sRot2, "Group2B_Target")

		-- GROUP 3
		spawnTarget(0.0, 20.0, 0.0, 0.0, 0.0, 0.0, sRot, "Group3B_Target")
		spawnTarget(0.0, 20.0, 3.0, 0.0, 0.0, 0.0, sRot, "Group3B_Target")
		spawnTarget(0.0, 20.0, 0.0, 3.0, 0.0, 0.0, sRot, "Group3B_Target")
		spawnTarget(0.0, 20.0, 0.0, 0.0, 3.0, 0.0, sRot, "Group3B_Target")
		spawnTarget(0.0, 20.0, 1.5, 0.0, 0.0, 3.0, sRot, "Group3B_Target")
		spawnTarget(0.0, 20.0, 0.0, 1.5, 0.0, 3.0, sRot, "Group3B_Target")

		-- GROUP 4
		spawnTarget(20.0, 0.0, 0.0, 0.0, 0.0, 0.0, sRot, "Group4B_Target")
		spawnTarget(20.0, 0.0, 3.0, 0.0, 0.0, 0.0, sRot, "Group4B_Target")
		spawnTarget(20.0, 0.0, 0.0, 3.0, 0.0, 0.0, sRot, "Group4B_Target")
		spawnTarget(20.0, 0.0, 0.0, 0.0, 3.0, 0.0, sRot, "Group4B_Target")
		spawnTarget(20.0, 0.0, 1.5, 0.0, 0.0, 3.0, sRot, "Group4B_Target")
		spawnTarget(20.0, 0.0, 0.0, 1.5, 0.0, 3.0, sRot, "Group4B_Target")

		grp1_TargetID = ECS:getEntityTagged("Group1B_Target")
		grp2_TargetID = ECS:getEntityTagged("Group2B_Target")
		grp3_TargetID = ECS:getEntityTagged("Group3B_Target")
		grp4_TargetID = ECS:getEntityTagged("Group4B_Target")

		targetsSpawned = true
	end
end

function checkTargetsActivated()

	if grp1_destroyed and grp2_destroyed and grp3_destroyed and grp4_destroyed then
		powerOffCountdown = 0.5
		BarrierDown = true
	end

	local grp1_activationCount = 0
	local grp2_activationCount = 0
	local grp3_activationCount = 0
	local grp4_activationCount = 0

	--GROUP 1
	if not grp1_destroyTargets then
		for i = 1, #grp1_TargetID do
			if ECS:hasTagInEntity(grp1_TargetID[i], "Activated") then
				grp1_activationCount = grp1_activationCount + 1
			end
		end

		if grp1_activationCount == #grp1_TargetID then
			grp1_destroyTargetsCountdown = 0.5
			grp1_destroyTargets = true
		elseif not (grp1_activationCount == #grp1_TargetID) and not grp1_deactivateTargets then 
			grp1_deactivateTargetsCountdown = 1.0
			grp1_deactivateTargets = true
		end
	end

	--GROUP 2
	if not grp2_destroyTargets then
		for i = 1, #grp2_TargetID do
			if ECS:hasTagInEntity(grp2_TargetID[i], "Activated") then
				grp2_activationCount = grp2_activationCount + 1
			end
		end

		if grp2_activationCount == #grp2_TargetID then
			grp2_destroyTargetsCountdown = 0.5
			grp2_destroyTargets = true
		elseif not (grp2_activationCount == #grp2_TargetID) and not grp2_deactivateTargets then 
			grp2_deactivateTargetsCountdown = 1.0
			grp2_deactivateTargets = true
		end
	end

	--GROUP 3
	if not grp3_destroyTargets then
		for i = 1, #grp3_TargetID do
			if ECS:hasTagInEntity(grp3_TargetID[i], "Activated") then
				grp3_activationCount = grp3_activationCount + 1
			end
		end

		if grp3_activationCount == #grp3_TargetID then
			grp3_destroyTargetsCountdown = 0.5
			grp3_destroyTargets = true
		elseif not (grp3_activationCount == #grp3_TargetID) and not grp3_deactivateTargets then 
			grp3_deactivateTargetsCountdown = 1.0
			grp3_deactivateTargets = true
		end
	end

	--GROUP 4
	if not grp4_destroyTargets then
		for i = 1, #grp4_TargetID do
			if ECS:hasTagInEntity(grp4_TargetID[i], "Activated") then
				grp4_activationCount = grp4_activationCount + 1
			end
		end

		if grp4_activationCount == #grp4_TargetID then
			grp4_destroyTargetsCountdown = 0.5
			grp4_destroyTargets = true
		elseif not (grp4_activationCount == #grp4_TargetID) and not grp4_deactivateTargets then 
			grp4_deactivateTargetsCountdown = 1.0
			grp4_deactivateTargets = true
		end
	end
end

function powerOffBarrier()
	PhysicsSystem:LuaRemoveActor(barrierID)
	ECS:addEntityToDestroyQueue(barrierID)
end

function handlePowerOffCountdown(dt)
    if powerOffCountdown > 0.0 then
        powerOffCountdown = powerOffCountdown - dt        
    end
end

function handleDeactivateTargetsCountdown(dt)
	if grp1_deactivateTargetsCountdown > 0.0 then
		grp1_deactivateTargetsCountdown = grp1_deactivateTargetsCountdown - dt
	end

	if grp2_deactivateTargetsCountdown > 0.0 then
		grp2_deactivateTargetsCountdown = grp2_deactivateTargetsCountdown - dt
	end

	if grp3_deactivateTargetsCountdown > 0.0 then
		grp3_deactivateTargetsCountdown = grp3_deactivateTargetsCountdown - dt
	end

	if grp4_deactivateTargetsCountdown > 0.0 then
		grp4_deactivateTargetsCountdown = grp4_deactivateTargetsCountdown - dt
	end
end

function handleDestroyTargetsCountdown(dt)
	if grp1_destroyTargetsCountdown > 0.0 then
		grp1_destroyTargetsCountdown = grp1_destroyTargetsCountdown - dt
	end

	if grp2_destroyTargetsCountdown > 0.0 then
		grp2_destroyTargetsCountdown = grp2_destroyTargetsCountdown - dt
	end

	if grp3_destroyTargetsCountdown > 0.0 then
		grp3_destroyTargetsCountdown = grp3_destroyTargetsCountdown - dt
	end

	if grp4_destroyTargetsCountdown > 0.0 then
		grp4_destroyTargetsCountdown = grp4_destroyTargetsCountdown - dt
	end
end

function DestroyTargets()
	if grp1_destroyTargets and grp1_destroyTargetsCountdown <= 0.0 and not grp1_destroyed then
		for i = 1, #grp1_TargetID do
			ECS:addEntityTag(grp1_TargetID[i], "Destroy")
		end
		grp1_destroyed = true
	end

	if grp2_destroyTargets and grp2_destroyTargetsCountdown <= 0.0 and not grp2_destroyed then
		for i = 1, #grp2_TargetID do
			ECS:addEntityTag(grp2_TargetID[i], "Destroy")
		end
		grp2_destroyed = true
	end

	if grp3_destroyTargets and grp3_destroyTargetsCountdown <= 0.0 and not grp3_destroyed then
		for i = 1, #grp3_TargetID do
			ECS:addEntityTag(grp3_TargetID[i], "Destroy")
		end
		grp3_destroyed = true
	end

	if grp4_destroyTargets and grp4_destroyTargetsCountdown <= 0.0 and not grp4_destroyed then
		for i = 1, #grp4_TargetID do
			ECS:addEntityTag(grp4_TargetID[i], "Destroy")
		end
		grp4_destroyed = true
	end
end

function DeactivatingTargets()
	if grp1_deactivateTargets and grp1_deactivateTargetsCountdown <= 0.0 and not grp1_destroyed and not grp1_destroyTargets then
		for i = 1, #grp1_TargetID do
			if ECS:hasTagInEntity(grp1_TargetID[i], "Activated") then
				ECS:removeEntityTag(grp1_TargetID[i], "Activated")
			end
		end
		grp1_deactivateTargets = false
	end

	if grp2_deactivateTargets and grp2_deactivateTargetsCountdown <= 0.0 and not grp2_destroyed and not grp2_destroyTargets then
		for i = 1, #grp2_TargetID do
			if ECS:hasTagInEntity(grp2_TargetID[i], "Activated") then
				ECS:removeEntityTag(grp2_TargetID[i], "Activated")
			end
		end
		grp2_deactivateTargets = false
	end

	if grp3_deactivateTargets and grp3_deactivateTargetsCountdown <= 0.0 and not grp3_destroyed and not grp3_destroyTargets then
		for i = 1, #grp3_TargetID do
			if ECS:hasTagInEntity(grp3_TargetID[i], "Activated") then
				ECS:removeEntityTag(grp3_TargetID[i], "Activated")
			end
		end
		grp3_deactivateTargets = false
	end

	if grp4_deactivateTargets and grp4_deactivateTargetsCountdown <= 0.0 and not grp4_destroyed and not grp4_destroyTargets then
		for i = 1, #grp4_TargetID do
			if ECS:hasTagInEntity(grp4_TargetID[i], "Activated") then
				ECS:removeEntityTag(grp4_TargetID[i], "Activated")
			end
		end
		grp4_deactivateTargets = false
	end
end