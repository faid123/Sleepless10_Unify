local eEntID
local policePrefabID
local armyPrefabID
local shieldPrefabID
local clownPrefabID
local helicopterPrefabID
-- FOR ENEMY
local eXPos
local eZPos
local ePrevXPos
local ePrevZPos
local eEnemyCount = 0
local eEnemyCountMax
local activeEntities
local eEnemyTimer
local eHeliTimer
local eRot = Quat:new(0,0,0,1)
local eEnemyTransform
local eForwardPrime  = Vector3:new()
local spawnerTransform

local currentStageLevel
local spawnOnceHeli = false
local enemyHeliCount = 0

-- for wanted level
local playerLuaScript
local playerID

function resetAll()
	eEnemyTimer = 3
	eHeliTimer 	= 0
	eEnemyCount = 0
	enemyHeliCount = 0
	spawnOnceHeli = false
	setMaxEnemy(10)
	policePrefabID 		= ECS:getPrefabID("Prefab_Police")
	armyPrefabID 		= ECS:getPrefabID("Prefab_Army")
	shieldPrefabID 		= ECS:getPrefabID("Prefab_Shield")
	clownPrefabID 		= ECS:getPrefabID("Prefab_Clown")
	helicopterPrefabID	= ECS:getPrefabID("Prefab_HelicopterEnemy") 
end

function setMaxEnemy(num)
	eEnemyCountMax = num
end


function spawnLvl1(threatLevel, spawnPos)
	local randomNum = 0
	if threatLevel >= 1 and threatLevel < 2 then
		local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
	elseif threatLevel >= 2 and threatLevel < 3 then
		randomNum = Random.randInRange(1,2)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		end
	elseif threatLevel >= 3 and threatLevel < 4 then
		randomNum = Random.randInRange(1,2)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		end
		-- increase spawn rate
	elseif threatLevel >= 4 and threatLevel < 5 then
		randomNum = Random.randInRange(1,2)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		end
		if spawnOnceHeli == false and enemyHeliCount == 0 and eHeliTimer >= 10 then
			local heliID 	= ECS:instantiatePrefab(helicopterPrefabID, "HelicopterEnemy", spawnPos, eRot, false, false, false)
			spawnOnceHeli = true
			eHeliTimer = -20
		end
	elseif threatLevel >= 5 then
		randomNum = Random.randInRange(1,2)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		end
		-- local bossID  = ECS:instantiatePrefab(bossPrefabID, "BossEnemy", spawnPos, eRot, false, false, false)
	end
end

function spawnLvl2(threatLevel, spawnPos)
	local randomNum = 0
	if threatLevel >= 1 and threatLevel < 2 then
		randomNum = Random.randInRange(1,2)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		end
	elseif threatLevel >= 2 and threatLevel < 3 then
		randomNum = Random.randInRange(1,3)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		end
	elseif threatLevel >= 3 and threatLevel < 4 then
		randomNum = Random.randInRange(1,3)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		end
		-- increase spawn rate
	elseif threatLevel >= 4 and threatLevel < 5 then
		randomNum = Random.randInRange(1,3)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		end
		if spawnOnceHeli == false and enemyHeliCount == 0 and eHeliTimer >= 10 then
			local heliID 	= ECS:instantiatePrefab(helicopterPrefabID, "HelicopterEnemy", spawnPos, eRot, false, false, false)
			spawnOnceHeli = true
			eHeliTimer = -20
		end
	elseif threatLevel >= 5 then
		randomNum = Random.randInRange(1,3)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		end
		-- local bossID  = ECS:instantiatePrefab(bossPrefabID, "BossEnemy", spawnPos, eRot, false, false, false)
	end

end

