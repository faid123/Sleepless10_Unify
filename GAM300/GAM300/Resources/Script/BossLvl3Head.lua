-- local variables

local playerID
local player_env

local eEntID
-- Component Variables
local bTransform
local bRigidDynamic
local bEmitter
local bMaterial
local eSetEnemyRot  = Vector3:new(0.0, 0.0, 0.0)
local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

-- taret position of where rocket will launch to
local playerTransform
local playerPos

local flashTimer = 1
local canApplyLaserDamage   = true
local laserdmgdelayTime     = 0.7

local UI_Boss1
local UI_Boss2
local bossHPFill
local bossHPContainer
local bossHPbar
local Prefab_UI_BossHealthColor
local Prefab_UI_BossHealthBar
local bossHPVec = Vector3:new(0,0,0)
local bossHPRot = Quat:new(0,0,0,1)
local spawnUIOnce = false

-- for boss barrier
local isFirstBarrier 	= false
local isSecondBarrier 	= false
local isThirdBarrier 	= false
local barrierActive     = false

local barrierPrefabID
local barrierID1
local barrierID2
local barrierID3
local healthTimer = 1.0

-- global variables
bossHealth = 100.0
--Player Table
bossLvlHead = {}

local timer = 0
local turnNow = false

-- Boss Shake
local shakeTimer = 0.0
local shakeTime = 0.5
local startingPos = Vector3:new()
local startPosStored = false

local targetRotation = 0.0
local prevTargetRotation = 0.0

local cutsceneBossDefeatID
local doOnce = false

local laserID
local tagList

-- Awake is called once and only once when script is loaded.
function Awake()
    Prefab_UI_BossHealthColor   = ECS:getPrefabID("Prefab_UI_BossHealthColor")
    Prefab_UI_BossHealthBar     = ECS:getPrefabID("Prefab_UI_BossHealthBar")
    cutsceneBossDefeatID        = ECS:getPrefabID("Prefab_AAABossDefeatCutscene3")
    UI_Boss1 = ECS:instantiatePrefab(Prefab_UI_BossHealthBar, "UI_BossHealthBarLvl2", bossHPVec, bossHPRot, false, false, false)
    UI_Boss2 = ECS:instantiatePrefab(Prefab_UI_BossHealthColor, "UI_BossHealthColorLvl2", bossHPVec, bossHPRot, false, false, false)
    barrierPrefabID = ECS:getPrefabID("Prefab_Barrier")
end

-- Start is called once and only once after Awake() is called.
function Start()
	-- get the player entity id
	playerID = ECS:getEntityID("Player")
    playerTransform = ECS:getComponent_Transform(playerID)
	player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()

    UI_Boss1 = ECS:getEntityID("UI_BossHealthBarLvl2")
    UI_Boss2 = ECS:getEntityID("UI_BossHealthColorLvl2")
    if ECS:hasComponent_SpriteCom(UI_Boss1) then
        local spritecom = ECS:getComponent_SpriteCom(UI_Boss1)
        spritecom._drawSprite = true
        spritecom.SpriteName = "bosshealth_nebulas"
        -- Console.PrintBool(spritecom._drawSprite)
    end
    if ECS:hasComponent_SpriteCom(UI_Boss2) then
        local spritecom = ECS:getComponent_SpriteCom(UI_Boss2)
        spritecom._drawSprite = true
        -- Console.PrintBool(spritecom._drawSprite)
    end
end

