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
local objectiveCleared		= false
local clearDelayTimer

-- Police
local polObjCounter = 0

-- Army
local armyObjCounter = 0

-- Shield
local shieldObjCounter = 0

-- Clown
local clownObjCounter = 0

-- Building
local buildObjCounter = 0

-- Trees
local treeObjCounter = 0

-- Civilian
local civiObjCounter = 0

-- UI
local UIObjID
local spriteCom

-- Text Font
local fontKillCountID
local fontCounterID

-- General
local doOnce = false

-- UnifyScenario Table
unifyScenario = {}

function Awake()
	
end

function Start()
	playerID = ECS:getEntityID("Player")
	UIObjID = ECS:getEntityID("UI_Challenge")
end

function Update(dt)
	scenarioID = ScriptSystem:getLuaEntityID()

	initInUpdate()

	-- WHILE THE SCENARIO IS NOT ACTIVE
	if not activated then

		if checkTriggered() then
			activated = true
			initUponActivation()
		end
	end
	
	if activated and not objectiveCleared then
		whileActivated()
	end

	if objectiveCleared then
		handleClearDelayTime(dt)
		
		if clearDelayTimer <= 0.0 then
			onScenarioClear()
		end
	end

	clearScenarioCheat()
end

function checkTriggered()
	if ECS:hasTagInEntity(scenarioID, "Triggered") then
		return true
	end

	if ECS:hasComponent_Transform(scenarioID) then
		scenarioTransform = ECS:getComponent_Transform(scenarioID)
	end

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

function handleClearDelayTime(dt)
	if clearDelayTimer > 0.0 then
		clearDelayTimer = clearDelayTimer - dt
	end
end

function initInUpdate()
	if not doOnce then
		unifyScenario["uScene_Env"] = ECS:getComponent_LuaScriptCom(scenarioID):GetEnvironment()
		unifyScenario["uScene_Env"]["polSceneActivated"] = false
		unifyScenario["uScene_Env"]["armySceneActivated"] = false
		unifyScenario["uScene_Env"]["shieldSceneActivated"] = false
		unifyScenario["uScene_Env"]["clownSceneActivated"] = false
		unifyScenario["uScene_Env"]["civiSceneActivated"] = false
		unifyScenario["uScene_Env"]["buildSceneActivated"] = false
		unifyScenario["uScene_Env"]["treeSceneActivated"] = false

		doOnce = true
	end
end

function initUponActivation()	
	if ECS:hasTagInEntity(scenarioID, "PoliceScenario") then
		unifyScenario["uScene_Env"]["polSceneActivated"] = activated
	elseif ECS:hasTagInEntity(scenarioID, "ArmyScenario") then
		unifyScenario["uScene_Env"]["armySceneActivated"] = activated
	elseif ECS:hasTagInEntity(scenarioID, "ShieldScenario") then
		unifyScenario["uScene_Env"]["shieldSceneActivated"] = activated
	elseif ECS:hasTagInEntity(scenarioID, "ClownScenario") then
		unifyScenario["uScene_Env"]["clownSceneActivated"] = activated
	elseif ECS:hasTagInEntity(scenarioID, "BuildingScenario") then
		unifyScenario["uScene_Env"]["buildSceneActivated"] = activated
	elseif ECS:hasTagInEntity(scenarioID, "CivilianScenario") then
		unifyScenario["uScene_Env"]["civiSceneActivated"] = activated
	elseif ECS:hasTagInEntity(scenarioID, "TreeScenario") then
		unifyScenario["uScene_Env"]["treeSceneActivated"] = activated
	end
end

function whileActivated()
	if ECS:hasTagInEntity(scenarioID, "PoliceScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().polObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	elseif ECS:hasTagInEntity(scenarioID, "ArmyScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().armyObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	elseif ECS:hasTagInEntity(scenarioID, "ShieldScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().shieldObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	elseif ECS:hasTagInEntity(scenarioID, "ClownScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().clownObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	elseif ECS:hasTagInEntity(scenarioID, "BuildingScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().buildObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	elseif ECS:hasTagInEntity(scenarioID, "CivilianScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().civiObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	elseif ECS:hasTagInEntity(scenarioID, "TreeScenario") then
		if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().treeObjectiveCleared then
			objectiveCleared = true
			clearDelayTimer  = 1.0
		end
	end		
end

function onScenarioClear()
	ECS:getComponent_LuaScriptCom(playerID):GetEnvironment().wantedLevel = 
		ECS:getComponent_LuaScriptCom(playerID):GetEnvironment().wantedLevel + 0.8
	PhysicsSystem:LuaRemoveActor(scenarioID)
	ECS:addEntityToDestroyQueue(scenarioID)
end

function clearScenarioCheat()
	if Inputs.get_Key(InputKey.P_KEY) then
		objectiveCleared = true
		clearDelayTimer = 0.0
	end
end