-- player
local playerID
local playerPos

-- Scenario
local scenarioID
local scenarioTransform
local globalRot
local activated				= false
local activationDistance	= 20.0
local scenarioCleared		= false

-- Cage
local cageID
local cagePrefabID
local cagePos
local powerOffCountdown = 0.0

-- Target
local targetPrefabID
local targetSpawnPos			 = Vector3:new()
local targetID = {}
local deactivateTargets			 = false
local deactivateTargetsCountdown = 0.0

-- Civilian
local civilianPrefabID
local maxCivilianSpawn	= 5
local civilianID		= {}
local releaseCivilians  = false

-- UI
local UIObjID
local spriteCom

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
local doOnce			= false

-- Laser tutorial table
LaserScenario = {} 

function Awake()

end

function Start()
	playerID = ECS:getEntityID("Player")
	cagePrefabID = ECS:getPrefabID("Prefab_Cage")
	targetPrefabID = ECS:getPrefabID("Prefab_LaserTarget")
	civilianPrefabID = ECS:getPrefabID("Prefab_Constructor")
	UIObjID = ECS:getEntityID("UI_Challenge")
	spriteCom = ECS:getComponent_SpriteCom(UIObjID)
end

function Update(dt)
	scenarioID = ScriptSystem:getLuaEntityID()
	scenarioRotateVectorByQuaternion()

	if scenarioID == ECS:getEntityID("LaserScenarioTutorial") then
		if ECS:hasComponent_Transform(scenarioID) then
			scenarioTransform = ECS:getComponent_Transform(scenarioID)
		end		
	end

	initInUpdate()

	-- WHILE THE SCENARIO IS NOT ACTIVE
	if not activated then
		if checkTriggered() then
			activated = true
			LaserScenario["laserScene_Env"]["laserSceneActivated"] = activated

			spawnCage()

			spawnCivilian(0.0, 0.0, 0.0, 0.0)
			spawnCivilian(3.0, 0.0, 0.0, 0.0)
			spawnCivilian(0.0, 3.0, 0.0, 0.0)
			spawnCivilian(0.0, 0.0, 3.0, 0.0)
			spawnCivilian(0.0, 0.0, 0.0, 3.0)

			spawnTarget(15.0, 5.0, 5)
		end
	end
	
	-- WHILE THE SCENARIO IS ACTIVE
	if activated then
		civilianID = ECS:getEntityTagged("Trapped")
		-- IF CIVILIANS ARE NOT TO BE RELEASED
		if not releaseCivilians then
			-- CHECK IF PLAYERS HAVE ACTIVATED ALL TARGETS
			checkTargetsActivated()
			-- IF NOT ALL TARGETS ARE ACTIVATED, DEACTIVATE ALL ACTIVATED TARGETS
			if deactivateTargets then
				handleDeactivateTargetsCountdown(dt)
				if deactivateTargetsCountdown <= 0.0 then
					DeactivatingTargets()
				end
			end
		-- IF CIVILIANS ARE TO BE RELEASED
		elseif releaseCivilians then
			-- POWER DOWN THE CAGE AND RELEASED TRAPPED CIVILIANS
			handlePowerOffCountdown(dt)
			if powerOffCountdown <= 0.0 then
				powerOffCage()
			end
		end

		if scenarioCleared and #targetID == 0 then
			ECS:getComponent_LuaScriptCom(playerID):GetEnvironment().wantedLevel = 
				ECS:getComponent_LuaScriptCom(playerID):GetEnvironment().wantedLevel + 0.8
			PhysicsSystem:LuaRemoveActor(scenarioID)
			ECS:addEntityToDestroyQueue(scenarioID)
		end
	end

	clearScenarioCheat()
end

function checkTriggered()
	 playerPos = PhysicsSystem:getGlobalPosePosition(playerID)
	 local distance = Vector3.Length(playerPos - scenarioTransform.pos)
	 if distance < 0.0 then 
		distance = distance * -1.0
	 end

	 if distance <= activationDistance then 
		return true
	 end 

	 return false
end

function spawnCage()
	cagePos = scenarioTransform.pos
	cagePos.y = 7.0
	cageID = ECS:instantiatePrefab(cagePrefabID, "Cage", cagePos, sRot, false, false, false)
	if ECS:hasComponent_Transform(cageID) and ECS:hasComponent_RigidStatic(cageID) then
		local cageTransform = ECS:getComponent_Transform(cageID)
		cageTransform.pos = cagePos
		local cageRigid = ECS:getComponent_RigidStatic(cageID)
		cageRigid:setActorName("LaserCage")
		cageRigid.pos = cagePos
	end
end

