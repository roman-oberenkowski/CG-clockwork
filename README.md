# CG-clockwork 
# Mechanical clock. At least 2 gearwheels, pendulum, 2 hands. Mechanical elements (ex. gearwheels) whould be synchronized.
Project requirements (text by W. Andrzejewski):
Project subjects and grading rules
# Overall rules
Projects should be prepared in two person groups. Solo projects and 3 person groups are also allowed, though 3 person groups will be graded more harshly.
Subjects will be presented during classes. Each group’s representative will divide the subjects amongst all of the people within his group. I expect to receive the list of teams and subjects on 30 april 2020 or earlier.
Two teams from the same group cannot prepare projects with the same subject.
I will announce dates during winter session in which you will be able to have the projects graded. Each project will be graded based on its technical quality and the students ability to answer several questions.
Trying to return completed projects after deadline ends tragically.
Keep in mind that I am not always available during session. I might not be able to grade your project if you do not come at the specified date.
Any plagiarized projects get the worst possible score with no hope of redemption. If a student cannot answer several basic questions about his project, then I assume he did not do it himself and the project is plagiarized.
You can use any programming environment you like, as long as you will not ask me to run in on my own computer.
When using some exotic programming environment you are on your own. If you have any troubles with it… good for you. Its your own fault.
# Non-exotic environments are:
Linux+gcc in any post stone age version.
Visual Studio .NET 2019 or earlier.
Codeblocks.
# General requirements
Use OpenGL and GLM unless you asked if you could use something else and I agreed.
Virtual world is composed of several (greater than 4 and less than infinity) independent 3d objects. If the projects subject is in conflict with this requirement, ask me what to do about it.
Program should not be static. There should be some animation either interactive or not.
All of the objects should be textured. Use at least two textures.
Use at least two light sources. Lighting of the scene should be visible.
Non trivial 3D models. The scene should not be only composed of simple testing models available in programming libraries.
What is allowed
Use any external library as long as it does not have anything to do with computer graphics.
Use 3D models downloaded from internet.
Create your own 3D models in some external software.
Use any existing external libraries that allow to read 3D models. However, if those libraries have some functionality for drawing of those models, then you cant use this functionality.
# What is not allowed
Use any ready drawing procedures. Every line of code that is used for drawing must be your own and you must know it and understand it completely.
Use of pink color.
Plagiarism.
You cant use any of the deprecated functions mentioned below:
glBegin, glEnd – use glDrawArrays or glDrawElements instead
glVertex, glNormal, glTexCoord – use glDrawArrays or glDrawElements instead
glRotate, glTranslate, glScale – use rotate/translate/scale from GLM library instead
gluLookAt – use lookAt from GLM library instead
glFrustum, gluPerspective, glOrtho – use frustum/perspective/ortho from GLM library instead
glCreateList, glDeleteList, glCallList – use glDrawArrays or glDrawElements instead
glPushMatrix, glPopMatrix – use any data structure containing mat4 objects (GLM library) instead
glVertexPointer, glNormalPointer, glTexCoordPointer, glColorPointer – use glVertexAttribPointer instead
glEnableClientState, glDisableClientState – use glEnableVertexAttribArray/glDisableVertexAttribArray instead
# General grading rules
Not everything can be anticipated, so every case will be considered independently, but the general rules are as follows. You get 2.0 grade free of charge. It is increased if the following conditions are met:
Usage of non-trivial models (retrieved from files): +0.5 to the grade
Correct management of 3D objects in the scene (movement according to project requirements and appropriate usage of camera): +0.5 to the grade
Correct texturing and lighting of objects as well as correct usage of OpenGL mechanisms for drawing: +1.0 to the grade.
Ability to answer questions about the project: +1.0 to the grade. Warning! This does not mean that you can buy the project and get the grade 4.0. If I catch you on not knowing how your own project works, you get 2.0.
Rules for grading projects prepared by 3 person teams are the same, but I will not overlook any flaw in the project.

