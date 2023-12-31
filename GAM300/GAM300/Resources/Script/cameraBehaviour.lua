local eEntID
local pCam
local camRigidDynamic
local transformCam
local cam1
local cam2
local otherCam
local otherCamRigid
local changeValue = 0.5

function Awake()	
	cam1 = ECS:getEntityID("PlayerCamera1")
	cam2 = ECS:getEntityID("PlayerCamera2")
	eEntID = cam1
end


function Start()	
	if ECS:hasComponent_RigidDynamic(eEntID) then
		pCam = ECS:getComponent_Camera(eEntID)
		camRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
		transformCam = ECS:getComponent_Transform(eEntID)
		PhysicsSystem:setGravityFlag(eEntID, true)
		otherCam = ECS:getComponent_Camera(cam2)
		pCam = ECS:getComponent_Camera(eEntID)
	end
end

function Update(dt)
	-- setting PhysicsSystem pos 
	if(ECS:hasComponent_RigidDynamic(eEntID)) then	
		PhysicsSystem:setGlobalPose(eEntID, pCam._pos, PhysicsSystem:convertEulerToQuat(pCam._rotation))
	end

	-- move cam2	
	if (mContactReportCallback.cameraCollision1) then
		--Console.WriteLine("decrease cam2")
		equal()
		otherCam._displaceFor = pCam._displaceFor - changeValue
		CameraSystem:updateCollisionCamera(dt)
	else
		--Console.WriteLine("increase cam2")
		equal()
		otherCam._displaceFor = pCam._displaceFor + changeValue
		CameraSystem:updateCollisionCamera(dt)
	end
	
end

function equal()
	otherCam._pos = pCam._pos
	otherCam._rotation = pCam._rotation
	otherCam._up = pCam._up

	otherCam._displaceRight = pCam._displaceRight
	otherCam._displaceUp = pCam._displaceUp
	otherCam._displaceFor = pCam._displaceFor
end