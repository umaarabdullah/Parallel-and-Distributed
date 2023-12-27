JC = javac
J = java

# In the build target, added the -d build flag to specify the output directory as build for the compiled .class files.
build:
	$(JC) RMIServerInterface.java RMIServerImpl.java RMIServer.java -d build	


run:
	$(J) -cp build RMIServer

# A phony target is one that is not really the name of a file; 
# rather it is just a name for a recipe to be executed when you make an explicit request.
.PHONY: build

clean:
	@powershell Remove-Item -Path "build\*.class" -Force -ErrorAction SilentlyContinue

.PHONY: clean
