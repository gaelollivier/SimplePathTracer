#!/usr/bin/python
from PySpt import *
from PySptApp import *

class App(PySptApp):
	def setupScene(self):
	
		# Materials
		self.white = Material(vec3(1))
		
		# Objects
		self.sphere = Sphere(vec3(0, 0, 0), 2, self.white)
		self.plane = Plane(vec3(0, -2, 0), vec3(0, 1, 0), self.white)
		
		self.scene.addNode(self.sphere)
		self.scene.addNode(self.plane)
		
		# Light
		self.light = Light(vec3(0, 10, 5), vec3(1, 1, 1), 2)
		
		self.scene.addNode(self.light)
		
		# Camera
		self.camera = Camera(vec2(16, 9), vec3(0, 2, 10), vec3(0, 0, 0))
		
		self.scene.addNode(self.camera)

App().run()