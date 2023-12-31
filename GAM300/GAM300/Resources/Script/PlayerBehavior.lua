
--Variables
local pTransform
local pRigidDynamic
local pCam
local pMaterial
local pAnim
pHealth       = 0
local eEntID
local pBulletPrefabID
local pRocketPrefabID
local pLaserPrefabID
local pWeaponPrefabID
local pFlashTimer
local EnemyID
local pAmmoPrefabID
local WeaponID
local weaponTransform
local toggleWeapon = 1
local inputCounter = 0
local laserUnlocked = false
local rktUnlocked = false
local bEmitter


local healthUIspawned = false
local healingID 
local healingSprite

local UltiID
local UltiUISprite
-- Main Camera variables
local cameraEntID
local camCom

-- Ray Variables
local rayRot
local rayQuat
local rayForwardPrime = Vector3:new()
local rayEnd          = Vector3:new()
local actorID


local shootDelayTimer = 0.0
local pistolFirerate  = 0.25
local laserFirerate   = 0.7
local rocketFirerate  = 0.7
local dashDelayTimer  = 1.0
local dashDelayTime   = 1.0
local speedCap        = 20.0
local maxDist         = 100.0
local walkForce       = 22.5
local dashForce
local dashLifetime    = 10.0
local rightwardOffset = 1
local rocketRightOffset = 1.5
local forwardOffset   = 1.5
local laserAmmo       = 20
local rocketAmmo      = 15
local wAnim 

local vector = Vector3:new(0.0, 5.0, 0.0)

local forward    = Vector3:new(0.0, 0.0, -1.0)
local backward   = Vector3:new(0.0, 0.0, 1.0)
local leftward   = Vector3:new(-1.0, 0.0, 0.0)
local rightward  = Vector3:new(1.0, 0.0, 0.0)
local upward     = Vector3:new(0.0, 1.0, 0.0)
local downward     = Vector3:new(0.0, -1.0, 0.0)

local forwardPrime       = Vector3:new()
local backwardPrime      = Vector3:new()
local leftwardPrime      = Vector3:new()
local rightwardPrime     = Vector3:new()
local laserPrime         = Vector3:new()
local playerVeloctiy     = Vector3:new()
local newPos             = Vector3:new()
local camRot             = Vector3:new()
local bulletRot          = Vector3:new()
local weaponSpawnPos     = Vector3:new()
local weaponUpwardPrime  = Vector3:new()
local playerCamRot       = Vector3:new(0.0, 0.0, 0.0)
local unitDir            = Vector3:new(0.0, 1.0, 0.0)
local origin             = Vector3:new(0.0, 10.0, 0.0)
local jumpForce          = 50000.0

local newRot          = Quat:new()
local playerRotQuat   = Quat:new()
local bulletRotQuat   = Vector3:new()

local pCanShoot       = true
local pCanDash        = true
local pIsAlive        = true
local pInvulnerable   = false
local pIsWalking      = false
local weaponSpawned   = false
--local rAmmo           
rAmmo = 10
--local lAmmo           
lAmmo = 20
--local currPower       = 19.53
currPower       = 19.53

local ammoDistance 
local powerDistance
local rcubeEntityID   = {}
local lcubeEntityID   = {}
local pCubeEntityID   = {}
local aCubeTransform
local pCubeTransform
local lCubeTransform


-- for weapon unlock
local dEntID
local dTransform
local dRigidStatic

local laserID
local laserTransform
local laserRigidStatic
local laserType
local laserExist = true

local dnewPos    = Vector3:new()
local dnewRot    = Quat:new()

local dspawned = false
local despawnTimerDrop

local dspawned2 = false
local despawnTimerDrop2

local rocketDropDistance
local laserDropDistance

local droptype
local dropExist = true
local unlockedWeapons = {}
local rocket = "rocket"
local laser = "laser"

local lazerList   = {}
local nLazer
local rocketList  = {}
local nRocket

-- end of weapon unlock
local pTimer      = 5
local startTimer  = false

local timer = 0;
local playerCamID ;

local isSpawnBoss = false
local bossHeadID = 0
local bossLowerBodyID= 0
local bossUpperBodyID = 0
local bossOnce        = false

local bossHeadTransform    = Vector3:new()
local bossUpperTransform   = Vector3:new()
local bossLowerTransform   = Vector3:new()
local bossScript
local bossTimeBool

-- for wanted level
local wantedUIID
wantedLevel = 1

local cutSceneID1
local cutSceneID2
local cutSceneID3

local countdown = 0

--Boss level1 stuff by wan

local UI_Boss1
local UI_Boss2
local bossHPFill
local bossHPContainer
local bossHPbar

-- end

-- for spawning of boss code (instantiating once)
local spawnBoss = false
local currentStageLevel
local bossLevel2PanelPrefabID
local bossLevel2BodyPrefabID
local bossLevel2PrefabID3
local bossLevel2PrefabID1
local bossLevel2PrefabID2
local bossLevel2PrefabID3
local bossLevel2CutsceneID
local bossLevel3CutsceneID
--boss ui
local uiBossBarID
local uiBossFillID

--bossLevel3
local boss3_headID
local boss3_upperTurrID
local boss3_lowerTurrID
local boss3_Weap1ID
local boss3_Weap2ID


-- for Cutscene
local CutsceneBool

local rocketDelayTimer = 1.5
canGetDamagedByExplosion = true

local maxFOVCamera = 30
local orginalFOV = 0

-- Objective Text 
local UIObjID

--playerUI
local UI_pHealthID
local UI_pHealthBarID

healTimer = 0


--Healing and Ulti ui
local pHealingUI
local pUltiUI


--AmmoScript Logic Var
local UI_AmmoCounterID
local UI_AmmoCounterID1

--Bool Count
local rocketCounter  = false
local laserCounter   = false

local scrollTrigger = false
local scrollCounter = 1

-- Health Shake
local shakeTimer = 0.0
local shakeTime = 0.5
local H_BarStartPos = Vector3:new()
local H_ColorStartPos = Vector3:new()
local H_BarTransform
local H_ColorTransform
local startPosStored = false

-- Squish and Stretch
local squishTimer = 0.0
local squishTime = 0.3
local stretchTimer = 0.0
local stretchTime = 0.3
local checkland = true
local contactland = true

-- player turning rate
local turningRate = 8.0
local lerpTime = 8.0
local currentLerpTime = 0.0 

--local canCollectCube = true
canCollectCube = true
-- player Shake
local PlayerStartScale = Vector3:new()
local startUlt = false

local w = false
local s = false
local a = false
local d = false

local dashingForward = false
local dashingBackward = false
local dashingLeft = false
local dashingRight = false

local dashingTimer = 0.0

playVideoOnce = false
currentVideo = 0

local UI_PowerBarID
local powerBarSprite

local maxID
local maxSprite

gamePatTriggerOnce = true

godMode = false
gmodeTimer = 0.5

canPlayHealSound = true

t2 = false
t3 = false
t4 = false
t5 = false


--Player Table
player = {}

-- Awake is called once and only once when script is loaded.
function Awake()
    shootDelayTimer = pistolFirerate
    pBulletPrefabID = ECS:getPrefabID("Prefab_Bullet")
    pRocketPrefabID = ECS:getPrefabID("Prefab_Rocket")
    pLaserPrefabID = ECS:getPrefabID("Prefab_Laser")
    pWeaponPrefabID = ECS:getPrefabID("Prefab_Weapon")
    pHealingUI = ECS:getPrefabID("Prefab_Healing")
    pUltiUI = ECS:getPrefabID("Prefab_Ultimate")
    -- FOR BOSS
    bossLevel2PrefabID1 =  ECS:getPrefabID("Prefab_Boss_Head_Test")
    bossLevel2BodyPrefabID =  ECS:getPrefabID("Prefab_Boss2_Body")
    -- bossLevel2PanelPrefabID = ECS:getPrefabID("Prefab_BossPanelLvl2")
    bossLevel2PrefabID2 =  ECS:getPrefabID("Prefab_WeaponTest1")
    bossLevel2PrefabID3 =  ECS:getPrefabID("Prefab_WeaponTest2")
    bossLevel2CutsceneID = ECS:getPrefabID("Prefab_AAABossIntroCutscene2")
	bossLevel3CutsceneID = ECS:getPrefabID("Prefab_AAABossIntroCutscene3")
	--bossHeadID = ECS:getPrefabID("Prefab_Boss_Head")
	--bossLowerBodyID = ECS:getPrefabID("Prefab_Boss_LowerBody")
	--bossUpperBodyID = ECS:getPrefabID("Prefab_Boss_UpperBody")
	if(UISystem:getGameState() == 1) then
	    bossHeadID        = ECS:getEntityID("Boss_Head")
	    bossLowerBodyID   = ECS:getEntityID("Boss_LowerBody")
	    bossUpperBodyID   = ECS:getEntityID("Boss_UpperBody")
	end

	playVideoOnce = false
	
	
	
	--UI For Ammo Counter LOGIC
	UI_AmmoCounterID       = ECS:getEntityID("Text_LaserAmmo")
	UI_AmmoCounterID1      = ECS:getEntityID("Text_RocketAmmo")
	
	--boss3_headID      = ECS:getPrefabID("Prefab_3_BossHead")
	boss3_headID      = ECS:getPrefabID("Prefab_3_Boss_Head")
	boss3_upperTurrID = ECS:getPrefabID("Prefab_3_BossUpperTurr")
	boss3_lowerTurrID = ECS:getPrefabID("Prefab_3_BossLowerTurr")
	boss3_Weap1ID     = ECS:getPrefabID("Prefab_3_BossHomingRocket")
	boss3_Weap2ID     = ECS:getPrefabID("Prefab_3_BossRockShower")
	
	UI_pHealthID      = ECS:getEntityID("UI_Health_Color")
	UI_pHealthBarID      = ECS:getEntityID("UI_Health_UI")
    UI_PowerBarID = ECS:getEntityID("UI_PowerUp_Colour")
    maxID = ECS:getEntityID("UI_PowerUP_Max")
    maxSprite = ECS:getComponent_SpriteCom(maxID)
    powerBarSprite = ECS:getComponent_SpriteCom(UI_PowerBarID)
    powerBarSprite.barPercentage = 19.53
	
	Console.PrintFloat(boss3_headID)
	--Console.WriteLine("boss3_headID:")
	
	--cutSceneID1 = ECS:getPrefabID("AAABossIntroCutscene")
    cutSceneID1 = 1
	--cutSceneID2 = ECS:getPrefabID("AAFrontBGCutscene")

	
	--Console.PrintFloat(cutSceneID1)
	
    pFlashTimer     = 1
    pHealth         = 75
