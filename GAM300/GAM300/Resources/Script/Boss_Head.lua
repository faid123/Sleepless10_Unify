local pEntID
local eSetEnemyRot      = Vector3:new(0.0, 0.0, 0.0)
local bTransform   		= Vector3:new(0.0, 0.0, 0.0)
local bRigidBod  		= Vector3:new(0.0, 0.0, 0.0)
local timer             = 0
local turnNow           = false

local flashTimer        = 1

local bossHealth        = 100
local bossTimeBool      = false
boss                    = {}

local bMaterial        
local bRigidDynamic     

local laserdmgdelayTime     = 0.7
local canApplyLaserDamage   = true
local shootDelayTimer       = 0
local bCanShoot             = false
local shootDelayTime  		= 0.5

local defeatcutsceneID
local frontbgcutsceneID

local x = 0
local y = 0
local z = 0

local cOnce = false

local currWanted
local playerLuaScript

local bulletRotQuat   = Quat:new(0,0,0,1)

local uiBossFillID

local UIObjID

local tagList
local stayOnce = false

-- Boss Shake
local shakeTimer = 0.0
local shakeTime = 0.5
local startingPos = Vector3:new()
local startPosStored = false

local bEmitter

local targetRotation = 0.0
local prevTargetRotation = 0.0

-- Awake is called once and only once when script is loaded.
function Awake()
	
	--cutSceneID3 = 1
	--AAABossDefeatCutscene
	defeatcutsceneID = ECS:getPrefabID("Prefab_AAABossDefeatCutscene")  -- 0
	frontbgcutsceneID = ECS:getPrefabID("Prefab_AAFrontBGCutscene") -- 3
	
	--Console.PrintFloat(defeatcutsceneID)
	--Console.PrintFloat(frontbgcutsceneID)
	
end

-- Start is called once and only once after Awake() is called.
function Start()
    ----Console.WriteLine("Civilian script loaded")
     --EntID1 = ECS:getEntityID("Boss_Head")
	 EntID2 = ECS:getEntityID("Boss_UpperBody")
	 EntID3 = ECS:getEntityID("Boss_LowerBody")
	 
	 --boss["Boss_Env"] = ECS:getComponent_LuaScriptCom(EntID1):GetEnvironment()
	 UIObjID = ECS:getEntityID("UI_Challenge")
	 uiBossFillID       = ECS:getEntityID("UI_BossHealthColor")
	 
	 UIObjID = ECS:getEntityID("UI_Challenge")
    -- Console.PrintEntityID(EntID1)
end

