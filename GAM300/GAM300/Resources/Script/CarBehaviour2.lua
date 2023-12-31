local timer = 4
local delayTime = 5
local constTimer = 4
local count = 0
local maxCount
local startPos =  Vector3:new()
local endPos =  Vector3:new()

local waypoints = {}
local EntID1
local pEntID
local ePosNew
local eSpeedCap = 50.0
local eWalkForce = 900
local addForce = Vector3:new(0.0, 0.0, 12.0)
local carHitForce = 3000.0

local changeDirection = false
local checkWP = false
local eRot 
local eRotQuat
local newPos
local newRot
local eRotNew      = Vector3:new(0.0, 0.0, 0.0)

local angle = 90;

local eRigidDynamic
local eTransform

local pRigidDynamic

local aEposNew = 0
local aEposCurr = 0
local tmp = 0

-- Absolute End, Absolute Current
local aEnd = Vector3:new(0.0, 0.0, 0.0)
local aCurrent = Vector3:new(0.0, 0.0, 0.0)

-- Dist Variabl
local endpoint =  Vector3:new(0.0, 0.0, 0.0)
local distance = 0
local currPos = 0
local currHT = 0
local eSetEnemyRot      = Vector3:new(0.0, 0.0, 0.0)
local carVelocity =  Vector3:new(0.0, 0.0, -1.0)

local forward           = Vector3:new(0.0, 0.0, -1.0)
local backward          = Vector3:new(0.0, 0.0, 1.0)
local leftward          = Vector3:new(-1.0, 0.0, 0.0)
local rightward         = Vector3:new(1.0, 0.0, 0.0)

local forwardPrime      = Vector3:new()
local backwardPrime     = Vector3:new()
local leftwardPrime     = Vector3:new()
local rightwardPrime    = Vector3:new()

local eIsWalking


-- to color building
local eTransform
local eMaterial
local eHealth
local eFlashTimer
local Unified
local canApplyLaserDamage = true
local laserDmgDelayTimer  = 0.0
local laser_BuildingHit
local explode_BuildingHit

local playerLuaScript

-- Awake is called once and only once when script is loaded.
function Awake()
	eHealth = 25
    eFlashTimer = 1
    Unified = false
end

-- Start is called once and only once after Awake() is called.
function Start()
	waypoints = ResourceManager:GetWayPoint("A")
	maxCount = #waypoints
    EntID1 = ScriptSystem:getLuaEntityID()
	pEntID = ECS:getEntityID("Player")
    -- Console.PrintEntityID(EntID1)
end
	
	
function destroy()
	 PhysicsSystem:LuaRemoveActor(EntID1)
    ECS:addEntityToDestroyQueue(EntID1)
end

