#!/usr/bin/python
import os.path
from PySpt import *
from PySptApp import *

modelsDirectory = os.path.realpath(os.path.dirname(__file__) + '/../../models')

class App(PySptApp):
	def setupScene(self):
	
		# Rendering config
		self.session.setResolution(vec2(1600, 900))
		self.session.setNbSamples(1000)
	
		# Materials
		self.floorMaterial = Material(vec3(1))
		
		self.floorMaterial.setReflection(1)
		self.floorMaterial.setGlossiness(0.3)
		
		self.dragonMaterial = Material(vec3(1))
		
		self.dragonMaterial.setReflection(1)
		self.dragonMaterial.setSpecular(0)
		self.dragonMaterial.setDiffuse(0.01)
		
		# Load dragon model
		loader = AssimpLoader(self.dragonMaterial)
		self.dragon = loader.loadFile(modelsDirectory + '/stanford_dragon/dragon.obj')
		
		self.dragon.setScale(vec3(0.5))
		self.dragon.setRotation(15, vec3(0, 1, 0))
		
		self.scene.addNode(self.dragon)
		
		self.floor = Plane(vec3(0, 0, 0), vec3(0, 1, 0), self.floorMaterial)
		self.scene.addNode(self.floor)
		
		# Light
		self.light = Light(vec3(5, 10, 10), vec3(1, 1, 1), 2)
		self.light2 = Light(vec3(5, 10, -10), vec3(1, 1, 1), 2)
		
		self.scene.addNode(self.light)
		self.scene.addNode(self.light2)
		
		self.scene.update()
				
		# Camera
		self.camera = Camera(vec2(16, 9), vec3(0, 3, 8), vec3(-0.25, 2, 0))
		
		self.scene.addNode(self.camera)

App().run()