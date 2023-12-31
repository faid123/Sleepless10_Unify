-- Police
local polSceneID
local polObjCounter = 0
local polActivated = false
local polCleard = false

-- Army
local armySceneID
local armyObjCounter = 0
local armyActivated = false
local armyCleared = false

-- Shield
local shieldSceneID
local shieldObjCounter = 0
local shieldActivated = false
local shieldCleared = false

-- Clown
local clownSceneID
local clownObjCounter = 0
local clownActivated = false
local clownCleared = false

-- Building
local buildSceneID
local buildObjCounter = 0
local buildActivated = false
local buildCleared = false

-- Trees
local treeSceneID
local treeObjCounter = 0
local treeActivated = false
local treeCleared = false

-- Civilian
local civiSceneID
local civiObjCounter = 0
local civiActivated = false
local civiCleared = false

-- Rocket Tutorial
local rktTutorialID
local rktTutorialActivated = false
local rktTutorialCleared = false

-- Laser Tutorial
local laserTutorialID
local laserTutorialActivated = false
local laserTutorialCleared = false

-- UI
local UIObjID
local spriteCom

-- Text Font
local fontObjText_1
local fontKillCountID_1
local fontCounterID_1

local fontObjText_2
local fontKillCountID_2
local fontCounterID_2

local fontObjText_3
local fontKillCountID_3
local fontCounterID_3

local fontObjText_4
local fontKillCountID_4
local fontCounterID_4

local fontObjText_5
local fontKillCountID_5
local fontCounterID_5

-- TextManager
local textManagerID

-- Obj board
local eTransform
local startingX = -220.0
local destinationX = -115
local travelSpeed = 350.0

local textStartX1 = -2.65
local textStartX2 = textStartX1 + 0.710
local textStartX3 = textStartX2 + 0.03

local textFinalX1 = -1.60
local textFinalX2 = textFinalX1 + 0.710
local textFinalX3 = textFinalX2 + 0.030

local y1 = 0.270
local y2 = y1 - 0.12
local y3 = y2 - 0.12
local y4 = y3 - 0.13
local y5 = y4 - 0.12

local y6 = 0.24
local y7 = y6 - 0.12
local y8 = y7 - 0.12
local y9 = y8 - 0.13
local y10 = y9 - 0.12

local fontsize = 40.0

-- Text table
textManager = {}

-- Notification
local toNotify = false
local shakeTimer = 0.0
local shakeTime = 0.5
local objBoardStartPos = Vector3:new()
local objBoardTransform
local resetNotify = false

local pEntID

function Awake()

end

