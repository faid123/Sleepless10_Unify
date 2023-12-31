local GunHP = 3
local LaserHP = 3
local RocketHP = 3
local eFlashTimer
local eMaterial
local eRigidBody


local isHit = false

local ID
local BulletID

local Rigid
local tagString

local ObjectiveID
local objSpriteCom

-- colorist building
local IDListNum = 0
local IDList = {}

local IdOfBuilding = {}
local IdOfBuildingNum = 0;

local material

-- colorist enemy
local IdOfEnemy = {}
local IdOfEnemyNum = 0;

function Awake(  )
	-- body
	eFlashTimer = 1
end

function Start(  )
	IdOfBuilding = ECS:getEntityTagged("Building")
	IdOfBuildingNum = #IdOfBuilding
	for i = 0, IdOfBuildingNum do
		IDList[i] = false
	end
end

function Update( dt )
	-- there is collision with bullet
	ID = ScriptSystem:getLuaEntityID()
	
	IdOfEnemy = ECS:getEntityTagged("Enemy")
	IdOfEnemyNum = #IdOfEnemy

	if ECS:hasComponent_Material(ID) and ECS:hasComponent_RigidStatic(ID) then
        eMaterial = ECS:getComponent_Material(ID)
        eRigidBody = ECS:getComponent_RigidStatic(ID)
    end

	if ECS:hasComponent_RigidStatic(ID) then
		Rigid = ECS:getComponent_RigidStatic(ID)
		isHit = Rigid.getHit
	end	

	if (isHit == true) then
		BulletID = mContactReportCallback.detectedBullet
		
		if (GunHP) > 0 and (ECS:hasTagInEntity(BulletID, "Bullet")) then
		eFlashTimer = 8
			GunHP = GunHP - 1
			UISystem:decreasePistolHealth(34);
			----Console.WriteLine("GunHP: ")
			--Console.PrintInt(GunHP)
		elseif (LaserHP > 0) and (ECS:hasTagInEntity(BulletID, "Lazer")) then
		eFlashTimer = 8
			LaserHP = LaserHP - 1
			UISystem:decreaseLaserHealth(34);
			----Console.WriteLine("Laser: ")
			--Console.PrintInt(Laser)
		elseif (RocketHP > 0 ) and (ECS:hasTagInEntity(BulletID, "Rocket")) then
		eFlashTimer = 8
			RocketHP = RocketHP - 1
			UISystem:decreaseRocketHealth(34);
			----Console.WriteLine("RocketHP: ")
			--Console.PrintInt(RocketHP)
		end
		Rigid.getHit = false
	end

	-- complete lv
	if ((GunHP == 0) and (RocketHP == 0) and (LaserHP == 0)) then
		ObjectiveID =  ECS:getEntityID("UI_OBJ_Overload")
		if ECS:hasComponent_SpriteCom(ObjectiveID) then
			objSpriteCom = ECS:getComponent_SpriteCom(ObjectiveID)
			objSpriteCom:setName("RoomCompleted")
			unifyAllBuilding(true)
			unifyAllEnemy(true)
		end
		------Console.WriteLine("Lv3 complete")
		ScriptSystem:setNState(ScriptSystem:getGameState())
		ScriptSystem:setGameState(6)
	end

	if eFlashTimer > 1.5 then
        eFlashTimer = eFlashTimer - dt * 10
        flashOnHit()
    end
end

function flashOnHit()

    if eFlashTimer > 1 then
            eMaterial._saturation.z = eFlashTimer
    end
end

function unifyAllBuilding( check )
	if(check == true) then 
		if(not(IDListNum == IdOfBuildingNum)) then
			for i = 1, IdOfBuildingNum do 
				if(not IDList[i]) then
					local id = IdOfBuilding[i]
					material = ECS:getComponent_Material(IdOfBuilding[i])
					if(material._saturation.y == 0.0) then
						material._saturation.y = 1
					end
				end
			end
		end
	end
end

function unifyAllEnemy( check )
	if(check == true) then 
		for i = 1, IdOfEnemyNum do 
			local id = IdOfEnemy[i]
			material = ECS:getComponent_Material(IdOfEnemy[i])
			if(material._saturation.y == 0.0) then
				material._saturation.y = 1
				--Console.WriteLine("test")
			end
		end
	end
end