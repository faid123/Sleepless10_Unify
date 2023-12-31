-- colorist enemy
local IdOfEnemyNum = 0;
local IdOfEnemy = {}
local preIdOfEnemy = {}
local checkKilledList = {}

local Consecultively = 0
local timer = 0.0

local resetTime = 2
local target = 3

local material
local once = true

local ObjectiveID
local objSpriteCom

-- colorist building
local IDListNum = 0
local IDList = {}

local IdOfBuilding = {}
local IdOfBuildingNum = 0;

-- gate open
local GateID

function Awake(  )
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
	updateIDOfEnemy()
	checkForConsecultive()

	if(timer > 0) then
		timer = timer - dt
	elseif ((not(Consecultively == target) and timer <= 0)) then
		Consecultively = 0
		------Console.WriteLine("timer up")
	end

	if(once and target == Consecultively) then
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
		------Console.WriteLine("Lv2 gate open")
	end

	preIdOfEnemy = IdOfEnemy
end

function updateIDOfEnemy()
	IdOfEnemy = ECS:getEntityTagged("Enemy")
	IdOfEnemyNum = #IdOfEnemy
	if (not(#IdOfEnemy == #preIdOfEnemy)) then
		------Console.WriteLine("start updaed")
				
			-- replacing check list
			local curCheckList = {}
			for i=0,#IdOfEnemy do
				-- loop to check if there is similar id from pre
				local changed = true
				for j = 0, #preIdOfEnemy do
					if (IdOfEnemy[i] == preIdOfEnemy[j]) then
						curCheckList[i] = checkKilledList[j]
						changed = false
					end
				end
				if (changed == true) then
					curCheckList[i] = true
				end
			end
			checkKilledList = curCheckList
		
		------Console.WriteLine("end updaed")
	end
end

function checkForConsecultive(  )
	if(not(#IdOfEnemy == 0)) then		
		for i = 1 , #IdOfEnemy do			
			material = ECS:getComponent_Material(IdOfEnemy[i])
			if(material._saturation.y > 0.0) and checkKilledList[i] == true then
				Consecultively = Consecultively + 1
				timer = resetTime
				checkKilledList[i] = false
				--Console.WriteLine("Consecultively : ")
				--Console.PrintInt(Consecultively)
			end
			----Console.PrintInt(i)
		end

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