function Start()

    pEntID = ECS:getEntityID("Player")
	UIObjID = ECS:getEntityID("UI_Challenge")
	spriteCom = ECS:getComponent_SpriteCom(UIObjID)
	objBoardTransform = ECS:getComponent_Transform(UIObjID)

	-- Store starting position of the UI board
	objBoardStartPos.x = objBoardTransform.pos.x
	objBoardStartPos.y = objBoardTransform.pos.y
	objBoardStartPos.z = objBoardTransform.pos.z

	if ECS:hasComponent_SpriteCom(UIObjID) then
		--spriteCom._drawSprite = true
		spriteCom.SpriteName = "objectives"
	end

	-- Max Unify Count
	fontKillCountID_1 = ECS:getEntityID("Text_KillCounter_1")
	fontKillCountID_2 = ECS:getEntityID("Text_KillCounter_2")
	fontKillCountID_3 = ECS:getEntityID("Text_KillCounter_3")
	fontKillCountID_4 = ECS:getEntityID("Text_KillCounter_4")
	fontKillCountID_5 = ECS:getEntityID("Text_KillCounter_5")

	-- Current Unify Count
	fontCounterID_1 = ECS:getEntityID("Text_Counter_1")
	fontCounterID_2 = ECS:getEntityID("Text_Counter_2")
	fontCounterID_3 = ECS:getEntityID("Text_Counter_3")
	fontCounterID_4 = ECS:getEntityID("Text_Counter_4")
	fontCounterID_5 = ECS:getEntityID("Text_Counter_5")
	
	-- Scenario Objective
	fontObjText_1 = ECS:getEntityID("Text_Obj_1")
	fontObjText_2 = ECS:getEntityID("Text_Obj_2")
	fontObjText_3 = ECS:getEntityID("Text_Obj_3")
	fontObjText_4 = ECS:getEntityID("Text_Obj_4")
	fontObjText_5 = ECS:getEntityID("Text_Obj_5")

	local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
	textcom._colour = Vector4:new(1,1,1,0)

	textcom = ECS:getComponent_TextCom(fontKillCountID_2)
	textcom._colour = Vector4:new(1,1,1,0)

	textcom = ECS:getComponent_TextCom(fontKillCountID_3)
	textcom._colour = Vector4:new(1,1,1,0)

	textcom = ECS:getComponent_TextCom(fontKillCountID_4)
	textcom._colour = Vector4:new(1,1,1,0)

	textcom = ECS:getComponent_TextCom(fontKillCountID_5)
	textcom._colour = Vector4:new(1,1,1,0)

	-- Set Max Unify Counter depending on current level
	if UISystem:getGameState() == 1 then
		buildSceneID = ECS:getEntityID("buildingScenario")
		civiSceneID = ECS:getEntityID("civilianScenario")
		treeSceneID = ECS:getEntityID("treeScenario")
		polSceneID = ECS:getEntityID("policeScenario")
		armySceneID = ECS:getEntityID("armyScenario")
		
		buildObjCounter = 5
		civiObjCounter = 5
		treeObjCounter = 5
		polObjCounter = 5
		armyObjCounter = 5

		local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
		textcom.TextName = "/ " .. buildObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_2)
		textcom.TextName = "/ " .. civiObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_3)
		textcom.TextName = "/ " .. treeObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_4)
		textcom.TextName = "/ " .. polObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_5)
		textcom.TextName = "/ " .. armyObjCounter
	elseif UISystem:getGameState() == 2 then
		buildSceneID = ECS:getEntityID("buildingScenario")
		civiSceneID = ECS:getEntityID("civilianScenario")
		laserTutorialID = ECS:getEntityID("LaserScenarioTutorial")
		polSceneID = ECS:getEntityID("policeScenario")
		shieldSceneID = ECS:getEntityID("shieldScenario")

		buildObjCounter = 10
		civiObjCounter = 7
		polObjCounter = 10
		shieldObjCounter = 8

		local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
		textcom.TextName = "/ " .. buildObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_2)
		textcom.TextName = "/ " .. civiObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_3)
		textcom.TextName = "/ " .. polObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_5)
		textcom.TextName = "/ " .. shieldObjCounter
	elseif UISystem:getGameState() == 3 then
		buildSceneID = ECS:getEntityID("buildingScenario")
		civiSceneID = ECS:getEntityID("civilianScenario")
		rktTutorialID = ECS:getEntityID("RocketTutorialScenario")
		polSceneID = ECS:getEntityID("policeScenario")
		clownSceneID = ECS:getEntityID("clownScenario")

		buildObjCounter = 5
		civiObjCounter = 5
		polObjCounter = 5
		clownObjCounter = 5

		local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
		textcom.TextName = "/ " .. buildObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_2)
		textcom.TextName = "/ " .. civiObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_3)
		textcom.TextName = "/ " .. polObjCounter

		textcom = ECS:getComponent_TextCom(fontKillCountID_5)
		textcom.TextName = "/ " .. clownObjCounter	
	end

	-- Update Text_CurrentCounter
	local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
	textcounter.TextName = "0"
	textcounter._colour = Vector4:new(1,1,1,0)
	
	textcounter = ECS:getComponent_TextCom(fontCounterID_2)
	textcounter.TextName = "0"
	textcounter._colour = Vector4:new(1,1,1,0)

	textcounter = ECS:getComponent_TextCom(fontCounterID_3)
	textcounter.TextName = "0"
	textcounter._colour = Vector4:new(1,1,1,0)

	textcounter = ECS:getComponent_TextCom(fontCounterID_4)
	textcounter.TextName = "0"
	textcounter._colour = Vector4:new(1,1,1,0)

	textcounter = ECS:getComponent_TextCom(fontCounterID_5)
	textcounter.TextName = "0"
	textcounter._colour = Vector4:new(1,1,1,0)

	-- Update Text_Objective
	local textobj = ECS:getComponent_TextCom(fontObjText_1)
	textobj.TextName = "???????"
	textobj._colour = Vector4:new(1,1,1,1)
	
	textobj = ECS:getComponent_TextCom(fontObjText_2)
	textobj.TextName = "???????"
	textobj._colour = Vector4:new(1,1,1,1)

	textobj = ECS:getComponent_TextCom(fontObjText_3)
	textobj.TextName = "???????"
	textobj._colour = Vector4:new(1,1,1,1)

	textobj = ECS:getComponent_TextCom(fontObjText_4)
	textobj.TextName = "???????"
	textobj._colour = Vector4:new(1,1,1,1)

	textobj = ECS:getComponent_TextCom(fontObjText_5)
	textobj.TextName = "???????"
	textobj._colour = Vector4:new(1,1,1,1)

	-- Set Obj board to notify player of new objectives
	toNotify = true
end

function Update(dt)
	textManagerID = ScriptSystem:getLuaEntityID()
	initInUpdate()

	if not textManager["textMan_Env"].allObjectivesCleared then 
		checkScenariosActivated()
		whileActivated()
		onScenarioCleared()
		moveObjBoard(dt)
		checkAllCleared()
		setAllClear()
		handleShakeTimer(dt)
		notify(dt)
	else
		removeObjs()
		spriteCom._drawSprite = false
	end
end

