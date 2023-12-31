-- colorist Building
local IDListNum = 0
local IDList = {}

local IdOfBuilding = {}
local IdOfBuildingNum = 0;

local material
local CaseID
local ObjectiveID
local objSpriteCom
local once = true

-- colorist enemy
local IdOfEnemy = {}
local IdOfEnemyNum = 0;

-- gate open
local GateID

function Awake()
	-- body
end

function Start(  )
	IdOfBuilding = ECS:getEntityTagged("Building")
	IdOfBuildingNum = #IdOfBuilding
	for i = 0, IdOfBuildingNum do
		IDList[i] = false
	end
end

function Update( dt )

	IdOfEnemy = ECS:getEntityTagged("Enemy")
	IdOfEnemyNum = #IdOfEnemy

	if(not(IDListNum == IdOfBuildingNum)) then
		for i = 1, IdOfBuildingNum do 
			if(not IDList[i]) then
				local id = IdOfBuilding[i]
				material = ECS:getComponent_Material(IdOfBuilding[i])
				if(material._saturation.y> 0.0) then
					IDList[i] = true
					IDListNum = IDListNum + 1
					--Console.PrintInt(IDListNum)
				end
			end
		end
	end

	if(once and IDListNum == IdOfBuildingNum) then
		CaseID = ECS:getEntityID("WeaponCase")
		ECS:addEntityToDestroyQueue(CaseID)
        PhysicsSystem:LuaRemoveActor(CaseID)
		GateID = ECS:getEntityID("FenceGate")
		ECS:addEntityToDestroyQueue(GateID)
        PhysicsSystem:LuaRemoveActor(GateID)
		ObjectiveID =  ECS:getEntityID("UI_OBJ_Overload")
		if ECS:hasComponent_SpriteCom(ObjectiveID) then
			objSpriteCom = ECS:getComponent_SpriteCom(ObjectiveID)
			objSpriteCom:setName("RoomCompleted")
			--unifyAllBuilding(true)
			unifyAllEnemy(true)
		end
		once = false
		--Console.WriteLine("LV1 gate open")
	end

	
end

function unifyAllBuilding( check )
	if(check == true) then 
		for i = 1, IdOfBuildingNum do 
			local id = IdOfBuilding[i]
			material = ECS:getComponent_Material(IdOfBuilding[i])
			if(material._saturation.y == 0.0) then
				material._saturation.y = 1
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

function gateOpen (check)
	
end