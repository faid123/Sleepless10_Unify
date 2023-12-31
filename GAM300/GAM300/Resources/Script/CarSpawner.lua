local eEntID
local pCarPrefabID
local pCarPrefabID1
local pCarPrefabID2
local pCarPrefabID3
local pCarPrefabID4
-- FOR ENEMY
local eXPos
local eZPos
local ePrevXPos
local ePrevZPos
local eCarCount = 0
local eCarCountMax
local activeEntities
local eCarTimer
local eRot = Quat:new(0,0,0,1)
local eCarTransform
local eForwardPrime  = Vector3:new()
local spawnerTransform

local isSpawn

function resetAll()
	eCarTimer = 8
	eCarCount = 0
	setMaxVehicle(18)
	-- change to Prefab_Car
	pCarPrefabID = ECS:getPrefabID("Prefab_BasicCar")
	pCarPrefabID1 = ECS:getPrefabID("Prefab_BasicCarB")
	pCarPrefabID2 = ECS:getPrefabID("Prefab_PoliceCarMove")
	pCarPrefabID3 = ECS:getPrefabID("Prefab_BasicCarD")
	pCarPrefabID4 = ECS:getPrefabID("Prefab_BasicCarE")
end

function setMaxVehicle(num)
	eCarCountMax = num
end

function spawnVehicle(dt, pos, entID)
	if eCarTimer <= 0 then
		if eCarCount < eCarCountMax then
			--eXPos = Random.Float() * pos.x -- max x pos
			--eZPos = Random.Float() * pos.z -- max z pos
			local spawnPos = eCarTransform.pos + eForwardPrime
			spawnPos.x = pos.x
			spawnPos.z = pos.z
			-- if ePrevXPos ~= eXPos and ePrevZPos ~= eZPos then
				-- if isEnemyInsight(spawnPos, playerCamRot, 100) == false then
				-- check for tag A in VehicleSpawner entity
				if ECS:hasTagInEntity(entID, "A") then
					local enemyID = ECS:instantiatePrefab(pCarPrefabID, "C", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"A")
					ECS:addEntityTag(enemyID, "Building")
					ECS:addEntityTag(enemyID, "City")
					ECS:addEntityTag(enemyID, "Tree")
				elseif ECS:hasTagInEntity(entID, "B") then
					local enemyID = ECS:instantiatePrefab(pCarPrefabID1, "CarB", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"B")
					ECS:addEntityTag(enemyID, "Building")
					ECS:addEntityTag(enemyID, "City")
					ECS:addEntityTag(enemyID, "Tree")
				elseif ECS:hasTagInEntity(entID, "C") then
					local enemyID = ECS:instantiatePrefab(pCarPrefabID2, "CarC", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"C")
					ECS:addEntityTag(enemyID, "Building")
					ECS:addEntityTag(enemyID, "City")
					ECS:addEntityTag(enemyID, "Tree")
				elseif ECS:hasTagInEntity(entID, "D") then
					local enemyID = ECS:instantiatePrefab(pCarPrefabID3, "CarD", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"D")
					ECS:addEntityTag(enemyID, "Building")
					ECS:addEntityTag(enemyID, "City")
					ECS:addEntityTag(enemyID, "Tree")
				elseif ECS:hasTagInEntity(entID, "E") then
					eCarCountMax = 5
					local enemyID = ECS:instantiatePrefab(pCarPrefabID4, "CarE", spawnPos, eRot, false, false, false)
					ECS:addEntityTag(enemyID,"E")
					ECS:addEntityTag(enemyID, "Building")
					ECS:addEntityTag(enemyID, "City")
					ECS:addEntityTag(enemyID, "Tree")
				end	
					-- eCarCount = eCarCount + 1
					--Console.WriteLine("car spawned")
					-- --Console.PrintFloat(eCarCount)
					ePrevXPos = eXPos
					ePrevZPos = eZPos
				-- end
			-- end
			------Console.WriteLine("ArmySpawner script running")
		end
		eCarTimer = 50
	end
	-- do a wait time before spawning
	eCarTimer = eCarTimer - dt*10
	-- --Console.PrintFloat(eCarTimer)
end

-- vector3, vector3, float
function isEnemyInsight(origin, unitDir, maxDist)
	return PhysicsSystem:ApplyRaycast(origin, unitDir, maxDist)
end

function initInUpdate()
    if ECS:hasComponent_Transform(eEntID) then
        eCarTransform = ECS:getComponent_Transform(eEntID)
    end
end

function updateCarCount()
	enemies = ECS:getEntityTagged("Car")
	-- this returns the total number of enemies #enemies
	-- but when enemy is added to destroy queue it doesn't remove from the tagged list
	eCarCount = #enemies
	-- ----Console.WriteLine("Enemy count")
	-- --Console.PrintFloat(eCarCount)
end
-- END FOR ENEMY

-- Awake is called once and only once when script is loaded.
function Awake()
	resetAll()
end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("ArmySpawner script loaded")
end

-- Updates the Game Object per game loop
-- types of enemies police, army, shield
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
	updateCarCount()
    -- if ECS:hasTagInEntity(eEntID, "VehicleSpawner") == true then
        if ECS:hasComponent_Transform(eEntID) then
            spawnerTransform = ECS:getComponent_Transform(eEntID)
			initInUpdate()
			-- spawn enemies
			spawnVehicle(dt, spawnerTransform.pos, eEntID)
        end
    -- end
end