-- Updates the Game Object per game loop
function Update(dt)

	-- EntID1 is Car
	-- get the car tag and assign the waypoint accordingly
	EntID1 = ScriptSystem:getLuaEntityID()

	-- Get the info needed for the functions
	carVelocity = PhysicsSystem:getBodyLinearVelocity(EntID1)
    PhysicsSystem:setBodyLinearVelocity(EntID1, carVelocity, eSpeedCap)

	if ECS:hasTagInEntity(EntID1, "A") == true then
		startPos = ResourceManager:getWayPointVector3("A",count);
		endpoint = ResourceManager:getWayPointVector3("A",count+1);
	elseif ECS:hasTagInEntity(EntID1, "B") == true then
		startPos = ResourceManager:getWayPointVector3("B",count);
		endpoint = ResourceManager:getWayPointVector3("B",count+1);
	elseif ECS:hasTagInEntity(EntID1, "C") == true then
		startPos = ResourceManager:getWayPointVector3("C",count);
		endpoint = ResourceManager:getWayPointVector3("C",count+1);
	elseif ECS:hasTagInEntity(EntID1, "D") == true then
		startPos = ResourceManager:getWayPointVector3("D",count);
		endpoint = ResourceManager:getWayPointVector3("D",count+1);
	elseif ECS:hasTagInEntity(EntID1, "E") == true then
		startPos = ResourceManager:getWayPointVector3("E",count);
		endpoint = ResourceManager:getWayPointVector3("E",count+1);
	end
	
	initInUpdate()
	enemyRotateVectorByQuaternion()
	updateEnemyPos()
	updateEnemyRot()
	carHitPlayerBehaviour()
	unifyBehaviour(dt)
	--PhysicsSystem:setGravityFlag(EntID1, true)
	
    if ECS:hasComponent_RigidDynamic(EntID1) and
	   ECS:getComponent_Transform(EntID1)    then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        eTransform = ECS:getComponent_Transform(EntID1)
        eRot =  eRigidDynamic.rigidRot
        ePosNew = PhysicsSystem:getGlobalPosePosition(EntID1)
		currHT  = eTransform.pos.y
		currPos = eTransform.pos
	end

	if ECS:hasComponent_RigidDynamic(pEntID) then
        pRigidDynamic = ECS:getComponent_RigidDynamic(pEntID)
		
	end

	ePosCurr = PhysicsSystem:getGlobalPosePosition(EntID1)
	
	
	if count < maxCount-1 then
	-- how to get waypoint
		if ECS:hasTagInEntity(EntID1, "A") == true then
			waypoints = ResourceManager:GetWayPoint("A")
			maxCount = #waypoints
		elseif ECS:hasTagInEntity(EntID1, "B") == true then
			waypoints = ResourceManager:GetWayPoint("B")
			maxCount = #waypoints
		elseif ECS:hasTagInEntity(EntID1, "C") == true then
			waypoints = ResourceManager:GetWayPoint("C")
			maxCount = #waypoints
		elseif ECS:hasTagInEntity(EntID1, "D") == true then
			waypoints = ResourceManager:GetWayPoint("D")
			maxCount = #waypoints
		elseif ECS:hasTagInEntity(EntID1, "E") == true then
			waypoints = ResourceManager:GetWayPoint("E")
			maxCount = #waypoints
		end
		--Console.PrintFloat(endpoint.z)
		
		--Console.PrintFloat(ePosCurr.z)
		--Console.WriteLine("Waypoint Found!")
		checkWP = true
		
		--PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, eSpeedCap)
		
		if checkWP == true then
		
			--Console.PrintFloat(ePosCurr)
			-- Compare distance of self with endpoint
			--compareDist(endpoint,count)
			distance = Vector3.Length(endpoint - ePosCurr)
--[[ 			Console.WriteLine("--------DISTANCE---------")
			Console.PrintFloat(distance)
			Console.WriteLine("--------DISTANCE---------") ]]
			--Console.PrintFloat(distance)
			if distance < 0 then
				distance = distance *-1
			end
			if distance <= 5.5 then
					--Console.WriteLine("Waypoint Reached! Looking for next COUUNT:")
					--Console.WriteLine("Waypoint Reached! Looking for next COUUNT:")
					count = count + 1
					--Console.PrintFloat(count)
					if ECS:hasTagInEntity(EntID1, "A") == true then
						endpoint = ResourceManager:getWayPointVector3("A",count)
					elseif ECS:hasTagInEntity(EntID1, "B") == true then
						endpoint = ResourceManager:getWayPointVector3("B",count)
					elseif ECS:hasTagInEntity(EntID1, "C") == true then
						endpoint = ResourceManager:getWayPointVector3("C",count)
					elseif ECS:hasTagInEntity(EntID1, "D") == true then
						endpoint = ResourceManager:getWayPointVector3("D",count)
					elseif ECS:hasTagInEntity(EntID1, "E") == true then
						endpoint = ResourceManager:getWayPointVector3("E",count)
					end
					checkWP = false
					-- Set destination by rotating character at correct place
					-- walk
			end
				
			-- if havent reach
			if distance > 3 then
					--Console.WriteLine("Walking")
					lookTowards(EntID1, endpoint)
					local DirectionVector = Vector3.Normalized(endpoint - startPos)

		
					
--[[ 						Console.WriteLine("MAX COUNT VALUE:")
					Console.PrintFloat(count)
					Console.PrintFloat(maxCount)
					Console.WriteLine("-----------------")
					Console.PrintFloat(DirectionVector.x)
					Console.PrintFloat(DirectionVector.y)
					Console.PrintFloat(DirectionVector.z)
					Console.PrintFloat(eWalkForce)
					Console.WriteLine("-----------------") ]]

					PhysicsSystem:addForceToBody(EntID1, DirectionVector, eWalkForce, 0)

					-- Set destination by rotating character at correct place
					-- walk
			end
				
				--if distance > 10
						-- Count ++
						-- update endpoint
						-- update distance
		end
	elseif count == maxCount - 1 then
		count = 0
	end
	--Console.WriteLine("Route Finished!")
	PhysicsSystem:setBodyLinearVelocity(EntID1, Vector3.new(0,0,0), 0)
	
	--Console.PrintFloat(distance)
