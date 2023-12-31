local eEntID
local pCam
local camRigidDynamic
local transformCam
local cam1
local cam2
local identity = 0
local otherCam
local changeValue = 0.1
local once = true
local isWalk = false
local playerID
local player_env
local isDecreasing = false
local isIncreasing = false

function Awake()
	cam1 = ECS:getEntityID("PlayerCamera1")
	cam2 = ECS:getEntityID("PlayerCamera2")
	playerID = ECS:getEntityID("Player")
	eEntID = cam2
end


function Start()	
	if ECS:hasComponent_RigidDynamic(eEntID) then
		pCam = ECS:getComponent_Camera(eEntID)
		camRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
		transformCam = ECS:getComponent_Transform(eEntID)
		PhysicsSystem:setGravityFlag(eEntID, true)
		otherCam = ECS:getComponent_Camera(cam1)
	end

end

function Update(dt)
	if(ECS:hasComponent_RigidDynamic(eEntID)) then	
		PhysicsSystem:setGlobalPose(eEntID, pCam._pos, PhysicsSystem:convertEulerToQuat(pCam._rotation))
	end

	-- case 1	1 on 2 on =  move
	-- case 2	1 on 2 off = dont move
	-- case 3	1 off 2 off = move
	-- case 4	1 off 2 on = dont move
	-- move cam 1
	if(mContactReportCallback.cameraCollision2 and mContactReportCallback.cameraCollision1 and pCam._displaceFor > 1.0) then
		-- Console.WriteLine("decreasing ")
		isDecreasing = true
		if(isDecreasing and isIncreasing) then
			isDecreasing = false
			isIncreasing = false
		else
			otherCam._displaceFor = pCam._displaceFor			
		end
		mContactReportCallback.cameraCollision1 = false
		mContactReportCallback.cameraCollision2 = false

	--elseif (mContactReportCallback.cameraCollision2 and not mContactReportCallback.cameraCollision1) then
	--	otherCam._displaceFor = pCam._displaceFor
	elseif((not mContactReportCallback.cameraCollision2) and (not mContactReportCallback.cameraCollision1) and pCam._displaceFor < 12.0) then
		-- Console.WriteLine("increasing ")
		isIncreasing = true
		if(isDecreasing and isIncreasing) then
			isDecreasing = false
			isIncreasing = false
		else
			otherCam._displaceFor = pCam._displaceFor
		end
	end

	if(mContactReportCallback.cameraCollision1) then
		mContactReportCallback.cameraCollision1 = false
	end
	if(mContactReportCallback.cameraCollision2) then
		mContactReportCallback.cameraCollision2 = false
	end
end