end

-- Start is called once and only once after Awake() is called.
function Start()
    --Console.WriteLine("PlayerBehavior script loaded")
    eEntID = ECS:getEntityID("Player")
    currentStageLevel = UISystem:getGameState()
    -- playerCamID = ECS:getEntityID("PlayerCamera1")
    pTimer = 5
    player["Player_Env"] = ECS:getComponent_LuaScriptCom(eEntID):GetEnvironment()
    playerCamID = ECS:getEntityID("PlayerCamera1")

    wantedUIID = ECS:getEntityID("UI_ThreatIcon")
    player["Player_Env"]["dashLifetime"] = 0.0

    --Cutscene variable
    CutsceneBool = false
    player["Player_Env"]["CutsceneBool"] = CutsceneBool
    player["Player_Env"]["playVideoOnce"] = playVideoOnce
    player["Player_Env"]["currentVideo"] = currentVideo
    UISystem:toggleCutscene(false)
	
	--Share health for health incr
	player["Player_Env"]["pHealth"] = pHealth
	

    UIObjID = ECS:getEntityID("UI_Challenge")

    if(laserUnlocked == false) then
        if(UISystem:getGameState() == 2)
                then laserUnlocked = true
        end
    end
    if(rktUnlocked == false) then
        if(UISystem:getGameState() == 3)
                then rktUnlocked = true
                laserUnlocked = true
        end
    end

    rocketCounter  = false
    laserCounter   = false

    H_BarTransform = ECS:getComponent_Transform(UI_pHealthBarID)
    H_ColorTransform = ECS:getComponent_Transform(UI_pHealthID)
    H_BarStartPos.x = H_BarTransform.pos.x
    H_BarStartPos.y = H_BarTransform.pos.y
    H_BarStartPos.z = H_BarTransform.pos.z

    H_ColorStartPos.x = H_ColorTransform.pos.x
    H_ColorStartPos.y = H_ColorTransform.pos.y
    H_ColorStartPos.z = H_ColorTransform.pos.z

    pTransform = ECS:getComponent_Transform(eEntID)
    PlayerStartScale.x = pTransform.scale.x
    PlayerStartScale.y = pTransform.scale.y
    PlayerStartScale.z = pTransform.scale.z

    player["Player_Env"]["laserUlt"] = false
end

