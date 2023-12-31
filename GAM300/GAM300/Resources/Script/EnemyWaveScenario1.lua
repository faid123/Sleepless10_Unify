local eEntID
local pEnemyPrefabID
local playerID
local pFencePrefabID

-- FOR ENEMY
local eXPos
local eZPos
local ePrevXPos
local ePrevZPos
local eEnemyCountStart = 0
local eEnemyCountEnd = 0
local eEnemyCount = 0
local spawnEnemyCount = 0
local eEnemyCountMax = 10
local activeEntities
local eEnemyTimer
local eRot = Quat:new(0,0,0,1)
local eEnemyTransform
local eRigidDynamic
local eMaterial
local eForwardPrime  = Vector3:new()
local spawnerTransform
local timer = 20
local restart = false

local playerPos 
local currDistance 
local fenceDistance = 20
local triggerDistance = 20
local isTriggered = false
local isSpawnFence = false
local stopScript = false
local fenceCount = 8

local killCount = 0
local killObjective = 10
local entities = {}

local fontKillCountID
local fontCounterID

-- for wanted level
local playerLuaScript

function resetAll()
	eEnemyTimer = 8
	eUnified = false
	isTriggered = false
	diedTimer = 2
	eEnemyCount = 0
	pEnemyPrefabID = ECS:getPrefabID("Prefab_Police")
	pFencePrefabID = ECS:getPrefabID("Prefab_ScenarioFence")
end

function spawnFence(dt, pos, offsetx, offsetz, xapart, zapart, rot)
	local fencePos = pos + eForwardPrime + Vector3.new(-fenceDistance+offsetx, 0, fenceDistance+offsetz)
	--fencePos.x = fencePos.x + fenceDistance
	--fencePos.z = fencePos.z + fenceDistance
	for i = 1, 7 do
		local fenceID = ECS:instantiatePrefab(pFencePrefabID, "Fence", fencePos, eRot, false, false, false)
		if ECS:hasComponent_Transform(fenceID) then
			local fenceTransform = ECS:getComponent_Transform(fenceID)
			--PhysicsSystem:setGlobalPose(fenceID, fencePos, eRot)
			fencePos.x = fencePos.x + xapart
			fencePos.z = fencePos.z + zapart
			fenceTransform.pos = fencePos
			fenceTransform.rotate = Vector3.new(0, rot, 0)
			local rigid = ECS:getComponent_RigidStatic(fenceID)
			rigid.pos = fencePos
			--Console.WriteLine("SPAWNED FENCE SUCCESSFULLY!")
			--Console.PrintFloat(fenceTransform.rotate.x)
			--Console.PrintFloat(fenceTransform.rotate.y)
			--Console.PrintFloat(fenceTransform.rotate.z)
		end
		ECS:addEntityTag(fenceID, "Scenario_Fence")
	end
	isSpawnFence = true
	
end

function spawnEnemy(dt, pos)
	if eEnemyTimer <= 0 then
		if spawnEnemyCount < eEnemyCountMax then
			local spawnPos = eEnemyTransform.pos + eForwardPrime
			spawnPos.x = pos.x
			spawnPos.z = pos.z
					local enemyID = ECS:instantiatePrefab(pEnemyPrefabID, "Enemy", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"EnemyWaveScenario1")
					ePrevXPos = eXPos
					ePrevZPos = eZPos
					spawnEnemyCount = spawnEnemyCount +1
					
		end
		eEnemyTimer = 8
	end
	-- do a wait time before spawning
	eEnemyTimer = eEnemyTimer - dt*10
	-- --Console.PrintFloat(eEnemyTimer)
end

-- vector3, vector3, float
function isEnemyInsight(origin, unitDir, maxDist)
	return PhysicsSystem:ApplyRaycast(origin, unitDir, maxDist)
end

