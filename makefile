Compiler = g++
Name32 = -oPazzers
Name64 = -oPazzers64
LibSDL = -lSDL -lSDLmain -lSDL_mixer -lSDL_ttf -lSDL_image 
LibMustBe = -lm -ldl -lpthread -lfreetype
WhereLib32 = -Llib
WhereLib64 = -Llib64
WhereInclude = -Iinclude 
File = Pazzers.cpp 

build: $(File)
	$(Compiler) $(WhereInclude) $(File) $(WhereLib32) $(LibSDL) $(LibMustBe) $(Name32)

build64: $(File)
	$(Compiler) $(WhereInclude) $(File) $(WhereLib64) $(LibSDL) $(LibMustBe) $(Name64)