-- Updates the Game Object per game loop
function Update(dt)
    --Get the ID of the entity with the attached script
    --eEntID          = ScriptSystem:getLuaEntityID()
    eLuaScript = ECS:getComponent_LuaScriptCom(eEntID)
    
    player["Player_Env"]["lAmmo"] = lAmmo
	player["Player_Env"]["rAmmo"] = rAmmo
    player["Player_Env"]["currPower"] = currPower
     player["Player_Env"]["canCollectCube"] = canCollectCube
    player["Player_Env"]["wantedLevel"] = wantedLevel
	player["Player_Env"]["bossTimeBool"] = bossTimeBool
	player["Player_Env"]["weaponCounter"] = toggleWeapon
	player["Player_Env"]["healTimer"] = healTimer

    --if Inputs.get_Key_Triggered(InputKey.KEY_6) then
    --    pHealth = 0
    --end


    updateAmmoText()
    playVideoCutscene()
	--boss global variables
	if ECS:hasComponent_LuaScriptCom(bossHeadID) then
        bossScript = ECS:getComponent_LuaScriptCom(bossHeadID)
		--Console.PrintFloat(bossHP)
    end
	
	-- Get the ID of the ammo cubes
    rcubeEntityID    = ECS:getEntityTagged("rAmmo")
    nRAmmo           = #rcubeEntityID

    -- Get the ID of the ammo cubes
    lcubeEntityID    = ECS:getEntityTagged("lAmmo")
    nLAmmo           = #lcubeEntityID
    
    -- Get the ID of pCubes
    pCubeEntityID   = ECS:getEntityTagged("pCube")
    nPCube          = #pCubeEntityID

    -- Get ID of enemy
    enemyEntID      = ECS:getEntityTagged("Enemy")
    nEnemyID        = #enemyEntID
    
    -- Get bool of Cutscene
    CutsceneBool = player["Player_Env"]["CutsceneBool"]
	
	
	--Update pHealth bar
	if ECS:hasComponent_SpriteCom(UI_pHealthID) then
		local pBar = ECS:getComponent_SpriteCom(UI_pHealthID)
		pBar.barPercentage = pHealth + 21.1
	end
	
	-- wans cheat
	--UISystem:swapToWeapon(2)
	--toggleWeapon = 3
	--Console.PrintFloat(rAmmo)
	
	--Ammo Counter Logic
	
	
	if rocketCounter == true then
		--show counter
		if ECS:hasComponent_TextCom(UI_AmmoCounterID1) then
			local ammoScript = ECS:getComponent_TextCom(UI_AmmoCounterID1)
			
			ammoScript.TextName = rAmmo
            ammoScript._size = 60.72999954223633
			-- Console.WriteLine("Rocket:")
			-- Console.PrintFloat(rocketAmmo)
		end
	end
	
	if rocketCounter == false then
		--show counter
		if ECS:hasComponent_TextCom(UI_AmmoCounterID1) then
			local ammoScript = ECS:getComponent_TextCom(UI_AmmoCounterID1)
			-- Console.WriteLine("RocketFalse:")
			ammoScript._size = 0
		end
	end
	
	if laserCounter == true then
		--show counter
		if ECS:hasComponent_TextCom(UI_AmmoCounterID) then
			local ammoScript = ECS:getComponent_TextCom(UI_AmmoCounterID)
			
			ammoScript.TextName = lAmmo
            ammoScript._size = 60.72999954223633
			-- Console.WriteLine("Laser")
			--Console.PrintFloat(rocketAmmo)
		end
	end
	
	if laserCounter == false then
		--show counter
		if ECS:hasComponent_TextCom(UI_AmmoCounterID) then
			local ammoScript = ECS:getComponent_TextCom(UI_AmmoCounterID)
			-- Console.WriteLine("LaserFalse")
			ammoScript._size = 0
		end
	end

    -- Check if player is still alive
    if pIsAlive then
        if eEntID == ECS:getEntityID("Player") then
            -- Console.WriteLine("WANTED LEVEL PLAYER: ")
            -- Console.PrintFloat(wantedLevel)
            if ECS:hasComponent_RigidDynamic(eEntID) and ECS:hasComponent_Transform(eEntID) and ECS:hasComponent_Material(eEntID) and ECS:hasComponent_AnimationCom(eEntID) then
                -- Get the component variables of the player
                pRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
                pTransform = ECS:getComponent_Transform(eEntID)
                if ECS:hasComponent_Emitter(eEntID) then
                    bEmitter = ECS:getComponent_Emitter(eEntID)
                end
                pMaterial = ECS:getComponent_Material(eEntID)
                pAnim = ECS:getComponent_AnimationCom(eEntID)
				
				-- function for saturation follow health
				if pHealth > 3 then
					pMaterial._saturation.y = ((100/80) * pHealth) / 100
				end
				
				if pHealth < 3 and pHealth > 0 then
					pMaterial._saturation.y = 0.2
				end
                
                -- Get the global position and rotation of the player through Physx
                newPos = PhysicsSystem:getGlobalPosePosition(eEntID)
                newRot = PhysicsSystem:getGlobalPoseRotation(eEntID)
                
                -- Update the player to look in the same direction as camera
                updatePlayertoCamRotation(dt)

                -- Update player's position and mass
                updatePlayerPos()
                updatePlayerMass()

                -- Rotate the player's global rotation by quaternion and store in 'prime variables
                --PhysicsSystem:rotate_vector_by_angle(forward, pTransform.rotate.y, forwardPrime)
                --PhysicsSystem:rotate_vector_by_angle(backward, pTransform.rotate.y, backwardPrime)
                --PhysicsSystem:rotate_vector_by_angle(leftward, pTransform.rotate.y, leftwardPrime)
                --PhysicsSystem:rotate_vector_by_angle(rightward, pTransform.rotate.y, rightwardPrime)

                PhysicsSystem:rotate_vector_by_quaternion(forward, pRigidDynamic.rigidRot, forwardPrime)
                PhysicsSystem:rotate_vector_by_quaternion(backward, pRigidDynamic.rigidRot, backwardPrime)
                PhysicsSystem:rotate_vector_by_quaternion(leftward, pRigidDynamic.rigidRot, leftwardPrime)
                PhysicsSystem:rotate_vector_by_quaternion(rightward, pRigidDynamic.rigidRot, rightwardPrime)

                -- Set Rigidbody damping
                PhysicsSystem:setBodyLinearDamping(eEntID)
                PhysicsSystem:setBodyAngularDamping(eEntID)

                -- Check if the player's weapon model has been spawned
                -- if not spawned, then spawn the weapon
                if not weaponSpawned then
                    spawnWeapon()
                    weaponSpawned = true
                end
                
                -- Get the transform component of the weapon entity
                weaponTransform = ECS:getComponent_Transform(WeaponID)

                -- Update the gravity flag of the player
                --updateGravityFlag()

                -- Update the animation of the player
                updatePlayerAnim()

                -- Check and handle input for the player
                playerInput(dt)
                
                -- Update player velocity
                playerVeloctiy = PhysicsSystem:getBodyLinearVelocity(eEntID)
                PhysicsSystem:setBodyLinearVelocity(eEntID, playerVeloctiy, speedCap)

                -- Handle the timer for flashing anim when player gets hit
                if pFlashTimer > 1.5 then
                    pFlashTimer = pFlashTimer - dt * 10
                    flashOnHit()
                else
                    pInvulnerable = false
                end
				-- laser 
                --[[
				if(not(#lcubeEntityID == 0)) then
                    for i = 1, nLAmmo do
                        if ECS:hasComponent_Transform(lcubeEntityID[i]) then
                           lCubeTransform = ECS:getComponent_Transform(lcubeEntityID[i]) 
                           --lCubeTouchPlayer()
                        end
                    end
                end
			  --rocket
                if(not(#rcubeEntityID == 0)) then
                    for i = 1, nRAmmo do
                        if ECS:hasComponent_Transform(rcubeEntityID[i]) then
                           rCubeTransform = ECS:getComponent_Transform(rcubeEntityID[i]) 
                           --rCubeTouchPlayer()
                        end
                    end
                end
               
                if(not(#pCubeEntityID == 0)) then
                    for i = 1, nPCube do
                        if ECS:hasComponent_Transform(pCubeEntityID[i]) then
                           pCubeTransform = ECS:getComponent_Transform(pCubeEntityID[i]) 
                           --pCubeTouchPlayer()
                        end
                    end
                end
                ]]
                
                --Console.PrintFloat(currPower)
                --Console.PrintFloat(pistolFirerate)

                handleHelicopterRocketDelay(dt)

                -- Check and update player's in-game status
				if godMode == false then
				    updatePlayerStatus(dt)
                else
                    currPower = 75.1
                    startUlt = true
                    unlockAllWeapons()
                    rAmmo = 10
                    lAmmo = 20
				end
                

                -- spawn the boss
                if spawnBoss == false then
                    spawnBossNow()
                end

                
                if (timer > 0.0) then
                    timer = timer - dt
                    CameraSystem._screenShake = true
                else
                    CameraSystem._screenShake = false
                end

                unlockAllWeapons()
            end
        end
        
        spawnHealingUI()
        if startUlt == true then
            canCollectCube = false
            powerBarSprite._colour.x = Random.Float()
            powerBarSprite._colour.y = Random.Float()
            powerBarSprite._colour.z = Random.Float()
            --UISystem:decreasePower(0.0751)
            powerBarSprite.barPercentage = powerBarSprite.barPercentage - 0.1502
            if powerBarSprite.barPercentage <= 19.53 then
                powerBarSprite.barPercentage = 19.53
            end
            currPower = currPower - 0.1502

            if currPower <= 19.53 then
                currPower = 19.53
            end
            UltiUISprite._drawSprite= true
            --Console.PrintFloat(currPower)
        end

        -- Handle shooting delay timer
        if startUlt == false then
            --pTimer = 10
            canCollectCube = true
            powerBarSprite._colour.x = 1.0
            powerBarSprite._colour.y = 1.0
            powerBarSprite._colour.z = 1.0
            UltiUISprite._drawSprite= false
        end

        if currPower >= 75.1 then
            powerBarSprite._colour.x = Random.Float()
            powerBarSprite._colour.y = Random.Float()
            powerBarSprite._colour.z = Random.Float()
            maxSprite._drawSprite = true
        end

        if currPower <= 19.53 then
            startUlt = false
            if (toggleWeapon == 1) then
                pistolFirerate = 0.25
            elseif (toggleWeapon == 2) then
                laserFirerate = 0.7
            else
                rocketFirerate = 0.7
            end
        end

        checkWantedLevel()
		checkSpawnBoss()
		if isSpawnBoss == true and bossOnce == false then
            --check current level

		    if(UISystem:getGameState() == 1) then -- if currentLevel == 1st Level
                local pos = Vector3:new(6,16,24)
			    local pos1 = Vector3:new(6,17,20)
			    local pos2 = Vector3:new(0.597,99,35.949)
			
			    local wan = Quat:new(0,0,0,1)

                cutSceneID1 = ECS:getPrefabID("Prefab_AAABossIntroCutscene")
                --cutSceneID1 = ECS:getPrefabID("AAABossIntroCutscene")
	            cutSceneID2 = ECS:getPrefabID("Prefab_AAFrontBGCutscene")

			    Console.PrintFloat(cutSceneID1)
			    Console.PrintFloat(cutSceneID2)
			    ECS:instantiatePrefab(cutSceneID1, "AAABossIntroCutscene", pos2,wan,false, false, false)
			
			    -- ECS:instantiatePrefab(cutSceneID2, "AAFrontBGCutscene", pos1,wan,false, false, false)
		
			    --ECS:instantiatePrefab(bossHeadID, "Boss_Head", pos2,wan,false, false, false)
			
			    --ECS:instantiatePrefab(bossUpperBodyID, "Boss_UpperBody", pos1,wan,false, false, false)
			
			    --ECS:instantiatePrefab(bossLowerBodyID, "Boss_LowerBody", pos,wan,false, false, false)
			
			    isSpawnBoss = false
			    bossOnce = true
            
            end
			
		end

        -- HEALTH BAR SHAKE LOGIC
        HealthBarShake()
        PlayerShake()
        handleShakeTimer(dt)
        PlayerLandSquish()
        PlayerJumpStretch()
        
        handleSquishTimer(dt)
        handleStretchTimer(dt)
        updatePowerBarSprite()
        checkLanding()
        handleHealTimer(dt)
        healthbarHealFlash(healTimer)
        --dashTrail()
    else
        ScriptSystem:setNState(ScriptSystem:getGameState())
        ScriptSystem:setGameState(5)
    end
	
	if countdown < 25 and isSpawnBoss then
				countdown = countdown + dt * 1
	end
			
			if countdown >= 25 then
				--ECS:instantiatePrefab(cutSceneID3, "AAABossDefeatCutscene", pos2,wan,false, false, false)
			
				--ECS:instantiatePrefab(cutSceneID2, "AAFrontBGCutscene", pos2,wan,false, false, false)
				
			end
			
			--Console.PrintFloat(countdown)
end

-- Check for player input and handle accordingly
function playerInput(dt)

	if Inputs.get_KeyReleaseTrigger(InputKey.G_KEY) and godMode == false then
        godMode = true
		Console.WriteLine("GODMODE ACTIVATED!")
    end
	
	if godMode then
		gmodeTimer = gmodeTimer - dt
		pHealth = 75
	end
	
	if Inputs.get_KeyReleaseTrigger(InputKey.G_KEY) and godMode == true and gmodeTimer <= 0 then
        godMode = false
		Console.WriteLine("GODMODE UN-ACTIVATED!")
		gmodeTimer = 0.5
    end
	
    -- check if in cutscene
    if RenderSystem:getPlayVideo() == true then
        do return end
    end
    -- Handle player movement input
    playerMovement(dt)

    -- Handle player jumping input
    playerJump(dt)

    -- Handle changing weapon and bullet type
    toggleWeaps()

    -- Handle logic for power up
    activatePOWER()

    -- Check if the player can shoot again
    if pCanShoot then
        -- Handle shooting logic
        handleShooting()
    else
        -- Update the cooldown for shooting
        handleShootingDelay(dt)
    end

    -- Check if the player can dash
    if not pCanDash then
        -- Update the cooldown timer for dashing
        handleDashingDelay(dt)
    end
end

-- Handle player movement input
function playerMovement(dt)
    if Inputs.get_KeyReleaseTrigger(InputKey.M_KEY) then
        wantedLevel = wantedLevel + 1
        --AudioSystem:PlayUISound("SFX_ThreatUp")
    end

    if (w and a and not d) or (w and d and not a) or (s and a and not d) or (s and d and not a) then
        walkForce = 15
    else
        walkForce = 22.5
    end

    dash(dt)

    if Inputs.get_Key(InputKey.W_KEY) or Inputs.get_Controller(InputControllerKey.UP_DPAD) then

        if not dashingForward then
            PhysicsSystem:setGlobalPose(eEntID, PhysicsSystem:getGlobalPosePosition(eEntID) + forwardPrime * dt * walkForce, 
                PhysicsSystem:getGlobalPoseRotation(eEntID))
        end

        if (Inputs.get_Key_Triggered(InputKey.LEFTSHIFT_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_BUMPER)) and pCanDash then
            dashingForward = true
            pCanDash = false
            AudioSystem:PlaySFX("PlayerDash_SFX", eEntID)
        end

        if pIsWalking == false then
            pIsWalking = true
        end
        w = true
    else
        w = false
    end

    if Inputs.get_Key(InputKey.S_KEY) or Inputs.get_Controller(InputControllerKey.DOWN_DPAD) then

        if not dashingBackward then
            PhysicsSystem:setGlobalPose(eEntID, PhysicsSystem:getGlobalPosePosition(eEntID) + backwardPrime * dt * walkForce, 
                PhysicsSystem:getGlobalPoseRotation(eEntID))
        end

        if (Inputs.get_Key_Triggered(InputKey.LEFTSHIFT_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_BUMPER)) and pCanDash then
            dashingBackward = true
            pCanDash = false
            AudioSystem:PlaySFX("PlayerDash_SFX", eEntID)
        end

        if pIsWalking == false then
            pIsWalking = true
        end
        s = true
    else
        s = false
    end

    if Inputs.get_Key(InputKey.A_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_DPAD) then

        if not dashingLeft then
            PhysicsSystem:setGlobalPose(eEntID, PhysicsSystem:getGlobalPosePosition(eEntID) + leftwardPrime * dt * walkForce, 
                PhysicsSystem:getGlobalPoseRotation(eEntID))
        end

        if (Inputs.get_Key_Triggered(InputKey.LEFTSHIFT_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_BUMPER)) and pCanDash then
            dashingLeft = true
            pCanDash = false
            AudioSystem:PlaySFX("PlayerDash_SFX", eEntID)
        end

        if pIsWalking == false then
            pIsWalking = true
        end
        a = true
    else
        a = false
    end

    if Inputs.get_Key(InputKey.D_KEY) or Inputs.get_Controller(InputControllerKey.RIGHT_DPAD) then

        if not dashingRight then
            PhysicsSystem:setGlobalPose(eEntID, PhysicsSystem:getGlobalPosePosition(eEntID) + rightwardPrime * dt * walkForce, 
                PhysicsSystem:getGlobalPoseRotation(eEntID))
        end

        if (Inputs.get_Key_Triggered(InputKey.LEFTSHIFT_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_BUMPER)) and pCanDash then
            dashingRight = true
            pCanDash = false
            AudioSystem:PlaySFX("PlayerDash_SFX", eEntID)
        end

        if pIsWalking == false then
            pIsWalking = true
        end
        d = true
    else
        d = false
    end

    if (Inputs.get_Key(InputKey.W_KEY)==false and Inputs.get_Key(InputKey.S_KEY)==false and 
        Inputs.get_Key(InputKey.A_KEY)==false and Inputs.get_Key(InputKey.D_KEY)==false) and
        (Inputs.get_Controller(InputControllerKey.RIGHT_DPAD) == false
        and Inputs.get_Controller(InputControllerKey.DOWN_DPAD)== false
        and Inputs.get_Controller(InputControllerKey.UP_DPAD)== false
        and Inputs.get_Controller(InputControllerKey.LEFT_DPAD)== false) then
            pIsWalking = false
    end
end

-- Update player gravity
function updateGravityFlag()
    if mContactReportCallback.playerOnFloor then
        PhysicsSystem:setGravityFlag(eEntID, true)
    else
        PhysicsSystem:setGravityFlag(eEntID, false)
    end
end

-- Handle player jumping input
function playerJump(dt)
    if (Inputs.get_Key_Triggered(InputKey.SPACE_KEY) or Inputs.get_Controller(InputControllerKey.B_BUTTON)) and mContactReportCallback.playerOnFloor then
        --Console.WriteLine("jump")
        PhysicsSystem:addForceToBody(eEntID, upward, jumpForce, 0)
        mContactReportCallback.playerOnFloor = false
        PhysicsSystem:setGravityFlag(eEntID, false)
        AudioSystem:PlaySFX("PlayerJump_SFX", eEntID)
        stretchTimer = stretchTime
        --squishTimer = squishTime
    end
end

-- Update the player's facing direction to the camera's facing direction
function updatePlayertoCamRotation(dt)
    if ECS:hasComponent_Camera(playerCamID) then
        --pCam =  ECS:getComponent_Camera(playerCamID)
        pCam = CameraSystem:getPlayerCamera() 
        if orginalFOV == 0 then
            orginalFOV = pCam._m_fov
        end
        --camRot.x = pCam._rotation.x
        camRot.y = pCam._rotation.y
        --camRot.z = pCam._rotation.z
        playerCamRot.x = 0
        playerCamRot.y = camRot.y
        playerCamRot.z = 0

        local targetRotation = Quat:new()
        targetRotation = PhysicsSystem:convertEulerToQuat(playerCamRot)
        pRigidDynamic.rigidRot = Quat.Slerp(pRigidDynamic.rigidRot, targetRotation, turningRate * dt)
        PhysicsSystem:setGlobalPose(eEntID, newPos, pRigidDynamic.rigidRot)
        pTransform.rotate = PhysicsSystem:convertQuatToEuler(pRigidDynamic.rigidRot)
    end
end

-- Update the bullet's facing direction to the camera's facing direction
function updateBullettoCamRotation()
    if ECS:hasComponent_Camera(playerCamID) then
        --pCam = ECS:getComponent_Camera(playerCamID)
        pCam = CameraSystem:getPlayerCamera() 
        bulletRot = pCam._rotation
        bulletRotQuat = PhysicsSystem:convertEulerToQuat(bulletRot)
        if toggleWeapon == 1 then
            RayCast("Bullet")
        elseif toggleWeapon == 2 then
            RayCast("Laser")
        elseif toggleWeapon == 3 then
            RayCast("Rocket")
        end
    end
    
end

function RayCast(weaponType)
    PhysicsSystem:rotate_vector_by_quaternion(forward, bulletRotQuat, rayForwardPrime)
    pCam._pos = pCam._pos + rayForwardPrime * 4.0
    local bulletSpawnPos = weaponTransform.pos + rayForwardPrime * 3
    PhysicsSystem:ApplyRaycast(pCam._pos, rayForwardPrime, 500.0, rayEnd, weaponType, bulletSpawnPos, pCam._camEye)
    actorID = PhysicsSystem:RaycastHitEntityID(pCam._pos, rayForwardPrime, 1000.0)
    player["Player_Env"]["TargetPoint"] = rayEnd
end

-- Update player's position
function updatePlayerPos()
    pTransform.pos.x = newPos.x
    pTransform.pos.y = newPos.y
    pTransform.pos.z = newPos.z

    pRigidDynamic.rigidPos.x = newPos.x
    pRigidDynamic.rigidPos.y = newPos.y
    pRigidDynamic.rigidPos.z = newPos.z
end

-- Update player's mass
function updatePlayerMass()
    PhysicsSystem:setBodyMass(eEntID)
end

-- Logic for handling shooting
function handleShooting()
    -- wAnim = ECS:hasComponent_AnimationCom(WeaponID)
    if Inputs.get_Mos(InputKey.LEFTMOUSE_BUTTON) or Inputs.get_Controller(InputControllerKey.RIGHT_BUMPER) then
        -- Fire rocket
        if toggleWeapon == 3 and rAmmo > 0 then 
            pCanShoot = false
            spawnBullet()
            
            timer = 0.1
            pCam._aggressive = 0.8
            wAnim.currentAnim = 2
            if not startUlt then
                rocketFirerate = 0.7
                wAnim.speed = 22
                rAmmo = rAmmo - 1
            else
                rocketFirerate = 0.4
                wAnim.speed = 38
            end
			
        end

        -- Fire laser
        if toggleWeapon == 2 and lAmmo > 0 then
            pCanShoot = false
            spawnBullet()
            
            timer = 0.1
            pCam._aggressive = 0.5
            wAnim.currentAnim = 2
            if not startUlt then
                player["Player_Env"]["laserUlt"] = false
                laserFirerate = 0.7
                wAnim.speed = 22
                lAmmo = lAmmo - 1
            else
                player["Player_Env"]["laserUlt"] = true
                laserFirerate = 0.4
                wAnim.speed = 38
            end
        end

        -- Fire bullet
        if toggleWeapon == 1 then
            pCanShoot = false
            spawnBullet()
            timer = 0.1
            pCam._aggressive = 0.1
            wAnim.currentAnim = 2
            if not startUlt then
                pistolFirerate = 0.25
                wAnim.speed = 60
            else
                pistolFirerate = 0.1
                wAnim.speed = 70
            end
        end
    end
end

-- Update shooting cooldown
function handleShootingDelay(dt)
    if shootDelayTimer > 0.0 then
        shootDelayTimer = shootDelayTimer - dt
    end

    if shootDelayTimer <= 0.0 then
        pCanShoot = true
        if (toggleWeapon == 1) then  
            shootDelayTimer = pistolFirerate
        elseif (toggleWeapon == 2) then
            shootDelayTimer = laserFirerate
        elseif (toggleWeapon == 3) then
            shootDelayTimer = rocketFirerate
        end
    end
end

-- Spawn player projectile in-game
function spawnBullet()
    updateBullettoCamRotation()
    local bulletSpawnPos = weaponTransform.pos + rayForwardPrime * 3

    if toggleWeapon == 1 then
        local bulletID = ECS:instantiatePrefab(pBulletPrefabID, "Bullet", bulletSpawnPos,bulletRotQuat,false, false, false)
        if actorID then
            if ECS:hasComponent_RigidStatic(actorID) then
                local actorRigidStatic= ECS:getComponent_RigidStatic(actorID)
                if actorRigidStatic:getActorName() == "RiotShield" then
                    -- IF TRUE, THEN TAG IT AS BLOCKED SO IT DOESN'T DAMAGE ANYTHING
                    ECS:addEntityTag(bulletID, "Blocked")
                end
            end
        end
        AudioSystem:PlaySFX("SFX_PlayerShoot", eEntID)
		
    elseif toggleWeapon == 2 then
        local bulletDirQuat = PhysicsSystem:lookat(bulletSpawnPos, rayEnd, unitDir)
        local laserID = ECS:instantiatePrefab(pLaserPrefabID, "Laser", bulletSpawnPos,bulletDirQuat,false, false, false)
        -- CHECK IF LASER IS HITTING A CAGE OR A BARRIER
        if actorID then
            if ECS:hasComponent_RigidStatic(actorID) then
                local actorRigidStatic = ECS:getComponent_RigidStatic(actorID)
                if actorRigidStatic:getActorName() == "LaserCage" or actorRigidStatic:getActorName() == "RocketCage"
                    or actorRigidStatic:getActorName() == "Barrier" then
                    -- IF TRUE, THEN TAG IT AS BLOCKED SO IT DOESN'T DAMAGE ANYTHING
                    ECS:addEntityTag(laserID, "Blocked")
                end
            end
        end
        AudioSystem:PlaySFX("PlayerLaser_SFX", eEntID)
		
    elseif toggleWeapon == 3 then
        local rocketID = ECS:instantiatePrefab(pRocketPrefabID, "Rocket", bulletSpawnPos,bulletRotQuat,false, false, false)
        AudioSystem:PlaySFX("PlayerRocketFire_SFX", eEntID)
    end
end

-- Player dashing
function handleDashing(Vector3, dt)
    if (Inputs.get_Key_Triggered(InputKey.LEFTSHIFT_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_BUMPER)) and pCanDash then
        --dashing = true
        --PhysicsSystem:addForceToBody(eEntID, Vector3, dashForce, 2)
        --local oldPos = PhysicsSystem:getGlobalPosePosition(eEntID)
        --local rot = PhysicsSystem:getGlobalPoseRotation(eEntID)
        --PhysicsSystem:setGlobalPose(eEntID, oldPos + Vector3 * dt * 300.0, rot)
        pCanDash = false
        AudioSystem:PlaySFX("PlayerDash_SFX", eEntID)
    end
end

-- lerp 
function lerp(a, b, f)
    return (a * (1.0 - f)) + (b * f)
end
function expoDecayIncr(c, k, t)
    return c * (1 - math.pow(2.718, -k*t));
end

function BezierBlend(t)

    return t * t * (3.0 - 2.0 * t)
end

function ParametricBlend(t)

    local sqt = t * t
    return sqt / (2.0 * (sqt - t) + 1.0)
end


function dash(dt)
    local oldPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    local rot = PhysicsSystem:getGlobalPoseRotation(eEntID)
    dashForce = 100

    if dashingTimer <= 0.1 then
        if dashingForward then
            --Console.WriteLine("dashing forward")
            PhysicsSystem:setGlobalPose(eEntID, oldPos + forwardPrime * dt * dashForce, rot)
            dashingTimer = dashingTimer + dt
        end

        if dashingBackward then
            PhysicsSystem:setGlobalPose(eEntID, oldPos + backwardPrime * dt * dashForce, rot)
            dashingTimer = dashingTimer + dt
        end
        
        if dashingLeft then
            PhysicsSystem:setGlobalPose(eEntID, oldPos + leftwardPrime * dt * dashForce, rot)
            dashingTimer = dashingTimer + dt
        end
        
        if dashingRight then
            PhysicsSystem:setGlobalPose(eEntID, oldPos + rightwardPrime * dt * dashForce, rot)
            dashingTimer = dashingTimer + dt
        end
    else
        
        dashingForward = false
        dashingBackward = false
        dashingLeft = false
        dashingRight = false
        dashingTimer = 0.0
    end
end

-- lerp /\
function dashLerp(currentTime)
    local halfTime = dashDelayTime/2 
    if(currentTime > halfTime) then
        local percent = dashDelayTime - currentTime
        return expoDecayIncr(maxFOVCamera, 5, percent)
    end
    return lerp(0,maxFOVCamera, currentTime/halfTime)
end
-- Player dashing delay
function handleDashingDelay(dt)
    if dashDelayTimer > 0.0 then
        dashDelayTimer = dashDelayTimer - dt
        pCam._m_fov = orginalFOV + dashLerp(dashDelayTimer)
        if pCam._m_fov < 45 then
            pCam._m_fov = 45
        end
        player["Player_Env"]["dashLifetime"] = dashDelayTimer
    end

    if dashDelayTimer <= 0.0 then
        pCanDash = true
        dashDelayTimer = dashDelayTime
        --dashing = false
        dashingForward = false
        dashingBackward = false
        dashingLeft = false
        dashingRight = false
    end
end

-- Cooldown for switching weapons
function toggleWeaps()
    if Inputs:get_MosScroll_Trigger() ~= 0 then
        scrollTrigger = true
        scrollCounter = scrollCounter + Inputs:get_MosScroll_Trigger()
        if scrollCounter == 0 then
            scrollCounter = 3
        elseif scrollCounter == 4 then
            scrollCounter = 1
        end            
    end
    -- for gamepat
    if(Inputs.get_GamePad_RT_Y()) then
        if(gamePatTriggerOnce) then
            gamePatTriggerOnce = false
            scrollTrigger = true
            scrollCounter = scrollCounter + 1
            if scrollCounter == 4 then
                scrollCounter = 1
            end  
        end
    else
        gamePatTriggerOnce = true
    end
    wMaterial = ECS:getComponent_Material(WeaponID)

    if (scrollTrigger and scrollCounter == 1) or (Inputs.get_Key_Triggered(InputKey.KEY_1) and (toggleWeapon ~= 1)) then
        if UISystem:getGameState() ~= 1 then
            AudioSystem:PlayUISound("SFX_WeaponPistol")
        end
        scrollTrigger = false
        scrollCounter = 1
        toggleWeapon = 1
        if not startUlt then
            pistolFirerate = 0.25
        else
            pistolFirerate = 0.1
        end
        shootDelayTimer = 0
        pCanShoot = false
        UISystem:swapToWeapon(0)


        wMaterial.m_model = "Pistol_Bone"
        wMaterial.m_texture = "Pistol"
        wMaterial.isInitialised = false
		
		--Ammo Script UI
		laserCounter  = false
		rocketCounter = false
		--Console.WriteLine("Hide Text")
		
        if startTimer == true then
            pistolFirerate = 0.25
        end

    elseif (scrollTrigger and scrollCounter == 2) or (Inputs.get_Key_Triggered(InputKey.KEY_2) and (toggleWeapon ~= 2))then
        
        scrollTrigger = false
        scrollCounter = 2
        if laserUnlocked then
            toggleWeapon = 2
            if not startUlt then
                laserFirerate = 0.7
            else
                laserFirerate = 0.4
            end
            shootDelayTimer = 0
            pCanShoot = false
            UISystem:swapToWeapon(1)
            AudioSystem:PlayUISound("SFX_WeaponLaser")

            wMaterial.m_model = "Railgun_Bone"
            wMaterial.m_texture = "Railgun"
            wMaterial.isInitialised = false
			
			--Ammo Script UI
			laserCounter  = true
			rocketCounter = false
			-- Console.WriteLine("Text:LaserCounter")
			
            --if startTimer == true then
            --    laserFirerate = 0.7
            --end
        end
    elseif (scrollTrigger and scrollCounter == 3) or (Inputs.get_Key_Triggered(InputKey.KEY_3) and (toggleWeapon ~= 3))then
        
        scrollTrigger = false
        scrollCounter = 3
        if rktUnlocked then
            toggleWeapon = 3
            if not startUlt then
                rocketFirerate = 0.7
            else
                rocketFirerate = 0.4
            end
            shootDelayTimer = 0
            pCanShoot = false
            UISystem:swapToWeapon(2)
            AudioSystem:PlayUISound("SFX_WeaponRocket")

            wMaterial.m_model = "Rocket_launcher_Bone"
            wMaterial.m_texture = "Rocket Launcher"
            wMaterial.isInitialised = false
			
			--Ammo Script UI
			laserCounter  = false
			rocketCounter = true
			-- Console.WriteLine("Text:Rocket")
			
            if startTimer == true then
                rocketFirerate = 0.7
            end
        end

    end
end

-- Update the status and health of the player
function updatePlayerStatus(dt)
    -- If player collided with enemy and player health is greater than 0
    -- Player takes damage
    if pRigidDynamic.getHit and pHealth > 0 and pIsAlive then	
        if mContactReportCallback.enemyHitPlayer then
            EnemyID = mContactReportCallback.collidedEnemy
            if not ECS:hasTagInEntity(EnemyID, "Unified") and not ECS:hasTagInEntity(EnemyID, "ClownMinion") then
                if not pInvulnerable then 
                    AudioSystem:PlaySFX("PlayerDamaged_SFX", eEntID)
                    pFlashTimer = 8
                    pHealth = pHealth - 5
                    UISystem:decreaseHealth(5)
                    shakeTimer = shakeTime
                    pInvulnerable = true
                    mContactReportCallback.enemyHitPlayer = false
                end
            end
        end

        if mContactReportCallback.bossLaserHitPlayer then
            if not pInvulnerable then 
                AudioSystem:PlaySFX("PlayerDamaged_SFX", eEntID)
                pFlashTimer = 8
                pHealth = pHealth - 5
                UISystem:decreaseHealth(5)
                shakeTimer = shakeTime
                pInvulnerable = true
				mContactReportCallback.bossLaserHitPlayer = false
				--Console.WriteLine("LASERED!")
            end
        end

        if mContactReportCallback.bossRocketHitPlayer then
            if not pInvulnerable then 
                AudioSystem:PlaySFX("PlayerDamaged_SFX", eEntID)
                pFlashTimer = 8
                pHealth = pHealth - 5
                UISystem:decreaseHealth(5)
                shakeTimer = shakeTime
                pInvulnerable = true
				mContactReportCallback.bossRocketHitPlayer = false
            end
        end
		
		if mContactReportCallback.enemyBulletHitPlayer then
            if not pInvulnerable then 
                AudioSystem:PlaySFX("PlayerDamaged_SFX", eEntID)
                pFlashTimer = 8
                pHealth = pHealth - 5
                UISystem:decreaseHealth(5)
                shakeTimer = shakeTime
                pInvulnerable = true
                mContactReportCallback.enemyBulletHitPlayer = false
            end
        end

        if mContactReportCallback.explosionHitPlayer then
            if not pInvulnerable then 
                --Console.WriteLine("rocket")
                AudioSystem:PlaySFX("PlayerDamaged_SFX", eEntID)
                pFlashTimer = 8
                pHealth = pHealth - 10
                UISystem:decreaseHealth(10)
                shakeTimer = shakeTime
                mContactReportCallback.explosionHitPlayer = false
                pInvulnerable = true
            end
        end

        if mContactReportCallback.minionExplosionHitPlayer then
            if not pInvulnerable then
                AudioSystem:PlaySFX("PlayerDamaged_SFX", eEntID)
                pFlashTimer = 8
                pHealth = pHealth - 5
                UISystem:decreaseHealth(5)
                shakeTimer = shakeTime
                pInvulnerable = true
                mContactReportCallback.minionExplosionHitPlayer = false
            end
        end
    end
    -- Else player is dead and set saturation of player zero to 0
    if pHealth <= 0 then
        pIsAlive = false
        pMaterial._saturation.y = 0
        AudioSystem:PlayUISound("SFX_GameOver")
    end
	
	
	
	
	
end

-- If player is hit, set saturation to 1 
function flashOnHit()
    if pFlashTimer > 1 then
        pMaterial._saturation.z = pFlashTimer
    end
end
--[[
function pCubeTouchPlayer()

    luckyNumber   = Random.randInRange(1,2)
    powerDistance = Vector3.Distance(pCubeTransform.pos, pTransform.pos)
     
    if powerDistance <= 3 then
        if currPower < 75.1 and canCollectCube then
            currPower = currPower + 3.755
            if currPower >= 75.1 then
                currPower = 75.1
            end
            --Console.PrintFloat(currPower)
            --UISystem:increasePower(3.755)
            powerBarSprite.barPercentage = powerBarSprite.barPercentage + 3.755
            if powerBarSprite.barPercentage >= 75.1 then
                powerBarSprite.barPercentage = 75.1
                maxSprite._drawSprite = true
            end
            --Console.PrintFloat(currPower)
            -- Console.WriteLine("Current Power /10 ^")
            --Console.WriteLine("yo")
        end
    end
end
]]
function updatePowerBarSprite()
    powerBarSprite.barPercentage = currPower
    --Console.PrintFloat(powerBarSprite.barPercentage)
    --[[if powerBarSprite.barPercentage >= 75.1 then
        powerBarSprite.barPercentage = 75.1
        maxSprite._drawSprite = true
    end]]
end
--[[
--rocket ammo pickup
function rCubeTouchPlayer()

    local ammoDistance  = Vector3.Distance(rCubeTransform.pos, pTransform.pos)
    --if pRigidDynamic.getHit  and ammoDistance <= 3 then
    if ammoDistance <= 3 then
		
		rAmmo = rAmmo + 1
		
        if rAmmo >= 10 then 
            rAmmo = 10
        end
		
		
		--Console.WriteLine("Rocket Ammo Picked Up!")
		--Console.PrintFloat(rAmmo)
		
    end
	
end

--rocket ammo pickup
function lCubeTouchPlayer()

    local ammoDistance  = Vector3.Distance(lCubeTransform.pos, pTransform.pos)
    --if pRigidDynamic.getHit  and ammoDistance <= 3 then
    if ammoDistance <= 3 then
		
		lAmmo = lAmmo + 1
        if lAmmo >= 20 then 
            lAmmo = 20
        end
		--Console.WriteLine("Laser Ammo Picked Up!")
		--Console.PrintFloat(lAmmo)
		
    end
	
end
]]
function activatePOWER()
--if Inputs.get_GamePad_LT_Y() then
--    Console.WriteLine("LT")
--    Console.PrintFloat(Inputs.get_GamePad_LT_Y())
--end
    if Inputs.get_Key_Triggered(InputKey.E_KEY) or Inputs.get_GamePad_LT_Y() then
        if currPower >= 75.1 then
            shootDelayTimer = 0
            startUlt = true
            maxSprite._drawSprite = false
            if(toggleWeapon == 1) then
                pistolFirerate = 0.1    
            elseif (toggleWeapon == 2) then
                laserFirerate = 0.4
            elseif (toggleWeapon == 3) then
                rocketFirerate = 0.4
            end
        end
    end
end

-- Update player's animation
function updatePlayerAnim()
    wAnim = ECS:getComponent_AnimationCom(WeaponID)
    if mContactReportCallback.playerOnFloor == false then
        pAnim.currentAnim = 1
        pAnim.speed = 30
    elseif not pIsWalking then
        pAnim.currentAnim = 0
        pAnim.speed = 30
        if pCanShoot then
            wAnim.currentAnim = 0
            wAnim.speed = 23
        end
    elseif pIsWalking then
        pAnim.currentAnim = 2
        pAnim.speed = 40
         if pCanShoot then
            wAnim.currentAnim = 1
            wAnim.speed = 23
        end
    end
end

-- Handle player weapon spawning
function spawnWeapon()
    weaponSpawnPos = pRigidDynamic.rigidPos + rightwardPrime * rightwardOffset
    WeaponID = ECS:instantiatePrefab(pWeaponPrefabID, "Weapon", weaponSpawnPos,pRigidDynamic.rigidRot,false, false, false)
    PhysicsSystem:setGravityFlag(WeaponID, true)
end


function checkWantedLevel()
    local sprite = ECS:getComponent_SpriteCom(wantedUIID)
    if wantedLevel >= 1 and wantedLevel < 2 then
        sprite.SpriteName = "threatlevel_icon1"
    elseif wantedLevel >= 2 and wantedLevel < 3 and not t2 then
        sprite.SpriteName = "threatlevel_icon2"
        AudioSystem:PlayUISound("SFX_ThreatUp")
        t2 = true
    elseif wantedLevel >= 3 and wantedLevel < 4 and not t3 then
        sprite.SpriteName = "threatlevel_icon3"
        AudioSystem:PlayUISound("SFX_ThreatUp")
        t3 = true
    elseif wantedLevel >= 4 and wantedLevel < 5 and not t4 then
        sprite.SpriteName = "threatlevel_icon4"
        AudioSystem:PlayUISound("SFX_ThreatUp")
        t4 = true
    elseif wantedLevel < 1 then
        sprite.SpriteName = "threatlevel_icon0"
    elseif wantedLevel >= 5 and not t5 then
        sprite.SpriteName = "threatlevel_icon5"
        AudioSystem:PlayUISound("SFX_ThreatMax")
        t5 = true
    end

    if wantedLevel >= 5 then
        UISystem:setThreat(wantedLevel * 20)
    else
        UISystem:setThreat((wantedLevel * 100) % 100)
    end
        
end

function unlockAllWeapons()
    if Inputs.get_Key(InputKey.K_KEY) or godMode then
       laserUnlocked = true
       rktUnlocked = true
       ECS:getComponent_SpriteCom(ECS:getEntityID("UI_Weapon_Laser"))._drawSprite = true
       ECS:getComponent_SpriteCom(ECS:getEntityID("UI_Weapon_Rocket"))._drawSprite = true
    end
end

function handleHelicopterRocketDelay(dt)
    if rocketDelayTimer > 0.0 then
        rocketDelayTimer = rocketDelayTimer - dt
    end

    if rocketDelayTimer <= 0.0 then
        canGetDamagedByExplosion = true
    end
end

function spawnBossNow()
    if wantedLevel >= 5 and ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().allObjectivesCleared then
        if currentStageLevel == 1 then
            -- instantiate boss prefabs (weapons and body) for level 1
			uiBossBarID        = ECS:getEntityID("UI_BossHealthBar")
	        uiBossFillID       = ECS:getEntityID("UI_BossHealthColor")
			if ECS:hasComponent_SpriteCom(uiBossBarID) then
				local sprite = ECS:getComponent_SpriteCom(uiBossBarID)
				
				sprite._drawSprite = true
				
			end
			
			if ECS:hasComponent_SpriteCom(uiBossFillID) then
				local sprite = ECS:getComponent_SpriteCom(uiBossFillID)
				
				sprite._drawSprite = true
				
			end
			
			
			
			spawnBoss = true

        elseif currentStageLevel == 2 then
            ECS:instantiatePrefab(bossLevel2CutsceneID, "AAABossIntroCutscene2", Vector3:new(0,0,1), Quat:new(0,0,0,1), false, false, false)

            -- instantiate boss prefabs (weapons and body) for level 2
            local rigid1 = Vector3:new()
            local rigid2 = Vector3:new()
            local rigid3 = Vector3:new()
            local rigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
            rigid1.x = 1.75 ---10.0
            rigid1.y = 2.0 -- 10.0
            rigid1.z = 32.5 --40.0
            rigid2.x = -10 ---15.0
            rigid2.y = 6 -- 5.0
            rigid2.z = 25 -- 40.0
            rigid3.x = -10 ---5.0
            rigid3.y = 8.18 --10.0
            rigid3.z = 25 --rigid1.z
            --Panel
            -- local id = ECS:instantiatePrefab(bossLevel2PanelPrefabID, "Boss_Panel_LVL2", Vector3:new(-10,4,32.5), pRigidDynamic.rigidRot, false, false, false)
            -- local id2 =ECS:instantiatePrefab(bossLevel2PanelPrefabID, "Boss_Panel2_LVL2", rigid1, pRigidDynamic.rigidRot, false, false, false)
            local id3 = ECS:instantiatePrefab(bossLevel2BodyPrefabID, "Boss_Body1_LVL2", Vector3:new(-10,4,25), pRigidDynamic.rigidRot, false, false, false)
            local id4 = ECS:instantiatePrefab(bossLevel2BodyPrefabID, "Boss_Body2_LVL2", rigid2, pRigidDynamic.rigidRot, false, false, false)
            local id5 = ECS:instantiatePrefab(bossLevel2BodyPrefabID, "Boss_Body3_LVL2", rigid3, pRigidDynamic.rigidRot, false, false, false)
            if ECS:hasComponent_Transform(id3) then
                local transform = ECS:getComponent_Transform(id3)
                transform.pos.y = 5.0
            end
            if ECS:hasComponent_Transform(id4) then
                local transform = ECS:getComponent_Transform(id4)
                transform.pos.y = 8.0
            end

            local boss2ID1 = ECS:instantiatePrefab(bossLevel2PrefabID1, "Boss_Head_LVL2", Vector3:new(0.0,13.6,38.0), pRigidDynamic.rigidRot, false, false, false)
            local boss2ID2 = ECS:instantiatePrefab(bossLevel2PrefabID2, "Boss_Weapon1", Vector3:new(-9.297, 14.858, 35.885), pRigidDynamic.rigidRot, false, false, false)
            local boss2ID3 = ECS:instantiatePrefab(bossLevel2PrefabID3, "Boss_Weapon2", Vector3:new(10.212, 11.828, 36.043), pRigidDynamic.rigidRot, false, false, false)
            spawnBoss = true

            -- for video
            -- playVideoOnce = true
            -- currentVideo = 5
			
        elseif currentStageLevel == 3 then
            ECS:instantiatePrefab(bossLevel3CutsceneID, "AAABossIntroCutscene3", Vector3:new(0,0,1), Quat:new(0,0,0,1), false, false, false)

			-- Inable to instantiate, dont know why
			local b3upTurID = ECS:instantiatePrefab(boss3_upperTurrID, "3_BossUpperTurr", Vector3:new(0.515,6.893,37.540), Quat:new(0,0,0,1), false, false, false)
			local b3lowTurID = ECS:instantiatePrefab(boss3_lowerTurrID, "3_BossLowerTurr", Vector3:new(0.765,3,37.540), Quat:new(0,0,0,1), false, false, false)	
			local b3wp1ID = ECS:instantiatePrefab(boss3_Weap1ID, "3_BossHomingRocket", Vector3:new(10.086,11.828,38.121), Quat:new(0,0,0,1), false, false, false)
			local b3wp2ID = ECS:instantiatePrefab(boss3_Weap2ID, "3_BossRockShower", Vector3:new(-9.27,16.873,38.527), Quat:new(0,0,0,1), false, false, false)
			local bossHead3ID = ECS:instantiatePrefab(boss3_headID, "3_Boss_Head", Vector3:new(0.765,15.759,37.540), Quat:new(0,0,0,1), false, false, false)

            -- Console.WriteLine("Boss HEAD NAME:")
            -- Console.WriteLine(ECS:getEntityName(bossHead3ID))
            if bossHead3ID <= 0 then
                local bossHead3ID = ECS:instantiatePrefab(boss3_headID, "3_Boss_Head", Vector3:new(0.765,15.759,37.540), Quat:new(0,0,0,1), false, false, false)
            end

			if ECS:hasComponent_RigidDynamic(bossHead3ID) then
				local bPos = Vector3:new()
				bPos = PhysicsSystem:getGlobalPosePosition(bossHead3ID)
				bPos.y = 15.688
				PhysicsSystem:setGlobalPose(bossHead3ID, bPos, Quat:new(0,0,0,1)) 
			end


            if b3lowTurID ~= nil then
                if ECS:hasEntity(b3lowTurID) == true then
                    -- Console.WriteLine(ECS:getEntityName(b3lowTurID))
                end
            end

			spawnBoss = true

            -- for video
            -- playVideoOnce = true
            -- currentVideo = 8
        end
        
        local sprite = ECS:getComponent_SpriteCom(wantedUIID)
        sprite._drawSprite = false
        
        sprite = ECS:getComponent_SpriteCom(ECS:getEntityID("UI_ThreatColor"))
        sprite._drawSprite = false

        ECS:getComponent_SpriteCom(ECS:getEntityID("UI_ThreatUI"))._drawSprite = false

    end
end

function checkSpawnBoss()
    if ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().allObjectivesCleared then
        isSpawnBoss = true
    end
end

function playVideoCutscene()
    -- Console.WriteLine("PLAYING VIDEO CUTSCENE FUNCTION")
    -- Console.WriteLine(currentVideo)
    -- Console.WriteLine("setPlayVideo")
    -- Console.PrintBool(RenderSystem:getPlayVideo())
    -- Console.WriteLine("Current video")
    -- Console.WriteLine(currentVideo)
    if playVideoOnce == true then
        -- Console.WriteLine("PLAYVIDEO ONCE TRUE")
        if currentVideo == 1 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("intro.mp4")
            RenderSystem:setVideoEndTime(30.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(true)
            playVideoOnce = false
            Console.WriteLine("Playing currentVideo 1")
        elseif currentVideo == 2 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("boss_lazerernator.mp4")
            RenderSystem:setVideoEndTime(9.0)
            RenderSystem:setPlayVideo(true)
            playVideoOnce = false
            RenderSystem:setVideoMinimapAfter(true)
            Console.WriteLine("Playing currentVideo 2")
        elseif currentVideo == 3 then
            UISystem:toggleCutscene(true)
            ---- RenderSystem:toggleMinimap(false)
            -- RenderSystem:setVideoStart(true)
            -- RenderSystem:setVideoName("weapon_laser.mp4")
            -- RenderSystem:setVideoEndTime(15.0)
            -- RenderSystem:setPlayVideo(true)
            Console.WriteLine("Playing currentVideo 3")
            playVideoOnce = false
        elseif currentVideo == 4 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("cutscene_map1.mp4")
            RenderSystem:setVideoEndTime(6.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(false)
            Console.WriteLine("Playing currentVideo 4")
            playVideoOnce = false
        elseif currentVideo == 5 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("boss_rocketernator.mp4")
            RenderSystem:setVideoEndTime(9.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(true)
            playVideoOnce = false
        elseif currentVideo == 6 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            --AudioSystem:StopUISound("lv3BossEnter")
            --AudioSystem:UnpauseBGM("BG_Level3")
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("weapon_rocket.mp4")
            RenderSystem:setVideoEndTime(15.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(false)
            playVideoOnce = false
        elseif currentVideo == 7 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("cutscene_map2.mp4")
            RenderSystem:setVideoEndTime(6.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(false)
            playVideoOnce = false
        elseif currentVideo == 8 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("boss_nebulas.mp4")
            RenderSystem:setVideoEndTime(10.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(true)
            playVideoOnce = false
        elseif currentVideo == 9 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("final_ending.mp4")
            RenderSystem:setVideoEndTime(46.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(false)
            playVideoOnce = false
        elseif currentVideo == 10 then
            UISystem:toggleCutscene(true)
            -- RenderSystem:toggleMinimap(false)
            RenderSystem:setVideoStart(true)
            RenderSystem:setVideoName("credits.mp4")
            RenderSystem:setVideoEndTime(31.0)
            RenderSystem:setPlayVideo(true)
            RenderSystem:setVideoMinimapAfter(false)
            playVideoOnce = false
        end
    end
end

function HealthBarShake()
    if shakeTimer > 0.0 then
        local randX = Random.randInRange(-1.5, 1.5)
        local randZ = Random.randInRange(-1.5, 1.5)

        H_BarTransform.pos.x = H_BarStartPos.x + randX * shakeTimer
        H_BarTransform.pos.y = H_BarStartPos.y + randZ * shakeTimer

        H_ColorTransform.pos.x = H_ColorStartPos.x + randX * shakeTimer
        H_ColorTransform.pos.y = H_ColorStartPos.y + randZ * shakeTimer
    else
        H_BarTransform.pos.x = H_BarStartPos.x
        H_BarTransform.pos.y = H_BarStartPos.y
        --H_BarTransform.pos.z = H_BarStartPos.z

        H_ColorTransform.pos.x = H_ColorStartPos.x
        H_ColorTransform.pos.y = H_ColorStartPos.y
        --H_ColorTransform.pos.z = H_ColorStartPos.z
    end
    healthBarHitFlash(shakeTimer)
end

function PlayerShake()
    if shakeTimer > 0.0 then
        pTransform.scale.x = PlayerStartScale.x + Random.randInRange(-0.02, 0.02) * shakeTimer
        pTransform.scale.y = PlayerStartScale.y + Random.randInRange(-0.01, 0.01) * shakeTimer
        pTransform.scale.z = PlayerStartScale.z + Random.randInRange(-0.02, 0.02) * shakeTimer
    else
        pTransform.scale.x = PlayerStartScale.x
        pTransform.scale.y = PlayerStartScale.y
        pTransform.scale.z = PlayerStartScale.z
    end
end

function PlayerJumpStretch()
    if stretchTimer > 0.0 then
        pTransform.scale.y = lerp(PlayerStartScale.y, PlayerStartScale.y + 0.15, ParametricBlend(stretchTimer))
    --else
       --pTransform.scale.y = PlayerStartScale.y
    end

end

function PlayerLandSquish()
    if squishTimer > 0.0 then
        --pTransform.scale.y = PlayerStartScale.y + Random.randInRange(-0.05, 0.0) * squishTimer
        
        pTransform.scale.y = lerp(PlayerStartScale.y, PlayerStartScale.y - 0.15, ParametricBlend(squishTimer))
    --else
        --pTransform.scale.y = PlayerStartScale.y
    end

end

function handleShakeTimer(dt)
    if shakeTimer > 0.0 then
        shakeTimer = shakeTimer - dt
    end
end


function handleSquishTimer(dt)
    if squishTimer > 0.0 then
        squishTimer = squishTimer - dt
    end
end

function handleStretchTimer(dt)
    if stretchTimer > 0.0 then
        stretchTimer = stretchTimer - dt
    end
end

function checkLanding()
    checkland = contactland
    contactland = mContactReportCallback.playerOnFloor

    if checkland == false and contactland == true then
        squishTimer = squishTime
    end
end

function handleHealTimer(dt)
    if healTimer > 0.0 then
        healTimer = healTimer - dt
        --healthEmit()
    else
        healingSprite._drawSprite = false
        --bEmitter.emit = false
    end

end

--[[
function dashTrail()

    bEmitter.colorBegin.x = Random.Float()
    bEmitter.colorBegin.y = Random.Float()
	bEmitter.colorBegin.z = Random.Float()
	
    bEmitter.colorEnd.x = Random.Float()
    bEmitter.colorEnd.y = Random.Float()
	bEmitter.colorEnd.z = Random.Float()

	bEmitter.sizeBegin = 0.6
	bEmitter.sizeEnd   = 0
	
	bEmitter.lifeTime = 1
	
	--speed, duration
	
end]]

function healthBarHitFlash(t)
    if ECS:hasComponent_SpriteCom(UI_pHealthBarID) then
        local healthsprite = ECS:getComponent_SpriteCom(UI_pHealthBarID)
        healthsprite._colour.x = lerp(1, 10, ParametricBlend(t))
    end
end

function healthbarHealFlash(t)
    if ECS:hasComponent_SpriteCom(UI_pHealthBarID) then
        local healthsprite = ECS:getComponent_SpriteCom(UI_pHealthBarID)
        healthsprite._colour.y = lerp(1, 10, ParametricBlend(t))
        --if canPlayHealSound then
        --    AudioSystem:PlayUISound("SFX_HealthRegen")
        --    canPlayHealSound = false
        --end
        if t >= 0 then
            healingSprite._drawSprite = true
            healingSprite._alphaVal = lerp(0.0, 5.0, t)
        else
            --Console.WriteLine("healed")
            healingSprite._drawSprite = false
        end
    end
end

function healthEmit()
    
    bEmitter.velocityVariation.x = 25
	bEmitter.velocityVariation.y = 25
	bEmitter.velocityVariation.z = 25

    bEmitter.colorBegin.y = 1
	
    bEmitter.colorEnd.y = 0.2

	bEmitter.sizeBegin = 0.5
	bEmitter.sizeEnd   = 0
	
	bEmitter.lifeTime = 0.5
    bEmitter.emit = true
end

function spawnHealingUI()
    if healthUIspawned == false then
        
        healingID = ECS:instantiatePrefab(pHealingUI, "Healing", pRigidDynamic.rigidPos,pRigidDynamic.rigidRot,false, false, false)
        healthUIspawned = true 
        healingSprite = ECS:getComponent_SpriteCom(healingID)
        healingSprite._drawSprite= false
        healingSprite._alphaVal = 0
        UltiID = ECS:instantiatePrefab(pUltiUI, "Ultimate", pRigidDynamic.rigidPos,pRigidDynamic.rigidRot,false, false, false)
        UltiUISprite = ECS:getComponent_SpriteCom(UltiID)
        UltiUISprite._drawSprite= false
    else
        local transform = ECS:getComponent_Transform(healingID)
        local transform2 = ECS:getComponent_Transform(UltiID)
        transform.pos = pTransform.pos
        transform2.pos = pTransform.pos + upward * 0.5
        
    end
end

function updateAmmoText()
    if RenderSystem:hasVideoName() == true then
        if RenderSystem:getVideoName() == "final_ending.mp4" or RenderSystem:getVideoName() == "weapon_laser.mp4" or RenderSystem:getVideoName() == "weapon_rocket.mp4" then
            if UISystem:getGameState() == 1 or UISystem:getGameState() == 2 or UISystem:getGameState() == 3 then
                if ECS:hasComponent_TextCom(UI_AmmoCounterID) then
                    local ammoComp = ECS:getComponent_TextCom(UI_AmmoCounterID)
                    ammoComp.TextName = ""
                    ammoComp._size = 0.0
                    laserCounter = false
                end

                if ECS:hasComponent_TextCom(UI_AmmoCounterID1) then
                    local ammoComp = ECS:getComponent_TextCom(UI_AmmoCounterID1)
                    ammoComp.TextName = ""
                    ammoComp._size = 0.0
                    rocketCounter = false
                end
                godMode = false
            end
        end
    end
end