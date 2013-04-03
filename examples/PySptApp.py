import os.path
import sys
import time
import __main__
from PySpt import *

class PySptApp:
	def __init__(self):
		self.clock = time.time()
		self.session = RenderSession()
		self.session.setResolution(vec2(1280, 720))
		self.session.setNbSamples(100)
		self.session.setNbThreads(20)
		self.session.setUseBVH(True)
		self.session.setBVHMaxDepth(20)
		
		self.scene = Scene()
		self.session.setScene(self.scene)
		
		self.renderer = Renderer()
		self.renderer.setRenderSession(self.session)
		
		# Build output filename
		outputDir = os.path.realpath(os.path.dirname(__main__.__file__))
		mainFilename, ext = os.path.splitext(os.path.basename(__main__.__file__))
		self.outputFilename = os.path.join(outputDir, mainFilename + ".png")
		
	def setupScene(self):
		pass
		
	def run(self):
		print 'Loading scene...'
		self.setupScene()
		self.scene.update()
		print 'Scene ready, Building BVH...'
		self.renderer.buildBVH()
		print 'BVH OK, start rendering...'
		
		try:
			while self.session.getRenderedSamples() < self.session.getNbSamples():
				self.renderer.render()
				Gui.saveImage(self.renderer, self.outputFilename)
				self.session.setTotalEllapsedTime(time.time() - self.clock)
				sys.stdout.write(' ' * 80 + '\r')
				sys.stdout.write(self.session.getStats() + '\r')
				sys.stdout.flush()
		except:
			print '\nRendering interupted, saving...'
			Gui.saveImage(self.renderer, 'output.png')
			
		print '\nRendering done'
		