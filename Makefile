LIBS=-lGL -lglfw -lGLEW -lassimp
HEADERS= constants.h  lodepng.h shaderprogram.h shader.h stb_image.h mesh.h model.h
FILES= lodepng.cpp  main_file.cpp  shaderprogram.cpp  stb_image.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
