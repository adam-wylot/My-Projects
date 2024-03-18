===============================================================================================================
		   				   KOMPILACJA:
Do kompilacji programu zalecane jest użycie gotowego pliku Makefile. Żeby tego dokonać
udajemy się w konsoli do lokalizacji tego miejsca i wpisujemy polecenie "make". Jeżeli
chce się wyczyścić poprzednie pliki można wpisać polecenie "make clean".

Po skompilowaniu programu ten jest gotowy do użytkowania.

===============================================================================================================
	      				     URUCHAMIANIE PROGRAMU:
1. Należy udać się do lokalizacji skompilowanego wcześniej programu.

2. Następnie wpisujemy polecenie:
		./mrowka <parametry>


	(Opis wymaganych oraz możliwych parametrów znajduje się poniżej)

===============================================================================================================
					      PARAMETRY WYWOŁANIA:
Wymagane:
     - liczba wierszy ( >= 5 )
     - liczba kolumn ( >= 5 )
     - liczba iteracji ( >= 0 )
     - przedrostek do plików z zapisem
     - początkowy kierunek mrówki <0 - 3>

	      [ 0 - w górę
		1 - w prawo
		2 - w dół
		3 - w lewo ]



Opcjonalne:
     -a			| * rysuje w konsoli wszystkie iteracje przez jakie przechodzi program
     -m <ścieżka>	| * w miejscu <ścieżka> można podać lokalizację siatki początkowej.
			|      (UWAGA: plik musi być w identycznym formacie co pliki zapisywane przez program)
     -p <0 - 100>	| * początkowe procentowe wypełnienie siatki czarnymi polami

===============================================================================================================