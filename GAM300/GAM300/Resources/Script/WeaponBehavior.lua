--stuff goes here
-- Player Cam
local pCam
-- Component Variables
local wTransform
local wRigidDynamic
local wMaterial
local eEntID
local playerID
local pTransform
local rightward  = Vector3:new(1.0, 0.0, 0.0)
local rightwardPrime  = Vector3:new()
local pRotNew       = Quat:new()
local laserUnlocked = false
local rktUnlocked = false
local rightwardOffset = 0.85

-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Laser Behavior script loaded")

   if(laserUnlocked == false) then
        if(UISystem:getGameState() == 2) then 
                 laserUnlocked = true
        end
    end
    if(rktUnlocked == false) then
        if(UISystem:getGameState() == 3)then
                 rktUnlocked = true
                laserUnlocked = true
        end
    end
end
                                                                                                                                                                                           
-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    wTransform = ECS:getComponent_Transform(eEntID)
    wRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    wMaterial = ECS:getComponent_Material(eEntID)

    if ECS:hasEntity("Player") then
        playerID = ECS:getEntityID("Player")
        pTransform = ECS:getComponent_Transform(playerID)
        pRigidDynamic = ECS:getComponent_RigidDynamic(playerID)
        pCam = CameraSystem:getPlayerCamera() 
        pRotNew = PhysicsSystem:getGlobalPoseRotation(playerID)
    
        --PhysicsSystem:rotate_vector_by_angle(rightward, pTransform.rotate.y, rightwardPrime)
        PhysicsSystem:rotate_vector_by_quaternion(rightward, pRigidDynamic.rigidRot, rightwardPrime)
        wTransform.pos = pTransform.pos + rightwardPrime * rightwardOffset

        wTransform.rotate.y = pCam._rotation.y
        wTransform.rotate.x = pCam._rotation.x * -1.0
        wTransform.rotate.z = pCam._rotation.z * -1.0
        
        wRigidDynamic.rigidPos = wTransform.pos
        wRigidDynamic.rigidRot = PhysicsSystem:convertEulerToQuat(wTransform.rotate)

        --toggleWeapon()
    end
end

function toggleWeapon()
    if Inputs.get_Key_Triggered(InputKey.KEY_1) then
        wMaterial.m_model = "Pistol_Bone"
        wMaterial.m_texture = "Pistol"
        wMaterial.isInitialised = false
    elseif Inputs.get_Key_Triggered(InputKey.KEY_2) then
        if laserUnlocked then
            wMaterial.m_model = "Railgun_Bone"
            wMaterial.m_texture = "Railgun"
            wMaterial.isInitialised = false
        end
    elseif Inputs.get_Key_Triggered(InputKey.KEY_3) then
        if rktUnlocked then
            wMaterial.m_model = "Rocket_launcher_Bone"
            wMaterial.m_texture = "Rocket Launcher"
            wMaterial.isInitialised = false
        end
    end
end