function initInUpdate()
	if not doOnce then
		textManager["textMan_Env"] = ECS:getComponent_LuaScriptCom(textManagerID):GetEnvironment()

		textManager["textMan_Env"]["polUnifyCount"]		= 0
		textManager["textMan_Env"]["armyUnifyCount"]	= 0
		textManager["textMan_Env"]["shieldUnifyCount"]	= 0
		textManager["textMan_Env"]["clownUnifyCount"]	= 0
		textManager["textMan_Env"]["civiUnifyCount"]	= 0
		textManager["textMan_Env"]["buildUnifyCount"]	= 0
		textManager["textMan_Env"]["treeUnifyCount"]	= 0

		textManager["textMan_Env"]["polObjectiveCleared"]			= false
		textManager["textMan_Env"]["armyObjectiveCleared"]			= false
		textManager["textMan_Env"]["shieldObjectiveCleared"]		= false
		textManager["textMan_Env"]["clownObjectiveCleared"]			= false
		textManager["textMan_Env"]["civiObjectiveCleared"]			= false
		textManager["textMan_Env"]["buildObjectiveCleared"]			= false
		textManager["textMan_Env"]["treeObjectiveCleared"]			= false
		textManager["textMan_Env"]["rktTutorialObjectiveCleared"]	= false
		textManager["textMan_Env"]["laserTutorialObjectiveCleared"] = false
		textManager["textMan_Env"]["allObjectivesCleared"]			= false

		doOnce = true
	end
end

function checkScenariosActivated()
	--Console:PrintFloat(civiSceneID)
	if UISystem:getGameState() == 1 then
		if not buildActivated and not textManager["textMan_Env"].buildObjectiveCleared then
			if ECS:getComponent_LuaScriptCom(buildSceneID):GetEnvironment().buildSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_1) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_1)
					textObj.TextName = "Unify Buildings"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_1) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_1) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
					textcom._colour = Vector4:new(1,1,1,1)
				end
				
				buildActivated = true
				toNotify = true
			end
		end

		if not civiActivated and not textManager["textMan_Env"].civiObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(civiSceneID):GetEnvironment().civiSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_2) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_2)
					textObj.TextName = "Unify Civilians"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_2) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_2) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				civiActivated = true
				toNotify = true
			end
		end

		if not treeActivated and not textManager["textMan_Env"].treeObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(treeSceneID):GetEnvironment().treeSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_3) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_3)
					textObj.TextName = "Unify Trees"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_3) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_3) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				treeActivated = true
				toNotify = true
			end
		end

		if not polActivated and not textManager["textMan_Env"].polObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(polSceneID):GetEnvironment().polSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_4) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_4)
					textObj.TextName = "Unify Policemen"
				end

				if ECS:hasComponent_TextCom(fontCounterID_4) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_4) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				polActivated = true
				toNotify = true
			end
		end

		if not armyActivated and not textManager["textMan_Env"].armyObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(armySceneID):GetEnvironment().armySceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_5) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_5)
					textObj.TextName = "Unify Soldiers"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_5) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_5) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				armyActivated = true
				toNotify = true
			end
		end
	elseif UISystem:getGameState() == 2 then
		if not buildActivated and not textManager["textMan_Env"].buildObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(buildSceneID):GetEnvironment().buildSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_1) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_1)
					textObj.TextName = "Unify Buildings"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_1) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_1) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
					textcom._colour = Vector4:new(1,1,1,1)
				end
				
				buildActivated = true
				toNotify = true
			end
		end

		if not civiActivated and not textManager["textMan_Env"].civiObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(civiSceneID):GetEnvironment().civiSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_2) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_2)
					textObj.TextName = "Unify Civilians"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_2) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_2) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				civiActivated = true
				toNotify = true
			end
		end

		if not polActivated and not textManager["textMan_Env"].polObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(polSceneID):GetEnvironment().polSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_3) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_3)
					textObj.TextName = "Unify Policemen"
				end

				if ECS:hasComponent_TextCom(fontCounterID_3) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_3) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				polActivated = true
				toNotify = true
			end
		end

		if not laserTutorialActivated and not textManager["textMan_Env"].laserTutorialObjectiveCleared then
			if ECS:getComponent_LuaScriptCom(laserTutorialID):GetEnvironment().laserSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_4) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_4)
					textObj.TextName = "Save Civilians"
				end

				if ECS:hasComponent_TextCom(fontCounterID_4) then
                    local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
                    textcounter.TextName = ""
				end

                if ECS:hasComponent_TextCom(fontKillCountID_4) then
                    local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
                    textcom.TextName = ""
                end

				laserTutorialActivated = true
				toNotify = true
			end
		end

		if not shieldActivated and not textManager["textMan_Env"].shieldObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(shieldSceneID):GetEnvironment().shieldSceneActivated then 
				if ECS:hasComponent_TextCom(fontObjText_5) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_5)
					textObj.TextName = "Unify Shieldmen"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_5) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_5) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				shieldActivated = true
				toNotify = true
			end
		end
	elseif UISystem:getGameState() == 3 then
		if not buildActivated and not textManager["textMan_Env"].buildObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(buildSceneID):GetEnvironment().buildSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_1) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_1)
					textObj.TextName = "Unify Buildings"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_1) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_1) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
					textcom._colour = Vector4:new(1,1,1,1)
				end
				
				buildActivated = true
				toNotify = true
			end
		end

		if not civiActivated and not textManager["textMan_Env"].civiObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(civiSceneID):GetEnvironment().civiSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_2) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_2)
					textObj.TextName = "Unify Civilians"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_2) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_2) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				civiActivated = true
				toNotify = true
			end
		end

		if not polActivated and not textManager["textMan_Env"].polObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(polSceneID):GetEnvironment().polSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_3) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_3)
					textObj.TextName = "Unify Policemen"
				end

				if ECS:hasComponent_TextCom(fontCounterID_3) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_3) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				polActivated = true
				toNotify = true
			end
		end

		if not rktTutorialActivated and not textManager["textMan_Env"].rktTutorialObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(rktTutorialID):GetEnvironment().rktSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_4) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_4)
					textObj.TextName = "Save Civilians"
				end

				rktTutorialActivated = true
				toNotify = true
			end
		end

		if not clownActivated and not textManager["textMan_Env"].clownObjectiveCleared then 
			if ECS:getComponent_LuaScriptCom(clownSceneID):GetEnvironment().clownSceneActivated then
				if ECS:hasComponent_TextCom(fontObjText_5) then
					AudioSystem:PlaySFX("SFX_ObjectiveComplete", pEntID)
					local textObj = ECS:getComponent_TextCom(fontObjText_5)
					textObj.TextName = "Unify Clowns"
				end
		
				if ECS:hasComponent_TextCom(fontCounterID_5) then
					local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
					textcounter._colour = Vector4:new(1,1,1,1)
				end
				
				if ECS:hasComponent_TextCom(fontKillCountID_5) then
					local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
					textcom._colour = Vector4:new(1,1,1,1)
				end	

				clownActivated = true
				toNotify = true
			end
		end
	end
