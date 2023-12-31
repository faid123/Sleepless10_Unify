-- Target
local targetID
local targetTransform
local targetRotQuat
local targetStaticBody
local targetMaterial

-- TargetLED
local targetLEDSpawned		= false
local targetLEDprefabID
local targetLED_ID
local targetLED_Transform
local targetLED_RigidStatic
local targetLED_Material
local yOffset				= 1.6
local ledRotOffset			= Quat:new(0,0.707,0,0.707)

-- General
local laserdmgdelayTime		= 0.0
local canApplyLaserDamage	= true

function Awake()
end

function Start()
	targetLEDprefabID = ECS:getPrefabID("Prefab_TargetLED")
end

function Update(dt)
	targetID = ScriptSystem:getLuaEntityID()
	targetTransform = ECS:getComponent_Transform(targetID)
	targetStaticBody = ECS:getComponent_RigidStatic(targetID)
	targetMaterial = ECS:getComponent_Material(targetID)
	spawnTargetLED()
	handleLaserDelay(dt)
	checkActivationStatus()
	onCollide()
	destroyTargets()
end

function onCollide()
    if targetStaticBody.getHit and mContactReportCallback.laserHitTarget and canApplyLaserDamage  then
		if not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
			laserdmgdelayTime = 0.5
			canApplyLaserDamage = false
			if not ECS:hasTagInEntity(targetID, "Activated") then
				ECS:addEntityTag(targetID, "Activated")
			end
		end
	elseif targetStaticBody.getHit and mContactReportCallback.explosionHitTarget then
		if not ECS:hasTagInEntity(targetID, "Activated") then
			ECS:addEntityTag(targetID, "Activated")
		end
	elseif mContactReportCallback.bulletHitTarget and mContactReportCallback._detectedTarget == targetID then
		if not ECS:hasTagInEntity(targetID, "Activated") then
			ECS:addEntityTag(targetID, "Activated")
		end
    end
end

function handleLaserDelay(dt)
    if laserdmgdelayTime > 0.0 then
        laserdmgdelayTime = laserdmgdelayTime - dt        
    end

    if laserdmgdelayTime <= 0.0 then
        canApplyLaserDamage = true
    end
end

function spawnTargetLED()
	if not targetLEDSpawned then
		targetLED_ID = ECS:instantiatePrefab(targetLEDprefabID, "TargetLED", targetStaticBody.pos,targetStaticBody.rot,false, false, false)

		if ECS:hasComponent_Transform(targetLED_ID) then
			targetLED_Transform = ECS:getComponent_Transform(targetLED_ID)
			targetLED_Transform.pos = targetStaticBody.pos
			targetLED_Transform.pos.y = targetLED_Transform.pos.y + yOffset
			targetLED_Transform.rotate = PhysicsSystem:convertQuatToEuler(targetStaticBody.rot)
			targetLED_Transform.rotate.y = targetLED_Transform.rotate.y + 90.0
		end

		if ECS:hasComponent_RigidStatic(targetLED_ID) then
			targetLED_RigidStatic = ECS:getComponent_RigidStatic(targetLED_ID)
			targetLED_RigidStatic.rot = PhysicsSystem:convertEulerToQuat(targetLED_Transform.rotate)
		end

		if ECS:hasComponent_Material(targetLED_ID) then
			targetLED_Material = ECS:getComponent_Material(targetLED_ID)
		end

		targetLEDSpawned = true
	end
end

function checkActivationStatus()
	if ECS:hasTagInEntity(targetID, "Activated") then
		targetLED_Material.colour.x = 0.0
		targetLED_Material.colour.y = 1.0
	else
		targetLED_Material.colour.x = 1.0
		targetLED_Material.colour.y = 0.0
	end
end

function destroyTargets()
	if ECS:hasTagInEntity(targetID, "Destroy") then
		PhysicsSystem:LuaRemoveActor(targetLED_ID)
		ECS:addEntityToDestroyQueue(targetLED_ID)

		PhysicsSystem:LuaRemoveActor(targetID)
		ECS:addEntityToDestroyQueue(targetID)
	end
end