-- Updates the Game Object per game loop
function Update(dt)
	
	eEntID = ScriptSystem:getLuaEntityID()
    bossLvlHead["Boss_Env"] = ECS:getComponent_LuaScriptCom(eEntID):GetEnvironment()
    PhysicsSystem:setGravityFlag(eEntID, true)
    tagList = ECS:getEntityTagged("LowerLaser")
    for i=0, #tagList, 1 do
        local id = tagList[i]
        if id ~= nil then
            laserID = tagList[i]
        end
    end

    if Inputs.get_Key_Triggered(InputKey.KEY_5) then
        bossHealth = 0
    end
	
    if ECS:hasEntity(eEntID) then
        bTransform = ECS:getComponent_Transform(eEntID)
        bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
        bEmitter = ECS:getComponent_Emitter(eEntID)
        bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
        bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
        bMaterial = ECS:getComponent_Material(eEntID)

        playerPos = PhysicsSystem:getGlobalPosePosition(playerID)
        bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
		
		bPos.y = 15.759
		
		PhysicsSystem:setGlobalPose(eEntID, bPos, Quat:new(0,0,0,1)) 
		bPos = PhysicsSystem:getGlobalPosePosition(eEntID)

        if not startPosStored then
            startingPos.x = bRigidDynamic.rigidPos.x
            startingPos.y = bRigidDynamic.rigidPos.y
            startingPos.z = bRigidDynamic.rigidPos.z
            startPosStored = true
        end

		
		--Console.PrintFloat(bPos.y)

		
        
        -- if spawnUIOnce == false then
        --     UI_Boss1 = ECS:instantiatePrefab(Prefab_UI_BossHealthBar, "UI_BossHealthBarLvl2", bossHPVec, bossHPRot, false, false, false)
        --     UI_Boss2 = ECS:instantiatePrefab(Prefab_UI_BossHealthColor, "UI_BossHealthColorLvl2", bossHPVec, bossHPRot, false, false, false)
        --     if ECS:hasComponent_SpriteCom(UI_Boss1) then
        --         local spritecom = ECS:getComponent_SpriteCom(UI_Boss1)
        --         spritecom._drawSprite = true
        --         -- Console.PrintBool(spritecom._drawSprite)
        --     end
        --     if ECS:hasComponent_SpriteCom(UI_Boss2) then
        --         local spritecom = ECS:getComponent_SpriteCom(UI_Boss2)
        --         spritecom._drawSprite = true
        --         -- Console.PrintBool(spritecom._drawSprite)
        --     end

        --     spawnUIOnce = true
        -- end
        --local bRotTemp = bRotNew commented
        --bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)  commented
        bossLvlHead["Boss_Env"]["bossHealth"] = bossHealth

        if timer < 5 then
            timer = timer + dt * 10
            turnNow = false
        end
        
        if timer > 5 then
            timer = 0
            turnNow = true
        end
        
        -- Turn only after delate
        if ECS:hasComponent_Transform(eEntID) then
            bTransform = ECS:getComponent_Transform(eEntID)
            bTransform.rotate = eSetEnemyRot
        end

        spawnBarrier()
        increaseBossHealth(dt)
        handleLaserDelay(dt)
        updateBossRot()
        lookTowards(eEntID, playerID)
        flashBoss(dt)
        
        -- BOSS SHAKE LOGIC
        handleShakeTimer(dt)
        bossShake()

        onHit()
        updateUI()
        -- Console.WriteLine("Boss Health")
        -- Console.WriteLine(bossHealth)
    end
    if (bossHealth < 1) then
        updateUI()
        shakeTimer = shakeTime
        updateParticleColors()
        bEmitter.emit = true

        if doOnce == false then
            ECS:instantiatePrefab(cutsceneBossDefeatID, "AAABossDefeatCutscene3", Vector3:new(0,0,1),Quat:new(0,0,0,1),false, false, false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("final_ending.mp4")
            RenderSystem:setVideoEndTime(46.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(false)
            doOnce = true
        end
        -- UISystem:setGameState(6)
    end
end

function updateUI()
    -- Realtime update health in relation to boss HP fill
	if ECS:hasComponent_SpriteCom(UI_Boss2) then
		bossHPbar = ECS:getComponent_SpriteCom(UI_Boss2)
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

-- On hit function
function onHit()

    --if bRigidDynamic.getHit and mContactReportCallback.bulletHitBoss then
    if not barrierActive then
        if  bRigidDynamic.getHit and mContactReportCallback.bulletHitBoss then
            if bossHealth > 0 then
                AudioSystem:PlaySFX("SFX_HitMarker", playerID)
                bossHealth = bossHealth - 2
                flashTimer = 8
                shakeTimer = shakeTime
	    		--Console.WriteLine("HitB")
                --mContactReportCallback.bulletHitBoss = false
                --bRigidDynamic.getHit = false
            end
        elseif bRigidDynamic.getHit and mContactReportCallback.lazerHitBoss and canApplyLaserDamage then
            if not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
                if bossHealth > 0 then
                    AudioSystem:PlaySFX("SFX_HitMarker", playerID)
                    bossHealth = bossHealth - 6
                    flashTimer = 8
                    if player_env.laserUlt then
                        laserdmgdelayTime = 0.4
                    else
                        laserdmgdelayTime = 0.7
                    end
                    shakeTimer = shakeTime
                    canApplyLaserDamage = false
	    	    	--Console.WriteLine("HitL"
                end
            end
        elseif bRigidDynamic.getHit and mContactReportCallback.explosionHitBoss then
            print("explosionHitBoss")
            if bossHealth > 0 then
                AudioSystem:PlaySFX("SFX_HitMarker", playerID)
                bossHealth = bossHealth - 8
                flashTimer = 8
                shakeTimer = shakeTime
	    		--Console.WriteLine("HitR")
                --mContactReportCallback.rocketHitBoss = false
                --bRigidDynamic.getHit = false
            end
        end
    end
    bRigidDynamic.getHit = false

	 --if bRigidDynamic.getHit then
		--Console.WriteLine("HitD") 
	 --end
	
	--Console.WriteLine("go through")
    --eRigidDynamic.getHit = false
end

function flashBoss(dt)
    if flashTimer > 1.5 then
        flashTimer = flashTimer - dt*10
        flashOnHit()
    end
end

function flashOnHit()
    if flashTimer > 1 then
        bMaterial._saturation.z = flashTimer
    end
end


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

function spawnBarrier()
	if bossHealth <= 75.0 and isFirstBarrier == false and isSecondBarrier == false and isThirdBarrier == false then
		barrierID1 = ECS:instantiatePrefab(barrierPrefabID, "Barrier", Vector3:new(-1.269,16.0,36.213), Quat:new(0,0,0,1), false, false, false)
        AudioSystem:MuteSound("SFX_EnemyLaser")
        if ECS:hasComponent_Transform(barrierID1) == true then
            local transform = ECS:getComponent_Transform(barrierID1)
            transform.pos = Vector3:new(-1.269,16.0,36.213)
        end
        Console.WriteLine("Spawned first Barrier")
		isFirstBarrier = true
        barrierActive = true
	elseif bossHealth <= 50.0 and isFirstBarrier == true and isSecondBarrier == false and isThirdBarrier == false then
		barrierID2 = ECS:instantiatePrefab(barrierPrefabID, "Barrier", Vector3:new(-1.269,16.0,36.213), Quat:new(0,0,0,1), false, false, false)
        AudioSystem:MuteSound("SFX_EnemyLaser")
        if ECS:hasComponent_Transform(barrierID2) == true then
            local transform = ECS:getComponent_Transform(barrierID2)
            transform.pos = Vector3:new(-1.269,16.0,36.213)
        end
		Console.WriteLine("Spawned second Barrier")
        isSecondBarrier = true
        barrierActive = true
	elseif bossHealth <= 25.0 and isFirstBarrier == true and isSecondBarrier == true and isThirdBarrier == false  then
		barrierID3 = ECS:instantiatePrefab(barrierPrefabID, "Barrier", Vector3:new(-1.269,16.0,36.213), Quat:new(0,0,0,1), false, false, false)
        AudioSystem:MuteSound("SFX_EnemyLaser")
        if ECS:hasComponent_Transform(barrierID3) == true then
            local transform = ECS:getComponent_Transform(barrierID3)
            transform.pos = Vector3:new(-1.269,16.0,36.213)
        end
		Console.WriteLine("Spawned third Barrier")
        isThirdBarrier = true
        barrierActive = true
	end
end

function increaseBossHealth(dt)
    healthTimer = healthTimer - dt
    if isFirstBarrier == true and isSecondBarrier == false and isThirdBarrier == false then
        local barrierCount = ECS:getEntityTagged("Barrier")
        if #barrierCount >= 1 then
            if healthTimer <= 0.0 then
                if bossHealth <= 100.0 then
                    bossHealth = bossHealth + 1.5
                end
                healthTimer = 1.0
            end
        else
            barrierActive = false
        end
    elseif isSecondBarrier == true and isFirstBarrier == true and isThirdBarrier == false then
        local barrierCount = ECS:getEntityTagged("Barrier")
        if #barrierCount >= 1 then
            if healthTimer <= 0.0 then
                if bossHealth <= 100.0 then
                    bossHealth = bossHealth + 1.5
                end
                healthTimer = 1.0
            end
        else
            barrierActive = false
        end
    elseif isThirdBarrier == true and isFirstBarrier == true and isSecondBarrier == true then
        local barrierCount = ECS:getEntityTagged("Barrier")
        if #barrierCount >= 1 then
            if healthTimer <= 0.0 then
                if bossHealth <= 100.0 then
                    bossHealth = bossHealth + 1.5
                end
                healthTimer = 1.0
            end
        else
            barrierActive = false
        end
    end
end

function bossShake()
    if shakeTimer > 0.0 then
        bRigidDynamic.rigidPos.x = startingPos.x + Random.randInRange(-1.0, 1.0) * shakeTimer
        --bRigidDynamic.rigidPos.y = startingPos.y + Random.randInRange(-1.0, 1.0) * shakeTimer
        bRigidDynamic.rigidPos.z = startingPos.z + Random.randInRange(-1.0, 1.0) * shakeTimer
		PhysicsSystem:setGlobalPose(eEntID, bRigidDynamic.rigidPos, Quat:new(0,0,0,1)) 
    else
        bRigidDynamic.rigidPos.x = startingPos.x
        bRigidDynamic.rigidPos.y = startingPos.y
        bRigidDynamic.rigidPos.z = startingPos.z
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