end

function whileActivated()
	if UISystem:getGameState() == 1 then
		if buildActivated and not textManager["textMan_Env"].buildObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
			textcounter.TextName = tostring(textManager["textMan_Env"]["buildUnifyCount"])

			if textManager["textMan_Env"]["buildUnifyCount"] >= buildObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["buildObjectiveCleared"] = true
			end
		end

		if civiActivated and not textManager["textMan_Env"].civiObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
			textcounter.TextName = tostring(textManager["textMan_Env"]["civiUnifyCount"])
		
			if textManager["textMan_Env"]["civiUnifyCount"] >= civiObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["civiObjectiveCleared"] = true
			end
		end

		if treeActivated and not textManager["textMan_Env"].treeObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
			textcounter.TextName = tostring(textManager["textMan_Env"]["treeUnifyCount"])
		
			if textManager["textMan_Env"]["treeUnifyCount"] >= treeObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["treeObjectiveCleared"] = true
			end
		end

		if polActivated and not textManager["textMan_Env"].polObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
			textcounter.TextName = tostring(textManager["textMan_Env"]["polUnifyCount"])
		
			if textManager["textMan_Env"]["polUnifyCount"] >= polObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["polObjectiveCleared"] = true
			end
		end

		if armyActivated and not textManager["textMan_Env"].armyObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
			textcounter.TextName = tostring(textManager["textMan_Env"]["armyUnifyCount"])
		
			if textManager["textMan_Env"]["armyUnifyCount"] >= armyObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["armyObjectiveCleared"] = true
			end
		end
	elseif UISystem:getGameState() == 2 then
		if buildActivated and not textManager["textMan_Env"].buildObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
			textcounter.TextName = tostring(textManager["textMan_Env"]["buildUnifyCount"])
		
			if textManager["textMan_Env"]["buildUnifyCount"] >= buildObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["buildObjectiveCleared"] = true
			end
		end
		
		if civiActivated and not textManager["textMan_Env"].civiObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
			textcounter.TextName = tostring(textManager["textMan_Env"]["civiUnifyCount"])
		
			if textManager["textMan_Env"]["civiUnifyCount"] >= civiObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["civiObjectiveCleared"] = true
			end
		end
		
		if polActivated and not textManager["textMan_Env"].polObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
			textcounter.TextName = tostring(textManager["textMan_Env"]["polUnifyCount"])
		
			if textManager["textMan_Env"]["polUnifyCount"] >= polObjCounter then
				 textManager["textMan_Env"]["polObjectiveCleared"] = true
			end
		end
		
		if shieldActivated and not textManager["textMan_Env"].shieldObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
			textcounter.TextName = tostring(textManager["textMan_Env"]["shieldUnifyCount"])
		
			if textManager["textMan_Env"]["shieldUnifyCount"] >= shieldObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["shieldObjectiveCleared"] = true
			end
		end
	elseif UISystem:getGameState() == 3 then
		if buildActivated and not textManager["textMan_Env"].buildObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
			textcounter.TextName = tostring(textManager["textMan_Env"]["buildUnifyCount"])
		
			if textManager["textMan_Env"]["buildUnifyCount"] >= buildObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["buildObjectiveCleared"] = true
			end
		end
		
		if civiActivated and not textManager["textMan_Env"].civiObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
			textcounter.TextName = tostring(textManager["textMan_Env"]["civiUnifyCount"])
		
			if textManager["textMan_Env"]["civiUnifyCount"] >= civiObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["civiObjectiveCleared"] = true
			end
		end
		
		if polActivated and not textManager["textMan_Env"].polObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
			textcounter.TextName = tostring(textManager["textMan_Env"]["polUnifyCount"])
		
			if textManager["textMan_Env"]["polUnifyCount"] >= polObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["polObjectiveCleared"] = true
			end
		end
		
		if clownActivated and not textManager["textMan_Env"].clownObjectiveCleared then
			local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
			textcounter.TextName = tostring(textManager["textMan_Env"]["clownUnifyCount"])
		
			if textManager["textMan_Env"]["clownUnifyCount"] >= clownObjCounter then
				 AudioSystem:PlaySFX("SFX_ObjectiveComplete", eEntID)
				 textManager["textMan_Env"]["clownObjectiveCleared"] = true
			end
		end
	end
