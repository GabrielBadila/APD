# Modelarea unui automat celular (Game of Life) (Romanian language)



	Algoritmi paraleli si distribuiti
	
	Tema 1 - Modelarea unui automat celular (Game of Life)
	
	Badila Gabriel Alin
	333 CA
	
	
	============================================================================
	
	Arhiva temei contine doua fisiere sursa, unul pentru implementarea seriala 
	si altul pentru implementarea paralela.

	In cazul implementarii seriale, fisierul contine pe langa funtia "main", 
	alte trei functii: "copyMatrix" (copiaza o matrice in alta matrice), 
	"bordMatrix" (bordeaza o matrice astfel, prima linie si prima coloana fiind 
	precedate de ultima linie, respectiv ultima coloana, si ultima linie si 
	ultima coloana fiind urmate de prima linie, respectiv prima coloana, precum 
	si colturile atingand colturile opuse) si "evolve" (in care pe baza 
	conditiilor impuse de joc se realizeaza trecerea matricei de la o iteratie 
	la alta). In "main" se realizeaza citireea numarului de linii si coloane din 
	fisier pentru alocarea spatiului necesar in matrice si se salveaza valorile. 
	Apoi pentru fiecare iteratie se apeleaza functiile prezentate mai sus in 
	ordinea urmatoare: "bordMatrix", "evolve", "copyMatrix". In final se scriu 
	rezultatele obtinute in fisierul de iesire si se elibereaza memoria.

	Implementarea paralela(OpenMP) este realizata prin adaugarea unor structuri 
	de forma "#pragma omp parallel". Astfel am decis sa paralelizez functia 
	"copyMatrix" prin adaugarea structurii "#pragma omp parallel for private(i,j)"
	inainte de for-urile imbricate, unde este realizata copierea matricei. In 
	functia "bordMatrix" am paralelizat for-urile unde este realizata bordarea 
	liniilor si a coloanelor. La copierea colturilor am folosit structura 
	"#pragma omp master" pentru ca executia acelui bloc sa fie realizata doar de 
	thread-ul master. In functia "evolve" am paralelizat operatiile de modificare 
	a matricei utilizand pe langa structura de baza "#pragma omp parallel for" 
	si structura "collapse(2)" pentru a extinde paralelizarea asupra ambelor 
	for-uri.
	
	============================================================================
	
	In fisierul results.xlsx, atasat arhivei, se regasesc graficele pentru 
	rularea temei asupra unui fisier de intrare, generat random, ce contine o 
	matrice cu 5000 de linii si 5000 de coloane pentru doua cazuri: 100 si 1000 
	de iteratii.