function spawnCivilian(foreOffset, backOffset, leftOffset, rightOffset)
	local civilianSpawnPos = scenarioTransform.pos
	civilianSpawnPos = civilianSpawnPos + forwardPrime * foreOffset
	civilianSpawnPos = civilianSpawnPos + backwardPrime * backOffset
	civilianSpawnPos = civilianSpawnPos + leftwardPrime * leftOffset
	civilianSpawnPos = civilianSpawnPos + rightwardPrime * rightOffset
	local _civilianID = ECS:instantiatePrefab(civilianPrefabID, "Civilian", civilianSpawnPos, sRot, false, false, false)
	ECS:addEntityTag(_civilianID,"Trapped")
	if ECS:hasComponent_Transform(_civilianID) then
		local civiTransform = ECS:getComponent_Transform(_civilianID)
		civiTransform.pos = civilianSpawnPos
		civiTransform.rotate = PhysicsSystem:convertQuatToEuler(sRot)
		local rigid = ECS:getComponent_RigidDynamic(_civilianID)
		rigid.rigidPos = civiTransform
		rigid.rigidRot = sRot
	end
end

function spawnTarget(horiOffset, vertOffset, maxSpawnCount)
	targetSpawnPos = scenarioTransform.pos + backwardPrime * horiOffset
	targetSpawnPos = targetSpawnPos + (leftwardPrime * vertOffset) * (maxSpawnCount / 2)
	targetSpawnPos.y = 3.0
	for targetCount = 1, maxSpawnCount do
		targetID[targetCount] = ECS:instantiatePrefab(targetPrefabID, "Target", targetSpawnPos, sRot2, false, false, false)
		ECS:addEntityTag(targetID[targetCount],"LaserTarget")
		if ECS:hasComponent_Transform(targetID[targetCount]) then
			local targetTransform = ECS:getComponent_Transform(targetID[targetCount])
			targetTransform.pos = targetSpawnPos
			targetTransform.rotate = PhysicsSystem:convertQuatToEuler(sRot2)
			local rigid = ECS:getComponent_RigidStatic(targetID[targetCount])
			rigid.pos = targetSpawnPos
			rigid.rot = sRot2
		end
		targetSpawnPos = targetSpawnPos + rightwardPrime * vertOffset
	end
end

function scenarioRotateVectorByQuaternion()
    globalRot = PhysicsSystem:getGlobalPoseRotation(scenarioID)

    PhysicsSystem:rotate_vector_by_quaternion(forward, globalRot, forwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(backward, globalRot, backwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(leftward, globalRot, leftwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(rightward, globalRot, rightwardPrime)
end

function checkTargetsActivated()
	local activationCount = 0
	for i = 1, #targetID do
		if ECS:hasTagInEntity(targetID[i], "Activated") then
			activationCount = activationCount + 1
		end
	end

	if activationCount == #targetID then
		releaseCivilians = true
		powerOffCountdown = 0.5
	elseif not (activationCount == #targetID) and not deactivateTargets then 
		deactivateTargetsCountdown = 1.0
		deactivateTargets = true
	end
end

function powerOffCage()
	for i = 1, #targetID do
		ECS:addEntityTag(targetID[i], "Destroy")
		targetID[i] = nil
	end
	
	   PhysicsSystem:LuaRemoveActor(cageID)
	   ECS:addEntityToDestroyQueue(cageID)
	
	for j = 1, #civilianID do
		if ECS:hasTagInEntity(civilianID[j], "Trapped") then
			ECS:removeEntityTag(civilianID[j], "Trapped")
		end
	end

	ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().laserTutorialObjectiveCleared = true
	scenarioCleared = true
end

function handlePowerOffCountdown(dt)
    if powerOffCountdown > 0.0 then
        powerOffCountdown = powerOffCountdown - dt        
    end
end

function handleDeactivateTargetsCountdown(dt)
	if deactivateTargetsCountdown > 0.0 then
		deactivateTargetsCountdown = deactivateTargetsCountdown - dt
	end
end

function DeactivatingTargets()
	for i = 1, #targetID do
		if ECS:hasTagInEntity(targetID[i], "Activated") then
			ECS:removeEntityTag(targetID[i], "Activated")
		end
	end
	deactivateTargets = false
end

function initInUpdate()
	if not doOnce then
		LaserScenario["laserScene_Env"] = ECS:getComponent_LuaScriptCom(scenarioID):GetEnvironment()
		LaserScenario["laserScene_Env"]["laserSceneActivated"] = false
		doOnce = true
	end
end

function clearScenarioCheat()
	if Inputs.get_Key(InputKey.P_KEY) then
		if not activated then
			ECS:getComponent_LuaScriptCom(playerID):GetEnvironment().wantedLevel = 
				ECS:getComponent_LuaScriptCom(playerID):GetEnvironment().wantedLevel + 0.8
			PhysicsSystem:LuaRemoveActor(scenarioID)
			ECS:addEntityToDestroyQueue(scenarioID)
		else
			powerOffCage()
		end
	end
end