end

function onScenarioCleared()
	if textManager["textMan_Env"].buildObjectiveCleared and not buildCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_1)
		textObj.TextName = "Unify Buildings"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
		textcounter.TextName = "Cleared"
		textcounter._colour = Vector4:new(1,1,1,1)
		--textcounter._size = 25.0
		--textcounter._pos.y = textcounter._pos.y - 1.0
		local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
		textcom.TextName = ""

		buildCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].civiObjectiveCleared and not civiCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_2)
		textObj.TextName = "Unify Civilians"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
		textcounter.TextName = "Cleared"
		textcounter._colour = Vector4:new(1,1,1,1)
		--textcounter._size = 25.0
		local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
		textcom.TextName = ""

		civiCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].treeObjectiveCleared and not treeCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_3)
		textObj.TextName = "Unify Trees"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
		textcounter.TextName = "Cleared"
		textcounter._colour = Vector4:new(1,1,1,1)
		--textcounter._size = 25.0
		local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
		textcom.TextName = ""

		treeCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].laserTutorialObjectiveCleared and not laserTutorialCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_4)
		textObj.TextName = "Save Civilians"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
		textcounter.TextName = "Cleared"
		--textcounter._size = 25.0
		textcounter._colour = Vector4:new(1,1,1,1)

		laserTutorialCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].rktTutorialObjectiveCleared and not rktTutorialCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_4)
		textObj.TextName = "Save Civilians"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
		textcounter.TextName = "Cleared"
		--textcounter._size = 25.0
		textcounter._colour = Vector4:new(1,1,1,1)

		rktTutorialCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].polObjectiveCleared and not polCleard then
		if UISystem:getGameState() == 1 then
			local textObj = ECS:getComponent_TextCom(fontObjText_4)
			textObj.TextName = "Unify Policemen"

			local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
			textcounter.TextName = "Cleared"
			textcounter._colour = Vector4:new(1,1,1,1)
			--textcounter._size = 25.0

			local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
			textcom.TextName = ""

			polCleard = true
			toNotify = true
		elseif UISystem:getGameState() == 2 or UISystem:getGameState() == 3 then
			local textObj = ECS:getComponent_TextCom(fontObjText_3)
			textObj.TextName = "Unify Policemen"

			local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
			textcounter.TextName = "Cleared"
			textcounter._colour = Vector4:new(1,1,1,1)
			--textcounter._size = 25.0

			local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
			textcom.TextName = ""

			polCleard = true
			toNotify = true
		end
	end

	if textManager["textMan_Env"].armyObjectiveCleared and not armyCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_5)
		textObj.TextName = "Unify Soldiers"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
		textcounter.TextName = "Cleared"
		textcounter._colour = Vector4:new(1,1,1,1)
		--textcounter._size = 25.0

		local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
		textcom.TextName = ""

		armyCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].shieldObjectiveCleared and not shieldCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_5)
		textObj.TextName = "Unify Shieldmen"

		local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
		textcounter.TextName = "Cleared"
		textcounter._colour = Vector4:new(1,1,1,1)

		local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
		textcom.TextName = ""

		shieldCleared = true
		toNotify = true
	end

	if textManager["textMan_Env"].clownObjectiveCleared and not clownCleared then
		local textObj = ECS:getComponent_TextCom(fontObjText_5)
		textObj.TextName = "Unify Clown"
		
		local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
		textcounter.TextName = "Cleared"
		textcounter._colour = Vector4:new(1,1,1,1)

		local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
		textcom.TextName = ""

		clownCleared = true
		toNotify = true
	end
end

