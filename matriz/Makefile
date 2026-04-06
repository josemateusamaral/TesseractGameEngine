
.PHONY: clean all 

all: matriz.o
	# Compile and link the source and object files into a final executable binary
	$(CXX) main.cpp matriz.o
	
matriz.o: matriz.h matriz.cpp 
	$(CXX) $(CFLAGS) -c matriz.cpp

clean:
	rm -f *.o 
	rm -f a.out
	rm -fR doc

doc:
	mkdir -p doc 
	doxygen

# .PHONY: clean doxygen

# CXX=g++
# RM=rm

# te: bin/janela.o bin/ponto.o bin/ponto3.o bin/forma.o bin/cubo.o bin/esfera.o te.o bin/Vec3.o bin/cone.o matriz.o
# 	$(CXX) matriz.o te.o bin/janela.o bin/ponto.o bin/ponto3.o bin/forma.o bin/cubo.o bin/esfera.o bin/Vec3.o bin/cone.o -o te -lSDL2

# # receita para gerar o binário final teste_PROJECAO
# teste: bin/janela.o bin/ponto.o bin/ponto3.o bin/forma.o bin/cubo.o bin/esfera.o bin/teste.o bin/Vec3.o bin/cone.o
# 	$(CXX) bin/teste.o bin/janela.o bin/ponto.o bin/ponto3.o bin/forma.o bin/cubo.o bin/esfera.o bin/Vec3.o bin/cone.o -o teste -lSDL2

# # receita para gerar o (arquivo objeto) teste_PROJECAO.o
# bin/teste.o: teste.cpp
# 	$(CXX) -o bin/teste.o -c teste.cpp -lSDL2

# te.o: te.cpp
# 	$(CXX) -o te.o -c te.cpp -lSDL2

# matriz.o: matriz.cpp
# 	$(CXX) -o matriz.o -c matriz.cpp -lSDL2


# bin/cone.o: formas/cone/cone.cpp formas/cone/cone.h
# 	$(CXX) -o bin/cone.o -c formas/cone/cone.cpp -lSDL2

# bin/esfera.o: formas/esfera/esfera.cpp formas/esfera/esfera.h
# 	$(CXX) -o bin/esfera.o -c formas/esfera/esfera.cpp -lSDL2

# bin/cubo.o: formas/cubo/cubo.cpp formas/cubo/cubo.h
# 	$(CXX) -o bin/cubo.o -c formas/cubo/cubo.cpp -lSDL2




# # receita para gerar o (arquivo objeto) janela.o
# bin/janela.o: utils/janela.cpp utils/janela.h
# 	$(CXX) -o bin/janela.o -c utils/janela.cpp -lSDL2

# # receita para gerar o (arquivo objeto) ponto.o
# bin/ponto.o: pontos/ponto.cpp pontos/ponto.h
# 	$(CXX) -o bin/ponto.o -c pontos/ponto.cpp -lSDL2

# # receita para gerar o (arquivo objeto) ponto3.o
# bin/ponto3.o: pontos/ponto3.cpp pontos/ponto3.h
# 	$(CXX) -o bin/ponto3.o -c pontos/ponto3.cpp -lSDL2

# bin/forma.o: formas/forma.cpp formas/forma.h
# 	$(CXX) -o bin/forma.o -c formas/forma.cpp -lSDL2

# bin/Vec3.o: utils/Vec3.cpp utils/Vec3.h
# 	$(CXX) -o bin/Vec3.o -c utils/Vec3.cpp -lSDL2





# # receita para apagar arquivos (geralmente são arquivos gerados
# # no processo de desenvolvimento)
# clean:
# 	$(RM) -f bin/ponto.o
# 	$(RM) -f bin/ponto3.o
# 	$(RM) -f bin/janela.o
# 	$(RM) -f teste
# 	$(RM) -f bin/teste.o
# 	$(RM) -f bin/forma.o
# 	$(RM) -f bin/cubo.o
# 	$(RM) -f bin/esfera.o
# 	$(RM) -f bin/Vec3.o
# 	$(RM) -f bin/cone.o


# # receita para gerar a documentação, o arquivo Doxyfile está
# # configurado para gerar um diretório chamado docs
# doc:
# 	doxygen

# # receita para apagar o conteúdo do diretório de documentação
# cleandoc:
# 	$(RM) -fR docs

# run:
# 	./teste_PROJECAO
