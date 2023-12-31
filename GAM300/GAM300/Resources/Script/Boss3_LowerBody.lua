local EntID1
local eTransform        = Vector3:new(0.0, 0.0, 0.0)
local forwardPrime      = 1
local laserID
local bulletSpawnPos    = Vector3:new(0.0, 0.0, 0.0)
--Weapons

local timer             = 0
local cTimer            = 0
local coolBool          = false
local step              = 0
local timetoturn        = 0
local turnBool          = false
local eLuaScript
local boss              = {}
local bossScript
local bossHP            = 100
local bossHeadID

local bulletSpawnPos1    = Vector3:new(0.0, 0.0, 0.0)
local bulletSpawnPos2    = Vector3:new(0.0, 0.0, 0.0)

local bulletSpawnPos3    = Vector3:new(0.0, 0.0, 0.0)
local bulletSpawnPos4    = Vector3:new(0.0, 0.0, 0.0)

local bulletSpawnPos5    = Vector3:new(0.0, 0.0, 0.0)
local bulletSpawnPos6    = Vector3:new(0.0, 0.0, 0.0)

local bulletSpawnPos7    = Vector3:new(0.0, 0.0, 0.0)
local bulletSpawnPos8    = Vector3:new(0.0, 0.0, 0.0)

local offse              = Vector3:new(0.0, 0.0, 0.0)

local rotCount           = 0

local shootOncePlease    = false

local bLowerLaser1ID
local bLowerLaser2ID
local bLowerLaser3ID
local bLowerLaser4ID

local laser1ID 
local laser2ID 
local laser3ID 
local laser4ID 

local laser1_transform
local laser1_rigidDynamic
local laser2_transform
local laser2_rigidDynamic
local laser3_transform
local laser3_rigidDynamic
local laser4_transform
local laser4_rigidDynamic

local bossTimeBool
local playerScript

local dissappearBool = false

local boolOffsetOnce = false

local offset1 = Vector3:new()
local offset2 = Vector3:new()
local offset3 = Vector3:new()
local offset4 = Vector3:new()

local currWanted
local playerLuaScript
local bulletRotQuatSP   = Quat:new(0,0,0,1)
local playerID
local tagList2
local tagList3

lowerBodyProp                    = {}

-- Bool to determine if cw or ccw for
-- respective bodyparts
local cwInt     = 0
local cwBool    = false
local s_cwBool  = false
local instantOnce = false

local canPlayLaserSound = true

-- Awake is called once and only once when script is loaded.
function Awake()

		bLowerLaser1ID = ECS:getPrefabID("Prefab_Boss_LowerLaser1")
		bLowerLaser2ID = ECS:getPrefabID("Prefab_Boss_LowerLaser1")
		bLowerLaser3ID = ECS:getPrefabID("Prefab_Boss_LowerLaser1")
		bLowerLaser4ID = ECS:getPrefabID("Prefab_Boss_LowerLaser1")

		-- bossHeadID     = ECS:getEntityID("Boss_Head") 
		--bLowerLaser2ID = ECS:getPrefabID("Prefab_Boss_LowerLaser2")
		--bossScript = ECS:getComponent_LuaScriptCom("Boss_Head")
end

-- Start is called once and only once after Awake() is called.
function Start()
    ----Console.WriteLine("Civilian script loaded")
	--EntID1 = ScriptSystem:getLuaEntityID()
    EntID1 = ECS:getEntityID("3_BossUpperTurr")
    -- Console.PrintEntityID(EntID1)
	playerID = ECS:getEntityID("Player")
	
	lowerBodyProp["lb_env"] = ECS:getComponent_LuaScriptCom(EntID1):GetEnvironment()

	
	
end

