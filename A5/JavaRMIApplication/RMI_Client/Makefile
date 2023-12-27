JC = javac
J = java

#In the build target, added the -d build flag to specify the output directory as build for the compiled .class files.
build1:
	$(JC) RMIServerInterface.java RMIClient.java -d build

run1:
	$(J) -cp build RMIClient

build2:
	$(JC) RMIServerInterface.java RMIClient1.java -d build

run2:
	$(J) -cp build RMIClient1

build: build1 build2

# A phony target is one that is not really the name of a file; 
# rather it is just a name for a recipe to be executed when you make an explicit request.
.PHONY: build

clean:
	@powershell Remove-Item -Path "build\*.class" -Force -ErrorAction SilentlyContinue

.PHONY: clean
