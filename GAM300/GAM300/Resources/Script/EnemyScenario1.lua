local eEntID
local pEnemyPrefabID
local playerID
local pFencePrefabID

-- FOR ENEMY
local eXPos
local eZPos
local ePrevXPos
local ePrevZPos
local eEnemyCount = 0
local eEnemyCountMax = 10
local activeEntities
local eEnemyTimer
local eRot = Quat:new(0,0,0,1)
local eEnemyTransform
local eForwardPrime  = Vector3:new()
local spawnerTransform

local playerPos 
local currDistance 
local fenceDistance = 20
local triggerDistance = 20
local isTriggered = false
local isSpawnFence = false
local stopScript = false
local fenceCount = 8

-- for objective
killCount = 0
local killObjective = 2
local entities = {}

-- for wanted level
local playerLuaScript
-- SCENARIO 1 GLOBAL VARIABLE TABLE
local eLuaScript
local scenarioID
scenario_one = {}


function resetAll()
	eEnemyTimer = 8
	eEnemyCount = 0
	pEnemyPrefabID = ECS:getPrefabID("Prefab_Police")
	pFencePrefabID = ECS:getPrefabID("Prefab_Fence")
end

function spawnFence(dt, pos, offsetx, offsetz, xapart, zapart, rot)
	local fencePos = pos + eForwardPrime + Vector3.new(-fenceDistance+offsetx, 0, fenceDistance+offsetz)
	--fencePos.x = fencePos.x + fenceDistance
	--fencePos.z = fencePos.z + fenceDistance
	for i = 1, fenceCount do
		local fenceID = ECS:instantiatePrefab(pFencePrefabID, "Fence", fencePos, eRot, false, false, false)
		if ECS:hasComponent_Transform(fenceID) then
			local fenceTransform = ECS:getComponent_Transform(fenceID)
			--PhysicsSystem:setGlobalPose(fenceID, fencePos, eRot)
			fencePos.x = fencePos.x + xapart
			fencePos.z = fencePos.z + zapart
			fenceTransform.pos = fencePos
			fenceTransform.rotate = Vector3.new(0, rot, 0)
			-- Console.WriteLine("SPAWNED FENCE SUCCESSFULLY!")
			-- Console.PrintFloat(fenceTransform.rotate.x)
			-- Console.PrintFloat(fenceTransform.rotate.y)
			-- Console.PrintFloat(fenceTransform.rotate.z)
		end
		ECS:addEntityTag(fenceID, "scenario1")
	end
	isSpawnFence = true
	
end

function spawnEnemy(dt, pos)
	if eEnemyTimer <= 0 then
		if eEnemyCount < eEnemyCountMax then
			local spawnPos = eEnemyTransform.pos + eForwardPrime
			spawnPos.x = pos.x
			spawnPos.z = pos.z
					local enemyID = ECS:instantiatePrefab(pEnemyPrefabID, "Enemy", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"scenario1")
					ePrevXPos = eXPos
					ePrevZPos = eZPos
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
		eLuaScript = ECS:getComponent_LuaScriptCom(eEntID)
    end
	-- check and get the luascriptcomponent
    if ECS:hasComponent_LuaScriptCom(playerID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(playerID)
    end
end

function updateEnemyCount()
	enemies = ECS:getEntityTagged("EnemyScenario1")
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
	if stopScript == false then
		playerID = ECS:getEntityID("Player")
	end
    ----Console.WriteLine("ShieldSpawner script loaded")
	-- get the scenario environment table
	scenarioID = ECS:getEntityID("EnemyScenario1")
	scenario_one["Scenario1_Env"] = ECS:getComponent_LuaScriptCom(scenarioID):GetEnvironment()
end

-- Updates the Game Object per game loop
-- types of enemies police, army, shield
function Update(dt)

	if stopScript == false then
		eEntID = ScriptSystem:getLuaEntityID()
		updateEnemyCount()
		if eEntID == ECS:getEntityID("EnemyScenario1") then
			if ECS:hasComponent_Transform(eEntID) then
				spawnerTransform = ECS:getComponent_Transform(eEntID)
				initInUpdate()
				scenario_one["Scenario1_Env"]["killCount"] = killCount
				checkTriggered()
				-- spawn enemies
				if killCount < killObjective then
					if isTriggered == true then
						spawnEnemy(dt, spawnerTransform.pos)
						if isSpawnFence == false then
							-- adding 1 fence = +10 offset
							spawnFence(dt, spawnerTransform.pos, 0, 52, 10, 0, 0) -- front z-axis
							spawnFence(dt, spawnerTransform.pos, 0, -30, 10, 0, 0) -- back z-axis
							spawnFence(dt, spawnerTransform.pos, 3, -35, 0, 10, 90) -- left x-axis
							spawnFence(dt, spawnerTransform.pos, 85, -35, 0, 10, 90) -- right x-axis
						end
					end
				else
					-- destroy all entity that has "scenario1" as tag
					entities = ECS:getEntityTagged("scenario1")
					local totalEntities = #entities
					for entity = 1, totalEntities do
						-- Console.WriteLine("trying to delete entity")
						ECS:addEntityToDestroyQueue(entities[entity])
        				PhysicsSystem:LuaRemoveActor(entities[entity])
					 	Console.WriteLine("Destroyed Entity")
					end
					ECS:addEntityToDestroyQueue(eEntID)
        			PhysicsSystem:LuaRemoveActor(eEntID)
					Console.WriteLine("Destroyed LUAEntity")
					Console.WriteLine("Objective Completed")
					playerLuaScript:GetEnvironment().wantedLevel = playerLuaScript:GetEnvironment().wantedLevel+1
					Console.WriteLine("WANTED LEVEL: ")
            		Console.PrintFloat(playerLuaScript:GetEnvironment().wantedLevel)
					stopScript = true
				end
			end
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
	 else 
		isTriggered = false
	 end 
end






