local timer = 4
local delayTime = 3
local constTimer = 4
local count = 0
local maxCount
local startPos =  Vector3:new()
local endPos =  Vector3:new()

local waypoints = {}
local EntID1
local ePosNew
local eSpeedCap = 15.0
local eWalkForce = 50
local addForce =  Vector3:new(0.0, 0.0, 12.0)

local changeDirection = false
local theEnd = false
local eRot 
local eRotQuat

local angle = 90;

local eRigidDynamic
local eTransform

local aEposNew = 0
local aEposEnd = 0
local tmp = 0
local newPos
local eSetCarRot = Vector3:new(0.0, 0.0, 0.0)

-- Awake is called once and only once when script is loaded.
function Awake()
end

-- Start is called once and only once after Awake() is called.
function Start()
    ----Console.WriteLine("Civilian script loaded")
     EntID1 = ScriptSystem:getLuaEntityID()
    -- Console.PrintEntityID(EntID1)
end

function updateMovement()
	
	
    -- Adding force towards the player direction
     EntID1 = ScriptSystem:getLuaEntityID()
     --Console.PrintEntityID(EntID1)
    if ECS:hasComponent_RigidDynamic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        eTransform = ECS:getComponent_Transform(EntID1)
        eRot =  eRigidDynamic.rigidRot
        ePosNew = PhysicsSystem:getGlobalPosePosition(EntID1)
	
		
		startPos = ResourceManager:getWayPointVector3("A",count);
		-- Check if already past the way point
		-- if .z below 1 make it positive, then compare
		if ePosNew.z < 0 then
			aEposNew = ePosNew * -1			
		end
		
		if endPos.z < 0 then
			aEposEnd = endPos * -1			
		end
		
		if ePosNew.z > 0 then
			aEposNew = ePosNew		
		end
		
		if endPos.z > 0 then
			aEposEnd = endPos		
		end
		
		if aEposNew.z < aEposEnd.z then
			--Console.WriteLine("Walking")
			
		end
		
		if aEposNew.z >= aEposEnd.z then
			--Console.WriteLine("Stop")
			PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, 0)  -- this line causes exeption when car reaches dest
		end
		
		if aEposNew.z >= aEposEnd.z and tmp < maxCount then
		
			--Console.WriteLine("Still have chekpoint")
			tmp = tmp +1
			endPos = ResourceManager:getWayPointVector3("A",tmp);
			startPos = ResourceManager:getWayPointVector3("A",tmp-1);
			enemyVeloctiy = PhysicsSystem:getBodyLinearVelocity(EntID1)
			PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, eSpeedCap)
			addForce.x = 0
			addForce.z = 0
			
		end
		
		newPos = PhysicsSystem:getGlobalPosePosition(EntID1)
		 
		--local distance = Vector3.Length(endPos - newPos)
		
		--Console.PrintFloat(distance)
		--Console.PrintFloat(tmp)
		
		-- if distance <= 5  and tmp == maxCount then
			-- Console.WriteLine("Stop")
			-- endPos = ResourceManager:getWayPointVector3("A",tmp);
			-- PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, 0)
		-- end
     
		--Speed
         addForce =   Vector3.Normalized(endPos - startPos) 
		 Console.WriteLine("Check position")
		
		-- Console.PrintFloat(addForce.x)
		-- Console.PrintFloat(addForce.z)
        -- Console.PrintFloat(addForce.y)
        -- Console.PrintFloat(eRotNew.y)
    end

       -- PhysicsSystem:addForceToBody(EntID1, Vector3:new(addForce.x, 0, addForce.z), eWalkForce)
	   
	   -- keep it floating
	   if ePosNew.y < 10 then
			PhysicsSystem:addForceToBody(EntID1, Vector3:new(addForce.x, 0.7, addForce.z), eWalkForce)
	   end
	  
	   
       -- enemyVeloctiy = PhysicsSystem:getBodyLinearVelocity(EntID1)
       -- PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, eSpeedCap)

    
       -- eRotQuat = PhysicsSystem:convertEulerToQuat(eRotNew)
        -- PhysicsSystem:setGlobalPose(EntID1,  ePosNew, eRotQuat)
         -- eTransform.rotate = eRotNew
         -- Console.PrintFloat(eRot.y)
       -- eRigidDynamic.rigidRot = eRotQuat
end

function destory()
    ECS:addEntityToDestroyQueue(EntID1)
end

-- Updates the Game Object per game loop
function Update(dt)
    waypoints = ResourceManager:GetWayPoint("A")
    maxCount = #waypoints - 1

	if count == 0 then
		tmp = count +1
		startPos =  ResourceManager:getWayPointVector3("A",count);
		endPos = ResourceManager:getWayPointVector3("A",tmp);
	end

    if timer <= 0 then 
        timer =  constTimer
    else
        timer = timer - dt
    end

    if theEnd == false then 
         updateMovement()
    else 
        destory()
	end 
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

-- Updates the rigidbody on car rotation
--function updateEnemyRot()
--    
--    if eSetCarRot.y > 360 then
--        eSetCarRot.y = 0
--    elseif eSetCarRot.y < 0 then
--        eSetCarRot.y = 360
--    end
--    
--    eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
--        
--    PhysicsSystem:setGlobalPose(EntID1, newPos, eRotQuat)
--end
--
---- Updates the rotation vector quaternion
--function enemyRotateVectorByQuaternion()
--    newRot = PhysicsSystem:getGlobalPoseRotation(eEntID)
--
--    PhysicsSystem:rotate_vector_by_quaternion(forward, newRot, forwardPrime)
--    PhysicsSystem:rotate_vector_by_quaternion(backward, newRot, backwardPrime)
--    PhysicsSystem:rotate_vector_by_quaternion(leftward, newRot, leftwardPrime)
--    PhysicsSystem:rotate_vector_by_quaternion(rightward, newRot, rightwardPrime)
--end