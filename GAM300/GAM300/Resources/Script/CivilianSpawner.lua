local eEntID
local pEnemyPrefabID

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
	setMaxEnemy(3)
	pEnemyPrefabID = ECS:getPrefabID("Prefab_Civilian")
end

function setMaxEnemy(num)
	eEnemyCountMax = num
end

function spawnEnemy(dt, pos)
	if eEnemyTimer <= 0 then
		if eEnemyCount < eEnemyCountMax then
			local spawnPos = eEnemyTransform.pos + eForwardPrime
			spawnPos.x = pos.x
			spawnPos.z = pos.z
					local enemyID = ECS:instantiatePrefab(pEnemyPrefabID, "CivilianSpawner", spawnPos, eRot, false, false, false)
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
    end
end

function updateEnemyCount()
	enemies = ECS:getEntityTagged("Civilian")
	eEnemyCount = #enemies
end
-- END FOR ENEMY

-- Awake is called once and only once when script is loaded.
function Awake()
	resetAll()
end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Civilian script loaded")
end

function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
	updateEnemyCount()
    if eEntID == ECS:getEntityID("CivilianSpawner") then
        if ECS:hasComponent_Transform(eEntID) then
            spawnerTransform = ECS:getComponent_Transform(eEntID)
			initInUpdate()
			-- spawn enemies
			spawnEnemy(dt, spawnerTransform.pos)
        end
    end
end



