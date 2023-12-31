local eEntID
local pEnemyPrefabID
-- FOR ENEMY
local eXPos
local eZPos
local ePrevXPos
local ePrevZPos
local eEnemyCount = 0
local eEnemyCountMax
local activeEntities
local eEnemyTimer
local eRot = Quat:new(0,0,0,1)
local eEnemyTransform
local eForwardPrime  = Vector3:new()
local spawnerTransform

function resetAll()
	eEnemyTimer = 8
	eEnemyCount = 0
	setMaxEnemy(5)
	pEnemyPrefabID = ECS:getPrefabID("Prefab_Army")
end

function setMaxEnemy(num)
	eEnemyCountMax = num
end

function spawnEnemy(dt, pos)
	if eEnemyTimer <= 0 then
		if eEnemyCount < eEnemyCountMax then
			--eXPos = Random.Float() * pos.x -- max x pos
			--eZPos = Random.Float() * pos.z -- max z pos
			local spawnPos = eEnemyTransform.pos + eForwardPrime
			spawnPos.x = pos.x
			spawnPos.z = pos.z
			-- if ePrevXPos ~= eXPos and ePrevZPos ~= eZPos then
				-- if isEnemyInsight(spawnPos, playerCamRot, 100) == false then
					local enemyID = ECS:instantiatePrefab(pEnemyPrefabID, "PoliceEnemy", spawnPos, eRot, false, false, false)
					-- eEnemyCount = eEnemyCount + 1
					-- ----Console.WriteLine("police enemy spawned")
					-- --Console.PrintFloat(eEnemyCount)
					ePrevXPos = eXPos
					ePrevZPos = eZPos
				-- end
			-- end
			-- ----Console.WriteLine("PoliceSpawner script running")
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
    end
end

function updateEnemyCount()
	enemies = ECS:getEntityTagged("Army")
	-- this returns the total number of enemies #enemies
	-- but when enemy is added to destroy queue it doesn't remove from the tagged list
	eEnemyCount = #enemies
	-- ----Console.WriteLine("Enemy count")
	-- --Console.PrintFloat(eEnemyCount)
end
-- END FOR ENEMY

-- Awake is called once and only once when script is loaded.
function Awake()
	resetAll()
end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("PoliceSpawner script loaded")
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