end


function updateEnemyPos()
    eTransform = ECS:getComponent_Transform(EntID1)
    eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
    newPos = PhysicsSystem:getGlobalPosePosition(EntID1)

	local DirectionVector = Vector3.Normalized(endpoint - startPos)
	if DirectionVector.x == 0 and (not(DirectionVector.z == 0)) then
			eTransform = ECS:getComponent_Transform(EntID1)
			newPos.x = endpoint.x
	elseif  DirectionVector.z == 0 and (not(DirectionVector.x == 0)) then
			eTransform = ECS:getComponent_Transform(EntID1)
			newPos.z = endpoint.z
	end

    eTransform.pos.x = newPos.x
    eTransform.pos.y = newPos.y
    eTransform.pos.z = newPos.z
end

-- Updates the rigidbody on Enemy rotation
function updateEnemyRot()
    
    if eRotNew.y > 360 then
        eRotNew.y = 0
    elseif eRotNew.y < 0 then
        eRotNew.y = 360
    end
    
    eRotQuat = PhysicsSystem:convertEulerToQuat(eRotNew)
        
    PhysicsSystem:setGlobalPose(EntID1, newPos, eRotQuat)
end

function enemyRotateVectorByQuaternion()
    newRot = PhysicsSystem:getGlobalPoseRotation(EntID1)

    PhysicsSystem:rotate_vector_by_quaternion(forward, newRot, forwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(backward, newRot, backwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(leftward, newRot, leftwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(rightward, newRot, rightwardPrime)
end


-- function ref()
	    -- -- Adding force towards the player direction
     -- EntID1 = ScriptSystem:getLuaEntityID()
     -- --Console.PrintEntityID(EntID1)
    -- if ECS:hasComponent_RigidDynamic(EntID1) then
        -- eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        -- eTransform = ECS:getComponent_Transform(EntID1)
        -- eRot =  eRigidDynamic.rigidRot
        -- ePosNew = PhysicsSystem:getGlobalPosePosition(EntID1)
	-- end
	
	-- --how to get additional wp from array
	-- if count == 0 then
		-- tmp = count +1
		-- startPos =  ResourceManager:getWayPointVector3("A",count);
		-- endPos = ResourceManager:getWayPointVector3("A",tmp);
	-- end
	-- newPos = PhysicsSystem:getGlobalPosePosition(EntID1)
	-- local distance = Vector3.Length(endPos - newPos)
	-- Console.PrintFloat(distance)
	-- --Console.PrintFloat(tmp)
	-- if distance <= 5 and tmp == maxCount then
		-- Console.WriteLine("Stop")
		-- endPos = ResourceManager:getWayPointVector3("A",tmp);
		-- PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, 0)
	-- end
-- end

function compareDist(endpoint,count)
	
	if ECS:hasComponent_RigidDynamic(EntID1) and
	   ECS:getComponent_Transform(EntID1)    then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        eTransform = ECS:getComponent_Transform(EntID1)
        eRot =  eRigidDynamic.rigidRot
		
        ePosNew = PhysicsSystem:getGlobalPosePosition(EntID1)
		currHT  = eTransform.pos.y
		currPos = eTransform.pos
		
		if ECS:hasTagInEntity(EntID1, "A") == true then
			endpoint = ResourceManager:getWayPointVector3("A",count)
		elseif ECS:hasTagInEntity(EntID1, "B") == true then
			endpoint = ResourceManager:getWayPointVector3("B",count)
		elseif ECS:hasTagInEntity(EntID1, "C") == true then
			endpoint = ResourceManager:getWayPointVector3("C",count)
		elseif ECS:hasTagInEntity(EntID1, "D") == true then
			endpoint = ResourceManager:getWayPointVector3("D",count)
		elseif ECS:hasTagInEntity(EntID1, "E") == true then
			endpoint = ResourceManager:getWayPointVector3("E",count)
		end
		currPos = ePosNew 
		
		distance = endpoint.z - currPos
		
		if distance < 0 then
			
			distance = distance * -1
		
		end
		
		-- -- absolute both positions then compare
		-- if endpoint.z < 0.0 then
			-- aEnd.z = endpoint * - 1
		-- end
		
		-- if currPos < 0.0 then
			-- aCurrent.z = aEposCurr.z * -1
		-- end
		
		-- -- If positive, keep it that way
		-- if endpoint.z > 0.0 then
			-- aEnd.z = endpoint.z 
		-- end
		
		-- if currPos > 0.0 then
			-- aCurrent.z = aEposCurr.z
		-- end
		
		-- -- Compare Dist, Big minus small
		-- if aCurrent.z > aEnd.z then
			-- distance = aCurrent.z - aEnd.z
		-- end
		
		-- if aCurrent.z < aEnd.z then
			-- distance = aEnd.z - aCurrent.z
		-- end
		
		--Console.WriteLine("Dist")
		--distance = Vector3.Length(endpoint-currPos)	
	end	
end

-- Ent1 look at ent2 function
function lookTowards(EntID1, ent2Pos)
    -- Adding force towards the player direction
    if ECS:hasComponent_RigidDynamic(EntID1) and
	   ECS:hasComponent_Transform(EntID1) then
		
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
		eTransform = ECS:getComponent_Transform(EntID1)
        ent1Pos = eRigidDynamic.rigidPos
        
        
		
        targetRotation = radianTopi(atan2(ent1Pos.z - ent2Pos.z, ent1Pos.x - ent2Pos.x))
        --Trigo.floatToint(targetRotation)
        rotationDifference = eSetEnemyRot.y + targetRotation -90

        --rotationDifference = rotationDifference % 180
        Trigo.floatToint(rotationDifference)
        
        if rotationDifference > 180.0 then
            rotationDifference = rotationDifference - 360.0
        end

        if rotationDifference < -180.0 then
            rotationDifference = rotationDifference + 360.0
        end
        
        if rotationDifference < 0  then
            eSetEnemyRot.y = eSetEnemyRot.y + 2.0
        elseif rotationDifference > 0 then
            eSetEnemyRot.y = eSetEnemyRot.y - 2.0
        end
		
		eTransform.rotate = eSetEnemyRot
		--Console.WriteLine("Hello!")
		
    end
end
	
-- Logic for atan2
function atan2(y, x)
    if x > 0.0 then
        return Trigo.atan(y/x)
    end

    if x < 0.0 and y >= 0.0 then
        return Trigo.atan(y/x) + Trigo.pi()
    end

    if x < 0.0 and y < 0.0 then
        return Trigo.atan(y/x) - Trigo.pi()
    end

    if x == 0.0 and y > 0.0 then
        return Trigo.pi() / 2
    end

    if x == 0.0 and y < 0.0 then
        return -Trigo.pi() / 2
    end
    
    if x == 0.0 and y == 0.0 then
        return 0.0
    end
end

-- Convert radian to pi
function radianTopi(x)
    return 180/Trigo.pi()*x
end
	
-- Updates the rigidbody on car position
function updateCarPos()
    eTransform = ECS:getComponent_Transform(EntID1)
    eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
    newPos = PhysicsSystem:getGlobalPosePosition(EntID1)
    
    if  eTransform.pos.x == newPos.x and eTransform.pos.y == newPos.y and eTransform.pos.z == newPos.z then
        eIsWalking = true
    else 
        eIsWalking = false 
    end

    eTransform.pos.x = newPos.x
    eTransform.pos.y = newPos.y
    eTransform.pos.z = newPos.z

    eRigidDynamic.rigidPos.x = newPos.x
    eRigidDynamic.rigidPos.y = newPos.y
    eRigidDynamic.rigidPos.z = newPos.z
end

function carHitPlayerBehaviour()
	if mContactReportCallback.carHitPlayer then
		--Console.WriteLine("car hit player")
		local directionVector = Vector3.Normalized(endpoint - startPos)
		PhysicsSystem:addForceToBody(pEntID, directionVector, carHitForce, 2)
		mContactReportCallback.carHitPlayer = false
	end
end

function initInUpdate()
    if ECS:hasComponent_Material(EntID1) and ECS:hasComponent_Transform(EntID1) and ECS:hasComponent_RigidDynamic(EntID1) then
        eMaterial = ECS:getComponent_Material(EntID1)
    end

	if ECS:hasComponent_LuaScriptCom(pEntID) then
	    playerLuaScript = ECS:getComponent_LuaScriptCom(pEntID)
	end
end

function unifyBehaviour(dt)
	laser_BuildingHit = mContactReportCallback:getEntityHitID("Building","Laser")
    explode_BuildingHit = mContactReportCallback:getEntityHitID("Building","Explosion")

	if eFlashTimer > 1.5 then
        eFlashTimer = eFlashTimer - dt*10
        flashOnHit()
    end

	onHit()

	if (eMaterial._saturation.y > 0.0) then 
        Unified = true
        if changeWanted == false then
            local hasTag = ECS:hasTagInEntity(EntID1, "Tree")
            if not(hasTag) then
                --playerLuaScript:GetEnvironment().wantedLevel = playerLuaScript:GetEnvironment().wantedLevel + 0.1
                -- Console.WriteLine("Building WANTED LEVEL")
                -- Console.PrintFloat(playerLuaScript:GetEnvironment().wantedLevel)
                changeWanted = true
            end
        end
    end

	

	if (eHealth > 0 and Unified == false) then
        setSatZero()
    end

    if eHealth <= 0 then
        Unify(dt)
    end

	handleLaserDmgDelay(dt)

end

function setSatZero()

    eMaterial._saturation.y = 0
end

function setSatOne()

    eMaterial._saturation.y = 1
end

function onHit()
    if eRigidDynamic.getHit and mContactReportCallback.bulletHitBuilding and EntID1 == mContactReportCallback._detectedBuilding  then
        if Unified == false then
			AudioSystem:PlaySFX("SFX_HitMarker", pEntID)
            eHealth = eHealth - 5
            eFlashTimer = 8
        end
    elseif eRigidDynamic.getHit and mContactReportCallback.explosionHitBuilding then
        for i = 1, #explode_BuildingHit do
            if Unified == false and explode_BuildingHit[i] == EntID1 then
                local explosionID = mContactReportCallback._detectedExplosion
                if not ECS:hasTagInEntity(explosionID, "Boss_Explosion") then
                    if Unified == false then
						AudioSystem:PlaySFX("SFX_HitMarker", pEntID)
                        eHealth = eHealth - 13
                        eFlashTimer = 8
                        break
                    end
                end
            end
        end
    elseif eRigidDynamic.getHit and mContactReportCallback.laserHitBuilding and canApplyLaserDamage then
        for i = 1, #laser_BuildingHit do
            if Unified == false and laser_BuildingHit[i] == EntID1 and not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
				AudioSystem:PlaySFX("SFX_HitMarker", pEntID)
                eHealth = eHealth - 9
                eFlashTimer = 8
                canApplyLaserDamage = false
                if playerLuaScript:GetEnvironment().laserUlt then
				    laserDmgDelayTimer = 0.4
				else
				    laserDmgDelayTimer = 0.7
				end
                break
            end
        end
    end
    eRigidDynamic.getHit = false
end

function flashOnHit()
    if eFlashTimer > 1 then
            eMaterial._saturation.z = eFlashTimer
    end
end

function randomHue()
    if Unified == false then
            eMaterial._saturation.x = Random.Float() * 360
    end
end

function Unify(dt)
    if Unified == false then
        randomHue()
        Unified = true
        setSatOne()
        eMaterial._saturation.z = 1.5

		if playerLuaScript:GetEnvironment().pHealth < 75 then
			playerLuaScript:GetEnvironment().pHealth = playerLuaScript:GetEnvironment().pHealth + 5
			playerLuaScript:GetEnvironment().healTimer = 0.5
			AudioSystem:PlaySFX("SFX_HealthRegen", pEntID)
		end
    end
end

function handleLaserDmgDelay(dt)
    if laserDmgDelayTimer > 0.0 then
        laserDmgDelayTimer = laserDmgDelayTimer - dt        
    end

    if laserDmgDelayTimer <= 0.0 then
        canApplyLaserDamage = true
    end
end