-- Updates the Game Object per game loop
function Update(dt)

	tagList2 = ECS:getEntityTagged("boss") 
    for i=0, #tagList2, 1 do
        local id = tagList2[i]
        if id ~= nil then
            bossHeadID = tagList2[i]
        end
    end

	tagList3 = ECS:getEntityTagged("3_BossUpperTurr") 
    for i=0, #tagList3, 1 do
        local id = tagList3[i]
        if id ~= nil then
            EntID1 = tagList3[i]
        end
    end
	
	PhysicsSystem:setGravityFlag(EntID1, false)
	
	lowerBodyProp["lb_env"]["cwBool"]   = cwBool
	lowerBodyProp["lb_env"]["turnBool"] = turnBool
	
	if ECS:hasComponent_LuaScriptCom(playerID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(playerID)
    end
	
	currWanted = playerLuaScript:GetEnvironment().wantedLevel
	
	--Console.WriteLine("Wanted Level")
	--Console.PrintFloat(currWanted)
	

	local barrierCount = ECS:getEntityTagged("Barrier")
    if #barrierCount == 0 then
		--Less than 5 hide it
		if currWanted < 5 then
		
			if ECS:hasComponent_Transform(EntID1) then
			
			local bTransform Vector3:new(0,0,0)
			
			bTransform = ECS:getComponent_Transform(EntID1)
			
			--bTransform.pos.y = -100
			bulletRotQuatSP = PhysicsSystem:convertEulerToQuat(bTransform.rotate)
			PhysicsSystem:setGlobalPose(EntID1, bTransform.pos, bulletRotQuatSP) 
			
			--Console.WriteLine("Not yet")
			--Console.PrintFloat(bTransform.pos.y )
			
			end
			
		end
		
		if currWanted >= 5 then
			if  ECS:hasComponent_Transform(EntID1) then
			
			local bTransform Vector3:new(0,0,0)
			
			bTransform = ECS:getComponent_Transform(EntID1)
			
			--bTransform.pos.y = 2.800
			--bTransform.pos.x = 0.075
			--bTransform.pos.z = 36.030
			
			
			
			bulletRotQuatSP = PhysicsSystem:convertEulerToQuat(bTransform.rotate)
			PhysicsSystem:setGlobalPose(EntID1, bTransform.pos, bulletRotQuatSP) 
			
			--Console.WriteLine("Now")
			
			end
			
		end
		
		--boss["Boss_Env"] = ECS:getComponent_LuaScriptCom("Boss_Head")
		EntID1 = ScriptSystem:getLuaEntityID()
		
		if ECS:hasComponent_LuaScriptCom(bossHeadID) then
			bossScript = ECS:getComponent_LuaScriptCom(bossHeadID)
			--bossHP = bossScript:GetEnvironment().bossHealth
			bossTimeBool = bossScript:GetEnvironment().bossTimeBool
			--Console.PrintFloat(bossHP)
		end
		
		if bossHP > 0  then
				-- timer less than 5 shoot!
			if bossHP > 0 and timer < 5 and coolBool == false then
				timer = timer + dt 
				turnBool = false
				--Console.WriteLine("Wait")
				dissappearBool = true
			end
			
			-- timer more than 5 begin cool down!
			if timer > 5 and coolBool == false then
				coolBool = true
				turnBool = true
				timer = 0
			end
			
			if coolBool then
				if cTimer < 3 then
					cTimer = cTimer + dt 
					if canPlayLaserSound then
						--Console.WriteLine("play")
						AudioSystem:Play3DSFX("SFX_EnemyLaser", EntID1)
						canPlayLaserSound = false
					end
					--Console.WriteLine("Fire")
					dissappearBool = false
				end
			
				if cTimer > 3 then
					coolBool         = false
					cTimer           = 0
					s_cwBool         = true
					AudioSystem:Stop3DSFX("SFX_EnemyLaser", EntID1)
					canPlayLaserSound = true
				end
			end
			
			if s_cwBool == true then
				chooseRotation()
			end
			
			turn(dt)
			
			spawnBullet()
			
			-- EntID1 = ScriptSystem:getLuaEntityID()
		end
		
		
		if dissappearBool == true then
		
			if ECS:hasComponent_Transform(laser1ID) and ECS:hasComponent_RigidDynamic(laser1ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser1_transform         = ECS:getComponent_Transform(laser1ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser1_transform.pos.y   = -100	
				bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser1_transform.rotate)
				PhysicsSystem:setGlobalPose(laser1ID, laser1_transform.pos, bulletRotQuat) 
				--Console.WriteLine("TRUE LASER 1")
				
			end

			if ECS:hasComponent_Transform(laser2ID) and ECS:hasComponent_RigidDynamic(laser2ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser2_transform         = ECS:getComponent_Transform(laser2ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser2_transform.pos.y   = -100	
				bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser2_transform.rotate)
				PhysicsSystem:setGlobalPose(laser2ID, laser2_transform.pos, bulletRotQuat) 
				--Console.WriteLine("TRUE LASER 2")
				
			end

			if ECS:hasComponent_Transform(laser3ID) and ECS:hasComponent_RigidDynamic(laser3ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser3_transform         = ECS:getComponent_Transform(laser3ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser3_transform.pos.y   = -100	
				bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser3_transform.rotate)
				PhysicsSystem:setGlobalPose(laser3ID, laser3_transform.pos, bulletRotQuat2) 
				--Console.WriteLine("TRUE LASER 3")
				
			end

			if ECS:hasComponent_Transform(laser4ID) and ECS:hasComponent_RigidDynamic(laser4ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser4_transform         = ECS:getComponent_Transform(laser4ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser4_transform.pos.y   = -100	
				bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser4_transform.rotate)
				PhysicsSystem:setGlobalPose(laser4ID, laser4_transform.pos, bulletRotQuat2) 
				--Console.WriteLine("TRUE LASER 4")
				
			end
		end
		
		if dissappearBool == false then
		
			if ECS:hasComponent_Transform(laser1ID) and ECS:hasComponent_RigidDynamic(laser1ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser1_transform         = ECS:getComponent_Transform(laser1ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser1_transform.pos.y   = eTransform.pos.y	
				bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser1_transform.rotate)
				PhysicsSystem:setGlobalPose(laser1ID, laser1_transform.pos, bulletRotQuat) 
				--Console.WriteLine("FALSE LASER 1")
			end

			if ECS:hasComponent_Transform(laser2ID) and ECS:hasComponent_RigidDynamic(laser2ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser2_transform         = ECS:getComponent_Transform(laser2ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser2_transform.pos.y   = eTransform.pos.y	
				bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser2_transform.rotate)
				PhysicsSystem:setGlobalPose(laser2ID, laser2_transform.pos, bulletRotQuat) 
				--Console.WriteLine("FALSE LASER 2")
			end

			if ECS:hasComponent_Transform(laser3ID) and ECS:hasComponent_RigidDynamic(laser3ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser3_transform         = ECS:getComponent_Transform(laser3ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser3_transform.pos.y   = eTransform.pos.y	
				bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser3_transform.rotate)
				PhysicsSystem:setGlobalPose(laser3ID, laser3_transform.pos, bulletRotQuat2) 
				--Console.WriteLine("TRUE LASER 3")
				
			end

			if ECS:hasComponent_Transform(laser4ID) and ECS:hasComponent_RigidDynamic(laser4ID) then
				
				eTransform               = ECS:getComponent_Transform(EntID1)
				laser4_transform         = ECS:getComponent_Transform(laser4ID)
				--laser1_transform.rotate       = eTransform.rotate
				laser4_transform.pos.y   = eTransform.pos.y		
				bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser4_transform.rotate)
				PhysicsSystem:setGlobalPose(laser4ID, laser4_transform.pos, bulletRotQuat2) 
				--Console.WriteLine("TRUE LASER 4")
				
			end
		end
	else
		if ECS:hasComponent_Transform(laser1ID) and ECS:hasComponent_RigidDynamic(laser1ID) then
				
			eTransform               = ECS:getComponent_Transform(EntID1)
			laser1_transform         = ECS:getComponent_Transform(laser1ID)
			--laser1_transform.rotate       = eTransform.rotate
			laser1_transform.pos.y   = -100	
			bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser1_transform.rotate)
			PhysicsSystem:setGlobalPose(laser1ID, laser1_transform.pos, bulletRotQuat) 
			--Console.WriteLine("TRUE LASER 1")
			
		end

		if ECS:hasComponent_Transform(laser2ID) and ECS:hasComponent_RigidDynamic(laser2ID) then
			
			eTransform               = ECS:getComponent_Transform(EntID1)
			laser2_transform         = ECS:getComponent_Transform(laser2ID)
			--laser1_transform.rotate       = eTransform.rotate
			laser2_transform.pos.y   = -100	
			bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser2_transform.rotate)
			PhysicsSystem:setGlobalPose(laser2ID, laser2_transform.pos, bulletRotQuat) 
			--Console.WriteLine("TRUE LASER 2")
			
		end

		if ECS:hasComponent_Transform(laser3ID) and ECS:hasComponent_RigidDynamic(laser3ID) then
			
			eTransform               = ECS:getComponent_Transform(EntID1)
			laser3_transform         = ECS:getComponent_Transform(laser3ID)
			--laser1_transform.rotate       = eTransform.rotate
			laser3_transform.pos.y   = -100	
			bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser3_transform.rotate)
			PhysicsSystem:setGlobalPose(laser3ID, laser3_transform.pos, bulletRotQuat2) 
			--Console.WriteLine("TRUE LASER 3")
			
		end

		if ECS:hasComponent_Transform(laser4ID) and ECS:hasComponent_RigidDynamic(laser4ID) then
			
			eTransform               = ECS:getComponent_Transform(EntID1)
			laser4_transform         = ECS:getComponent_Transform(laser4ID)
			--laser1_transform.rotate       = eTransform.rotate
			laser4_transform.pos.y   = -100	
			bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser4_transform.rotate)
			PhysicsSystem:setGlobalPose(laser4ID, laser4_transform.pos, bulletRotQuat2) 
			--Console.WriteLine("TRUE LASER 4")
			
		end
	end
	
end


function destory()
    ECS:addEntityToDestroyQueue(EntID1)
end

function spawnBullet()
    --updateBullettoCamRotation()
	local bulletRotQuat   = Quat:new(0,0,0,1)
	local bulletRotQuat2  = Quat:new(0,0,0,1)
	
	if ECS:hasComponent_Transform(EntID1) then
		eTransform = ECS:getComponent_Transform(EntID1)
	end
	
	if coolBool == false and shootOncePlease == false and bossHP > 0 then
	
	bulletSpawnPos   = eTransform.pos
	--bulletSpawnPos.x = bulletSpawnPos.x
	

	--bulletSpawnPos.x = bulletSpawnPos.x + 5
	--bulletSpawnPos.z = bulletSpawnPos.z + 10
	
	
	-- bulletSpawnPos1    = Vector3:new(0.0, 0.0, 0.0)
	-- bulletSpawnPos2    = Vector3:new(0.0, 0.0, 0.0)

	-- bulletSpawnPos3    = Vector3:new(0.0, 0.0, 0.0)
	-- bulletSpawnPos4    = Vector3:new(0.0, 0.0, 0.0)

	-- bulletSpawnPos5    = Vector3:new(0.0, 0.0, 0.0)
	-- bulletSpawnPos6    = Vector3:new(0.0, 0.0, 0.0)

	-- bulletSpawnPos7    = Vector3:new(0.0, 0.0, 0.0)
	-- bulletSpawnPos8    = Vector3:new(0.0, 0.0, 0.0)
	
	laser1ID = ECS:instantiatePrefab(bLowerLaser1ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat,false, false, false)
	laser2ID = ECS:instantiatePrefab(bLowerLaser2ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat,false, false, false)
	laser3ID = ECS:instantiatePrefab(bLowerLaser3ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat2,false, false, false)
	
	--Stray Laser
	laser4ID = ECS:instantiatePrefab(bLowerLaser4ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat2,false, false, false)
	
	
	--laser2ID = ECS:instantiatePrefab(bLowerLaser2ID, "Prefab_Boss_LowerLaser2", bulletSpawnPos, bulletRotQuat,false, false, false)
		
	shootOncePlease = true
	--Console.WriteLine("SPAWNED")
	
	--Console.WriteLine("LASERSHOOT")
	end
    --AudioSystem:PlaySFX("PlayerLaser_SFX", EntID1)
	
	-- when all lasers already spawned
	if shootOncePlease and coolBool == true and dissappearBool == false then
	
		-- when time to shoot put back at correct Y axis and rotate accordingly
		if ECS:hasComponent_Transform(laser1ID) and ECS:hasComponent_RigidDynamic(laser1ID)
		then
			
			eTransform = ECS:getComponent_Transform(EntID1)			
			laser1_transform       = ECS:getComponent_Transform(laser1ID)
			laser1_transform.rotate     = eTransform.rotate

			laser2_transform	   = ECS:getComponent_Transform(laser2ID)
			laser2_transform.rotate		= eTransform.rotate

			laser3_transform	   = ECS:getComponent_Transform(laser3ID)
			laser3_transform.rotate		= eTransform.rotate
			laser3_transform.rotate.y	= laser3_transform.rotate.y - 90

			laser4_transform	   = ECS:getComponent_Transform(laser4ID)
			laser4_transform.rotate		= eTransform.rotate
			laser4_transform.rotate.y	= laser4_transform.rotate.y - 90
			
			
			 if boolOffsetOnce == false then
				
				--laser 1
				laser1_transform.pos   = eTransform.pos
				laser1_transform.pos.y   = eTransform.pos.y

				laser2_transform.pos   = eTransform.pos
				laser2_transform.pos.y = eTransform.pos.y

				laser3_transform.pos   = eTransform.pos
				laser3_transform.pos.y = eTransform.pos.y

				laser4_transform.pos   = eTransform.pos
				laser4_transform.pos.y = eTransform.pos.y
				
				offset1     = laser1_transform.pos
				offset1.z   = laser1_transform.pos.z - 2  -- to the left

				offset2		= laser2_transform.pos
				offset2.z	= laser2_transform.pos.z + 2

				offset3		= laser3_transform.pos
				offset3.z	= laser3_transform.pos.z - 2

				offset4		= laser4_transform.pos
				offset4.z	= laser4_transform.pos.z + 2

				boolOffsetOnce = true
			 end
			
			-- --LASER 1
			bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser1_transform.rotate)
			PhysicsSystem:setGlobalPose(laser1ID, offset1, bulletRotQuat) 
			
			-- --LASER 2
			bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser2_transform.rotate)
			PhysicsSystem:setGlobalPose(laser2ID, offset2, bulletRotQuat) 

			-- --LASER 3
			bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser3_transform.rotate)
			PhysicsSystem:setGlobalPose(laser3ID, offset3, bulletRotQuat2) 
			
			-- --LASER 4
			bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser4_transform.rotate)
			PhysicsSystem:setGlobalPose(laser4ID, offset4, bulletRotQuat2) 
			
			--Console.PrintFloat(laser1_transform.pos.y)
		
		end
	
	end

	
	
		
