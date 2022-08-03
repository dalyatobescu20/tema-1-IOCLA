 // TOBESCU DALYA-ALEXANDRA //
 // TEMA 1 IOCLA //


## get_size function:
    **am creat o functie care sa calculeze pozitia unde se
      afla in memorie acel element

## add_last function:
   **daca vectorul este gol, alocam memorie, in caz contrat realocam memoria
   **copiem informatiile din data_structure

## add_at function:
    **am pus conditie in cazul in care index-ul este mai mare sau egal
     cu lungimea vectorului,in acest caz apelez add_last
    **am deplasat byte cu byte la dreapta incepand cu pozitia indexului 
      pana la sfarsitul vectorului
    **similar cu functia add_last am copiat informatiile din data_structure

## find function:
    **aflam pozitia unde se afla in memorie elementul pe care vrem sa-l gasim
    **copiem datele din vector de la acea pozitie intr-un header
    **incepem sa copiem datele din vector, avand grija sa crestem size-ul in
      functie de datele pe care vrem sa le copiem.
    **am creat 3 if-uri in functie de tipul de date;
    **am afisat datele;

## delete_at function:
    **aflam pozitia unde se afla elementul pe care vrem sa-l eliminam
    **deplasam spre stanga vectorul
    **realocam memoria pentru vector, deoarece i-a scazut dimensiunea
    **eliberam memoria in cazul in care vectorul avea doar un element
    **micsoram dimensiunea vectorului

## print function:
    **este similara cu find, dar de data asta afisam vectorul in totalitate


## main:
    *am pus intr-un vector datele citite
    *in functie de comanda pe care o citesc apelez functiile corespunzatoare
    ** in cazul comenzilor print si print_at, am copiat datele citite in 
       data_structure tinand cont de tipul de date
    **am copiat exact dimensiunea de care aveam nevoie pentru fiecare element in data_structure