-- Updates the Game Object per game loop
function Update(dt)

	if Inputs.get_Key_Triggered(InputKey.KEY_5) then
		bossHealth = 0
    end
	
	tagList = ECS:getEntityTagged("Boss") 
    for i=0, #tagList, 1 do
        local id = tagList[i]
        if id ~= nil then
            EntID1 = tagList[i]
        end
    end
	
	boss["Boss_Env"] = ECS:getComponent_LuaScriptCom(EntID1):GetEnvironment()
	
	--EntID1 = ScriptSystem:getLuaEntityID()
	PhysicsSystem:setGravityFlag(EntID1, true)
	
	boss["Boss_Env"]["bossHealth"] = bossHealth
	
	
	
	pEntID = ECS:getEntityID("Player")
	updateBossRot()
	lookTowards(EntID1, pEntID)
	
	--UI stuff
	--[[
	if ECS:hasComponent_SpriteCom(uiBossFillID) then
		local sprite = ECS:getComponent_SpriteCom(uiBossFillID)
		sprite.barPercentage = bossHealth
		--Console.PrintFloat(barPercentage)
	end]]
	
	if ECS:hasComponent_LuaScriptCom(pEntID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(pEntID)
    end
	
	bMaterial = ECS:getComponent_Material(EntID1)
	bRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
	bEmitter = ECS:getComponent_Emitter(EntID1)

    if not startPosStored then
        startingPos.x = 0.075
        startingPos.y = 12.459
        startingPos.z = 36.060
        startPosStored = true
    end

    -- BOSS SHAKE LOGIC
    handleShakeTimer(dt)
    bossShake()

	if ECS:hasComponent_RigidDynamic(EntID1) and 
		ECS:hasComponent_Transform(EntID1) then
		
        bRigidBod = ECS:getComponent_RigidDynamic(EntID1)
		
        bRigidBod.rigidPos.x = x
		bRigidBod.rigidPos.y = y
		bRigidBod.rigidPos.z = z
		
		bTransform = ECS:getComponent_Transform(EntID1)
		
        bTransform.pos.x = x
		bTransform.pos.y = y
		bTransform.pos.z = z
		
		local velo = Vector3:new(0.0, 0.0, 0.0)
		PhysicsSystem:setBodyLinearVelocity(EntID1, velo, 0)
		
		
	end
	
	
	
	-- included a timer because the update causes
	-- the rotation to make the head fidget
	if timer < 5 then
		timer = timer + dt * 10
		turnNow = false
	end
	
	if timer > 5 then
		timer = 0
		turnNow = true
	end

    handleLaserDelay(dt)
	
	-- Turn only after delate
	 if ECS:hasComponent_Transform(EntID1) then
	   bTransform = ECS:getComponent_Transform(EntID1) 
	   bTransform.rotate = eSetEnemyRot
	end
	
	-- Boss health update globally
	--boss["Boss_Env"]["bossHP"] = bossHealth
	
	if flashTimer > 1.5 then
        flashTimer = flashTimer - dt*10
        flashOnHit()
    end
	
	onHit()
	updateUI()


	if bossHealth < 1 and cOnce == false then
		updateUI()
		shakeTimer = shakeTime
        updateParticleColors()
        bEmitter.emit = true

		local wan = Quat:new(0,0,0,1)
		local pos2 = Vector3:new(0,0,1)
		
		
		defeatcutsceneID = ECS:getPrefabID("Prefab_AAABossDefeatCutscene")  -- 0
		frontbgcutsceneID = ECS:getPrefabID("Prefab_AAFrontBGCutscene") -- 3
		
		ECS:instantiatePrefab(defeatcutsceneID, "AAABossDefeatCutscene", pos2,wan,false, false, false)
		-- ECS:instantiatePrefab(frontbgcutsceneID, "AAFrontBGCutscene", pos2,wan,false, false, false)
		
		RenderSystem:setVideoStart(true)
		RenderSystem:setVideoName("weapon_laser.mp4")
		RenderSystem:setVideoEndTime(15.0)
		RenderSystem:setPlayVideo(true)
        RenderSystem:setVideoMinimapAfter(false)
		--Console.WriteLine("video done loading")
		cOnce = true
		
		
	end
	
	
	--Console.PrintFloat(bossTimeBool)
	--Console.PrintFloat(bossHealth)
	--Console.PrintFloat(playerLuaScript:GetEnvironment().wantedLevel)
	--Console.WriteLine("wanzies")
	currWanted = playerLuaScript:GetEnvironment().wantedLevel
	--Console.PrintFloat(currWanted)
	
	
	--Less than 5 hide it
	if currWanted < 5 then
		if ECS:hasComponent_RigidDynamic(EntID1) and 
		ECS:hasComponent_Transform(EntID1) then
		
		bTransform = ECS:getComponent_Transform(EntID1)
		bTransform.pos.y = -100
		bulletRotQuat = PhysicsSystem:convertEulerToQuat(bTransform.rotate)
		PhysicsSystem:setGlobalPose(EntID1, bTransform.pos, bulletRotQuat) 
		
		
		end
		
	end
	
	if currWanted >= 5 and stayOnce == false and ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().allObjectivesCleared then
		if ECS:hasComponent_RigidDynamic(EntID1) and 
		ECS:hasComponent_Transform(EntID1) then
		
		bTransform = ECS:getComponent_Transform(EntID1)
		bTransform.pos.x = 0.075	
		bTransform.pos.y = 12.459
		bTransform.pos.z = 36.060
		
		
		
		bulletRotQuat = PhysicsSystem:convertEulerToQuat(bTransform.rotate)
		PhysicsSystem:setGlobalPose(EntID1, bTransform.pos, bulletRotQuat) 

		stayOnce = true;

		Console.WriteLine("Stay Once!")
		
		
		end
		
	end
	
	

	if bossHealth <= 0 then
		-- ECS:addEntityToDestroyQueue(EntID1)
		-- ECS:addEntityToDestroyQueue(EntID2)
		-- ECS:addEntityToDestroyQueue(EntID3)
		
		-- PhysicsSystem:LuaRemoveActor(EntID1)
		-- PhysicsSystem:LuaRemoveActor(EntID2)
		-- PhysicsSystem:LuaRemoveActor(EntID3)
		
	end

end


function destory()
    
	
end

function updateUI()
    -- Realtime update health in relation to boss HP fill
	if ECS:hasComponent_SpriteCom(uiBossFillID) then
		bossHPbar = ECS:getComponent_SpriteCom(uiBossFillID)
		--bossHPbar.Bar Percentage = bossHealth/100 * 100
		-- Console.PrintFloat(bossHPbar.barPercentage)
		if bossHealth >= 0 then
			bossHPbar.barPercentage = bossHealth/100 * 100
			--Console.PrintFloat(bossHPbar.barPercentage)
        else
            bossHPbar.barPercentage = 0
		end
		
	end
end

-- Ent1 look at ent2 function
function lookTowards(EntID1, EntID2)
    -- Getting player id and their position
    if ECS:hasComponent_RigidDynamic(EntID2) then
        pRigidDynamic = ECS:getComponent_RigidDynamic(EntID2)
        ent2Pos = pRigidDynamic.rigidPos
    end
    
    -- Adding force towards the player direction
    if ECS:hasComponent_RigidDynamic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        ent1Pos = eRigidDynamic.rigidPos
        
        targetRotation = radianTopi(atan2(ent1Pos.z - ent2Pos.z, ent1Pos.x - ent2Pos.x))
        --Trigo.floatToint(targetRotation)

		if targetRotation ~= prevTargetRotation then
			rotationDifference = eSetEnemyRot.y + targetRotation -90
			--rotationDifference = rotationDifference % 360.0
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
		end
		prevTargetRotation = targetRotation
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

    return 0.0
end

-- Convert radian to pi
function radianTopi(x)
    return 180/Trigo.pi()*x
end

function handleLaserDelay(dt)
    if laserdmgdelayTime > 0.0 then
        laserdmgdelayTime = laserdmgdelayTime - dt
        mContactReportCallback.lazerHitBoss = false
        bRigidDynamic.getHit = false
    end

    if laserdmgdelayTime <= 0.0 then
        canApplyLaserDamage = true
    end
end

-- Flashing on hit function
function flashOnHit()
    if flashTimer > 1 then
            bMaterial._saturation.z = flashTimer
    end
end

-- On hit function
function onHit()

    --if bRigidDynamic.getHit and mContactReportCallback.bulletHitBoss then
    if mContactReportCallback.bulletHitBoss then
        if bossHealth > 0 then
			AudioSystem:PlaySFX("SFX_HitMarker", pEntID)
            bossHealth = bossHealth - 2
            flashTimer = 8
			--Console.WriteLine("HitB")
			shakeTimer = shakeTime
            mContactReportCallback.bulletHitBoss = false
            --bRigidDynamic.getHit = false
        end
    elseif bRigidDynamic.getHit and mContactReportCallback.lazerHitBoss and canApplyLaserDamage then
        if bossHealth > 0 then
			AudioSystem:PlaySFX("SFX_HitMarker", pEntID)
            bossHealth = bossHealth - 6
            flashTimer = 8
            laserdmgdelayTime = 0.7
            canApplyLaserDamage = false
			shakeTimer = shakeTime
			--Console.WriteLine("HitL")
            
            
        end
    elseif bRigidDynamic.getHit and mContactReportCallback.rocketHitBoss then
        if bossHealth > 0 then
			AudioSystem:PlaySFX("SFX_HitMarker", pEntID)
            bossHealth = bossHealth - 8
            flashTimer = 8
			shakeTimer = shakeTime
			--Console.WriteLine("HitR")
            mContactReportCallback.rocketHitBoss = false
            bRigidDynamic.getHit = false
        end
    end
	
	 if bRigidDynamic.getHit then
		--Console.WriteLine("HitD") 
	 end
	
	--Console.WriteLine("go through")
    --eRigidDynamic.getHit = false
end

function stay(x,y,z)
	
	if ECS:hasComponent_RigidDynamic(EntID1) and 
		ECS:hasComponent_Transform(EntID1) then
		
        bRigidBod = ECS:getComponent_RigidDynamic(EntID1)
		
        bRigidBod.rigidPos.x = x
		bRigidBod.rigidPos.y = y
		bRigidBod.rigidPos.z = z
		
		bTransform = ECS:getComponent_Transform(EntID1)
		
        bTransform.pos.x = x
		bTransform.pos.y = y
		bTransform.pos.z = z
		
		local velo = Vector3:new(0.0, 0.0, 0.0)
		PhysicsSystem:setBodyLinearVelocity(EntID1, velo, 0)
		
		--Console.PrintFloat(x)
		--Console.PrintFloat(y)
		--Console.PrintFloat(z)
		
	end
	

end

function bossShake()
    if shakeTimer > 0.0 then
        bRigidDynamic.rigidPos.x = startingPos.x + Random.randInRange(-1.0, 1.0) * shakeTimer
        --bRigidDynamic.rigidPos.y = startingPos.y + Random.randInRange(-1.0, 1.0) * shakeTimer
        bRigidDynamic.rigidPos.z = startingPos.z + Random.randInRange(-1.0, 1.0) * shakeTimer
		PhysicsSystem:setGlobalPose(EntID1, bRigidDynamic.rigidPos, Quat:new(0,0,0,1)) 
    else
        bRigidDynamic.rigidPos.x = 0.075
        bRigidDynamic.rigidPos.y = 12.459
        bRigidDynamic.rigidPos.z = 36.060
    end
end

function handleShakeTimer(dt)
    if shakeTimer > 0.0 then
        shakeTimer = shakeTimer - dt
    end
end

function updateParticleColors()
    bEmitter.colorBegin.x = Random.Float()
    bEmitter.colorBegin.y = Random.Float()
    bEmitter.colorBegin.z = Random.Float()
    
    bEmitter.colorEnd.x = Random.Float()
    bEmitter.colorEnd.y = Random.Float()
    bEmitter.colorEnd.z = Random.Float()
end

function updateBossRot()
    
    if eSetEnemyRot.y > 360 then
        eSetEnemyRot.y = 0
    elseif eSetEnemyRot.y < 0 then
        eSetEnemyRot.y = 360
    end

    local newPos = PhysicsSystem:getGlobalPosePosition(EntID1)
    local eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
        
    PhysicsSystem:setGlobalPose(EntID1, newPos, eRotQuat)
end