function spawnLvl3(threatLevel, spawnPos)
	local randomNum = 0
	if threatLevel >= 1 and threatLevel < 2 then
		randomNum = Random.randInRange(1,3)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		end
	elseif threatLevel >= 2 and threatLevel < 3 then
		randomNum = Random.randInRange(1,4)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 4 then
			local clownID  	= ECS:instantiatePrefab(clownPrefabID, "ClownEnemy", spawnPos, eRot, false, false, false)
		end
	elseif threatLevel >= 3 and threatLevel < 4 then
		randomNum = Random.randInRange(1,4)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 4 then
			local clownID  	= ECS:instantiatePrefab(clownPrefabID, "ClownEnemy", spawnPos, eRot, false, false, false)
		end
		-- increase spawn rate
	elseif threatLevel >= 4 and threatLevel < 5 then
		randomNum = Random.randInRange(1,4)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 4 then
			local clownID  	= ECS:instantiatePrefab(clownPrefabID, "ClownEnemy", spawnPos, eRot, false, false, false)
		end
		if spawnOnceHeli == false and enemyHeliCount == 0 and eHeliTimer >= 10 then
			local heliID 	= ECS:instantiatePrefab(helicopterPrefabID, "HelicopterEnemy", spawnPos, eRot, false, false, false)
			spawnOnceHeli = true
			eHeliTimer = -20
		end
	elseif threatLevel >= 5 then
		randomNum = Random.randInRange(1,4)
		if randomNum == 1 then
			local policeID 	= ECS:instantiatePrefab(policePrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 2 then
			local armyID	= ECS:instantiatePrefab(armyPrefabID, "ArmyEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 3 then
			local shieldID  = ECS:instantiatePrefab(shieldPrefabID, "ShieldEnemy", spawnPos, eRot, false, false, false)
		elseif randomNum == 4 then
			local clownID  	= ECS:instantiatePrefab(clownPrefabID, "ClownEnemy", spawnPos, eRot, false, false, false)
		end
		-- local bossID  = ECS:instantiatePrefab(bossPrefabID, "BossEnemy", spawnPos, eRot, false, false, false)
	else
		-- do something
		-- Console.WriteLine("Threat Level")
		-- Console.WriteLine(threatLevel)
	end

end

function spawnEnemy(dt, pos)
	local threatLevel = playerLuaScript:GetEnvironment().wantedLevel 
	updateMaxEnemy(threatLevel)
	if eEnemyTimer <= 0 then
		if eEnemyCount < eEnemyCountMax then
			--eXPos = Random.Float() * pos.x -- max x pos
			--eZPos = Random.Float() * pos.z -- max z pos
			local spawnPos = eEnemyTransform.pos + eForwardPrime
			spawnPos.x = pos.x
			spawnPos.z = pos.z
			-- if ePrevXPos ~= eXPos and ePrevZPos ~= eZPos then
				-- if isEnemyInsight(spawnPos, playerCamRot, 100) == false then
				-- Console.WriteLine(currentStageLevel)
				if currentStageLevel == 1 then
					spawnLvl1(threatLevel, spawnPos)
					eEnemyCount = eEnemyCount + 1
				elseif currentStageLevel == 2 then -- for EditorMode
				-- elseif currentStageLevel == 2 then -- for GameMode
					spawnLvl2(threatLevel, spawnPos)
					eEnemyCount = eEnemyCount + 1
				elseif currentStageLevel == 3 then
					spawnLvl3(threatLevel, spawnPos)
					eEnemyCount = eEnemyCount + 1
				else

				end
				-- Console.WriteLine(eEnemyCount)
					-- local clownID 	= ECS:instantiatePrefab(policePrefabID, "ClownEnemy", spawnPos, eRot, false, false, false) (uncomment later)
					-- eEnemyCount = eEnemyCount + 1
					-- ----Console.WriteLine("police enemy spawned")
					-- --Console.PrintFloat(eEnemyCount)
					ePrevXPos = eXPos
					ePrevZPos = eZPos
				-- end
			-- end
			-- ----Console.WriteLine("PoliceSpawner script running")
		end
		if threatLevel < 3 then 
			eEnemyTimer = 10
		elseif threatLevel >= 3 then
			eEnemyTimer = 5
		end
	end
	-- do a wait time before spawning
	eEnemyTimer = eEnemyTimer - dt
	eHeliTimer = eHeliTimer + dt
	-- --Console.PrintFloat(eEnemyTimer)
end

-- vector3, vector3, float
function isEnemyInsight(origin, unitDir, maxDist)
	return PhysicsSystem:ApplyRaycast(origin, unitDir, maxDist)
end

function initInUpdate()
    if ECS:hasComponent_Transform(eEntID) then
        eEnemyTransform = ECS:getComponent_Transform(eEntID)
    end

	-- check and get the luascriptcomponent
    if ECS:hasComponent_LuaScriptCom(playerID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(playerID)
    end
end

function updateEnemyCount()
	local enemyPolice 	= ECS:getEntityTagged("Police")
	local enemyArmy		= ECS:getEntityTagged("Army")
	local enemyShield	= ECS:getEntityTagged("Shield")
	local enemyClown	= ECS:getEntityTagged("Clown")
	local enemyHeli		= ECS:getEntityTagged("Helicopter")

	enemyHeliCount = #enemyHeli
	-- this returns the total number of enemies #enemies
	-- but when enemy is added to destroy queue it doesn't remove from the tagged list
	eEnemyCount = #enemyPolice + #enemyArmy + #enemyShield + #enemyClown
	-- Console.WriteLine("Enemy count")
	-- Console.PrintFloat(eEnemyCount)
end


-- sets max amount of enemies on the screen based on threat level
function updateMaxEnemy(threatLevel)
	if threatLevel >= 1 and threatLevel < 2 then
		setMaxEnemy(6)
	elseif threatLevel >= 2 and threatLevel < 3 then
		setMaxEnemy(10)
	elseif threatLevel >= 3 and threatLevel < 4 then
		setMaxEnemy(13)
	else
		setMaxEnemy(17)

	end
end



-- END FOR ENEMY

-- Awake is called once and only once when script is loaded.
function Awake()
	resetAll()
end

-- Start is called once and only once after Awake() is called.
function Start()
	playerID = ECS:getEntityID("Player")
	currentStageLevel = UISystem:getGameState()
end

-- Updates the Game Object per game loop
-- types of enemies police, army, shield
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
	updateEnemyCount()
    if ECS:hasTagInEntity(eEntID, "PoliceSpawner") == true then
        if ECS:hasComponent_Transform(eEntID) then
            spawnerTransform = ECS:getComponent_Transform(eEntID)
			initInUpdate()
			-- spawn enemies

			spawnEnemy(dt, spawnerTransform.pos)
        end
    end
end