function moveObjBoard(dt)
	eEntID = ScriptSystem:getLuaEntityID()
    eTransform = ECS:getComponent_Transform(eEntID)

	-- move right
	if UISystem:getGameState() ~= 0 then
		if Inputs.get_Key(InputKey.TAB_KEY) or Inputs.get_Controller(InputControllerKey.Y_BUTTON) then
			toNotify = false	

			if resetNotify then
				-- Reset board to original position
				objBoardTransform.pos.x = objBoardStartPos.x
				objBoardTransform.pos.y = objBoardStartPos.y
				objBoardTransform.pos.z = objBoardStartPos.z

				-- Reset board to original colour
				spriteCom._colour.x = 1.0
				spriteCom._colour.y = 1.0
				spriteCom._colour.z = 1.0

				resetNotify = false
			end

			eTransform.pos.x = eTransform.pos.x + 1 * dt * travelSpeed

			local textobj = ECS:getComponent_TextCom(fontObjText_1)
			textobj._pos.x = textobj._pos.x + 0.01 * dt * travelSpeed
	
			textobj = ECS:getComponent_TextCom(fontObjText_2)
			textobj._pos.x = textobj._pos.x + 0.01 * dt * travelSpeed
		
			textobj = ECS:getComponent_TextCom(fontObjText_3)
			textobj._pos.x = textobj._pos.x + 0.01 * dt * travelSpeed
		
			textobj = ECS:getComponent_TextCom(fontObjText_4)
			textobj._pos.x = textobj._pos.x + 0.01 * dt * travelSpeed
		
			textobj = ECS:getComponent_TextCom(fontObjText_5)
			textobj._pos.x = textobj._pos.x + 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
			textcom._pos.x = textcom._pos.x + 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
			textcom._pos.x = textcom._pos.x + 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
			textcom._pos.x = textcom._pos.x + 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
			textcom._pos.x = textcom._pos.x + 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
			textcom._pos.x = textcom._pos.x + 0.01 * dt * travelSpeed

			local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
			textcounter._pos.x = textcounter._pos.x + 0.01 * dt * travelSpeed
	
			textcounter = ECS:getComponent_TextCom(fontCounterID_2)
			textcounter._pos.x = textcounter._pos.x + 0.01 * dt * travelSpeed

			textcounter = ECS:getComponent_TextCom(fontCounterID_3)
			textcounter._pos.x = textcounter._pos.x + 0.01 * dt * travelSpeed

			textcounter = ECS:getComponent_TextCom(fontCounterID_4)
			textcounter._pos.x = textcounter._pos.x + 0.01 * dt * travelSpeed

			textcounter = ECS:getComponent_TextCom(fontCounterID_5)
			textcounter._pos.x = textcounter._pos.x + 0.01 * dt * travelSpeed

		-- move left
		elseif (not Inputs.get_Key(InputKey.TAB_KEY) or not Inputs.get_Controller(InputControllerKey.Y_BUTTON)) and not toNotify then
			eTransform.pos.x = eTransform.pos.x - 1 * dt * travelSpeed

			local textobj = ECS:getComponent_TextCom(fontObjText_1)
			textobj._pos.x = textobj._pos.x - 0.01 * dt * travelSpeed

			textobj = ECS:getComponent_TextCom(fontObjText_2)
			textobj._pos.x = textobj._pos.x - 0.01 * dt * travelSpeed
		
			textobj = ECS:getComponent_TextCom(fontObjText_3)
			textobj._pos.x = textobj._pos.x - 0.01 * dt * travelSpeed
		
			textobj = ECS:getComponent_TextCom(fontObjText_4)
			textobj._pos.x = textobj._pos.x - 0.01 * dt * travelSpeed
		
			textobj = ECS:getComponent_TextCom(fontObjText_5)
			textobj._pos.x = textobj._pos.x - 0.01 * dt * travelSpeed

			local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
			textcounter._pos.x = textcounter._pos.x - 0.01 * dt * travelSpeed
	
			textcounter = ECS:getComponent_TextCom(fontCounterID_2)
			textcounter._pos.x = textcounter._pos.x - 0.01 * dt * travelSpeed

			textcounter = ECS:getComponent_TextCom(fontCounterID_3)
			textcounter._pos.x = textcounter._pos.x - 0.01 * dt * travelSpeed

			textcounter = ECS:getComponent_TextCom(fontCounterID_4)
			textcounter._pos.x = textcounter._pos.x - 0.01 * dt * travelSpeed

			textcounter = ECS:getComponent_TextCom(fontCounterID_5)
			textcounter._pos.x = textcounter._pos.x - 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
			textcom._pos.x = textcom._pos.x - 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
			textcom._pos.x = textcom._pos.x - 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
			textcom._pos.x = textcom._pos.x - 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
			textcom._pos.x = textcom._pos.x - 0.01 * dt * travelSpeed

			local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
			textcom._pos.x = textcom._pos.x - 0.01 * dt * travelSpeed
		end	
    end

    if eTransform.pos.x >= destinationX then
        eTransform.pos.x = destinationX
    end
	
    if eTransform.pos.x <= startingX then
        eTransform.pos.x = startingX
    end

	-- 1
	local textobj = ECS:getComponent_TextCom(fontObjText_1)
	textobj._pos.y = y1
    if textobj._pos.x <= textStartX1 then
        textobj._pos.x = textStartX1
    end

	if textobj._pos.x >= textFinalX1 then
        textobj._pos.x = textFinalX1
    end

	local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
	if textcounter.TextName == "Cleared" then
		textcounter._size = 25.0
		textcounter._pos.y = y6
	else
		textcounter._pos.y = y1
	end

	if textcounter._pos.x <= textStartX2 then
        textcounter._pos.x = textStartX2
    end

	if textcounter._pos.x >= textFinalX2 then
        textcounter._pos.x = textFinalX2
    end

	local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
	textcom._pos.y = y1
	if textcom._pos.x <= textStartX3 then
        textcom._pos.x = textStartX3
    end							   
								   
	if textcom._pos.x >= textFinalX3 then
        textcom._pos.x = textFinalX3
    end

	-- 2
	local textobj = ECS:getComponent_TextCom(fontObjText_2)
	textobj._pos.y = y2
    if textobj._pos.x <= textStartX1 then
        textobj._pos.x = textStartX1
    end

	if textobj._pos.x >= textFinalX1 then
        textobj._pos.x = textFinalX1
    end

	local textcounter = ECS:getComponent_TextCom(fontCounterID_2)
	if textcounter.TextName == "Cleared" then
		textcounter._size = 25.0
		textcounter._pos.y = y7
	else
		textcounter._pos.y = y2
	end
	if textcounter._pos.x <= textStartX2 then
        textcounter._pos.x = textStartX2
    end

	if textcounter._pos.x >= textFinalX2 then
        textcounter._pos.x = textFinalX2
    end

	local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
	textcom._pos.y = y2
	if textcom._pos.x <= textStartX3 then
        textcom._pos.x = textStartX3
    end							   
								   
	if textcom._pos.x >= textFinalX3 then
        textcom._pos.x = textFinalX3
    end

	-- 3
	local textobj = ECS:getComponent_TextCom(fontObjText_3)
	textobj._pos.y = y3
    if textobj._pos.x <= textStartX1 then
        textobj._pos.x = textStartX1
    end

	if textobj._pos.x >= textFinalX1 then
        textobj._pos.x = textFinalX1
    end

	local textcounter = ECS:getComponent_TextCom(fontCounterID_3)
	if textcounter.TextName == "Cleared" then
		textcounter._size = 25.0
		textcounter._pos.y = y8
	else
		textcounter._pos.y = y3
	end
	if textcounter._pos.x <= textStartX2 then
        textcounter._pos.x = textStartX2
    end

	if textcounter._pos.x >= textFinalX2 then
        textcounter._pos.x = textFinalX2
    end

	local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
	textcom._pos.y = y3
	if textcom._pos.x <= textStartX3 then
        textcom._pos.x = textStartX3
    end							   
								   
	if textcom._pos.x >= textFinalX3 then
        textcom._pos.x = textFinalX3
    end

	-- 4
	local textobj = ECS:getComponent_TextCom(fontObjText_4)
	textobj._pos.y = y4
    if textobj._pos.x <= textStartX1 then
        textobj._pos.x = textStartX1
    end

	if textobj._pos.x >= textFinalX1 then
        textobj._pos.x = textFinalX1
    end

	local textcounter = ECS:getComponent_TextCom(fontCounterID_4)
	if textcounter.TextName == "Cleared" then
		textcounter._size = 25.0
		textcounter._pos.y = y9
	else
		textcounter._pos.y = y4
	end
	if textcounter._pos.x <= textStartX2 then
        textcounter._pos.x = textStartX2
    end

	if textcounter._pos.x >= textFinalX2 then
        textcounter._pos.x = textFinalX2
    end

	local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
	textcom._pos.y = y4
	if textcom._pos.x <= textStartX3 then
        textcom._pos.x = textStartX3
    end							   
								   
	if textcom._pos.x >= textFinalX3 then
        textcom._pos.x = textFinalX3
    end

	-- 5
	local textobj = ECS:getComponent_TextCom(fontObjText_5)
	textobj._pos.y = y5
    if textobj._pos.x <= textStartX1 then
        textobj._pos.x = textStartX1
    end

	if textobj._pos.x >= textFinalX1 then
        textobj._pos.x = textFinalX1
    end

	local textcounter = ECS:getComponent_TextCom(fontCounterID_5)
	if textcounter.TextName == "Cleared" then
		textcounter._size = 25.0
		textcounter._pos.y = y10
	else
		textcounter._pos.y = y5
	end
	if textcounter._pos.x <= textStartX2 then
        textcounter._pos.x = textStartX2
    end

	if textcounter._pos.x >= textFinalX2 then
        textcounter._pos.x = textFinalX2
    end

	local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
	textcom._pos.y = y5
	if textcom._pos.x <= textStartX3 then
        textcom._pos.x = textStartX3
    end							   
								   
	if textcom._pos.x >= textFinalX3 then
        textcom._pos.x = textFinalX3
    end