end

function spawnBullet2()
    --updateBullettoCamRotation()
	local bulletRotQuat   = Quat:new(0,0,0,1)
	local bulletRotQuat2  = Quat:new(0,0,0,1)
	
	if ECS:hasComponent_Transform(EntID1) then
		eTransform = ECS:getComponent_Transform(EntID1)
	end
	
	if bossHP > 0 and instantOnce == false then
	
	bulletSpawnPos   = eTransform.pos

	
	laser1ID = ECS:instantiatePrefab(bLowerLaser1ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat,false, false, false)
	laser2ID = ECS:instantiatePrefab(bLowerLaser2ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat,false, false, false)
	laser3ID = ECS:instantiatePrefab(bLowerLaser3ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat2,false, false, false)
	
	--Stray Laser
	laser4ID = ECS:instantiatePrefab(bLowerLaser4ID, "Prefab_Boss_LowerLaser1", bulletSpawnPos, bulletRotQuat2,false, false, false)	
	
	--shootOncePlease = true
	
	instantOnce = true
	
	end
	
	-- when time to shoot put back at correct Y axis and rotate accordingly
		if ECS:hasComponent_Transform(laser1ID) and ECS:hasComponent_RigidDynamic(laser1ID)
		then
			
			eTransform = ECS:getComponent_Transform(EntID1)			
			laser1_transform       = ECS:getComponent_Transform(laser1ID)
			laser1_transform.rotate     = eTransform.rotate

			laser2_transform	   = ECS:getComponent_Transform(laser2ID)
			laser2_transform.rotate		= eTransform.rotate

			laser3_transform	   = ECS:getComponent_Transform(laser3ID)
			laser3_transform.rotate		= eTransform.rotate
			laser3_transform.rotate.y	= laser3_transform.rotate.y - 90

			laser4_transform	   = ECS:getComponent_Transform(laser4ID)
			laser4_transform.rotate		= eTransform.rotate
			laser4_transform.rotate.y	= laser4_transform.rotate.y - 90
			
			
			 if boolOffsetOnce == false then
				
				--laser 1
				laser1_transform.pos   = eTransform.pos
				laser1_transform.pos.y   = eTransform.pos.y

				laser2_transform.pos   = eTransform.pos
				laser2_transform.pos.y = eTransform.pos.y

				laser3_transform.pos   = eTransform.pos
				laser3_transform.pos.y = eTransform.pos.y

				laser4_transform.pos   = eTransform.pos
				laser4_transform.pos.y = eTransform.pos.y
				
				offset1     = laser1_transform.pos
				offset1.z   = laser1_transform.pos.z - 2  -- to the left

				offset2		= laser2_transform.pos
				offset2.z	= laser2_transform.pos.z + 2

				offset3		= laser3_transform.pos
				offset3.z	= laser3_transform.pos.z - 2

				offset4		= laser4_transform.pos
				offset4.z	= laser4_transform.pos.z + 2

				boolOffsetOnce = true
			 end
			
			-- --LASER 1
			bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser1_transform.rotate)
			PhysicsSystem:setGlobalPose(laser1ID, offset1, bulletRotQuat) 
			
			-- --LASER 2
			bulletRotQuat = PhysicsSystem:convertEulerToQuat(laser2_transform.rotate)
			PhysicsSystem:setGlobalPose(laser2ID, offset2, bulletRotQuat) 

			-- --LASER 3
			bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser3_transform.rotate)
			PhysicsSystem:setGlobalPose(laser3ID, offset3, bulletRotQuat2) 
			
			-- --LASER 4
			bulletRotQuat2 = PhysicsSystem:convertEulerToQuat(laser4_transform.rotate)
			PhysicsSystem:setGlobalPose(laser4ID, offset4, bulletRotQuat2) 
			
			--Console.PrintFloat(laser1_transform.pos.y)
		
		end

	
	
		
