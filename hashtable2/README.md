# Implementarea unei Tabele de Dispersie (HashTable)

În acest fișier C, se implementează o tabelă de dispersie (HashTable) pentru gestionarea unor informații despre proiecte, iar punctele comentate în funcția main() arată diferite operații efectuate pe această structură de date:

## Afișarea Tabelei de Dispersie:
Se parcurge fiecare bucket al tabelei de dispersie și se afișează proiectele stocate în lista înlănțuită asociată fiecărui bucket.

## Calculul Bugetului Total:
Se calculează suma bugetelor tuturor proiectelor pentru un beneficiar specificat (în acest caz, "ROMAC"). Acest lucru este făcut prin parcurgerea listei înlănțuite a bucket-ului în care ar trebui să se găsească proiectele cu acel beneficiar, pe baza unei funcții de hash.

## Construirea și Afișarea unei Liste de Proiecte:
Se construiește o listă înlănțuită a tuturor proiectelor care au bugetul mai mare decât o valoare specificată (în acest caz, 20000). Apoi se afișează proiectele din această listă.