end

function setAllClear()
	if Inputs.get_Key(InputKey.P_KEY) then
		if UISystem:getGameState() == 1 then
			textManager["textMan_Env"].buildObjectiveCleared = true
			textManager["textMan_Env"].civiObjectiveCleared = true
			textManager["textMan_Env"].treeObjectiveCleared = true
			textManager["textMan_Env"].polObjectiveCleared = true
			textManager["textMan_Env"].armyObjectiveCleared = true
		elseif UISystem:getGameState() == 2 then
			textManager["textMan_Env"].buildObjectiveCleared = true
			textManager["textMan_Env"].civiObjectiveCleared = true
			textManager["textMan_Env"].polObjectiveCleared = true
			textManager["textMan_Env"].laserTutorialObjectiveCleared = true
			textManager["textMan_Env"].shieldObjectiveCleared = true
		elseif UISystem:getGameState() == 3 then
			textManager["textMan_Env"].buildObjectiveCleared = true
			textManager["textMan_Env"].civiObjectiveCleared = true
			textManager["textMan_Env"].polObjectiveCleared = true
			textManager["textMan_Env"].rktTutorialObjectiveCleared = true
			textManager["textMan_Env"].clownObjectiveCleared = true
		end
	end
end

function checkAllCleared()
	if UISystem:getGameState() == 1 then
		if textManager["textMan_Env"].buildObjectiveCleared and textManager["textMan_Env"].civiObjectiveCleared
			and textManager["textMan_Env"].treeObjectiveCleared and textManager["textMan_Env"].polObjectiveCleared
			and textManager["textMan_Env"].armyObjectiveCleared then
				textManager["textMan_Env"].allObjectivesCleared = true
		end
	elseif UISystem:getGameState() == 2 then
		if textManager["textMan_Env"].buildObjectiveCleared and textManager["textMan_Env"].civiObjectiveCleared
			and textManager["textMan_Env"].laserTutorialObjectiveCleared and textManager["textMan_Env"].polObjectiveCleared
			and textManager["textMan_Env"].shieldObjectiveCleared then
				textManager["textMan_Env"].allObjectivesCleared = true
		end
	elseif UISystem:getGameState() == 3 then
		if textManager["textMan_Env"].buildObjectiveCleared and textManager["textMan_Env"].civiObjectiveCleared
			and textManager["textMan_Env"].rktTutorialObjectiveCleared and textManager["textMan_Env"].polObjectiveCleared
			and textManager["textMan_Env"].clownObjectiveCleared then
				textManager["textMan_Env"].allObjectivesCleared = true
		end
	end