end

function turn(dt)

	if ECS:hasComponent_Transform(EntID1) and turnBool then
	
		 eTransform = ECS:getComponent_Transform(EntID1)
		 
		 --Console.WriteLine("TurnFN")
		
		if cwBool == true then
			--Console.WriteLine("int.True")
			
			if not eTransform.rotate.y == 1 or 
			   not eTransform.rotate.y == 89 or 
			   not eTransform.rotate.y == 179 or
			   not eTransform.rotate.y == 269 or
			   not eTransform.rotate.y == 359
			   then
				
				
				eTransform.rotate.y = eTransform.rotate.y - dt * 100
					--Console.WriteLine("ROTATING")
					
			   else
				eTransform.rotate.y = eTransform.rotate.y - 1
				
			-- end
			end
		end
		
		if cwBool == false then
			--Console.WriteLine("int.False")
			
			if not eTransform.rotate.y == 1 or 
			   not eTransform.rotate.y == 89 or 
			   not eTransform.rotate.y == 179 or
			   not eTransform.rotate.y == 269 or
			   not eTransform.rotate.y == 359
			   then
				
				
				eTransform.rotate.y = eTransform.rotate.y + dt * 100
					--Console.WriteLine("ROTATING")
					
			   else
				eTransform.rotate.y = eTransform.rotate.y + 1
				
			-- end
			end
		end
		
		--Reset
		if eTransform.rotate.y > 360 then
		  eTransform.rotate.y = 0
		end

	end
	
end

function chooseRotation()

	cwInt = Random.Float() * 6
	--Console.PrintFloat(cwBool)
	
	if cwInt > 3 then
		--Console.WriteLine("True")
		cwBool = true
	end
	
	if cwInt < 3 then
		--Console.WriteLine("False")
		cwBool = false
	end
	
	s_cwBool         = false
	
end


