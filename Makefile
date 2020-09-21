LIBS=-lGL -lglfw -lGLEW -lassimp
HEADERS= constants.h    shader.h stb_image.h mesh.h model.h
FILES=  main_file.cpp    stb_image.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