end

function notify(dt)
	if toNotify then
		if shakeTimer <= 0.0 then
			shakeTimer = shakeTime
		end
		resetNotify = true
		objBoardTransform.pos.x = objBoardStartPos.x + Random.randInRange(-1.0, 1.0) * shakeTimer
        objBoardTransform.pos.y = objBoardStartPos.z + Random.randInRange(-1.0, 1.0) * shakeTimer

		spriteCom._colour.x = 3.0
		spriteCom._colour.y = 3.0
		spriteCom._colour.z = 1.0
	end
end

function handleShakeTimer(dt)
    if shakeTimer > 0.0 then
        shakeTimer = shakeTimer - dt
    end
end

function removeObjs()
	local textobj = ECS:getComponent_TextCom(fontObjText_1)
	textobj._pos.x = -1000

	textobj = ECS:getComponent_TextCom(fontObjText_2)
	textobj._pos.x = -1000
		
	textobj = ECS:getComponent_TextCom(fontObjText_3)
	textobj._pos.x = -1000
		
	textobj = ECS:getComponent_TextCom(fontObjText_4)
	textobj._pos.x = -1000
		
	textobj = ECS:getComponent_TextCom(fontObjText_5)
	textobj._pos.x = -1000

	local textcounter = ECS:getComponent_TextCom(fontCounterID_1)
	textcounter._pos.x = -1000
	
	textcounter = ECS:getComponent_TextCom(fontCounterID_2)
	textcounter._pos.x = -1000

	textcounter = ECS:getComponent_TextCom(fontCounterID_3)
	textcounter._pos.x = -1000

	textcounter = ECS:getComponent_TextCom(fontCounterID_4)
	textcounter._pos.x = -1000

	textcounter = ECS:getComponent_TextCom(fontCounterID_5)
	textcounter._pos.x = -1000

	local textcom = ECS:getComponent_TextCom(fontKillCountID_1)
	textcom._pos.x = -1000

	local textcom = ECS:getComponent_TextCom(fontKillCountID_2)
	textcom._pos.x = -1000

	local textcom = ECS:getComponent_TextCom(fontKillCountID_3)
	textcom._pos.x = -1000

	local textcom = ECS:getComponent_TextCom(fontKillCountID_4)
	textcom._pos.x = -1000

	local textcom = ECS:getComponent_TextCom(fontKillCountID_5)
	textcom._pos.x = -1000
end