function initInUpdate()
    if ECS:hasComponent_Transform(eEntID) then
        eEnemyTransform = ECS:getComponent_Transform(eEntID)
		eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
		 
    end

	-- check and get the luascriptcomponent
    if ECS:hasComponent_LuaScriptCom(playerID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(playerID)
    end
end

function updateEnemyCount()
	enemies = ECS:getEntityTagged("EnemyWaveScenario1")
	-- this returns the total number of enemies #enemies
	-- but when enemy is added to destroy queue it doesn't remove from the tagged list
	eEnemyCount = #enemies
	-- ----Console.WriteLine("Enemy count")
	-- --Console.PrintFloat(eEnemyCount)
end
-- END FOR ENEMY

-- Awake is called once and only once when script is loaded.
function Awake()
	if stopScript == false then
		resetAll()
	end
end

-- Start is called once and only once after Awake() is called.
function Start()
	playerID = ECS:getEntityID("Player")
    ----Console.WriteLine("ShieldSpawner script loaded")
	fontKillCountID = ECS:getEntityID("Text_KillCounter")
	fontCounterID = ECS:getEntityID("Text_Counter")
	if ECS:hasComponent_TextCom(fontKillCountID) then
		local textcom = ECS:getComponent_TextCom(fontKillCountID)
		textcom.TextName = "/10"
		--textcom._colour = Vector4:new(1,1,1,1)
	end
	if ECS:hasComponent_TextCom(fontCounterID) then
		local textcounter = ECS:getComponent_TextCom(fontCounterID)
		textcounter.TextName = "0"
	end

end

-- Updates the Game Object per game loop
-- types of enemies police, army, shield
function Update(dt)
	if stopScript == false then
			eEntID = ScriptSystem:getLuaEntityID()
		
			if eEntID == ECS:getEntityID("EnemyWaveScenario1") then
				if ECS:hasComponent_Transform(eEntID) then
					spawnerTransform = ECS:getComponent_Transform(eEntID)
					initInUpdate()

					checkTriggered()
				end
			end

			if isTriggered == true then
				if ECS:hasComponent_TextCom(fontCounterID) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				if ECS:hasComponent_TextCom(fontKillCountID) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID)
					textcom._colour = Vector4:new(1,1,1,1)
				end
				local UIObjID = ECS:getEntityID("UI_Challenge")
				if ECS:hasComponent_SpriteCom(UIObjID) then
					local spriteCom = ECS:getComponent_SpriteCom(UIObjID)
					spriteCom._drawSprite = true
					spriteCom.SpriteName = "challenge_police"
				end

				eEnemyCountStart = killObjective
			

				updateEnemyCount()

				if spawnEnemyCount == killObjective then
					killCount =  eEnemyCountStart - eEnemyCount 
				end

				if ECS:hasComponent_TextCom(fontCounterID) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID)
					textcounter.TextName = killCount
				end

				if killCount == killObjective and spawnEnemyCount > 0 then
					stopScript = true
				end

				spawnEnemy(dt, spawnerTransform.pos)
						
				if isSpawnFence == false then
					spawnFence(dt, spawnerTransform.pos, -17, 22, 10, 0, 0) -- front z-axis
					spawnFence(dt, spawnerTransform.pos, -17, -48, 10, 0, 0) -- back z-axis
					spawnFence(dt, spawnerTransform.pos, -14, -53, 0, 10, 90) -- left x-axis
					spawnFence(dt, spawnerTransform.pos, 58, -53, 0, 10, 90) -- right x-axis
				end

				if eEnemyCount == 0 and spawnEnemyCount==5 then
					stopScript = true
					isTriggered = false
				end
			end

			if stopScript == true then
				-- remove the font
				if ECS:hasComponent_TextCom(fontCounterID) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID)
					textcounter.TextName = ""
				end
				if ECS:hasComponent_TextCom(fontKillCountID) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID)
					textcom.TextName = ""
				end
				local UIObjID = ECS:getEntityID("UI_Challenge")
				if ECS:hasComponent_SpriteCom(UIObjID) then
					local spriteCom = ECS:getComponent_SpriteCom(UIObjID)
					spriteCom._drawSprite = false
				end
				-- for wanted level -- 
				playerLuaScript:GetEnvironment().wantedLevel = playerLuaScript:GetEnvironment().wantedLevel+1
				Console.WriteLine("WANTED LEVEL: ")
				Console.PrintFloat(playerLuaScript:GetEnvironment().wantedLevel)
				-- for wanted level -- 
				entities = ECS:getEntityTagged("Scenario_Fence")
				local totalEntities = #entities
				for entity = 1, totalEntities do
					ECS:addEntityToDestroyQueue(entities[entity])
					PhysicsSystem:LuaRemoveActor(entities[entity])
				end
				ECS:addEntityToDestroyQueue(eEntID)
				PhysicsSystem:LuaRemoveActor(eEntID)
			end
	end
end

function checkTriggered()
	 playerPos = PhysicsSystem:getGlobalPosePosition(playerID)
	 distance = Vector3.Length(playerPos - spawnerTransform.pos )
	 --Console.PrintFloat(distance)
	 if distance < 0 then 
		distance = distance * -1
	 end

	 if distance <= triggerDistance then 
		--Console.WriteLine("triggered")
		isTriggered = true
	 end 
end






