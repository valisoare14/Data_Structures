# Implementarea unui Arbore Binar de Căutare (BST) pentru Gestionarea Rezervărilor Hoteliere

În acest fișier C, se implementează un arbore binar de căutare (BST) pentru gestionarea rezervărilor la diferite hoteluri. Mai jos sunt descrierile operațiilor executate în punctele comentate din funcția main():

## Afișare Preordine:
- Se traversează arborele în preordine (rădăcină, stânga, dreapta) și se afișează informațiile fiecărui nod. Acest mod de parcurgere afișează rădăcina înainte de nodurile descendente.

## Contravaloare:
- Se calculează suma totală a sumelor de plată pentru toate rezervările care aparțin unui anumit client, indicat prin numele "Popescu Razvan". Se parcurge întregul arbore și se adaugă la sumă valoarea `suma_de_plata` din fiecare nod cu `nume_client` potrivit.

## Numărul de Noduri pe Nivel:
- Se calculează numărul de noduri prezente la fiecare nivel al arborelui. Rezultatul este stocat într-un vector, care este apoi afișat. Funcția `NumberOfNodesPerLevel` construiește vectorul, iar `displayVector` îl afișează.

## Ștergerea Minimului:
- Se elimină nodul cu valoarea minimă din arbore (cel mai din stânga nod care nu are copil la stânga). După eliminare, arborele este afișat din nou pentru a reflecta noua structură.
